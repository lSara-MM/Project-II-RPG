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

	LoadLootState(config);

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

bool LootManager::CleanUp()
{

	return true;
}

bool LootManager::LoadLootState(pugi::xml_node& xml_trees)
{

	return true;
}

bool LootManager::SaveLootState()
{

	return true;
}