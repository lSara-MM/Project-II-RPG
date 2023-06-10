#ifndef __STORE_H__
#define __STORE_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "GuiButton.h"
#include "Combat.h"

#include "Box2D/Box2D/Box2D.h"

#include <vector>
using namespace std;

// Module --------------------------------------
class Store : public Module
{
public:

	// Constructors & Destructors
	Store(unsigned cap = 5);
	~Store();

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

	SDL_Texture* inventory;
	SDL_Texture* potion;

	unsigned cap;
	int y;

	GuiButton* buyButton;
	GuiButton* closeStore;
	GuiButton* Add;
	GuiButton* Minus;

	Tween storeAnimation;//animacion tienda
	bool storeTransition_B;//controlar animacion tienda
	int posYstoreAnimation;//hacer animacion salida tienda

private:

	int SellQuantity = 0;
	int CurrentPrice = 0;

	const char* fxbuypath;
	uint buyfx;

};

#endif // __STORE_H__