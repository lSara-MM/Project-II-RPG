#ifndef __FORGE_H__
#define __FORGE_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "GuiButton.h"
#include "Combat.h"

#include "Box2D/Box2D/Box2D.h"

#include <vector>
using namespace std;

// Module --------------------------------------
class Forge : public Module
{
public:

	// Constructors & Destructors
	Forge(unsigned cap = 5);
	~Forge();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool IsForge(int ID0, int ID1, bool armor);

	bool OnGuiMouseHoverEvent(GuiControl* control);
	bool OnGuiMouseOutHoverEvent(GuiControl* control);
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	unsigned cap;
	int y;
	int forgePos = 0;
	bool toDelete = false;
	bool IsArmor = false;

	GuiButton* forgeButton;
	GuiButton* exitButton;

	SDL_Texture* forgeTexture;
	SDL_Texture* forgeInventoryTexture;

	Tween forgeAnimation;//animacion tienda
	bool forgeTransition_B;//controlar animacion tienda
	int posYforgeAnimation;//hacer animacion salida tienda

public:
	pugi::xml_document crafts;
private:
	const char* fxforgepath;
	uint forgefx;
};

#endif // __FORGE_H__