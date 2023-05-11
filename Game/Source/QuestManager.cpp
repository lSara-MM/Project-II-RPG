#include "QuestManager.h"

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

QuestManager::QuestManager() : Module()
{
	name.Create("questmanager");
}

// Destructor
QuestManager::~QuestManager()
{}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Quest Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Quest*>* item;
	Quest* pQuest = NULL;

	for (item = quests.start; item != NULL && ret == true; item = item->next)
	{
		pQuest = item->data;

		if (pQuest->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;
}

bool QuestManager::Start() {

	bool ret = true;

	//Iterates over the entities and calls Start
	ListItem<Quest*>* item;
	Quest* pQuest = NULL;

	for (item = quests.start; item != NULL && ret == true; item = item->next)
	{
		pQuest = item->data;

		if (pQuest->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool QuestManager::CleanUp()
{
	bool ret = true;
	ListItem<Quest*>* item;
	item = quests.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	quests.Clear();
	return ret;
}

Quest* QuestManager::CreateQuest(QuestType type)
{
	Quest* quest = nullptr;

	switch (type)
	{
	case QuestType::COLLECT:
		quest = new Quest(QuestType::COLLECT);
		break;
	case QuestType::FIND:
		quest = new Quest(QuestType::FIND);
		break;
	case QuestType::INTERACT:
		quest = new Quest(QuestType::INTERACT);
		break;
	case QuestType::TRAVEL:
		quest = new Quest(QuestType::TRAVEL);
		break;
	case QuestType::UNKNOWN:
		quest = new Quest(QuestType::UNKNOWN);
		break;
	default: break;
	}

	// Created entities are added to the list
	AddQuest(quest);

	return quest;
}

void QuestManager::DestroyQuest(Quest* quest)
{
	ListItem<Quest*>* item;

	for (item = quests.start; item != NULL; item = item->next)
	{
		if (item->data == quest) quests.Del(item);
	}
}

void QuestManager::AddQuest(Quest* quest)
{
	if (quest != nullptr) quests.Add(quest);
}

bool QuestManager::Update(float dt)
{
	bool ret = true;
	ListItem<Quest*>* item;
	Quest* pQuest = NULL;

	for (item = quests.start; item != NULL && ret == true; item = item->next)
	{
		pQuest = item->data;

		if (pQuest->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

List<Quest*> QuestManager::GetQuestByType(QuestType type)
{
	List<Quest*> result;
	for (int i = 0; i < 7; i++)
	{
		if (quests[i]->type == type) {
			result.Add(quests[i]);
		}
	}

	return result;
}