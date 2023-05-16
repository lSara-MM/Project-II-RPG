#include "EntityManager.h"

#include "App.h"
#include "Map.h"
#include "Physics.h"
#include "Textures.h"
#include "Scene.h"
#include "IntroScene.h"
#include "PuzzleManager.h"
#include "QuestManager.h"
#include "HouseOfTerrors.h"
#include "PracticeTent.h"
#include "Circus.h"

#include "Defs.h"
#include "Log.h"

#include "Combat.h"
//CHARACTHERS
#include "Player.h"
#include "NPC.h"
#include "Character.h"



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

	entityNode = config;

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
	case EntityType::COMBAT_CHARA:
		entity = new Character();
		break;

		// To test. Guarrada to fea
	case EntityType::MENU_CHARA:
		entity = new Character();

		// no entra xd
		entity->type == EntityType::MENU_CHARA;
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

	if (app->scene->player != nullptr)
	{
		if (app->scene->active)
		{
			app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);
		}
	}

	if (app->hTerrors->player != nullptr)
	{
		if (app->hTerrors->active)
		{
			app->hTerrors->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);

		}
	}
	
	if (app->practiceTent->player != nullptr)
	{
		if (app->practiceTent->active)
		{
			app->practiceTent->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);
		}
	}	
	
	if (app->circus->player != nullptr)
	{
		if (app->circus->active)
		{
			app->circus->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x),PIXEL_TO_METERS(y) }, 0);
		}
	}

	//app->scene->currentHP_Bard = data.child("bard").attribute("currentHp").as_int();
	//app->scene->currentHP_Protagonist = data.child("protagonist").attribute("currentHp").as_int();

	return true;
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node player = data.append_child("player");

	if (app->scene->active)
	{
		player.append_attribute("x") = app->scene->player->position.x;
		player.append_attribute("y") = app->scene->player->position.y;
	}
	
	if (app->hTerrors->active)
	{
		player.append_attribute("x") = app->hTerrors->player->position.x;
		player.append_attribute("y") = app->hTerrors->player->position.y;
	}	

	if (app->practiceTent->active)
	{
		player.append_attribute("x") = app->practiceTent->player->position.x;
		player.append_attribute("y") = app->practiceTent->player->position.y;
	}	
	
	if (app->circus->active)
	{
		player.append_attribute("x") = app->circus->player->position.x;
		player.append_attribute("y") = app->circus->player->position.y;
	}

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