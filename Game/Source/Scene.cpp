#include "Scene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "IntroScene.h"
#include "LoseScene.h"
#include "Combat.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "DialogueSystem.h"
#include "GuiManager.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
using namespace std;
#include <sstream>
#include <time.h>

Scene::Scene() : Module()
{
	name.Create("scene");
}

Scene::~Scene()
{}

bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	lobby_music = config.attribute("music").as_string();
	mute_B = false;

	mouseSpeed = config.attribute("mouseSpeed").as_float();
	enemyRange_I = config.attribute("enemyRange").as_int();;

	sceneNode = config;

	return ret;
}

bool Scene::Start()
{
	app->audio->PlayMusic(lobby_music, 0);

	if (app->input->playerName.empty())
	{
		app->input->playerName = "Player";
	}

	//Load Map
	app->map->Load(0);
	exit_B = false;

	npcSetID = 1;

	// Settings
	pSettings = nullptr;
	pPause = nullptr;
	pause_B = false;
	settings_B = false;

	InitEntities();

	app->entityManager->Enable();
	
	if (app->iScene->continuecreated_B)
	{
		app->LoadGameRequest();
		app->iScene->continuecreated_B = false;
	}

	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	app->map->Draw();

	Debug();

	app->input->GetMousePosition(mouseX_scene, mouseY_scene);

	/*Entity* entidad2 = app->entityManager->CreateEntity(EntityType::ENEMY_TANK_HOUSE);
	app->entityManager->AddEntity(entidad2);*/
	
	//ERIC: Prueba que no funciona.
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN || app->input->controller.X != 0) {
		app->fade->FadingToBlack(this, (Module*)app->combat, 30);
		app->SaveGameRequest();//guardar para volver misma posicion al volver de combate
		app->iScene->continuecreated_B = true;
	}

	//Borrar
	float speed = 0.2 * dt;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);

	if (pause_B || player->lockMovement) { app->input->HandleGamepadMouse(mouseX_scene, mouseY_scene, mouseSpeed, dt); }

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->dialogueSystem->Enable();
		app->dialogueSystem->CleanUp();
		app->dialogueSystem->LoadDialogue(0);
	}

	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		settings_B = !settings_B;

		if (settings_B)
		{
			pSettings = new Settings(this);
		}
		else
		{
			pSettings->CloseOverlay();
			pSettings->CleanUp();
		}
	}
	
	if (pause_B) { pPause->OpenPause(); }
	if (settings_B) { pSettings->OpenOverlay(); }
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->entityManager->Disable();

	if (pSettings != nullptr)
	{
		pSettings->CleanUp();
	}
	if (pPause != nullptr)
	{
		pPause->CleanUp();
	}
	
	app->dialogueSystem->CleanUp();
	app->guiManager->CleanUp();
	app->map->CleanUp();

	if (app->combat->active == true)
	{
		// this shouldnt be here later
		player->LoadAllPC();
		player->SetParty();
		//

		InitCombat();
		app->combat->active = false;
	}
	
	return true;
}

void Scene::Debug()
{
	// Start again level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 0);

	// Return Title
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		app->fade->FadingToBlack(this, (Module*)app->iScene, 0);
	}

	// Load / Save - keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	// Show Gui 
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {

		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
	}

	// Show collisions
	if (app->input->GetKey(SDL_SCANCODE_F8))
	{
		app->physics->collisions = !app->physics->collisions;
	}

	// Enable/Disable Frcap
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		frcap_B = !frcap_B;
		LOG("frame rate: %d", app->physics->frameRate);
	}

	// GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->physics->collisions = !app->physics->collisions;
		app->input->godMode_B = !app->input->godMode_B;
	}

	// Pause menu
	if (pause_B == false && (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->controller.START == 1))
	{
		pause_B = true;	

		if (pause_B)
		{
			pPause = new Pause(this);
			pSettings = pPause->pSettings;

			pSettings->created_B = !pSettings->created_B;
		}
		else
		{
			pPause->CleanUp();
		}

		LOG("PAUSE");
	}

	if (pause_B == true && (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->controller.B == 1))//POSAR CONTROL NORMAL
	{
		
		if (settings_B == true)
		{
			for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			settings_B = false;
			pSettings->CloseOverlay();
			pSettings->CleanUp();
		}
		else 
		{
			pause_B = false;
			if (pause_B)
			{
				pPause = new Pause(this);
				pSettings = pPause->pSettings;

				pSettings->created_B = !pSettings->created_B;
			}
			else
			{
				pPause->CleanUp();
			}

		}
		
		LOG("PAUSE");
	}


	// Mute / unmute
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {

		mute_B = !mute_B;
		LOG("MUTE");
	}

	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		LOG("Combat");
		//InitCombat(); //Lo pongo aqui porque llamarlo tras cada cleanUp trae problemas
		app->fade->FadingToBlack(this, (Module*)app->combat, 5);
		app->combat->active = true;
		//InitCombat(); //Lo pongo aqui porque llamarlo tras cada cleanUp trae problemas
	}
	
	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
}

bool Scene::InitEntities()
{
	for (pugi::xml_node itemNode = sceneNode.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		Npc* npc = (Npc*)app->entityManager->CreateEntity(EntityType::NPC);
		npc->parameters = itemNode;
		npc->Awake();

		listNpc.Add(npc);
	}

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->entityManager->entityNode.child("player");
	player->Awake();

	switch (app->entityManager->tpID)
	{
	case 0:
		player->position.x = 2365;
		player->position.y = 4429;
		break;
	case 1:
		player->position.x = 4277;
		player->position.y = 3869;
		break;
	case 21:
		player->position.x = 3437;
		player->position.y = 1085;
		break;

	default:
		break;
	}

	//app->entityManager->Awake();
	return true;
}

void Scene::InitCombat()
{
	srand(time(NULL));

	int randSize = rand() % 3 + 2;
	int randId;
	vector<int> arr;

	for (int i = 0; i < randSize; i++)
	{
		randId = rand() % 3;
		arr.push_back(randId);
	}

	app->combat->InitEnemies(name, arr);
	app->combat->InitAllies(player->arrParty);
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	return true;
}