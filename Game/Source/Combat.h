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
	
	bool AddCombatant(Character* chara, int modifier); //Eric:Añadir charathers a la lista de personajes, el int es para la variabilidad de los enemigos y QUIZA bosses con dos acciones
	bool OrderBySpeed();
	bool NextTurn(); //Lo enviaran los characthers cuando finalicen su turno
	bool MoveAllies(int charaPosition_I, int newPosition_I);//Swap de un PC en una posicion hasta otra posicion
	bool StartCombat();
	//TargetControl
	bool DeactivateTargetButton(int id);
	bool ActivateTargetButton(int id);

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	Player* player;
	bool frcap;

	//Turn Order
	List<Character*> listInitiative;
	int charaInTurn=1;

	Character* allies[4]; //4 es backline, 1 es frontline
	Character* enemies[4]; //4 es backline, 1 es frontline
	Character* targeted_Character=nullptr; //Last button selected


private:
	// Settings
	Settings options;
	Settings* pSettings = &options;

	Pause pauseMenu;
	Pause* pPause = &pauseMenu;
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