#ifndef __PuzzleManager_H__
#define __PuzzleManager_H__

#include "App.h"

#include "Module.h"
#include "Input.h"
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
	bool Dun1Awake(pugi::xml_node& config);
	bool Dun2Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();
	bool Dun1Start();
	bool Dun2Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);
	bool Dun1Update();
	bool Dun2Update();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	bool Dun1CleanUp();
	bool Dun2CleanUp();

	bool Palancas();

	bool Escape();

	bool Rescue();

	bool TeamMate();

public:

	pugi::xml_node sceneNode;

	int keyPalancas;
	int keyEscape;
	int keyRescue;

	int widthVertical;
	int widthHoritzontal;
	int widthPalanca;
	int widthPalancaSens;
	int widthNotas;
	int widthDoorEscape;
	int widthBoss;
	int widthLoset;
	int widthFireGuy;

	int heightVertical;
	int heightHoritzontal;
	int heightPalanca;
	int heightPalancaSens;
	int heightNotas;
	int heightDoorEscape;
	int heightBoss;
	int heightLoset;
	int heightFireGuy;

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
	iPoint posFireGuy;

	SDL_Texture* door;
	SDL_Texture* palanca;
	SDL_Texture* notas;
	SDL_Texture* doorEscape;
	SDL_Texture* boss;
	SDL_Texture* bossDeath;
	SDL_Texture* loset;
	SDL_Texture* fireGuy;
	SDL_Texture* textureE;

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
	PhysBody* FireGuy;

	bool palancas;
	bool escape;
	bool rescue;
	bool teamMate;

	bool esc1;
	bool esc2;
	bool esc3;

	bool intoCode;
	bool codeActive;

	bool palancasActive;

	bool bossActive;
	bool bossInvent;
	bool bossIsDead;
	bool losetActive;
	bool fightBoss = false;

	bool saveFireGuy;

	string code;
	string codeToCompare;
	string realCode;

	PlayerInput* numCode;

	SDL_Rect los;

	SDL_Rect bosDeath;

private:

	const char* texturepathDoor;
	const char* texturepathPalanca;
	const char* texturepathNotas;
	const char* texturepathDoorEscape;
	const char* texturepathBoss;
	const char* texturepathBossDeath;
	const char* texturepathLoset;
	const char* texturepathFireGuy;

	//fx
	const char* palancaPath;
	uint palancafx;

	const char* confirmPath;
	uint confirmInteractfx;

	const char* solvedpath;
	uint solvedfx;

};

#endif // __SCENE_H__
