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
	name.Create("PracticeTent");
}

PracticeTent::~PracticeTent()
{}

bool PracticeTent::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	practicePath = config.attribute("musTent").as_string();

	texturepathDummy = config.attribute("texturepathDummy").as_string();
	widthDummy = config.attribute("widthCollider").as_int();
	heigthDummy = config.attribute("heightCollider").as_int();
	posDummy.x = config.attribute("xCollider").as_int();
	posDummy.y = config.attribute("yCollider").as_int();

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
	app->audio->PlayMusic(practicePath, 1.0f);

	app->itemManager->Enable();
	app->questManager->Enable();

	InitEntities();
	app->entityManager->Enable();

	// If se ha guardado anteriormente, teleport el player donde estaba en el guardado
	if (app->input->coso)
	{
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(app->input->posX),PIXEL_TO_METERS(app->input->posY) }, 0);
		app->input->coso = false;
	}

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
		app->render->DrawTexture(textureE, posDummy.x - widthDummy / 2 + 20, posDummy.y - 60);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			LOG("Combat");
			app->combat->PreLoadCombat(name);
			app->fade->FadingToBlack(this, (Module*)app->combat, 45);

			if (app->questManager->quest3->active)
			{
				app->questManager->quest3->complete = true;
				app->questManager->quest3->active = true;
				app->questManager->SaveState();
			}
			DummySensor = false;
		}
	}

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
	return true;
}

bool PracticeTent::PostUpdate()
{
	bool ret = true;

	app->menus->menuOn = player->lockMovement;

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

	app->itemManager->Disable();
	app->inventory->Disable();

	app->entityManager->Disable();
	player = nullptr;

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

	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
			LOG("Combat");
			app->combat->PreLoadCombat(name);
			app->fade->FadingToBlack(this, (Module*)app->combat, 5);
		}
	}
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