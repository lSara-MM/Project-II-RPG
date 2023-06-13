#include "LootManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "IntroScene.h"
#include "Window.h"


LootManager::LootManager() : Module()
{
	name.Create("lootManager");
}

LootManager::~LootManager()
{}

bool LootManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Items");
	bool ret = true;
	pugi::xml_node& data = config;
	chests.clear();
	for (pugi::xml_node pugiNode = config.child("chest"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("chest"))
	{
		if (pugiNode.attribute("used").as_bool() == false)
		{
			Chest* chest = new Chest(pugiNode.attribute("x").as_int(), pugiNode.attribute("y").as_int(), (ChestTypes)pugiNode.attribute("type").as_int(), (ChestDungeon)pugiNode.attribute("dungeon").as_int(), pugiNode.attribute("texturepath").as_string(), pugiNode.attribute("id").as_int());
			chests.push_back(chest);
		}
	}
	if (app->iScene->continueGame_B)
	{
		LoadState();
	}
	else
	{
		SaveState(config);
	}
	return ret;
}

bool LootManager::Start()
{
	for (int i = 0; i < chests.size(); i++)
	{
		if ((chests[i]->used == false && app->BeastT->active && chests[i]->dungeon == ChestDungeon::BEASTS) || (chests[i]->used == false && app->hTerrors->active && chests[i]->dungeon == ChestDungeon::TERRORS))
		{
			chests[i]->Start();
		}
	}
	return true;
}

bool LootManager::Update(float dt)
{
	for (int i = 0; i < chests.size(); i++)
	{
		if ((chests[i]->used == false && app->BeastT->active && chests[i]->dungeon == ChestDungeon::BEASTS) || (chests[i]->used == false && app->hTerrors->active && chests[i]->dungeon == ChestDungeon::TERRORS))
		{
			chests[i]->Update(dt);
			if ((chests[i]->dungeon == ChestDungeon::BEASTS && app->BeastT->active == false) || (chests[i]->dungeon == ChestDungeon::TERRORS && app->hTerrors->active == false))
			{
				chests[i]->CleanUp();
			}
		}
	}
	return true;
}

bool LootManager::CleanUpDos()
{
	chests.clear();

	return true;
}

bool LootManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node chest;
	// save items
	for (int i = 0; i < chests.size(); i++)
	{
		chest = data.append_child("chest");
		chest.append_attribute("used") = chests[i]->used;
	}
	return true;
}

bool LootManager::LoadState()
{
	bool ret = true;
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		pugi::xml_node xml_trees = gameStateFile.first_child().child("lootManager");
		int i = 0;
		for (pugi::xml_node pugiNode = xml_trees.first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("chest"))
		{
			chests[i]->used = pugiNode.attribute("used").as_bool();
			i++;
		}
	}
	return true;
}