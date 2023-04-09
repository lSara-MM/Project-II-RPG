#include "EntityManager.h"
#include "Player.h"
#include "Characther.h"

#include "App.h"
#include "Map.h"
#include "Physics.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

//CHARACTHERS
//PCs
#include "PCProtagonist.h"
//ECs
#include "ECHouseTank.h"

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

	case EntityType::PC_PROTAGONIST:
		entity = new Protagonist();
		break;
	case EntityType::ENEMY_TANK_HOUSE:
		entity = new HouseTank();
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
	
	return true;
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node player = data.append_child("player");

	return true;
}

List<Entity*> EntityManager::GetEntitiesByType(EntityType type)
{
	List<Entity*> result;
	/*for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->type == type) {
			result.add((*it));
		}
	}*/

	/*ListItem<Entity*>* item = entities.start;
	while (item != NULL)
	{
		Entity* entity = item->data;
		if (entity->type == type)
		{
			//result.Del(entity); //Esto peta
		}
		item = item->next;
	}*/
	for (int i = 0; i < 7; i++)
	{
		if (entities[i]->type == type) {
			result.Add(entities[i]);
		}
	}


	return result;
}