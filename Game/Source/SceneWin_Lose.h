#ifndef __SCENEWINLOSE_H__
#define __SCENEWINLOSE_H__

#include "App.h"

#include "Module.h"
#include "Combat.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"
#include "Tweening.h"

struct SDL_Texture;

class SceneWin_Lose : public Module
{
public:

	SceneWin_Lose();

	// Destructor
	virtual ~SceneWin_Lose();

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

public:

	pugi::xml_node sceneNode;

	bool frcap_B = true;
	bool mute_B = true;

	bool win; // True - win / False - lose

	SDL_Texture* Win;
	SDL_Texture* Lose;

private:

	int mouseX_scene, mouseY_scene;
	float mouseSpeed;
	const char* lobby_music;

	const char* texturepathWin;
	const char* texturepathLose;

	bool transition_B;
	Tween backgroundAnimation;

	//Music
	const char* winMusicPath;
	const char* looseMusicPath;

	bool exit_B;
};

#endif // __SCENE_H__