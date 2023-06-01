#include "IntroScene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Scene.h"

#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

#include "EntityManager.h"
#include "QuestManager.h"
#include "PuzzleManager.h"
#include "Combat.h"

#include <iostream>
using namespace std;
#include <sstream>

IntroScene::IntroScene() : Module()
{
	name.Create("introScene");
}

// Destructor
IntroScene::~IntroScene()
{}

bool IntroScene::Awake(pugi::xml_node& config)
{
	LOG("Loading IntroScene");
	bool ret = true;

	// iterate all objects in the IntroScene
	// Check https://pugixml.org/docs/quickstart.html#access
	music_intro = config.attribute("audioIntroPath").as_string();
	//mouse_Speed = config.attribute("mouseSpeed").as_float();
	texturePath = config.attribute("background").as_string();

	//Save boton continue
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	IntroLoadNode = gameStateFile.child("save_state").child("introScene");

	LoadState(IntroLoadNode);

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	IntroSaveNode = saveStateNode.append_child("introScene");

	animationTitle.Set();
	animationTitle.AddTween(100, 100, BOUNCE_IN_OUT);

	animationBackground.Set();
	animationBackground.AddTween(100, 80, BACK_OUT);

	return ret;
}

// Called before the first frame
bool IntroScene::Start()
{
	app->audio->PlayMusic(music_intro, 1.0f);
	texture = app->tex->Load(texturePath);
	
	if (app->questManager->active) 
	{
		app->questManager->active = false;
	}

	// buttons
	for (int i = 0; buttons[i] != "\n"; i++)
	{
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + 1, this, { 25, 350 + 77 * i, 200, 70 }, ButtonType::START, buttons[i], 20));
	}

	pSettings = new Settings(this);
	listButtons.Add(pSettings->listSettingsButtons.start->data);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	transition_B = false;
	exit_B = false;

	return true;
}

// Called each loop iteration
bool IntroScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool IntroScene::Update(float dt)
{
	app->input->GetMousePosition(mouseX_intro, mouseY_intro);
	
	if (transition_B)
	{
		animationTitle.Backward();
		animationBackground.Backward();
	}
	else
	{
		animationTitle.Foward();
		animationBackground.Foward();
	}


	animationTitle.Step(1, false);
	animationBackground.Step(1, false);

	float point = animationBackground.GetPoint();
	int offset = -1300;

	app->render->DrawTexture(texture, offset + point * (0 - offset)-app->render->camera.x, 0-app->render->camera.y);

	point = animationTitle.GetPoint();
	
	app->render->TextDraw("TWISTED", 100, offset + point * (50 - offset), 100, Font::TITLE, { 181, 33, 33 });
	app->render->TextDraw("TENT", 250, offset + point * (160 - offset), 100, Font::TITLE, { 181, 33, 33 });

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		transition_B = true;
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}
		app->combat->firstCombat_B = false;
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);
	}
		

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;

	if (previousGame_B && !pSettings->settings_B)
	{
		listButtons.start->next->data->state = GuiControlState::NORMAL;
		LOG("Continue");
	}
	else if(!previousGame_B)
	{
		listButtons.start->next->data->state = GuiControlState::DISABLED;
	}

	app->input->HandleGamepadMouse(mouseX_intro, mouseY_intro, app->input->mouseSpeed_F, dt);

	return true;
}

// Called each loop iteration
bool IntroScene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	if (app->input->getInput_B) 
	{
		iPoint pos = { app->win->GetWidth() / 4, 650 };
		app->input->RenderTempText("Sign:  %%", app->input->temp.c_str(), pos, 40, Font::TEXT, { 255, 255, 255 });
	}

	if (app->input->playerName->input_entered && !introDone)
	{
		transition_B = true;
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;

		}

		app->combat->firstCombat_B = true;
		app->fade->FadingToBlack(this, (Module*)app->scene, 90); 
		introDone = true;

		//Quests y puzzles reinicio
		app->questManager->active = true;
		app->questManager->Start();

		app->puzzleManager->palancas = false;
		app->puzzleManager->escape = false;
		app->puzzleManager->rescue = false;
		app->puzzleManager->teamMate = false;
		app->puzzleManager->bossIsDead = false;
		
		app->puzzleManager->BarricadesExplote = 0;
		app->puzzleManager->RelicsCompleted = 0;
		app->puzzleManager->DoorsOpened = 0;
		app->puzzleManager->keyDoors = false;
		app->puzzleManager->DoorKey1Opened = false;
		app->puzzleManager->DoorKey2Opened = false;
		app->puzzleManager->chickenBoom = false;
		app->puzzleManager->relics = false;
		app->puzzleManager->keyInvent = false;
		app->puzzleManager->BarricadeExplote1 = false;
		app->puzzleManager->BarricadeExplote2 = false;
		app->puzzleManager->BarricadeExplote3 = false;
		app->puzzleManager->BarricadeExplote4 = false;
		app->puzzleManager->BarricadeExplote5 = false;
		app->puzzleManager->Relic1Invent = false;
		app->puzzleManager->Relic2Invent = false;
		app->puzzleManager->Relic3Invent = false;
		app->puzzleManager->RelicInColumn1 = false;
		app->puzzleManager->RelicInColumn2 = false;
		app->puzzleManager->RelicInColumn3 = false;

		app->questManager->SaveState();

		if (app->questManager->active)
		{
			app->questManager->quest1->active = false;
			app->questManager->quest2->active = false;
			app->questManager->quest3->active = false;

			app->questManager->quest1->complete = false;
			app->questManager->quest2->complete = false;
			app->questManager->quest3->complete = false;
		}

		app->questManager->SaveState();

		//Fuerza que el jugador en una nueva partida aparezca al inicio
		app->entityManager->tpID = 21;
	}

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		LOG("general %d, %d music, %d fx", pSettings->pAudio->general->volume100, pSettings->pAudio->music->volume100,pSettings->pAudio->fx->volume100);

	if (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN) {
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NORMAL;
		}
		pSettings->settings_B = false;
	}

	if (pSettings->settings_B) 
	{
		pSettings->OpenSettings();

		// TO DO: cambiar aixo, nose perque pero no deixa fer aixo des del settings
		for (ListItem<GuiButton*>* i = pSettings->pGame->listGameButtons.start; i != nullptr; i = i->next)
		{
			if (i->data->id == 807)	// Change TextSpeed button
			{
				i->data->text = app->dialogueSystem->GetTextSpeedSString();
			}
		}
	}
	else if (!pSettings->settings_B)
	{
		pSettings->CloseSettings();
	}

	app->guiManager->Draw();


	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
	{
		app->render->DrawTexture(app->input->cursorPressedTex, mouseX_intro, mouseY_intro);
	}

	else
	{
		app->render->DrawTexture(app->input->cursorIdleTex, mouseX_intro, mouseY_intro);
	}

	return ret;
}

