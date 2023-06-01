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


QuestManager::QuestManager() : Module()
{
	name.Create("questmanager");
}

// Destructor
QuestManager::~QuestManager()
{

}

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

	InitQuest();

	quest1->title = "Find The Fire Guy";
	quest2->title = "Drive the Oracle crazy";
	quest3->title = "Train is life, train is love";

	quest1->desc = "Save Fire Guy from the dungeon";
	quest2->desc = "Talk to the Oracle and make him angry";
	quest3->desc = "Practice into the Practice Tent";

	RQ1 = 255;
	GQ1 = 255;
	BQ1 = 255;

	RQ2 = 255;
	GQ2 = 255;
	BQ2 = 255;

	RQ3 = 255;
	GQ3 = 255;
	BQ3 = 255;

	quest1->complete = false;
	quest2->complete = false;
	quest3->complete = false;

	quest1->active = false;
	quest2->active = false;
	quest3->active = false;

	LoadState();

	if(app->puzzleManager->teamMate)
	{
		quest1->complete = true;
	}

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
		iPoint pos = { 10, 50 };
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
		iPoint pos = { 10, 100 };

		if (quest1->complete)
		{
			//Quest1 Completed
			//quest1->active = false;

			RQ1 = 124;
			GQ1 = 252;
			BQ1 = 0;
		}

		//Draw Quest1
		app->render->TextDraw(quest1->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { RQ1, GQ1, BQ1 });
		app->render->TextDraw(quest1->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { RQ1, GQ1, BQ1 });
	}
	if (quest2->active)
	{
		iPoint pos = { 10, 170 };

		//That's for a dynamic quest list position
		if (!quest1->active)
		{
			pos.y -= 70;
		}

		if (quest2->complete)
		{
			//Quest2 Completed
			//quest2->active = false;
			RQ2 = 124;
			GQ2 = 252;
			BQ2 = 0;
		}

		//Draw Quest2
		app->render->TextDraw(quest2->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { RQ2, GQ2, BQ2 });
		app->render->TextDraw(quest2->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { RQ2, GQ2, BQ2 });
	}
	if (quest3->active)
	{
		iPoint pos = { 10, 240 };

		//That's for a dynamic quest list position
		if (!quest2->active)
		{
			pos.y -= 70;
		}
		if (quest3->complete)
		{
			//Quest3 Completed
			//quest3->active = false;
			RQ3 = 124;
			GQ3 = 252;
			BQ3 = 0;
		}

		//Draw Quest3
		app->render->TextDraw(quest3->title.GetString(), pos.x, pos.y, 25, Font::TEXT, { RQ3, GQ3, BQ3 });
		app->render->TextDraw(quest3->desc.GetString(), pos.x, pos.y + 30, 15, Font::TEXT, { RQ3, GQ3, BQ3 });
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

bool QuestManager::InitQuest() 
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
		app->puzzleManager->palancas = gameStateFile.child("save_state").child("puzzle").attribute("palancas").as_bool();
		app->puzzleManager->escape = gameStateFile.child("save_state").child("puzzle").attribute("escape").as_bool();
		app->puzzleManager->rescue = gameStateFile.child("save_state").child("puzzle").attribute("rescue").as_bool();
		app->puzzleManager->teamMate = gameStateFile.child("save_state").child("puzzle").attribute("teamMate").as_bool();
		app->puzzleManager->bossIsDead = gameStateFile.child("save_state").child("puzzle").attribute("bossIsDead").as_bool();
		app->puzzleManager->bossInvent = gameStateFile.child("save_state").child("puzzle").attribute("bossInvent").as_bool();
		app->puzzleManager->keyPalancas = gameStateFile.child("save_state").child("puzzle").attribute("keyPalancas").as_int();
		app->puzzleManager->keyEscape = gameStateFile.child("save_state").child("puzzle").attribute("keyEscape").as_int();
		app->puzzleManager->keyRescue = gameStateFile.child("save_state").child("puzzle").attribute("keyRescue").as_int();
		app->puzzleManager->BarricadesExplote = gameStateFile.child("save_state").child("puzzle").attribute("BarricadesExplote").as_int();
		app->puzzleManager->RelicsCompleted = gameStateFile.child("save_state").child("puzzle").attribute("RelicsCompleted").as_int();
		app->puzzleManager->DoorsOpened = gameStateFile.child("save_state").child("puzzle").attribute("DoorsOpened").as_int();
		app->puzzleManager->keyDoors = gameStateFile.child("save_state").child("puzzle").attribute("keyDoors").as_bool();
		app->puzzleManager->DoorKey1Opened = gameStateFile.child("save_state").child("puzzle").attribute("DoorKey1Opened").as_bool();
		app->puzzleManager->DoorKey2Opened = gameStateFile.child("save_state").child("puzzle").attribute("DoorKey2Opened").as_bool();
		app->puzzleManager->chickenBoom = gameStateFile.child("save_state").child("puzzle").attribute("chickenBoom").as_bool();
		app->puzzleManager->relics = gameStateFile.child("save_state").child("puzzle").attribute("relics").as_bool();
		app->puzzleManager->keyInvent = gameStateFile.child("save_state").child("puzzle").attribute("keyInvent").as_bool();
		app->puzzleManager->BarricadeExplote1 = gameStateFile.child("save_state").child("puzzle").attribute("BarricadeExplote1").as_bool();
		app->puzzleManager->BarricadeExplote2 = gameStateFile.child("save_state").child("puzzle").attribute("BarricadeExplote2").as_bool();
		app->puzzleManager->BarricadeExplote3 = gameStateFile.child("save_state").child("puzzle").attribute("BarricadeExplote3").as_bool();
		app->puzzleManager->BarricadeExplote4 = gameStateFile.child("save_state").child("puzzle").attribute("BarricadeExplote4").as_bool();
		app->puzzleManager->BarricadeExplote5 = gameStateFile.child("save_state").child("puzzle").attribute("BarricadeExplote5").as_bool();
		app->puzzleManager->Relic1Invent = gameStateFile.child("save_state").child("puzzle").attribute("Relic1Invent").as_bool();
		app->puzzleManager->Relic2Invent = gameStateFile.child("save_state").child("puzzle").attribute("Relic2Invent").as_bool();
		app->puzzleManager->Relic3Invent = gameStateFile.child("save_state").child("puzzle").attribute("Relic3Invent").as_bool();
		app->puzzleManager->RelicInColumn1 = gameStateFile.child("save_state").child("puzzle").attribute("RelicInColumn1").as_bool();
		app->puzzleManager->RelicInColumn2 = gameStateFile.child("save_state").child("puzzle").attribute("RelicInColumn2").as_bool();
		app->puzzleManager->RelicInColumn3 = gameStateFile.child("save_state").child("puzzle").attribute("RelicInColumn3").as_bool();

		quest1->active = gameStateFile.child("save_state").child("quests").attribute("quest1").as_bool();
		quest2->active = gameStateFile.child("save_state").child("quests").attribute("quest2").as_bool();
		quest3->active = gameStateFile.child("save_state").child("quests").attribute("quest3").as_bool();
		quest1->complete = gameStateFile.child("save_state").child("quests").attribute("quest1_Complete").as_bool();
		quest2->complete = gameStateFile.child("save_state").child("quests").attribute("quest2_Complete").as_bool();
		quest3->complete = gameStateFile.child("save_state").child("quests").attribute("quest3_Complete").as_bool();
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
	puzzle.append_attribute("bossIsDead") = app->puzzleManager->bossIsDead;
	puzzle.append_attribute("bossInvent") = app->puzzleManager->bossInvent;

	puzzle.append_attribute("BarricadeExplote") = app->puzzleManager->BarricadesExplote;
	puzzle.append_attribute("RelicsCompleted") = app->puzzleManager->RelicsCompleted;
	puzzle.append_attribute("DoorsOpened") = app->puzzleManager->DoorsOpened;
	puzzle.append_attribute("keyDoors") = app->puzzleManager->keyDoors;
	puzzle.append_attribute("DoorKey1Opened") = app->puzzleManager->DoorKey1Opened;
	puzzle.append_attribute("DoorKey2Opened") = app->puzzleManager->DoorKey2Opened;
	puzzle.append_attribute("chickenBoom") = app->puzzleManager->chickenBoom;
	puzzle.append_attribute("relics") = app->puzzleManager->relics;
	puzzle.append_attribute("keyInvent") = app->puzzleManager->keyInvent;
	puzzle.append_attribute("BarricadeExplote1") = app->puzzleManager->BarricadeExplote1;
	puzzle.append_attribute("BarricadeExplote2") = app->puzzleManager->BarricadeExplote2;
	puzzle.append_attribute("BarricadeExplote3") = app->puzzleManager->BarricadeExplote3;
	puzzle.append_attribute("BarricadeExplote4") = app->puzzleManager->BarricadeExplote4;
	puzzle.append_attribute("BarricadeExplote5") = app->puzzleManager->BarricadeExplote5;
	puzzle.append_attribute("Relic1Invent") = app->puzzleManager->Relic1Invent;
	puzzle.append_attribute("Relic2Invent") = app->puzzleManager->Relic2Invent;
	puzzle.append_attribute("Relic3Invent") = app->puzzleManager->Relic3Invent;
	puzzle.append_attribute("RelicInColumn1") = app->puzzleManager->RelicInColumn1;
	puzzle.append_attribute("RelicInColumn2") = app->puzzleManager->RelicInColumn2;
	puzzle.append_attribute("RelicInColumn3") = app->puzzleManager->RelicInColumn3;

	quests.append_attribute("quest1") = quest1->active;
	quests.append_attribute("quest2") = quest2->active;
	quests.append_attribute("quest3") = quest3->active;
	quests.append_attribute("quest1_Complete") = quest1->complete;
	quests.append_attribute("quest2_Complete") = quest2->complete;
	quests.append_attribute("quest3_Complete") = quest3->complete;

	ret = saveDoc->save_file("save_game_Puzzle_Quest.xml");

	return ret;
}