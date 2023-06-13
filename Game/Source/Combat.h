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

enum CombatType
{
	DUMMY = -1,
	ENEMIES,
	BOSS
};

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
	bool PreLoadCombat(SString n, int boss = -1, int boss2 = -1);
	bool InitEnemies(vector<int> arr);
	bool InitAllies(array<Character*, 4> party);

	bool StartCombat();
	bool OrderBySpeed();
	bool NextTurn(); //Lo enviaran los characthers cuando finalicen su turno
	bool Flee();

	// Handle buttons
	void HandleCharaButtons(vector<Character*>* arr, int pos1 = -1, int pos2 = -1);	// If -1, disable all
	bool HandleSkillsButtons(Character* chara);

	void MoveCharacter(Character* chara, int newPosition_I);
	void RemoveCharacter(vector<Character*>* arr, Character* chara);
	// Take array and change position in combat (positionCombat_I) and texture position to 
	// its position in the array
	void UpdatePositions(vector<Character*>* arr, int pos = 0);

	// Getters
	int SearchInSkills(vector<Character*> arr, Character* chara);
	int SearchInVecID(vector<Character*> arr, int CharaId);
	int SearchInVec(vector<Character*> arr, int buttonId);

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);
	bool OnGuiMouseHoverEvent(GuiControl* control);
	bool OnGuiMouseOutHoverEvent(GuiControl* control);

	// Render GUI
	void RenderGuiChara(int charaID);
	void RenderSkillDescription(int controlID);	

	// Handle character's stats at the end of the combat
	void HandleEndCombat();

	// Save/Load
	bool SaveCombat(); //guardar stats playable characters en xml combat
	bool LoadCombat(); //cargar stats playable characters desde xml combat
	bool LoadCombat(vector<Character*>* vec); //cargar stats playable characters desde xml combat
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

	// buttons
	List<GuiButton*> listButtons;

private:
	bool isMoving;
	CombatType combatType;

	pugi::xml_node combatNode;

	// Auxiliar vector for texts
	vector<SString> auxTexts;
	int fontSizeSkills;
	int xText1;

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
	SDL_Texture* bookTex;//book combat
	
	bool isHovering; 
	bool exit_B;

	SString sceneFromName;
	vector<int> vecSetEnemies;

	//music combat
	const char* musCombat;

	//fx
	const char* swapPath;
	uint swapPositionfx;

	const char* winFxPath;
	uint winfx;

	const char* looseFxPath;
	uint loosefx;

	//animation combat UI
	Tween animationCombat;
	bool transitionCombat_B;
	float point;
	int offsetAni;
	//animacion flee
	Tween animationFlee;
	bool transitionFlee_B;
	int posxFlee_I;
	float pointFlee;
};

#endif // __COMBAT_H__