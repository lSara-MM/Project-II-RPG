#ifndef __BEASTTENT_H__
#define __BEASTTENT_H__

#include "App.h"

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Menus.h"

struct SDL_Texture;

class BeastTent : public Module
{
public:

	BeastTent();

	// Destructor
	virtual ~BeastTent();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Debug();
	bool InitEntities();
public:

	Player* player;
	bool frcap_B = true;

	pugi::xml_node sceneNode;

	//enter combat;
	int steps_I;

	SDL_Texture* BeastDungeon;

	const char* enterCombatpath;
	uint combatfx;
	const char* musBeastPath;

private:
	//Music path
	const char* texturePathBeastDungeon;
};

#endif // __HOUSEOFTERRORS_H__