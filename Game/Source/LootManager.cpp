#include "LootManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"


LootManager::LootManager() : Module()
{
	name.Create("lootManager");
}

LootManager::~LootManager()
{
}

bool LootManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Items");
	bool ret = true;

	for (pugi::xml_node pugiNode = config.child("chest"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("chest"))
	{
		if (pugiNode.attribute("used").as_bool() == false)
		{
			Chest* chest = new Chest(pugiNode.attribute("x").as_int(), pugiNode.attribute("y").as_int(), (ChestTypes)pugiNode.attribute("type").as_int(), (ChestDungeon)pugiNode.attribute("dungeon").as_int(), pugiNode.attribute("texturepath").as_string(), pugiNode.attribute("id").as_int());
			chests.push_back(chest);
		}
	}
	
	return ret;
}

bool LootManager::Start()
{
	for (int i=0; i<chests.size(); i++)
	{
		chests[i]->Start();
	}
	return true;
}

bool LootManager::Update(float dt)
{
	for (int i = 0; i < chests.size(); i++)
	{
		chests[i]->Update(dt);
	}
	return true;
}

bool LootManager::CleanUp()
{
	chests.clear();

	return true;
}

bool LootManager::SaveLootState()
{

	return true;
}