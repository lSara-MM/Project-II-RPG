#ifndef __MENUS_H__
#define __MENUS_H__

#include "App.h"

#include "Module.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"
#include "Tweening.h"

struct SDL_Texture;

class Menus : public Module
{
public:

	Menus();

	// Destructor
	virtual ~Menus();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	// Settings
	Settings* pSettings;
	Pause* pPause;

	const char* pause_music;
	const char* fxpausepath;
	uint pausefx;

	bool settings_B;
	bool pause_B;

	bool menuOn;
private:

	float deltaTime;
	bool exit_B;
};

#endif // __MENUS_H__