// Called before quitting
bool IntroScene::CleanUp()
{
	LOG("Freeing IntroScene");
	app->input->temp = "";

	listButtons.Clear();
	pSettings->CleanUp();

	app->tex->UnLoad(texture);

	delete pSettings;
	pSettings = nullptr;

	app->entityManager->Disable();

	app->guiManager->CleanUp();
	return true;
}

bool IntroScene::LoadState(pugi::xml_node& data)
{
	previousGame_B = data.child("previousGame").attribute("state_B").as_bool();

	return true;
}

bool IntroScene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node previousGame = data.append_child("previousGame");
	previousGame.append_attribute("state_B") = true;

	return true;
}

bool IntroScene::OnGuiMouseHoverEvent(GuiControl* control)
{
	

	return true;
}

bool IntroScene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button start click");
		if (!app->input->playerName->input_entered)
		{
			app->input->ActiveGetInput(app->input->playerName);
		}
		else
		{
			transition_B = true;
			for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = false;
			}

			app->combat->firstCombat_B = true;
			app->fade->FadingToBlack(this, (Module*)app->scene, 90);
		}
		break;
	case 2:
		LOG("Button continue click");
		transition_B = true;
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}

		app->questManager->Enable();

		app->combat->firstCombat_B = false;
		app->input->coso = true;
		app->LoadGameRequest();
		//app->fade->FadingToBlack(this, (Module*)app->scene, 90);
		continueGame_B = true;
		break;
	case 3:
		LOG("Button settings click");
		pSettings->settings_B = true;

		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::DISABLED;
		}
		break;
	case 4:
		LOG("Button Exit game click");
		exit_B = true;
		break;


		// Settings
	case 801:
		LOG("Button Close settings click");
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NORMAL;
		}
		pSettings->CloseSettings();
		break;

	case 802:
		LOG("Game settings click");
		pSettings->pGame->game_B = true;

		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 803:
		LOG("Graphics settings click");
		pSettings->pGraphics->graphics_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pAudio->CloseAudioSettings();
		break;


	case 804:
		LOG("Audio settings click");
		pSettings->pAudio->audio_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		break;

	case 805:
		LOG("Credits settings click");
		pSettings->pControl->control_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;


		// Game settings
	case 806:
		LOG("Button Language click");

		break;

	case 807:
		LOG("Button Text Speed click");
		control->text = app->dialogueSystem->ChangeTextSpeed();
		break;

	case 808:
		LOG("Button Return to Title click");

		break;

	case 809:
		LOG("Button Exit Game click");
		exit_B = true;
		break;


		// Control settings
	case 810:
		LOG("Button Move Up keyboard check");

		break;

	case 811:
		LOG("Button Move Up gamepad check");

		break;

	case 812:
		LOG("Button Move Left keyboard check");

		break;

	case 813:
		LOG("Button Move Left gamepad check");

		break;

	case 814:
		LOG("Button Move Right keyboard check");

		break;

	case 815:
		LOG("Button Move Right gamepad check");

		break;

	case 816:
		LOG("Button Move Down keyboard check");

		break;

	case 817:
		LOG("Button Move Down gamepad check");

		break;

	case 818:
		LOG("Button Interact keyboard check");

		break;

	case 819:
		LOG("Button Interact gamepad check");

		break;

	case 820:
		LOG("Button Inventory keyboard check");

		break;

	case 821:
		LOG("Button Party gamepad check");

		break;

	case 822:
		LOG("Button Quests keyboard check");

		break;

	case 823:
		LOG("Button Quests gamepad check");

		break;

	case 824:
		LOG("Button Map keyboard check");

		break;

	case 825:
		LOG("Button Map gamepad check");

		break;

	case 826:
		LOG("Button Settings keyboard check");

		break;

	case 827:
		LOG("Button Settings gamepad check");

		break;


		// Graphics settings
	case 828:
		LOG("Button Windows size");

		break;

	case 829:
		LOG("Checkbox Fullscreen check");
		app->win->fullscreen = !app->win->fullscreen;
		app->win->FullscreenWin();
		break;

	
	case 830:
		LOG("Checkbox Vsync check");
		app->render->vSync_B = !app->render->vSync_B;
		app->render->VSyncOn();
		break;

	case 831:
		LOG("Button Max fps");

		break;


		// Audio settings
	case 832:
		LOG("Slider bar General volume");
		app->audio->ChangeGeneralVolume(pSettings->pAudio->general->volume100);
		break;

	case 833:
		LOG("Slider bar Music volume");
		break;

	case 834:
		LOG("Slider bar Fx volume");
		break;
	}

	return true;
}