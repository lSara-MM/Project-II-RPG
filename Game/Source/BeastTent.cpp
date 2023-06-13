#include "BeastTent.h"
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
#include "PuzzleManager.h"
#include "QuestManager.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "DialogueSystem.h"
#include "GuiManager.h"
#include "Map.h"
#include "Pathfinding.h"
#include "ItemManager.h"
#include "LootManager.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
using namespace std;
#include <sstream>

BeastTent::BeastTent() : Module()
{
	name.Create("BeastTent");
}

BeastTent::~BeastTent()
{}

bool BeastTent::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	musBeastPath = config.attribute("musicD2").as_string();
	sceneNode = config;

	//mouseSpeed = config.attribute("mouseSpeed").as_float();

	texturePathBeastDungeon = config.attribute("texturePathDark").as_string();

	enterCombatpath = "Assets/Audio/Fx/entrar_sala.wav";
	combatfx = app->audio->LoadFx(enterCombatpath);

	return ret;
}

bool BeastTent::Start()
{	
	app->physics->collisions = false;
	app->input->godMode_B = false;

	app->itemManager->Enable();
	if (app->lootManager->active)
	{
		app->lootManager->Start();
	}
	app->lootManager->Enable();
	app->questManager->Enable();


	//Load Map
	app->map->Load(4);

	BeastDungeon = app->tex->Load(texturePathBeastDungeon);

	//Music
	app->audio->PlayMusic(musBeastPath, 1.0f);

	InitEntities();
	app->entityManager->Enable();
	app->puzzleManager->Enable();

	//enter combat
	steps_I = 0;

	// If se ha guardado anteriormente, teleport el player donde estaba en el guardado
	if (app->input->coso)
	{
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(app->input->posX),PIXEL_TO_METERS(app->input->posY) }, 0);
		app->input->coso = false;
	}

	player->lockMovement = false;

	return true;
}

bool BeastTent::PreUpdate()
{
	return true;
}

bool BeastTent::Update(float dt)
{
	//Draw Map
	app->map->Draw();

	//Load Debug keys
	Debug();

	//Inventory
	if ((app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_Y) == ButtonState::BUTTON_DOWN)
		&& !app->store->active && !app->dialogueSystem->active)
	{
		if (app->inventory->active)
		{
			player->lockMovement = false;
			app->inventory->inventoryTransition_B = true;
		}
		else 
		{
			player->lockMovement = true;
			app->inventory->Enable();
		}
	}
	if ((app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)//digan boton mando
		&& !app->store->active && !app->dialogueSystem->active)
	{
		if (app->inventory->active)
		{
			player->lockMovement = false;
			app->inventory->inventoryTransition_B = true;
		}
		else
		{
			player->lockMovement = true;
			app->inventory->Enable();
			app->inventory->partyWindow_B = true;
			app->inventory->buttonsChangeStat = true;
		}
	}

	if (!app->input->godMode_B)
	{
		if (steps_I > 650 + rand() % (751 - 650))
		{
			LOG("Combat");
			app->audio->PlayFx(combatfx);
			app->combat->PreLoadCombat(name);
			player->lockMovement = true;

			app->fade->FadingToBlack(this, (Module*)app->combat, 45); 
			app->questManager->SaveState();
			app->puzzleManager->CleanUp();
			app->puzzleManager->active = false;
			steps_I = 0;
		}
	}
	else
	{
		steps_I = 0;
	}

	return true;
}

bool BeastTent::PostUpdate()
{
	bool ret = true;

	app->menus->menuOn = player->lockMovement;

	return ret;
}

// Called before quitting
bool BeastTent::CleanUp()
{
	LOG("Freeing scene");

	app->itemManager->Disable();
	app->inventory->Disable();

	app->entityManager->Disable();
	player = nullptr;

	//CleanUp chests
	for (int i = 0; i < app->lootManager->chests.size(); i++)
	{
		if ((app->lootManager->chests[i]->used == false && app->BeastT->active && app->lootManager->chests[i]->dungeon == ChestDungeon::BEASTS) || (app->lootManager->chests[i]->used == false && app->hTerrors->active && app->lootManager->chests[i]->dungeon == ChestDungeon::TERRORS))
		{
			app->lootManager->chests[i]->CleanUp();
		}
	}

	if (BeastDungeon != nullptr)
	{
		app->tex->UnLoad(BeastDungeon);
	}

	app->puzzleManager->Dun2CleanUp();
	app->puzzleManager->Disable();

	app->map->CleanUp();

	return true;
}

void BeastTent::Debug()
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
}

bool BeastTent::InitEntities()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->entityManager->entityNode.child("player");
	player->Awake();
	player->position.x = 1952;
	player->position.y = 1469;

	return true;
}