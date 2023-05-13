#include "QuestManager.h"

#include "App.h"
#include "Map.h"
#include "Physics.h"
#include "Textures.h"
#include "Scene.h"
#include "IntroScene.h"
#include "PuzzleManager.h"
#include "DialogueSystem.h"

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
	LoadState();

	quest1->title = "Find The Fire Guy";
	quest2->title = "Drive the Oracle crazy";
	quest3->title = "Train is life, train is love";

	quest1->desc = "Save Fire Guy from the dungeon";
	quest2->desc = "Talk to the Oracle and make him angry";
	quest3->desc = "Practice into the Practice Tent";

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
		Uint8 R = 255;
		Uint8 G = 255;
		Uint8 B = 255;

		if (quest1->complete)
		{
			//Quest1 Completed
			SaveState();
			//quest1->active = false;

			R = 124;
			G = 252;
			B = 0;
		}

		//Draw Quest1
		app->render->TextDraw(quest1->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { 255, 255, 255 });
		app->render->TextDraw(quest1->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { 255, 255, 255 });
	}
	if (quest2->active)
	{
		iPoint pos = { app->win->GetWidth() - 240, 170 };
		Uint8 R = 255;
		Uint8 G = 255;
		Uint8 B = 255;

		//That's for a dynamic quest list position
		if (!quest1->active)
		{
			pos.y -= 70;
		}

		if (quest2->complete)
		{
			//Quest2 Completed
			SaveState();
			//quest2->active = false;
			R = 124;
			G = 252;
			B = 0;
		}

		//Draw Quest2
		app->render->TextDraw(quest2->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { R, G, B });
		app->render->TextDraw(quest2->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { R, G, B });
	}
	if (quest3->active)
	{
		iPoint pos = { app->win->GetWidth() - 240, 240 };
		Uint8 R = 255;
		Uint8 G = 255;
		Uint8 B = 255;

		//That's for a dynamic quest list position
		if (!quest2->active)
		{
			pos.y -= 70;
		}
		if (quest3->complete)
		{
			//Quest3 Completed
			SaveState();
			//quest3->active = false;
			R = 124;
			G = 252;
			B = 0;
		}

		//Draw Quest3
		app->render->TextDraw(quest3->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { 255, 255, 255 });
		app->render->TextDraw(quest3->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { 255, 255, 255 });
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

bool QuestManager::LoadState() {
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game_Puzzle_Quest.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file save_game_Puzzle_Quest.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		app->puzzleManager->palancas = gameStateFile.child("puzzle").attribute("palancas").as_bool();
		app->puzzleManager->escape = gameStateFile.child("puzzle").attribute("escape").as_bool();
		app->puzzleManager->rescue = gameStateFile.child("puzzle").attribute("rescue").as_bool();
		app->puzzleManager->teamMate = gameStateFile.child("puzzle").attribute("teamMate").as_bool();
		app->puzzleManager->keyPalancas = gameStateFile.child("puzzle").attribute("keyPalancas").as_int();
		app->puzzleManager->keyEscape = gameStateFile.child("puzzle").attribute("keyEscape").as_int();
		app->puzzleManager->keyRescue = gameStateFile.child("puzzle").attribute("keyRescue").as_int();

		quest1->active = gameStateFile.child("quests").attribute("quest1").as_bool();
		quest2->active = gameStateFile.child("quests").attribute("quest2").as_bool();
		quest3->active = gameStateFile.child("quests").attribute("quest3").as_bool();
		quest1->complete = gameStateFile.child("quests").attribute("quest1_Complete").as_bool();
		quest2->complete = gameStateFile.child("quests").attribute("quest2_Complete").as_bool();
		quest3->complete = gameStateFile.child("quests").attribute("quest3_Complete").as_bool();
	}

	return ret;
}

bool QuestManager::SaveState() 
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	pugi::xml_node puzzle = saveStateNode.append_child("puzzle");
	pugi::xml_node quests = saveStateNode.append_child("quests");

	puzzle.append_attribute("palancas") = app->puzzleManager->palancas;
	puzzle.append_attribute("escape") = app->puzzleManager->escape;
	puzzle.append_attribute("rescue") = app->puzzleManager->rescue;
	puzzle.append_attribute("teamMate") = app->puzzleManager->teamMate;
	puzzle.append_attribute("keyPalancas") = app->puzzleManager->keyPalancas;
	puzzle.append_attribute("keyEscape") = app->puzzleManager->keyEscape;
	puzzle.append_attribute("keyRescue") = app->puzzleManager->keyRescue;

	quests.append_attribute("quest1") = quest1->active;
	quests.append_attribute("quest2") = quest2->active;
	quests.append_attribute("quest3") = quest3->active;
	quests.append_attribute("quest1_Complete") = quest1->complete;
	quests.append_attribute("quest2_Complete") = quest2->complete;
	quests.append_attribute("quest3_Complete") = quest3->complete;

	ret = saveDoc->save_file("save_game_Puzzle_Quest.xml");

	return ret;
}