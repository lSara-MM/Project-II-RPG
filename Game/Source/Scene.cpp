#include "Scene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "IntroScene.h"
#include "PuzzleManager.h"
#include "QuestManager.h"
#include "LootManager.h"
#include "Combat.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "DialogueSystem.h"
#include "GuiManager.h"
#include "ItemManager.h"
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
	pause_music = config.attribute("pause").as_string();
	mute_B = false;

	//mouseSpeed = config.attribute("mouseSpeed").as_float();
	enemyRange_I = config.attribute("enemyRange").as_int();

	sceneNode = config;

	fxpausepath = "Assets/Audio/Fx/Clown_Button.wav";
	pausefx = app->audio->LoadFx(fxpausepath);

	return ret;
}

bool Scene::Start()
{
	app->input->godMode_B = false;
	app->physics->collisions = false;
	app->questManager->Enable();
	app->itemManager->Enable();
	app->lootManager->Enable();

	if (app->puzzleManager->active == true) {
		app->puzzleManager->CleanUp();
	}

	app->puzzleManager->active = false;

	app->audio->PlayMusic(lobby_music, 1.0);

	if (app->input->playerName->input.empty())
	{
		app->input->playerName->input = "Player";
	}

	//Load Map
	app->map->Load(0);
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

	InitEntities();

	player->lockMovement = false;

	app->entityManager->Enable();


	if (app->iScene->continueGame_B || app->input->coso)
	{
		//app->LoadGameRequest();
		app->iScene->continueGame_B = false;
	}

	//GUARRADA SUPER TEMPORAL
	app->itemManager->comb = 0;

	////CleanUp chests
	//for (int i = 0; i < app->lootManager->chests.size(); i++)
	//{
	//	if (app->lootManager->chests[i]->used == false)
	//	{
	//		app->lootManager->chests[i]->CleanUp();
	//	}
	//}

	// If se ha guardado anteriormente, teleport el player donde estaba en el guardado
	if (app->input->coso)
	{
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(app->input->posX),PIXEL_TO_METERS(app->input->posY) }, 0);
		app->input->coso = false;
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

	//Inventory
	if ((app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_Y) == ButtonState::BUTTON_DOWN)
		&& !app->store->active && !app->dialogueSystem->active)
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

	app->input->GetMousePosition(mouseX_scene, mouseY_scene);


	/*Entity* entidad2 = app->entityManager->CreateEntity(EntityType::ENEMY_TANK_HOUSE);
	app->entityManager->AddEntity(entidad2);*/
	

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) 
		app->fade->FadingToBlack(this, (Module*)app->sceneWin_Lose, 30);

	if (pause_B || player->lockMovement) { app->input->HandleGamepadMouse(mouseX_scene, mouseY_scene, app->input->mouseSpeed_F, dt); }

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	//if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	//{
	//	settings_B = !settings_B;

	//	if (settings_B)
	//	{
	//		pSettings = new Settings(this);
	//	}
	//	else
	//	{
	//		pSettings->CloseSettings();
	//		pSettings->CleanUp();
	//	}
	//}
	
	if (pause_B) { pPause->OpenPause(); }
	else
	{
		pPause->ClosePause();
	}
	if (settings_B) 
	{ 
		pSettings->OpenSettings();

		// TO DO: cambiar aixo, nose perque pero no deixa fer aixo des del settings
		for (ListItem<GuiButton*>* i = pSettings->pGame->listGameButtons.start; i != nullptr; i = i->next)
		{
			if (i->data->id == 807)	// Change TextSpeed button
			{
				i->data->text = app->dialogueSystem->GetTextSpeedSString();
			}
		}
	}
	else
	{
		pSettings->CloseSettings();
	}

	app->guiManager->Draw();

	if ((pause_B || player->lockMovement) && !app->store->active) {
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
		{
			app->render->DrawTexture(app->input->cursorPressedTex, mouseX_scene - app->render->camera.x, mouseY_scene - app->render->camera.y);
		}
		else
		{
			app->render->DrawTexture(app->input->cursorIdleTex, mouseX_scene - app->render->camera.x, mouseY_scene - app->render->camera.y);
		}
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->SaveToFile();

	app->entityManager->Disable();
	if (app->itemManager->active)
	{
		app->itemManager->Disable();
	}
	app->inventory->Disable();

	
	delete player;
	player = nullptr;

	listNpc.Clear();

	if (pSettings != nullptr)
	{
		pSettings->CleanUp();
	}
	if (pPause != nullptr)
	{
		pPause->CleanUp();
	}

	app->dialogueSystem->Disable();
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
	if (pause_B == false && player->pauseEnabled_B && (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_START) == BUTTON_DOWN))
	{
		app->audio->PlayFx(pausefx);
		app->audio->PlayMusic(pause_music);

		//inventory off
		app->inventory->Disable();

		pause_B = true;	

		if (pause_B)
		{
	/*		pPause = new Pause(this);
			pSettings = pPause->pSettings;*/

			pSettings->settings_B = !pSettings->settings_B;
		}
	/*	else
		{
			pPause->CleanUp();
		}*/

		LOG("PAUSE");
	}

	if (pause_B == true && (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN))
	{
		if (settings_B == true)
		{
			for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			settings_B = false;
			//pSettings->CloseSettings();
			//pSettings->CleanUp();
		}
		else 
		{
			pause_B = false;
			app->audio->PlayMusic(lobby_music);
			if (pause_B)
			{
				/*pPause = new Pause(this);*/
				/*pSettings = pPause->pSettings;*/

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

	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		LOG("Combat");
		app->combat->PreLoadCombat(name);
		app->fade->FadingToBlack(this, (Module*)app->combat, 5);
	}
	
	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
}

bool Scene::InitEntities()
{
	for (pugi::xml_node itemNode = sceneNode.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		if (!app->puzzleManager->teamMate && strcmp(itemNode.attribute("name").as_string(), "Yuroslava") == 0)
		{
			break;
		}

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
		player->lockMovement = false;
		app->audio->PlayMusic(lobby_music);
		//pPause->CleanUp();
		break;
		
	case 702: 
		LOG("Button Resume click");
		pause_B = false;
		player->lockMovement = false;
		app->audio->PlayMusic(lobby_music);
		//pPause->CleanUp();
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
		//pSettings = new Settings(this);
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
		//pSettings->CleanUp();
		break;

	case 802:
		LOG("Game settings click");
		pSettings->pGame->game_B = true;

		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 803:
		LOG("Graphics settings click");
		pSettings->pGraphics->graphics_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pAudio->CloseAudioSettings();
		break;


	case 804:
		LOG("Audio settings click");
		pSettings->pAudio->audio_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		break;

	case 805:
		LOG("Credits settings click");
		pSettings->pControl->control_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;


		// Game settings
	case 806:
		LOG("Button Language click");

		break;

	case 807:
		LOG("Button Text Speed click");
		control->text = app->dialogueSystem->ChangeTextSpeed();
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