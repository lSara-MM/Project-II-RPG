#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Module.h"
#include "Entity.h"
#include "GuiButton.h"

#include "Box2D/Box2D/Box2D.h"

#include <vector>
using namespace std;

// Module --------------------------------------
class Inventory : public Module
{
public:

	// Constructors & Destructors
	Inventory(unsigned cap = 8);
	~Inventory();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

	vector <GuiButton*> use;

public:

	SDL_Texture* itemsTexture;

	unsigned cap;
	int y;

};

#endif // __ITEMMANAGER_H__