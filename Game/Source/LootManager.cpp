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

	//texturePath = config.attribute("inventorypath").as_string();

	return ret;
}

bool LootManager::Start()
{

	return true;
}

bool LootManager::Update(float dt)
{

	return true;
}

bool ItemManager::CleanUp()
{

	return true;
}