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
	mouseSpeed = config.attribute("mouseSpeed").as_float();
	sceneNode = config;

	return ret;
}

bool Circus::Start()
{
	//Load Map
	app->map->Load(2);

	app->audio->PlayMusic(circusMusPath, 0);

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

bool Circus::PreUpdate()
{
	return true;
}

bool Circus::Update(float dt)
{
	//Draw Map
	app->map->Draw();

	app->input->GetMousePosition(mouseX_pos, mouseY_pos);

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

	if (pause_B || player->lockMovement) { app->input->HandleGamepadMouse(mouseX_pos, mouseY_pos, mouseSpeed, dt); }

	return true;
}

bool Circus::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

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
	Overlay* pOverlay = new Settings(this);

	if (pause_B) { pPause->OpenPause(); }
	if (settings_B) { pSettings->OpenOverlay(); }
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Circus::CleanUp()
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

	app->guiManager->CleanUp();
	app->map->CleanUp();

	return true;
}

void Circus::Debug()
{// Start again level
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

	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
}

bool Circus::InitEntities()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->entityManager->entityNode.child("player");
	player->Awake();
	player->position.x = 653;
	player->position.y = 1265;

	return true;
}

bool Circus::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	return true;
}