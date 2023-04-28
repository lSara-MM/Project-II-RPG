#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

#include "ItemManager.h"

#include "App.h"
#include "Module.h"

class ItemNode
{
public:
	ItemNode() {};
	~ItemNode() {};

	void CleanUp();

public:
	SString name;

	int quantity;
	int type;
	int kind;
	int hp;
	int attack;
	int critProbability;
	int critDamage;
	int precision;
	int armor;
	int esquiva;
	int resistencia;
	int speed;
	
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
	ItemNode* LoadNodes(pugi::xml_node& xml_trees, ItemNode* item);
	ItemNode* LoadQuantity(pugi::xml_node& xml_trees, ItemNode* item);

	bool LoadItemState();
	bool SaveItemState();

public:

	vector <ItemNode*> nodeList;
	vector <ItemNode*> itemCount;

	SDL_Texture* textItem_tex;
	const char* textItem_path;

private:
	pugi::xml_document items;
};

#endif // __ITEMMANAGER_H__
