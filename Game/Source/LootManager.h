#ifndef __LOOTMANAGER_H__
#define __LOOTMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Render.h"
#include "Scene.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "ItemManager.h"
#include "Player.h"
#include "Character.h"
#include "Combat.h"

#include <vector>
using namespace std;

class Chest
{
public:
	Chest() {};
	~Chest() {};
};

class LootManager : public Module
{
public:
	LootManager();
	~LootManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool LoadLootState(pugi::xml_node& xml_trees);
	bool SaveLootState();

private:

	pugi::xml_node& saveLoot;
};

#endif // __LOOTMANAGER_H__
