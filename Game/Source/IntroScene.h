#ifndef __INTROSCENE_H__
#define __INTROSCENE_H__

#include "App.h"
#include "Module.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"

struct SDL_Texture;

class IntroScene : public Module
{
public:

	IntroScene();

	// Destructor
	virtual ~IntroScene();

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

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

private:
	const char* music_intro;

	// buttons
	List<GuiButton*> listButtons;
	const char* buttons[6] = { "Play", "Continue", "Settings", "Credits", "Exit", "\n" };
	int bNum;
	
	Settings* pSettings;

	bool exit_B;
};

#endif // __INTROSCENE_H__