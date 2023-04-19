#ifndef __SCENE_H__
#define __SCENE_H__

#include "App.h"

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	int currentHP_Bard = 0;
	int currentHP_Protagonist = 0;
	bool isCharacterLoaded_B = true;

	pugi::xml_node sceneNode;

private:

	const char* lobby_music;
	// Settings
	Settings* pSettings;

	//Pause* pPause = &pauseMenu;

	bool exit_B;

	//Map render image

	SDL_Texture* backGround;
};

#endif // __SCENE_H__