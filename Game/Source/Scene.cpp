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
#include "ModuleParticles.h"
#include "Menus.h"

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

	//mouseSpeed = config.attribute("mouseSpeed").as_float();
	enemyRange_I = config.attribute("enemyRange").as_int();

	sceneNode = config;

	fxinventorypath = "Assets/Audio/Fx/Open_inv.wav";
	inventoryfx = app->audio->LoadFx(fxinventorypath);

	return ret;
}

bool Scene::Start()
{
	app->input->godMode_B = false;
	app->physics->collisions = false;
	app->moduleparticles->Enable();
	app->questManager->Enable();
	app->itemManager->Enable();
	app->lootManager->Enable();

	app->puzzleManager->active = false;

	app->audio->PlayMusic(lobby_music, 1.0f);

	if (app->input->playerName->input.empty())
	{
		app->input->playerName->input = "Player";
	}

	//Load Map
	app->map->Load(0);
	exit_B = false;

	npcSetID = 1;

	InitEntities();
	app->entityManager->Enable();


	if (app->iScene->continueGame_B || app->input->coso)
	{
		//app->LoadGameRequest();
		app->iScene->continueGame_B = false;
	}

	// If se ha guardado anteriormente, teleport el player donde estaba en el guardado
	if (app->input->coso && app->entityManager->tpID != 21)
	{
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(app->input->posX), PIXEL_TO_METERS(app->input->posY) }, 0);
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
		&& !app->store->active && !app->dialogueSystem->active && !app->forge->active && !player->OpenMap)
	{
		if (app->inventory->active)
		{
			player->lockMovement = false;
			app->inventory->inventoryTransition_B = true;
			//app->inventory->Disable();
			app->audio->PlayFx(inventoryfx);
		}
		else 
		{
			player->lockMovement = true;
			app->inventory->Enable();
			app->audio->PlayFx(inventoryfx);
		}
	}

	if ((app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_BACK) == ButtonState::BUTTON_DOWN)
		&& !app->store->active && !app->dialogueSystem->active && !app->forge->active && !player->OpenMap)
	{
		if (app->inventory->active)
		{
			player->lockMovement = false;
			app->inventory->inventoryTransition_B = true;
			app->audio->PlayFx(inventoryfx);
		}
		else
		{
			player->lockMovement = true;
			app->inventory->Enable();
			app->inventory->partyWindow_B = true;
			app->inventory->buttonsChangeStat = true;
			app->audio->PlayFx(inventoryfx);
		}
	}

	if (app->store->active)
	{
		player->lockMovement = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) 
		app->fade->FadingToBlack(this, (Module*)app->sceneWin_Lose, 45);

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	app->menus->menuOn = player->lockMovement;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (app->itemManager->active)
	{
		app->itemManager->Disable();
	}

	app->inventory->Disable();

	player = nullptr;
	app->entityManager->Disable();
	listNpc.Clear();

	app->dialogueSystem->Disable();
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

	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
			LOG("Combat");
			app->combat->PreLoadCombat(name);
			app->fade->FadingToBlack(this, (Module*)app->combat, 5);
		}
	}
	
	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
}

bool Scene::InitEntities()
{
	for (pugi::xml_node itemNode = sceneNode.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		if (!app->puzzleManager->teamMate && strcmp(itemNode.attribute("name").as_string(), "YUROSLAVA") == 0)
		{
			itemNode = itemNode.next_sibling("npc");
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
		player->position.x = 3450;
		player->position.y = 1984;
		break;	
	case 22:
		player->position.x = 1728;
		player->position.y = 3072;
		break;

	default:
		break;
	}

	//app->entityManager->Awake();
	return true;
}