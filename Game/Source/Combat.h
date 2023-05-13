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


	bool StartCombat();
	bool OrderBySpeed();

	bool NextTurn(); //Lo enviaran los characthers cuando finalicen su turno

	//TargetControl
		// Del 0-3 allies al 4-7 enemies 
	bool DisableTargetButton(int id);
	// Del 0-3 allies al 4-7 enemies
	bool EnableTargetButton(int id);

	//Del 1 al 4
	bool EnableSkillButton(int skillNum);
	//Del 1 al 4
	bool DisableSkillButton(int skillNum);

	// Characters to disable/enable buttons
	void HandleCharaButtons(vector<Character*>* arr, int pos1, int pos2);

	void MoveCharacter(vector<Character*>* arr, Character* chara, int newPosition_I);
	void RemoveCharacter(vector<Character*>* arr, Character* chara);
	void UpdatePositions(vector<Character*>* arr, int pos);

	bool PreLoadCombat(array<Character*, 4> arrParty_, SString n);
	bool InitEnemies(vector<int> arr);
	bool InitAllies(array<Character*, 4> party);

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);

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
	int lastPressedAbility_I = 0;

	bool firstCombat_B = false;//determinar si es la primera vez que se lucha en la partida, en true significa que no se ha luchado todavia

	// buttons
	List<GuiButton*> listButtons;
private:
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
	
	const char* actions[4] = { "Atk 1", "Atk 2", "Atk 3", "Atk 4" };

	bool exit_B;

	// Scene transition
	array<Character*, 4> arrParty;
	SString sceneFromName;
	vector<int> arrSetEnemies;
};

#endif // __COMBAT_H__