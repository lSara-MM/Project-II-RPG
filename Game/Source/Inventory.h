#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "GuiButton.h"
#include "Combat.h"

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

public:

	SDL_Texture* inventoryIMG;

	unsigned cap;
	int y;

	GuiButton* selectCharacter[4];

	Tween inventoryAnimation;//animacion inventario
	bool inventoryTransition_B;//controlar animacion inventario
};

#endif // __ITEMMANAGER_H__