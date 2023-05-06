#ifndef __PuzzleManager_H__
#define __PuzzleManager_H__

#include "App.h"

#include "Module.h"
#include "Combat.h"

#include "Point.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"
#include "Physics.h"

#include "SString.h"
#include <vector>

#include "Settings.h"

struct SDL_Texture;

class PuzzleManager : public Module
{
public:

	PuzzleManager();

	// Destructor
	virtual ~PuzzleManager();

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

	bool Palancas();

	bool Escape();

	bool Rescue();

	bool CodeInput();

public:

	pugi::xml_node sceneNode;

	int keyPalancas;
	int keyEscape;
	int keyRescue;

	int widthDoor;
	int widthPalanca;
	int widthPalancaSens;
	int widthNotas;
	int widthDoorEscape;
	int widthBoss;
	int widthLoset;

	int heightDoor;
	int heightPalanca;
	int heightPalancaSens;
	int heightNotas;
	int heightDoorEscape;
	int heightBoss;
	int heightLoset;

	iPoint posDoor1;
	iPoint posDoor2;
	iPoint posDoor3;
	iPoint posPalancas;
	iPoint posNotas1;
	iPoint posNotas2;
	iPoint posNotas3;
	iPoint posDoorEscape;
	iPoint posBoss;
	iPoint posLoset;

	SDL_Texture* Door;
	SDL_Texture* palanca;
	SDL_Texture* palancaSens;
	SDL_Texture* notas;
	SDL_Texture* doorEscape;
	SDL_Texture* boss;
	SDL_Texture* loset;

	PhysBody* Door1;
	PhysBody* Door2;
	PhysBody* Door3;
	PhysBody* DoorEscape;
	PhysBody* Palanca;
	PhysBody* PalancaSensor;
	PhysBody* nota1;
	PhysBody* nota2;
	PhysBody* nota3;
	PhysBody* DoorEscapeSensor;
	PhysBody* Boss;
	PhysBody* Loset;

	bool palanc;
	bool escape;
	bool rescue;

	bool esc1;
	bool esc2;
	bool esc3;

	bool codeActive;

	bool bossActive;
	bool bossInvent;
	bool losetActive;

	SString code;
	SString codeToCompare;
	SString realCode;

private:

	const char* texturepathDoor;
	const char* texturepathPalanca;
	const char* texturepathPalancaSens;
	const char* texturepathNotas;
	const char* texturepathDoorEscape;
	const char* texturepathBoss;
	const char* texturepathLoset;

};

#endif // __SCENE_H__
