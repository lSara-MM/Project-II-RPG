#include "PracticeTent.h"
#include "Scene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "IntroScene.h"
#include "Combat.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "DialogueSystem.h"
#include "GuiManager.h"
#include "Map.h"
#include "Pathfinding.h"
#include "ItemManager.h"
#include "QuestManager.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
using namespace std;
#include <sstream>

PracticeTent::PracticeTent() : Module()
{
	name.Create("practiceTent");
}

PracticeTent::~PracticeTent()
{}

bool PracticeTent::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	practisePath = config.attribute("musTent").as_string();
	pause_music = config.attribute("pause").as_string();

	texturepathDummy = config.attribute("texturepathDummy").as_string();
	widthDummy = config.attribute("widthCollider").as_int();
	heigthDummy = config.attribute("heightCollider").as_int();
	posDummy.x = config.attribute("xCollider").as_int();
	posDummy.y = config.attribute("yCollider").as_int();

	mute_B = false;
	mouseSpeed = config.attribute("mouseSpeed").as_float();
	sceneNode = config;

	fxpausepath = "Assets/Audio/Fx/Clown_Button.wav";
	pausefx = app->audio->LoadFx(fxpausepath);

	return ret;
}

bool PracticeTent::Start()
{
	dummy = app->tex->Load(texturepathDummy);
	Dummy = app->physics->CreateRectangle(posDummy.x, posDummy.y, widthDummy, heigthDummy, bodyType::STATIC); 
	Dummy->body->SetFixedRotation(true);

	DummySens = app->physics->CreateRectangleSensor(posDummy.x + widthDummy / 8, posDummy.y + heigthDummy / 8, widthDummy * 2, heigthDummy * 2, bodyType::STATIC);
	DummySens->body->SetFixedRotation(true);
	DummySens->ctype = ColliderType::DUMMY;

	textureE = app->tex->Load("Assets/GUI/UI_E.png");

	DummySensor = false;

	app->input->godMode_B = false;
	app->physics->collisions = false;
	//Load Map
	app->map->Load(3);

	//Music
	app->audio->PlayMusic(practisePath, 1.0f);
	exit_B = false;

	npcSetID = 1;

	// Settings
	//pSettings = nullptr;
	//pPause = nullptr;
	pSettings = new Settings(this);
	pPause = new Pause(this);
	pPause->pSettings = pSettings;
	pause_B = false;
	settings_B = false;

	// Pause 

	InitEntities();
	app->entityManager->Enable();

	//Init player inventory
	app->itemManager->SetPlayerForScene(player);

	//GUARRADA SUPER TEMPORAL
	app->itemManager->comb = 1;

	return true;
}

bool PracticeTent::PreUpdate()
{
	return true;
}

bool PracticeTent::Update(float dt)
{
	//Draw Map
	app->map->Draw();

	SDL_Rect rectDummy = {18, 13, widthDummy, heigthDummy };

	app->render->DrawTexture(dummy, posDummy.x - widthDummy / 2, posDummy.y - heigthDummy / 2, &rectDummy);

	if(DummySensor)
	{
		app->render->DrawTexture(textureE, posDummy.x + widthDummy / 2, posDummy.y + heigthDummy + 20);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			LOG("Combat");
			app->combat->PreLoadCombat(app->itemManager->arrParty, name);
			app->fade->FadingToBlack(this, (Module*)app->combat, 5);

			if (app->questManager->quest3->active)
			{
				app->questManager->quest3->complete = true;
				app->questManager->SaveState();
			}

			DummySensor = false;
		}
	}

	//Load Debug keys
	Debug();

	//Inventory
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (app->inventory->active)
		{
			player->lockMovement = false;
			app->inventory->Disable();
		}
		else 
		{
			player->lockMovement = true;
			app->inventory->Enable();
		}
	}

	app->input->GetMousePosition(mouseX_pos, mouseY_pos);

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

bool PracticeTent::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	/*if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
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
	}*/

	if (pause_B) { pPause->OpenPause(); }
	else
	{
		pPause->ClosePause();
	}
	if (settings_B) { pSettings->OpenSettings(); }
	else
	{
		pSettings->CloseSettings();
	}

	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool PracticeTent::CleanUp()
{
	LOG("Freeing scene");

	if (dummy != nullptr) 
	{
		app->tex->UnLoad(dummy);
	}

	if(textureE != nullptr)
	{
		app->tex->UnLoad(textureE);
	}

	if (Dummy != nullptr) 
	{
		Dummy->body->GetWorld()->DestroyBody(Dummy->body);
	}
	if (DummySens != nullptr) 
	{
		DummySens->body->GetWorld()->DestroyBody(DummySens->body);
	}

	DummySensor = false;

	app->entityManager->Disable();

	delete player;
	player = nullptr;

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

void PracticeTent::Debug()
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
	if (pause_B == false && (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_START) == BUTTON_DOWN))
	{
		pause_B = true;
		app->audio->PlayFx(pausefx);
		app->audio->PlayMusic(pause_music);

		if (pause_B)
		{
			/*pPause = new Pause(this);
			pSettings = pPause->pSettings;*/

			pSettings->settings_B = !pSettings->settings_B;
		}
	/*	else
		{
			pPause->CleanUp();
		}*/

		LOG("PAUSE");
	}

	if (pause_B == true && (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN))//POSAR CONTROL NORMAL
	{

		if (settings_B == true)
		{
			for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			settings_B = false;
			app->audio->PlayMusic(practisePath);
			pSettings->CloseSettings();
			pSettings->CleanUp();
		}
		else
		{
			pause_B = false;
			if (pause_B)
			{
				//pPause = new Pause(this);
				//pSettings = pPause->pSettings;

				pSettings->settings_B = !pSettings->settings_B;
			}
			else
			{
				/*pPause->CleanUp();*/
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

bool PracticeTent::InitEntities()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->entityManager->entityNode.child("player");
	player->Awake();
	player->position.x = 105;
	player->position.y = 341;

	return true;
}

bool PracticeTent::OnGuiMouseClickEvent(GuiControl* control)
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
		app->audio->PlayMusic(practisePath);
		/*pPause->CleanUp();*/
		break;

	case 702:
		LOG("Button Resume click");
		pause_B = false;
		app->audio->PlayMusic(practisePath);
		/*pPause->CleanUp();*/
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
		/*pSettings = new Settings(this);*/
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
	/*	pSettings->CleanUp();*/
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