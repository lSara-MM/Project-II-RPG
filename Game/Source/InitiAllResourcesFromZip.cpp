#include "InitiAllResourcesFromZip.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "AssetsManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

InitAllResourcesFromZip::InitAllResourcesFromZip() : Module()
{
	name.Create("InitAllResourcesFromZip");
}

// Destructor
InitAllResourcesFromZip::~InitAllResourcesFromZip()
{}

// Called before render is available
bool InitAllResourcesFromZip::Awake(pugi::xml_node& config)
{
	LOG("Loading InitiAllResourcesFromZip");

	bool ret = true;
	return ret;
}

// Called before the first frame
bool InitAllResourcesFromZip::Start()
{

	//char* buffer;
	//pugi::xml_document file;

	////TODO 5 (Solved): Load data from the "data.xml" with the function LoadData(), and send the buffer too
	//int bytes = app->assetsManager->LoadData("data.xml", &buffer);

	////Here we will make the space in memory that needs the program to open the files
	//pugi::xml_parse_result result = file.load_buffer(buffer, bytes);

	////Release the buffer
	//RELEASE_ARRAY(buffer);

	//LoadTexFile(file);
	//LoadAudioFile(file);
	//LoadGUI(file);

	return true;
}

// Called each loop iteration
bool InitAllResourcesFromZip::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool InitAllResourcesFromZip::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool InitAllResourcesFromZip::PostUpdate()
{

	return true;
}

// Called before quitting
bool InitAllResourcesFromZip::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void InitAllResourcesFromZip::LoadTexFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node tex = dataFile.child("data").child("texture");

	 texturepathAllyB = tex.attribute("AllyB").as_string();
	 texturepathAllyP = tex.attribute("AllyP").as_string();
	 texturepathCH_Prota_Spr = tex.attribute("CH_Prota_Sprites").as_string();
	 texturepathCH_Prota_Spr2 = tex.attribute("CH_Prota_Sprites2").as_string();
	 texturepathCheckBox = tex.attribute("CheckBox").as_string();
	 texturepathCombatBack = tex.attribute("CombatBack").as_string();
	 texturepathCompa_Spr = tex.attribute("Compa_Sprites").as_string();
	 texturepathLoseScreen = tex.attribute("LoseScreen").as_string();
	 texturepathWinScreen = tex.attribute("WinScreen").as_string();
	 texturepathEnemyCursedPuppet = tex.attribute("EnemyCursedPuppet").as_string();
	 texturepathFuegito_Chibi = tex.attribute("Fuegito_Chibi").as_string();
	 texturepathIntro_Bg = tex.attribute("Intro_Bg").as_string();
	 texturepathLogo_Pikum = tex.attribute("Logo_Pikum").as_string();
	 texturepathMarioneta2 = tex.attribute("Marioneta2").as_string();
	 texturepathMarioneta3 = tex.attribute("Marioneta3").as_string();
	 texturepathOraculo = tex.attribute("Oraculo").as_string();
	 texturepathProtagonist = tex.attribute("Protagonist").as_string();
	 texturepathPuzzles = tex.attribute("Puzzles").as_string();
	 texturepathBoss_Spr = tex.attribute("Boss_Sprite").as_string();

	//player->texture = app->tex->Load(tex.attribute("file2").as_string());
}

