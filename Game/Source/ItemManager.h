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

#include <vector>
using namespace std;

class ItemNode
{
public:
	ItemNode() {};
	~ItemNode() {};

public:
	SString name;
	SString path;

	int quantity;
	int type;
	int kind;
	int hp;
	int maxhp;
	int attack;
	int critProbability;
	int critDamage;
	int precision;
	int armor;
	int esquiva;
	int resistencia;
	int speed;
	bool equiped = false;
	int max;

	int whom=0;

	int space=0;

	int ID;

	GuiButton* button;

	bool CleanUp()
	{
		//RELEASE(button);
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

	void SetPlayerForScene(Player* player_)
	{
		player = player_;
		player->LoadAllPC();
		player->SetParty();
	}


public:

	ItemNode* tree = new ItemNode;

	SDL_Texture* itemsTexture;

	const char* texturePath;

	vector <ItemNode*> nodeList;

	Player* player;

	int invPos;

private:
	pugi::xml_document items;

public:
	//print stats
	int maxhp;
	int attack;
	int critProbability;
	int critDamage;
	int precision;
	int armor;
	int esquiva;
	int resistencia;
	int speed;
	
};

#endif // __ITEMMANAGER_H__
