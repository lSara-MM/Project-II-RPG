#ifndef __ITEMMANAGER_H__
#define __ITEMMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Render.h"

#include <vector>
using namespace std;

class ItemNode
{
public:
	ItemNode() {};
	~ItemNode() {};

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

	ItemNode* tree = new ItemNode;

	const char* textItem_path;

	vector <ItemNode*> nodeList;
	vector <ItemNode*> itemCount;

private:
	pugi::xml_document items;
};

#endif // __ITEMMANAGER_H__
