#include "EntityManager.h"

#include "App.h"
#include "Input.h"
#include "Map.h"
#include "Physics.h"
#include "Textures.h"

#include "Scene.h"
#include "IntroScene.h"
#include "HouseOfTerrors.h"
#include "BeastTent.h"
#include "PracticeTent.h"
#include "Circus.h"
#include "SceneWin_Lose.h"
#include "Combat.h"


#include "ItemManager.h"
#include "PuzzleManager.h"
#include "QuestManager.h"
#include "LootManager.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

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

bool EntityManager::LoadState(pugi::xml_node& data, Module* module_)
{
	app->input->sceneNameSaved = data.child("player").attribute("name").as_string();
	app->input->posX = data.child("player").attribute("x").as_int();
	app->input->posY = data.child("player").attribute("y").as_int();

	int fadeTime = 90;
	if (strcmp(app->input->sceneNameSaved.c_str(), app->sceneWin_Lose->name.GetString()) == 0) { fadeTime = 5; }


	if (strcmp(app->input->sceneNameSaved.c_str(), app->scene->name.GetString()) == 0)
	{
		app->fade->FadingToBlack(module_, (Module*)app->scene, fadeTime);
	}

	if (strcmp(app->input->sceneNameSaved.c_str(), app->circus->name.GetString()) == 0)
	{
		app->fade->FadingToBlack(module_, (Module*)app->circus, fadeTime);
	}

	if (strcmp(app->input->sceneNameSaved.c_str(), app->practiceTent->name.GetString()) == 0)
	{
		app->fade->FadingToBlack(module_, (Module*)app->practiceTent, fadeTime);
	}

	if (strcmp(app->input->sceneNameSaved.c_str(), app->hTerrors->name.GetString()) == 0)
	{
		app->fade->FadingToBlack(module_, (Module*)app->hTerrors, fadeTime);
	}

	if (strcmp(app->input->sceneNameSaved.c_str(), app->BeastT->name.GetString()) == 0)
	{
		app->fade->FadingToBlack(module_, (Module*)app->BeastT, fadeTime);
	}

	return true;
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node player = data.append_child("player");
	pugi::xml_node party = data.append_child("party");

	Player* pPlayer = nullptr;
	if (app->scene->active)
	{
		player.append_attribute("name") = app->scene->name.GetString();
		pPlayer = app->scene->player;
	}
	
	if (app->circus->active)
	{
		player.append_attribute("name") = app->circus->name.GetString();
		pPlayer = app->circus->player;
	}

	if (app->practiceTent->active)
	{
		player.append_attribute("name") = app->practiceTent->name.GetString();
		pPlayer = app->practiceTent->player;
	}	
	
	if (app->hTerrors->active)
	{
		player.append_attribute("name") = app->hTerrors->name.GetString();
		pPlayer = app->hTerrors->player;
	}

	if (app->BeastT->active)
	{
		player.append_attribute("name") = app->BeastT->name.GetString();
		pPlayer = app->BeastT->player;
	}


	if (!app->iScene->previousGame_B)
	{
		app->iScene->previousGame_B = true;
		app->iScene->SaveState(app->iScene->IntroSaveNode);
	}

	player.append_attribute("x") = pPlayer->position.x;
	player.append_attribute("y") = pPlayer->position.y;

	for (int i = 0; i < app->itemManager->arrParty.size(); i++)
	{
		if (app->itemManager->arrParty.at(i) != nullptr)
		{
			party.append_attribute("id") = app->itemManager->arrParty.at(i)->id;
		}
		else
		{
			break;
		}
	}
	pPlayer = nullptr;
	app->itemManager->loadParty_B = true;

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