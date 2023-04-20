#include "HouseOfTerrors.h"
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

HouseOfTerrors::HouseOfTerrors() : Module()
{
	name.Create("HouseOfTerrors");
}

HouseOfTerrors::~HouseOfTerrors()
{}

bool HouseOfTerrors::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	musHauntedPath = config.attribute("musicD1").as_string();
	mute_B = false;

	sceneNode = config;

	return ret;
}

bool HouseOfTerrors::Start()
{
	//Load Map
	app->map->Load(1);

	//Music
	app->audio->PlayMusic(musHauntedPath, 0);

	exit_B = false;

	npcSetID = 1;

	// Settings
	pSettings = nullptr;
	pPause = nullptr;
	pause_B = false;
	settings_B = false;

	// Pause 

	InitEntities();
	app->entityManager->Enable();

	return true;
}

bool HouseOfTerrors::PreUpdate()
{
	return true;
}

bool HouseOfTerrors::Update(float dt)
{
	//Draw Map
	app->map->Draw();

	//Load Debug keys
	Debug();

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

bool HouseOfTerrors::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool HouseOfTerrors::CleanUp()
{
	LOG("Freeing scene");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->entityManager->Disable();

	if (pSettings != nullptr)
	{
		pSettings->CleanUp();
	}
	if (pPause != nullptr)
	{
		pPause->CleanUp();
	}

	app->guiManager->CleanUp();
	app->map->CleanUp();

	return true;
}

void HouseOfTerrors::Debug()
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

bool HouseOfTerrors::InitEntities()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->scene->sceneNode.child("player");
	player->Awake();
	player->position.x = 1817;
	player->position.y = 1250;
	player->Start();

	return true;
}

bool HouseOfTerrors::OnGuiMouseClickEvent(GuiControl* control)
{

	return true;
}