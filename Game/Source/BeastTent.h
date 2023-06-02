#ifndef __BEASTTENT_H__
#define __BEASTTENT_H__

#include "App.h"

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"

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

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	int npcSetID;

	Player* player;
	bool frcap_B = true;
	bool pause_B = false;
	bool settings_B = false;
	bool mute_B = true;

	bool combatEnd = false;

	pugi::xml_node sceneNode;

	//enter combat;
	int steps_I;

	SDL_Texture* BeastDungeon;

	const char* enterCombatpath;
	uint combatfx;

private:
	int mouseX_pos, mouseY_pos;
	//Music path
	const char* musBeastPath;

	const char* pause_music;

	const char* texturePathBeastDungeon;

	// Settings
	Settings* pSettings;
	Pause* pPause;

	float mouseSpeed;

	const char* fxpausepath;
	uint pausefx;

	bool exit_B;
};

#endif // __HOUSEOFTERRORS_H__