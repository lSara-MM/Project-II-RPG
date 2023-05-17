#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Render.h"
#include "Scene.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "Inventory.h"
#include "Player.h"
#include "Character.h"
#include "Combat.h"

#include <vector>
using namespace std;

class ItemNode
{
public:
	ItemNode() {};
	~ItemNode() {};

	bool Start() 
	{
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, app->inventory, { 0, 0, 0, 0 }, ButtonType::SMALL); 
		return true; 
	}

public:
	SString name;
	SString path;

	int quantity = 0;
	int type = 0;
	int kind = 0;
	int hp = 0;
	int maxhp = 0;
	int attack = 0;
	int critProbability = 0;
	int critDamage = 0;
	int precision = 0;
	int armor = 0;
	int esquiva = 0;
	int resistencia = 0;
	int speed = 0;
	bool equiped = false;
	int max = 0;

	int whom = 0;

	int space = 0;

	int ID = 0;

	GuiButton* button;

	bool CleanUp()
	{
		if (button != nullptr)
		{
			delete(button);
			button = nullptr;
		}

		return true;
	}
};

class ItemManager : public Module
{
public:
	ItemManager();
	~ItemManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	int LoadItems();
	void AddQuantity(pugi::xml_node& xml_trees, const char* name);

	void LoadNodes(pugi::xml_node& xml_trees, ItemNode* item);
	void LoadQuantity(int x, int y, int i);
	void LoadButtons(int x, int y, int ID);
	void MinusQuantity(const char* name);

	void UseItem(ItemNode* item);

	bool LoadItemState(pugi::xml_node& xml_trees);
	bool SaveItemState();

	void AddCharaToParty(SString chara);
	void LoadAllPC();
	void SetParty();

public:

	ItemNode* tree = new ItemNode;

	SDL_Texture* itemsTexture;

	const char* texturePath;

	vector <ItemNode*> nodeList;

	array<Character*, 4> arrParty;
	vector<Character*> vecPC;

	int invPos = 0;

private:
	pugi::xml_document items;

public:
	//print stats
	int maxhp = 0;
	int attack= 0;
	int critProbability= 0;
	int critDamage= 0;
	int precision= 0;
	int armor= 0;
	int esquiva= 0;
	int resistencia= 0;
	int speed= 0;
	

	// TO DO GUARRADA SUPER TEMPORAL
	int comb = 0;
};

#endif // __ITEMMANAGER_H__
