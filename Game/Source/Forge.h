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
	Forge(unsigned cap = 8);
	~Forge();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void IsForge(int ID0, int ID1);

	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	unsigned cap;
	int y;

	GuiButton* forgeButton;
	GuiButton* exitButton;

private:

};

#endif // __FORGE_H__