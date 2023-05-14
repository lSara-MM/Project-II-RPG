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
#include "ItemManager.h"

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

	//Init player inventory
	app->itemManager->SetPlayerForScene(player);

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

	//Inventory
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (app->inventory->active)
		{
			app->inventory->Disable();
		}
		else {
			app->inventory->Enable();
		}
	}

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