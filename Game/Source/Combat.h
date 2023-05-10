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

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);



	// Sara functions, to test
	void MoveCharacter(vector<Character*>* arr, Character* chara, int newPosition_I);
	void RemoveCharacter(vector<Character*> arr, Character* chara);
	void UpdatePositions(vector<Character*>* arr);

	bool InitEnemies(SString scene, vector<int> arr);
	bool InitAllies(array<Character*, 4> party);

	bool SaveCombat();//guardar stats playable characters en xml combat
	bool LoadCombat();//cargar stats playable characters desde xml combat
	bool RestartCombatData();//al empezar partida nueva, poner stats xml combat igual a xml config

public:
	//Player* player;
	bool frcap;

	//Turn Order
	List<Character*> listInitiative;
	int charaInTurn = 0; //Empieza en 0, max 7

	vector<Character*> vecAllies;
	vector<Character*> vecEnemies;

	/*array<Character*, 4> listAllies;
	array<Character*, 4> listEnemies;*/

	Character* targeted_Character = nullptr; //Last button selected
	int lastPressedAbility_I = 0;

private:
	int mouseX_combat, mouseY_combat;
	float mouse_Speed;

	pugi::xml_node combatNode;

	//Fondo
	const char* texturePathBackground;
	SDL_Texture* textureBackground;
	const char* texturePathTargetButton;
	SDL_Texture* textureTargetButton;


	// buttons
	List<GuiButton*> listButtons;
	
	const char* actions[4] = { "Atk 1", "Atk 2", "Atk 3", "Atk 4" };

	bool exit_B;
};

#endif // __COMBAT_H__