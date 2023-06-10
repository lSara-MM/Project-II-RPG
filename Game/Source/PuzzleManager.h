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

	bool KeyDoorsPuz();
	bool ChickenBoomPuz();
	bool RelicsPuz();

	void RestartTimer();

public:

	pugi::xml_node sceneNode;

	//Contadores para puzzles
	int keyPalancas;
	int keyEscape;
	int keyRescue;

	int BarricadesExplote;
	int RelicsCompleted;
	int DoorsOpened;

	//Tamaños de width para objetos
	int widthVertical;
	int widthHoritzontal;
	int widthPalanca;
	int widthPalancaSens;
	int widthNotas;
	int widthDoorEscape;
	int widthBoss;
	int widthLoset;
	int widthFireGuy;

	int widthDoorKeys;
	int widthKey;
	int widthChicken;
	int widthBomb;
	int widthBarricade1;
	int widthBarricade2;
	int widthRelic;
	int widthRelicColumn;
	int widthBoss2;

	//Tamaños de height para objetos
	int heightVertical;
	int heightHoritzontal;
	int heightPalanca;
	int heightPalancaSens;
	int heightNotas;
	int heightDoorEscape;
	int heightBoss;
	int heightLoset;
	int heightFireGuy;

	int heightDoorKeys;
	int heightKey;
	int heightChicken;
	int heightBomb;
	int heightBarricade1;
	int heightBarricade2;
	int heightRelic;
	int heightRelicColumn;
	int heightBoss2;

	//Posiciones de objetos
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

	iPoint posDoorkey1;
	iPoint posDoorkey2;
	iPoint posDoorBoss;
	iPoint posKey;
	iPoint posChicken1;
	iPoint posChicken2;
	iPoint posBomb1;
	iPoint posBomb2;
	iPoint posBarricade1;
	iPoint posBarricade2;
	iPoint posBarricade3;
	iPoint posBarricade4;
	iPoint posBarricade5;
	iPoint posRelic1;
	iPoint posRelic2;
	iPoint posRelic3;
	iPoint posRelicColumn1;
	iPoint posRelicColumn2;
	iPoint posRelicColumn3;
	iPoint posBoss2;

	//Texturas para objetos
	SDL_Texture* door;
	SDL_Texture* palanca;
	SDL_Texture* notas;
	SDL_Texture* doorEscape;
	SDL_Texture* boss;
	SDL_Texture* bossDeath;
	SDL_Texture* loset;
	SDL_Texture* fireGuy;
	SDL_Texture* textureE;
	
	SDL_Texture* GeneralTextureDungeon2;
	SDL_Texture* Boss2Texture;

	//Colliders para objetos
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

	PhysBody* DoorKey1;
	PhysBody* DoorKey2;
	PhysBody* DoorBossP;
	PhysBody* keySensor;

	PhysBody* Chicken1;
	PhysBody* Chicken2;
	PhysBody* Bomb1;
	PhysBody* Bomb2;
	PhysBody* Barricade1;
	PhysBody* Barricade2;
	PhysBody* Barricade3;
	PhysBody* Barricade4;
	PhysBody* Barricade5;

	PhysBody* relic1;
	PhysBody* relic2;
	PhysBody* relic3;
	PhysBody* relicColumn1;
	PhysBody* relicColumn2;
	PhysBody* relicColumn3;

	PhysBody* boss2;

	//Bools para ver si se han completado puzzles o trozos de ellos
	bool palancas;
	bool escape;
	bool rescue;
	bool teamMate;

	bool keyDoors;
	bool chickenBoom;
	bool relics;

	bool esc1;
	bool esc2;
	bool esc3;

	bool intoCode;
	bool codeActive;

	bool palancasActive;

	bool bossSpectreActive_B;
	bool bossInvent;
	bool bossIsDead;
	bool bossIsDeadDun2;
	bool bossDun2Contact;
	bool losetActive;
	bool fightBoss1;
	bool fightBoss2;

	bool saveFireGuy;

	bool DoorContact1;
	bool DoorContact2;
	bool DoorKey1Opened;
	bool DoorKey2Opened;
	bool keyInvent;
	bool keySens;

	bool BarricadeContact1;
	bool BarricadeContact2;
	bool BarricadeContact3;
	bool BarricadeContact4;
	bool BarricadeContact5;
	bool BarricadeExplote1;
	bool BarricadeExplote2;
	bool BarricadeExplote3;
	bool BarricadeExplote4;
	bool BarricadeExplote5;
	bool BarricadeHasExploted1;
	bool BarricadeHasExploted2;
	bool BarricadeHasExploted3;
	bool BarricadeHasExploted4;
	bool BarricadeHasExploted5;
	bool BombContact1;
	bool BombContact2;
	bool BombCarryOn1;
	bool BombCarryOn2;
	bool BombPlant1;
	bool BombPlant2;

	bool RelicContact1;
	bool RelicContact2;
	bool RelicContact3;
	bool Relic1Invent;
	bool Relic2Invent;
	bool Relic3Invent;
	bool RelicColumnContact1;
	bool RelicColumnContact2;
	bool RelicColumnContact3;	
	bool RelicInColumn1;
	bool RelicInColumn2;
	bool RelicInColumn3;

	bool CurrentDarkFade;

	//Variables para meter codigos
	string code;
	string codeToCompare;
	string realCode;

	PlayerInput* numCode;

	//Coordenadas para situar texturas
	SDL_Rect los;
	SDL_Rect bosDeath;

	SDL_Rect KeyD1;
	SDL_Rect KeyD2;
	SDL_Rect BossD;
	SDL_Rect Ky;
	SDL_Rect Chick;
	SDL_Rect Bmb;
	SDL_Rect Barr1;
	SDL_Rect Barr2;
	SDL_Rect Rel1;
	SDL_Rect Rel2;
	SDL_Rect Rel3;

	uint32 mTicks;
	uint32 mStartTicks;
	uint32 DeltaTime;

	SDL_Texture* currentDark;

private:

	const char* texturepathDoor;
	const char* texturepathPalanca;
	const char* texturepathNotas;
	const char* texturepathDoorEscape;
	const char* texturepathBoss;
	const char* texturepathBossDeath;
	const char* texturepathLoset;
	const char* texturepathFireGuy;

	const char* texturepathPuzzleDungeon2;
	const char* texturepathBossDun2;

	//win dungeon
	const char* texturepathWintext;
	SDL_Texture* winText;
	Tween winAnimation;
	int posYanimation_I;
	bool completeDungeon_B;//no se si hay bool que controle que se ha terminado la dungeon, lo pongo para iniciar animacion pantalla vicotria

	//fx
	const char* palancaPath;
	uint palancafx;

	const char* confirmPath;
	uint confirmInteractfx;

	const char* solvedpath;
	uint solvedfx;

};

#endif // __SCENE_H__
