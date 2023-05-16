#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "App.h"

#include "Module.h"
#include "Player.h"
#include "Character.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Settings.h"

struct SDL_Texture;

class Combat : public Module
{
public:

	Combat();

	// Destructor
	virtual ~Combat();

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

	// Init everything
	bool PreLoadCombat(array<Character*, 4> arrParty_, SString n, int boss = -1);
	bool InitEnemies(vector<int> arr);
	bool InitAllies(array<Character*, 4> party);


	bool StartCombat();
	bool OrderBySpeed();
	bool NextTurn(); //Lo enviaran los characthers cuando finalicen su turno

	// Handle buttons
	void HandleCharaButtons(vector<Character*>* arr, int pos1 = -1, int pos2 = -1);	// If -1, disable all
	bool HandleSkillsButtons(Character* chara);

	void MoveCharacter(vector<Character*>* arr, Character* chara, int newPosition_I);
	void RemoveCharacter(vector<Character*>* arr, Character* chara);
	void UpdatePositions(vector<Character*>* arr, int pos = 0);

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);
	bool OnGuiMouseHoverEvent(GuiControl* control);
	int SearchInSkills(vector<Character*> arr, Character* chara);
	int SearchInVec(vector<Character*> arr, int id);

	// Save/Load
	bool SaveCombat(); //guardar stats playable characters en xml combat
	bool LoadCombat(); //cargar stats playable characters desde xml combat
	bool RestartCombatData(); //al empezar partida nueva, poner stats xml combat igual a xml config
public:
	//Player* player;
	bool frcap;

	//Turn Order
	List<Character*> listInitiative;
	int charaInTurn = 0; //Empieza en 0, max 7

	vector<Character*> vecAllies;
	vector<Character*> vecEnemies;

	Character* targeted_Character = nullptr; //Last button selected
	int lastPressedAbility_I = -1;

	bool firstCombat_B = false;//determinar si es la primera vez que se lucha en la partida, en true significa que no se ha luchado todavia

	// buttons
	List<GuiButton*> listButtons;
private:

	bool isMoving;

	//
	int mouseX_combat, mouseY_combat;
	float mouse_Speed;

	pugi::xml_node combatNode;

	//Fondo
	const char* texturePathBackground;
	SDL_Texture* textureBackground;

	const char* texturePathTurnsBar;
	SDL_Texture* textureTurnsBar;

	const char* texturePathTargetButton;
	SDL_Texture* textureTargetButton;

	const char* PathlastSelectedSkill;
	SDL_Texture* textureLastSelectedSkill;

	SDL_Texture* profileTex;//ui profile character
	SDL_Texture* skillTex;//ui skill character
	

	bool exit_B;

	// Scene transition
	array<Character*, 4> arrAuxParty;
	SString sceneFromName;
	vector<int> arrSetEnemies;

	//music combat
	const char* musCombat;

	//fx
	const char* swapPath;
	uint swapPositionfx;
};

#endif // __COMBAT_H__