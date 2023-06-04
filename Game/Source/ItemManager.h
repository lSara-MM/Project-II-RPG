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
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, app->inventory, { 0, 0, 0, 0 }, ButtonType::ITEM); 
		return true; 
	}

public:
	SString name;

	iPoint position;
	int quantity = 0;
	int type = 0;
	int kind = 0;
	int hp = 0;
	int maxhp = 0;
	int attack = 0;
	int critRate = 0;
	int critDamage = 0;
	int accuracy = 0;
	int armor = 0;
	int dodge = 0;
	int res = 0;
	int speed = 0;
	int max = 0;
	int price = 0;

	int whom = 0;

	int space = 0;
	int x = 0;
	int y = 0;

	int ID = 0;

	bool equiped = false;
	bool craft = false;
	bool toSell = false;

	bool printStats = false;

	GuiButton* button = nullptr;

	bool CleanUp()
	{
		app->guiManager->DestroyGuiControl(button);
		button = nullptr;
		printStats = false;
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
	bool PostUpdate();
	bool CleanUp();

	int LoadItems();
	void AddQuantity(int id, int quantity);

	void LoadNodes(pugi::xml_node& xml_trees, ItemNode* item);
	void LoadQuantity(int x, int y, ItemNode* item);
	void LoadButtons(int x, int y, ItemNode* item);
	void MinusQuantity(ItemNode* item);
	void ItemToSell(ItemNode* item);

	void LoadArmorItmes();

	void UseItem(ItemNode* item);

	bool LoadItemState(pugi::xml_node& xml_trees);
	bool SaveItemState();

	void AddCharaToParty(int id);
	void LoadAllPC();
	bool LoadParty();
	void SetParty();
	void PartyToNull();

	//Store
	void LoadSellItems(int x, int y, ItemNode* item);
	void LoadStoreItems(int x, int y, ItemNode* item);
	void LoadStoreButtons(int x, int y, ItemNode* item);

public:

	ItemNode* tree = new ItemNode;

	SDL_Texture* itemsTexture;
	SDL_Texture* coinTexture;
	SDL_Texture* SmallcoinTexture;

	SString itemPath;

	const char* texturePath;
	const char* textureInventoryPath;
	const char* partyPath;
	const char* charLockedPath;
	const char* protaPath;
	const char* compaPath;
	const char* twinsPath;
	const char* firePath;
	const char* texturePotionsPath;
	const char* coinPath;
	const char* smallcoinPath;

	vector <ItemNode*> nodeList;
	vector <ItemNode*> armorItems;

	array<Character*, 4> arrParty;
	vector<Character*> vecPC;

	int invPos = 0;
	bool loadParty_B;

public:
	//print stats
	int maxhp = 0;
	int attack = 0;
	int critProbability = 0;
	int critDamage = 0;
	int accuracy = 0;
	int armor = 0;
	int esquiva = 0;
	int resistencia = 0;
	int speed = 0;

	int coins = 0;

	// TO DO GUARRADA SUPER TEMPORAL
	int comb = 0;

private:
	pugi::xml_document items;
};

#endif // __ITEMMANAGER_H__
