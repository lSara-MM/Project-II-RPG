#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "App.h"

#include "Module.h"
#include "Player.h"
#include "Characther.h"

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
	bool InitEntities(); //No se si quitarlo o que 
	
	bool AddCombatant(Character* chara, int modifier); //Eric:A�adir charathers a la lista de personajes, el int es para la variabilidad de los enemigos y QUIZA bosses con dos acciones
	bool OrderBySpeed();
	//Es muy importante que antes de 
	bool EliminateCombatant(Character* chara, int modifier); 
	
	bool StartCombat();
	bool NextTurn(); //Lo enviaran los characthers cuando finalicen su turno
	
	//Swap de un PC en una posicion hasta otra posicion, las posiciones van del 1 (front) al 4 (back)
	bool MoveAllies(int charaPosition_I, int newPosition_I);
	//Swap de un Enemy en una posicion hasta otra posicion, las posiciones van del 1 (front) al 4 (back)
	bool MoveEnemies(int charaPosition_I, int newPosition_I);
	
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

public:
	Player* player;
	bool frcap;

	//Turn Order
	List<Character*> listInitiative;
	int charaInTurn=1;

	Character* allies[4]; //3 es backline, 0 es frontline
	Character* enemies[4]; //3 es backline, 0 es frontline
	Character* targeted_Character=nullptr; //Last button selected
	int lastPressedAbility_I = 0;

private:
	// Settings
	Settings* pSettings;

	Pause pauseMenu;
	Pause* pPause = &pauseMenu;

	pugi::xml_node combatNode;

	//TEXTURAS
		//Fondo
		const char* texturePathBackground;
		SDL_Texture* textureBackground;
		const char* texturePathTargetButton;
		SDL_Texture* textureTargetButton;


	// buttons
	List<GuiButton*> listButtons;

	const char* actions[4] = { "Atk 1", "Atk 2", "Atk 3", "Atk 4" };

	bool exit;
};

#endif // __COMBAT_H__