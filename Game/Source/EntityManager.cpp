#include "EntityManager.h"

#include "App.h"
#include "Map.h"
#include "Physics.h"
#include "Textures.h"
#include "Scene.h"
#include "IntroScene.h"
#include "PuzzleManager.h"

#include "Defs.h"
#include "Log.h"

#include "Combat.h"
//CHARACTHERS
#include "Player.h"
#include "NPC.h"

#include "Characther.h" 
//PCs
#include "PCProtagonist.h"
#include "PCBard.h"
//ECs
#include "ECHouseTank.h"
#include "ECHouseDPS.h"
#include "ECHouseHealer.h"

EntityManager::EntityManager() : Module()
{ 
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;
}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();
	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::NPC:
		entity = new Npc();
		break;
	case EntityType::PC_PROTAGONIST:
		entity = new Protagonist();
		break;
	case EntityType::PC_BARD:
		entity = new Bard();
		break;
	case EntityType::ENEMY_TANK_HOUSE:
		entity = new HouseTank();
		break;
	case EntityType::ENEMY_DPS_HOUSE:
		entity = new HouseDPS();
		break;
	case EntityType::ENEMY_HEALER_HOUSE:
		entity = new HouseHealer();
		break;

	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	float x = data.child("player").attribute("x").as_int();
	float y = data.child("player").attribute("y").as_int();

	app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);

	app->puzzleManager->palancas = data.child("puzzle").attribute("palancas").as_bool();
	app->puzzleManager->escape = data.child("puzzle").attribute("escape").as_bool();
	app->puzzleManager->rescue = data.child("puzzle").attribute("rescue").as_bool();
	app->puzzleManager->keyPalancas = data.child("puzzle").attribute("keyPalancas").as_int();
	app->puzzleManager->keyEscape = data.child("puzzle").attribute("keyEscape").as_int();
	app->puzzleManager->keyRescue = data.child("puzzle").attribute("keyRescue").as_int();

	//app->scene->currentHP_Bard = data.child("bard").attribute("currentHp").as_int();
	//app->scene->currentHP_Protagonist = data.child("protagonist").attribute("currentHp").as_int();

	return true;
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node player = data.append_child("player");
	pugi::xml_node puzzle = data.append_child("puzzle");

	player.append_attribute("x") = app->scene->player->position.x;
	player.append_attribute("y") = app->scene->player->position.y;
	
	puzzle.append_attribute("palancas") = app->puzzleManager->palancas;
	puzzle.append_attribute("escape") = app->puzzleManager->escape;
	puzzle.append_attribute("rescue") = app->puzzleManager->rescue;
	puzzle.append_attribute("keyPalancas") = app->puzzleManager->keyPalancas;
	puzzle.append_attribute("keyEscape") = app->puzzleManager->keyEscape;
	puzzle.append_attribute("keyRescue") = app->puzzleManager->keyRescue;

	if (!app->iScene->previousGame_B)
	{
		app->iScene->previousGame_B = true;

		app->iScene->SaveState(app->iScene->IntroSaveNode);
	}

	//pugi::xml_node bard = data.append_child("bard");
	//bard.append_attribute("currentHp") = app->scene->currentHP_Bard;

	//pugi::xml_node protagonist = data.append_child("protagonist");
	//protagonist.append_attribute("currentHp") = app->scene->currentHP_Protagonist;

	//app->scene->isCharacterLoaded_B = false;
 
	return true;
}

List<Entity*> EntityManager::GetEntitiesByType(EntityType type)
{
	List<Entity*> result;
	for (int i = 0; i < 7; i++)
	{
		if (entities[i]->type == type) {
			result.Add(entities[i]);
		}
	}

	return result;
}