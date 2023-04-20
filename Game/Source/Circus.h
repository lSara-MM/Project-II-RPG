#ifndef __CIRCUS_H__
#define __CIRCUS_H__

#include "App.h"

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"

struct SDL_Texture;

class Circus : public Module
{
public:

	Circus();

	// Destructor
	virtual ~Circus();

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

	pugi::xml_node sceneNode;

private:

	//Music path
	const char* circusMusPath;

	// Settings
	Settings* pSettings;
	Pause* pPause;

	bool exit_B;
};
#endif // __CIRCUS_H__