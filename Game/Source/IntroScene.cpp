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
	//mouse_Speed = config.attribute("mouseSpeed").as_int();

	//Save boton continue
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	IntroLoadNode = gameStateFile.child("save_state").child("introScene");

	LoadState(IntroLoadNode);


	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	IntroSaveNode = saveStateNode.append_child("introScene");

	return ret;
}

// Called before the first frame
bool IntroScene::Start()
{
	mouse_Speed = 0.5f;

	app->audio->PlayMusic(music_intro, 0);

	// buttons
	for (int i = 0; buttons[i] != "\n"; i++)
	{
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + 1, this, { 25, 180 + 77 * i, 200, 70 }, ButtonType::START, buttons[i], 20));
	}

	//listButtons.start->next->data->state = GuiControlState::DISABLED;

	pSettings = new Settings(this);
	listButtons.Add(pSettings->listSettingsButtons.start->data);

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

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;

	if (previousGame_B)
	{
		listButtons.start->next->data->state = GuiControlState::NORMAL;
		LOG("Continue");
	}
	else
	{
		listButtons.start->next->data->state = GuiControlState::DISABLED;
	}

	if (app->input->controller.j1_x > 0)
	{
		SDL_WarpMouseInWindow(app->win->window, mouseX_intro + (mouse_Speed * dt), mouseY_intro);

	}

	else if (app->input->controller.j1_x < 0)
	{
		SDL_WarpMouseInWindow(app->win->window, mouseX_intro - (mouse_Speed * dt), mouseY_intro);

	}

	else if (app->input->controller.j1_y > 0)
	{
		SDL_WarpMouseInWindow(app->win->window, mouseX_intro, mouseY_intro + (mouse_Speed * dt));

	}

	else if (app->input->controller.j1_y < 0)
	{
		SDL_WarpMouseInWindow(app->win->window, mouseX_intro, mouseY_intro - (mouse_Speed * dt));

	}

	return true;
}

// Called each loop iteration
bool IntroScene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;
	if (app->input->getInput_B) PlayerNameInput();
	if (app->input->nameEntered_B && !introDone) 
	{
		app->fade->FadingToBlack(this, (Module*)app->scene, 90); 
		introDone = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		LOG("general %d, %d music, %d fx", pSettings->pAudio->general->volume100, pSettings->pAudio->music->volume100,pSettings->pAudio->fx->volume100);

	if (pSettings->settings_B) { pSettings->OpenSettings(); }

	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool IntroScene::CleanUp()
{
	LOG("Freeing IntroScene");
	
	listButtons.Clear();

	pSettings->CleanUp();

	delete pSettings;
	pSettings = nullptr;

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


bool IntroScene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button start click");
		if (!app->input->nameEntered_B)
		{
			app->input->getInput_B = true;
		}
		else
		{
			app->fade->FadingToBlack(this, (Module*)app->scene, 90);
		}
		break;
	case 2:
		LOG("Button continue click");
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
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
		LOG("Controls settings click");
		pSettings->pControl->control_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pGraphics->CloseGraphics();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 804:
		LOG("Graphics settings click");
		pSettings->pGraphics->graphics_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pAudio->CloseAudioSettings();
		break;

	case 805:
		LOG("Audio settings click");
		pSettings->pAudio->audio_B = true;

		pSettings->pGame->CloseGameSettings();
		pSettings->pControl->CloseControlSettings();
		pSettings->pGraphics->CloseGraphics();
		break;


		// Game settings
	case 806:
		LOG("Button Language click");

		break;

	case 807:
		LOG("Button Text Speed click");

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

bool IntroScene::PlayerNameInput()
{
	SString temp;

	temp = "Sign:  %%";
	temp.Substitute("%", app->input->playerName.c_str());
	app->render->TextDraw(temp.GetString(), app->win->GetWidth() / 3, 500, 40, Font::TEXT, { 255, 255, 255 });

	return app->input->nameEntered_B;
}