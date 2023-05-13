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

	//Load / Save methods
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	pugi::xml_node IntroSaveNode;
	pugi::xml_node IntroLoadNode;

	bool previousGame_B = false;
	bool continueGame_B = false;

	int mouseX_intro, mouseY_intro;
	float mouse_Speed;

	Settings* pSettings;

private:
	const char* music_intro;

	SDL_Texture* texture;
	const char* texturePath;

	// buttons
	List<GuiButton*> listButtons;
	const char* buttons[6] = { "Play", "Continue", "Settings", "Exit", "\n" };
	Tween animationTitle;
	Tween animationBackground;
	bool transition_B;

	bool introDone = false;
	bool exit_B;
};

#endif // __INTROSCENE_H__