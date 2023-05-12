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

	initQuest();
	//app->LoadGameRequest();

	quest1->title = "Let's Start";
	quest2->title = "Enter the Dungeon";
	quest3->title = "Quest3";

	quest1->desc = "Go to the Practice Tent";
	quest2->desc = "Find the dungeon";
	quest3->desc = "Desc3";

	if(!quest1->complete)
		quest1->complete = false;

	if(!quest2->complete)
		quest2->complete = false;
	
	if(!quest3->complete)
		quest3->complete = false;

	quest1->active = false;
	quest2->active = false;
	quest3->active = false;

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

	//Draw Quest and check if completed
	if (quest1->active || quest2->active || quest3->active)
	{
		iPoint pos = { app->win->GetWidth() - 240, 50 };
		app->render->TextDraw("Quests:", pos.x, pos.y, 40, Font::TEXT, { 255, 255, 255 });
	}

	for (item = quests.start; item != NULL && ret == true; item = item->next)
	{
		pQuest = item->data;

		if (pQuest->active == false) continue;
		ret = item->data->Update(dt);
	}

	if (quest1->active)
	{
		iPoint pos = { app->win->GetWidth() - 240, 100 };

		//Draw Quest1
		app->render->TextDraw(quest1->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { 255, 255, 255 });
		app->render->TextDraw(quest1->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { 255, 255, 255 });

		if (quest1->complete)
		{
			//Quest1 Completed
			app->SaveGameRequest();
			quest1->active = false;
		}
	}
	if (quest2->active)
	{
		iPoint pos = { app->win->GetWidth() - 240, 170 };

		//That's for a dynamic quest list position
		if (!quest1->active)
		{
			pos.y -= 70;
		}

		//Draw Quest2
		app->render->TextDraw(quest2->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { 255, 255, 255 });
		app->render->TextDraw(quest2->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { 255, 255, 255 });

		if (quest2->complete)
		{
			//Quest2 Completed
			app->SaveGameRequest();
			quest2->active = false;
		}
	}
	if (quest3->active)
	{
		iPoint pos = { app->win->GetWidth() - 240, 240 };

		//That's for a dynamic quest list position
		if (!quest2->active)
		{
			pos.y -= 70;
		}
		//Draw Quest3
		app->render->TextDraw(quest3->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { 255, 255, 255 });
		app->render->TextDraw(quest3->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { 255, 255, 255 });

		if (quest3->complete)
		{
			//Quest3 Completed
			app->SaveGameRequest();
			quest3->active = false;
		}
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

bool QuestManager::initQuest() 
{
	quest1 = (Quest*)app->questManager->CreateQuest(QuestType::INTERACT);
	quest1->parameters = questNode.child("Quest1");
	quest1->Awake();

	quest2 = (Quest*)app->questManager->CreateQuest(QuestType::INTERACT);
	quest2->parameters = questNode.child("Quest2");
	quest2->Awake();	
	
	quest3 = (Quest*)app->questManager->CreateQuest(QuestType::INTERACT);
	quest3->parameters = questNode.child("Quest3");
	quest3->Awake();

	return true;
}