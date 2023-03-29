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
	
	bool AddCombatant(Characther* chara, int modifier); //Eric:A�adir charathers a la lista de personajes, el int es para la variabilidad de los enemigos y QUIZA bosses con dos acciones
	bool NextTurn(); //Lo enviaran los characthers cuando finalicen su turno

	// Settings
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	Player* player;
	bool frcap;

	//Turn Order
	List<Characther*> initiative;
	int charaInTurn;

	Characther* allies[4]; //4 es backline, 1 es frontline
	Characther* enemies[4]; //4 es backline, 1 es frontline

	

private:

	// Settings
	Settings options;
	Settings* pSettings = &options;

	Pause pauseMenu;
	Pause* pPause = &pauseMenu;

	bool exit;
};

#endif // __COMBAT_H__