#include "Circus.h"
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

Circus::Circus() : Module()
{
	name.Create("Circus");
}

Circus::~Circus()
{}

bool Circus::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	mute_B = false;
	circusMusPath = config.attribute("musicCircus").as_string();

	sceneNode = config;

	return ret;
}

bool Circus::Start()
{
	//Load Map
	app->map->Load(2);
	
	//pause menu
	pause_B = false;

	// Settings
	pSettings = new Settings(this);

	// Pause 
	//pPause->GUI_id = pSettings->GUI_id;
	//pPause->CreatePause(this);

	app->audio->PlayMusic(circusMusPath, 1);
	
	InitEntities();
	app->entityManager->Enable();
	return true;
}

bool Circus::PreUpdate()
{
	return true;
}

bool Circus::Update(float dt)
{
	//Draw Map
	app->map->Draw();

	//Load Debug keys
	Debug();

	
	/*Entity* entidad2 = app->entityManager->CreateEntity(EntityType::ENEMY_TANK_HOUSE);
	app->entityManager->AddEntity(entidad2);*/
	
	//ERIC: Prueba que no funciona.
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) 
	{ 
		Entity* prota1 = app->entityManager->CreateEntity(EntityType::PC_BARD);
		app->entityManager->AddEntity(prota1); //No se esta metiendo

		Entity* prota2 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
		app->entityManager->AddEntity(prota2);

		{/*Entity* prota3 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
		app->entityManager->AddEntity(prota3);

		Entity* prota4 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
		app->entityManager->AddEntity(prota4);*/}

		Entity* enemy1 = app->entityManager->CreateEntity(EntityType::ENEMY_TANK_HOUSE);
		app->entityManager->AddEntity(enemy1);

		Entity* enemy2 = app->entityManager->CreateEntity(EntityType::ENEMY_DPS_HOUSE);
		app->entityManager->AddEntity(enemy2);
		
		Entity* enemy3 = app->entityManager->CreateEntity(EntityType::ENEMY_HEALER_HOUSE);
		app->entityManager->AddEntity(enemy3);

		app->fade->FadingToBlack(this, (Module*)app->combat, 30);

		//!!!PONERLOS ORDENADOS, SI NO, PETA EL CODIGO Y PRINTA MENOS PERSONAJES, QUEDAIS AVISADOS!!!
		app->combat->AddCombatant((Character*)enemy1, 0);
		app->combat->AddCombatant((Character*)enemy2, 5);
		app->combat->AddCombatant((Character*)enemy3, 3);
		app->combat->AddCombatant((Character*)prota1, 1);
		app->combat->AddCombatant((Character*)prota2, -2);
		/*app->combat->AddCombatant((Character*)prota3, 5);
		app->combat->AddCombatant((Character*)prota4, 9);*/
	}
	
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		app->combat->MoveAllies(1,4);
		/*app->combat->AddCombatant((Characther*)prota2, -2);
		app->combat->AddCombatant((Characther*)prota3, 5);*/
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


	return true;
}

bool Circus::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		pSettings->settings_B = !pSettings->settings_B;
	}
	
	if (pSettings->settings_B) { pSettings->OpenSettings(); }
	//if (pPause->pause) { pPause->OpenPause(); }
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Circus::CleanUp()
{
	LOG("Freeing scene");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//player->Disable();

	app->entityManager->Disable();

	pSettings->CleanUp();
	//pPause->CleanUp();
	app->guiManager->CleanUp();
	app->map->CleanUp();

	return true;
}

void Circus::Debug()
{
	// Start again level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 0);

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
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
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
		app->input->godMode_B = !app->input->godMode_B;
	}

	//pause menu
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		pause_B = !pause_B;
		pSettings->settings_B = !pSettings->settings_B;

		if (!pSettings->settings_B)
		{
			pSettings->CloseSettings();
		}

		LOG("PAUSE");
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		!pause_B;
		//pPause->pause = !pPause->pause;
		/*if (!pPause->pause)
		{
			pPause->ClosePause();
		}*/

		LOG("PAUSE");
	}

	// Mute / unmute
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {

		mute_B = !mute_B;
		LOG("MUTE");
	}
		

	// God mode functions
	if (app->input->godMode_B)
	{
		app->physics->collisions = true;
	}

	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
}

bool Circus::InitEntities()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->scene->sceneNode.child("player");
	player->Awake();
	player->position.x = 653;
	player->position.y = 1265;
	player->Start();

	return true;
}

bool Circus::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);
	return true;
}