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
	Inventory(unsigned capx = 5, unsigned capy = 6);
	~Inventory();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool OnGuiMouseHoverEvent(GuiControl* control);
	bool OnGuiMouseOutHoverEvent(GuiControl* control);
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	SDL_Texture* inventoryIMG;
	SDL_Texture* partyIMG;
	SDL_Texture* charlockedIMG;
	SDL_Texture* protaIMG;
	SDL_Texture* compaIMG;
	SDL_Texture* twinsIMG;
	SDL_Texture* fireIMG;
	GuiButton* buttonInventory;
	GuiButton* buttonParty;
	GuiButton* PrevPage;
	GuiButton* NextPage;
	unsigned capx;
	unsigned capy;
	int page = 0;
	int y;

	GuiButton* selectCharacter[4];

	Tween inventoryAnimation;//animacion inventario
	bool inventoryTransition_B;//controlar animacion inventario
	int posXinventoryAnimation;//prueba para hacer animacion salida inventario.
	bool partyWindow_B;//qué party se muestra
};

#endif // __ITEMMANAGER_H__