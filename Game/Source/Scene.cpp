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

	sceneNode = config;

	mouseSpeed = config.attribute("mouseSpeed").as_float();

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
	
	if (app->iScene->continueGame_B)
	{
		app->LoadGameRequest();
		app->iScene->continueGame_B = false;
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
		app->iScene->continueGame_B = true;
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
			pSettings->CloseSettings();
			pSettings->CleanUp();
		}
	}
	
	if (pause_B) { pPause->OpenPause(); }
	if (settings_B) { pSettings->OpenSettings(); }
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->entityManager->CleanUp();
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
	if (pause_B == false && (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->controller.START == 1))
	{
		pause_B = true;	

		if (pause_B)
		{
			pPause = new Pause(this);
			pSettings = pPause->pSettings;

			pSettings->settings_B = !pSettings->settings_B;
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
			pSettings->CloseSettings();
			pSettings->CleanUp();
		}
		else 
		{
			pause_B = false;
			if (pause_B)
			{
				pPause = new Pause(this);
				pSettings = pPause->pSettings;

				pSettings->settings_B = !pSettings->settings_B;
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
	player->parameters = sceneNode.child("player");
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

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button start click");
		break;
	case 2:
		LOG("Button continue click");
		break;
	case 3:
		LOG("Button settings click");
		break;
	case 4:
		LOG("Button Exit game click");
		break;

		// Pause
	case 701: 
		LOG("Button Close pause click");
		pause_B = false;
		pPause->CleanUp();
		break;
		
	case 702: 
		LOG("Button Resume click");
		pause_B = false;
		pPause->CleanUp();
		break;
		
	case 703: 
		LOG("Button Return to title click");
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);		
		break;
		
	case 704: 
		LOG("Button Settings click");
		for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::DISABLED;
		}

		settings_B = true;
		pSettings = new Settings(this);
		break;
			
	case 705: 
		LOG("Button Exit click");
		exit_B = true;
		break;


		// Settings
	case 801:
		LOG("Button Close settings click");
		for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NORMAL;
		}

		settings_B = false;
		pSettings->CloseSettings();
		pSettings->CleanUp();
		break;

	case 802:
		LOG("Game settings click");
		pSettings->pGame->game_B = true;

		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 803:
		LOG("Controls settings click");
		pSettings->pControl->control_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 804:
		LOG("Graphics settings click");
		pSettings->pGraphics->graphics_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 805:
		LOG("Audio settings click");
		pSettings->pAudio->audio_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		break;


		// Game settings
	case 806:
		LOG("Button Language click");

		break;

	case 807:
		LOG("Button Text Speed click");

		break;
	case 808:
		LOG("Button Return to Title click");
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);
		break;

	case 809:
		LOG("Button Exit Game click");
		exit_B = true;
		break;


		// Control settings
	case 810:
		LOG("Button Move Up keyboard check");

		break;

	case 811:
		LOG("Button Move Up gamepad check");

		break;

	case 812:
		LOG("Button Move Left keyboard check");

		break;

	case 813:
		LOG("Button Move Left gamepad check");

		break;

	case 814:
		LOG("Button Move Right keyboard check");

		break;

	case 815:
		LOG("Button Move Right gamepad check");

		break;

	case 816:
		LOG("Button Move Down keyboard check");

		break;

	case 817:
		LOG("Button Move Down gamepad check");

		break;

	case 818:
		LOG("Button Interact keyboard check");

		break;

	case 819:
		LOG("Button Interact gamepad check");

		break;

	case 820:
		LOG("Button Inventory keyboard check");

		break;

	case 821:
		LOG("Button Party gamepad check");

		break;

	case 822:
		LOG("Button Quests keyboard check");

		break;

	case 823:
		LOG("Button Quests gamepad check");

		break;

	case 824:
		LOG("Button Map keyboard check");

		break;

	case 825:
		LOG("Button Map gamepad check");

		break;

	case 826:
		LOG("Button Settings keyboard check");

		break;

	case 827:
		LOG("Button Settings gamepad check");

		break;


		// Graphics settings
	case 828:
		LOG("Button Windows size");

		break;

	case 829:
		LOG("Checkbox Fullscreen check");
		app->win->fullscreen = !app->win->fullscreen;
		app->win->FullscreenWin();
		break;


	case 830:
		LOG("Checkbox Vsync check");
		app->render->vSync_B = !app->render->vSync_B;
		app->render->VSyncOn();
		break;

	case 831:
		LOG("Button Max fps");

		break;


		// Audio settings
	case 832:
		LOG("Slider bar General volume");
		app->audio->ChangeGeneralVolume(pSettings->pAudio->general->volume100);
		break;

	case 833:
		LOG("Slider bar Music volume");
		break;

	case 834:
		LOG("Slider bar Fx volume");
		break;
	}

	return true;
}