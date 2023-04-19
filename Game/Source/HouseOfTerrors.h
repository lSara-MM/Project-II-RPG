#ifndef __HOUSEOFTERRORS_H__
#define __HOUSEOFTERRORS_H__

#include "App.h"

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"

struct SDL_Texture;

class HouseOfTerrors : public Module
{
public:

	HouseOfTerrors();

	// Destructor
	virtual ~HouseOfTerrors();

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

	Player* player;
	bool frcap_B = true;
	bool pause_B;
	bool mute_B = true;

	pugi::xml_node sceneNode;

private:

	//Music
	const char* musHauntedPath;

	// Settings
	Settings* pSettings;

	//Pause* pPause = &pauseMenu;

	bool exit_B;

};

#endif // __HOUSEOFTERRORS_H__