void InitAllResourcesFromZip::LoadAudioFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node audio = dataFile.child("data").child("Audio");

	 texturepathAdivino = audio.attribute("Adivino").as_string();
	 texturepathButton_Menu = audio.attribute("Button_Menu").as_string();
	 texturepathChange_Position = audio.attribute("Change_Position").as_string();
	 texturepathClown_Button = audio.attribute("Clown_Button").as_string();
	 texturepathConfirm_Interaction = audio.attribute("Confirm_Interaction").as_string();
	 texturepathEnemy_Selected = audio.attribute("Enemy_Selected").as_string();
	 texturepathEnemy_Sala = audio.attribute("Enemy_Sala").as_string();
	 texturepathHeal = audio.attribute("Heal").as_string();
	 texturepathHit = audio.attribute("Hit").as_string();
	 texturepathInteraction_Barda = audio.attribute("Interaction_Barda").as_string();
	 texturepathNegation_Tienda = audio.attribute("Negation_Tienda").as_string();
	 texturepathOn_Button = audio.attribute("On_Button").as_string();
	 texturepathPalanca = audio.attribute("Palanca").as_string();
	 texturepathPasos_Hierba = audio.attribute("Pasos_Hierba").as_string();
	 texturepathPasos_Roca = audio.attribute("Pasos_Roca").as_string();
	 texturepathPasos_Solved = audio.attribute("Pasos_Solved").as_string();

	 texturepathCircusTent_Mus = audio.attribute("CircusTent_Mus").as_string();
	 texturepathHaunted_House = audio.attribute("Haunted_House").as_string();
	 texturepathIntro_Mus = audio.attribute("Intro_Mus").as_string();
	 texturepathLobby_Music = audio.attribute("Lobby_Music").as_string();
	 texturepathLogo_Intro = audio.attribute("Logo_Intro").as_string();
	 texturepathMus_Combat = audio.attribute("Mus_Combat").as_string();
	 texturepathMus_Ending = audio.attribute("Mus_Ending").as_string();
	 texturepathMus_Loose_Scene = audio.attribute("Mus_Loose_Scene").as_string();
	 texturepathMus_Pause = audio.attribute("Mus_Pause").as_string();
	 texturepathSala_Entretenimiento = audio.attribute("Sala_Entrenamiento").as_string();

	//app->audio->LoadFx(fx.attribute("file").as_string());
}

void InitAllResourcesFromZip::LoadGUI(const pugi::xml_document& dataFile)
{
	pugi::xml_node GUI = dataFile.child("data").child("gui");

	 texturepathUI_Battle_HealthBar = GUI.attribute("UI_Battle_HealthBar").as_string();
	 texturepathUI_Battle_TurnBar = GUI.attribute("UI_Battle_TurnBar").as_string();
	 texturepathUI_Button_CharacterSelect = GUI.attribute("UI_Button_CharacterSelect").as_string();
	 texturepathUI_Buttons = GUI.attribute("UI_Buttons").as_string();
	 texturepathUI_Diag_Choice_OFF = GUI.attribute("UI_Diag_Choice_OFF").as_string();
	 texturepathUI_Diag_Ctrl = GUI.attribute("UI_Diag_Ctrl").as_string();
	 texturepathUI_Diag_Next = GUI.attribute("UI_Diag_Next").as_string();
	 texturepathUI_E = GUI.attribute("UI_E").as_string();
	 texturepathUI_Frame_Rect = GUI.attribute("UI_Frame_Rect").as_string();
	 texturepathUI_Frame_RRect = GUI.attribute("UI_Frame_RRect").as_string();
	 texturepathUI_Frame_Sqrt = GUI.attribute("UI_Frame_Sqrt").as_string();
	 texturepathUI_Icons = GUI.attribute("UI_Icons").as_string();
	 texturepathUI_Marker_L = GUI.attribute("UI_Marker_L").as_string();
	 texturepathUI_Marker_M = GUI.attribute("UI_Marker_M").as_string();
	 texturepathUI_Marker_S = GUI.attribute("UI_Marker_S").as_string();
	 texturepathUI_Slider = GUI.attribute("UI_Slider").as_string();
	 texturepathUI_Slider2 = GUI.attribute("UI_Slider2").as_string();
	 texturepathUI_TextBox = GUI.attribute("UI_TextBox").as_string();

	 texturepathButtonInSettings = GUI.attribute("ButtonInSettings").as_string();
	 texturepathButtonSettings = GUI.attribute("ButtonSettings").as_string();
	 texturepathClose = GUI.attribute("Close").as_string();
	 texturepathDialogue = GUI.attribute("Dialogue").as_string();
	 texturepathGeneric = GUI.attribute("Generic").as_string();
	 texturepathStart = GUI.attribute("Start").as_string();

	//app->audio->PlayMusic(mus.attribute("file").as_string());
}