#ifndef __InitAllResourcesFromZip_H__
#define __InitAllResourcesFromZip_H__

#include "Module.h"
#include "App.h"

#include "Player.h"


struct SDL_Texture;

class InitAllResourcesFromZip : public Module
{
public:

	InitAllResourcesFromZip();

	// Destructor
	virtual ~InitAllResourcesFromZip();

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

	//load the texture
	void LoadTexFile(const pugi::xml_document& dataFile);

	//load the fx
	void LoadAudioFile(const pugi::xml_document& dataFile);

	//load and play the music
	void LoadGUI(const pugi::xml_document& dataFile);

public:

	Player* player;
	SDL_Texture* img;

	const char* texturepathAllyB;
	const char* texturepathAllyP;
	const char* texturepathCH_Prota_Spr;
	const char* texturepathCH_Prota_Spr2;
	const char* texturepathCheckBox;
	const char* texturepathCombatBack;
	const char* texturepathCompa_Spr;
	const char* texturepathLoseScreen;
	const char* texturepathWinScreen;
	const char* texturepathEnemyCursedPuppet;
	const char* texturepathFuegito_Chibi;
	const char* texturepathIntro_Bg;
	const char* texturepathLogo_Pikum;
	const char* texturepathMarioneta2;
	const char* texturepathMarioneta3;
	const char* texturepathOraculo;
	const char* texturepathProtagonist;
	const char* texturepathPuzzles;
	const char* texturepathBoss_Spr;

	const char* texturepathAdivino;
	const char* texturepathButton_Menu;
	const char* texturepathChange_Position;
	const char* texturepathClown_Button;
	const char* texturepathConfirm_Interaction;
	const char* texturepathEnemy_Selected;
	const char* texturepathEnemy_Sala;
	const char* texturepathHeal;
	const char* texturepathHit;
	const char* texturepathInteraction_Barda;
	const char* texturepathNegation_Tienda;
	const char* texturepathOn_Button;
	const char* texturepathPalanca;
	const char* texturepathPasos_Hierba;
	const char* texturepathPasos_Roca;
	const char* texturepathPasos_Solved;

	const char* texturepathCircusTent_Mus;
	const char* texturepathHaunted_House;
	const char* texturepathIntro_Mus;
	const char* texturepathLobby_Music;
	const char* texturepathLogo_Intro;
	const char* texturepathMus_Combat;
	const char* texturepathMus_Ending;
	const char* texturepathMus_Loose_Scene;
	const char* texturepathMus_Pause;
	const char* texturepathSala_Entretenimiento;

	const char* texturepathUI_Battle_HealthBar;
	const char* texturepathUI_Battle_TurnBar;
	const char* texturepathUI_Button_CharacterSelect;
	const char* texturepathUI_Buttons;
	const char* texturepathUI_Diag_Choice_OFF;
	const char* texturepathUI_Diag_Ctrl;
	const char* texturepathUI_Diag_Next;
	const char* texturepathUI_E;
	const char* texturepathUI_Frame_Rect;
	const char* texturepathUI_Frame_RRect;
	const char* texturepathUI_Frame_Sqrt;
	const char* texturepathUI_Icons;
	const char* texturepathUI_Marker_L;
	const char* texturepathUI_Marker_M;
	const char* texturepathUI_Marker_S;
	const char* texturepathUI_Slider;
	const char* texturepathUI_Slider2;
	const char* texturepathUI_TextBox;

	const char* texturepathButtonInSettings;
	const char* texturepathButtonSettings;
	const char* texturepathClose;
	const char* texturepathDialogue;
	const char* texturepathGeneric;
	const char* texturepathStart;

private:
	
};

#endif // __LOGOSCENE_H__
