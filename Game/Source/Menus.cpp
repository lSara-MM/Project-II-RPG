#include "Menus.h"
#include "App.h"
#include "Render.h"
#include "IntroScene.h"
#include "Scene.h"
#include "PracticeTent.h"
#include "HouseOfTerrors.h"
#include "BeastTent.h"
#include "SceneWin_Lose.h"
#include "FadeToBlack.h"
#include "PuzzleManager.h"
#include "CutScene.h"
#include "Inventory.h"

Menus::Menus() : Module()
{
	name.Create("Menus");
}

Menus::~Menus()
{}

bool Menus::Awake(pugi::xml_node& config)
{
	pause_music = config.attribute("pause").as_string();
	fxpausepath = "Assets/Audio/Fx/Clown_Button.wav";
	pausefx = app->audio->LoadFx(fxpausepath);
	return true;
}

bool Menus::Start()
{
	pSettings = new Settings(this);
	pPause = new Pause(this);
	pPause->pSettings = pSettings;
	pause_B = false;
	settings_B = false;
	return true;
}

bool Menus::Update(float dt)
{
	deltaTime = dt;
	if (pause_B) { pPause->OpenPause(); }
	else { pPause->ClosePause(); }

	if (settings_B)
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
	else
	{
		pSettings->CloseSettings();
	}

	// Pause menu
	if (pause_B == false && !app->cutScene->active && (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_START) == BUTTON_DOWN))
	{
		pause_B = true;
		app->audio->PlayFx(pausefx);
		app->audio->PlayMusic(pause_music, 1.0f);
		pSettings->settings_B = !pSettings->settings_B;
		app->inventory->Disable();
		LOG("PAUSE");
	}
	else if (pause_B == true && (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN))//POSAR CONTROL NORMAL
	{
		if (settings_B == true)
		{
			for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}
			settings_B = false;
		}
		else
		{
			pause_B = false;

			if (app->scene->active)
			{
				app->audio->PlayMusic(app->scene->lobby_music, 1.0f);
			}
			if (app->practiceTent->active)
			{
				app->audio->PlayMusic(app->practiceTent->practicePath, 1.0f);
			}
			if (app->BeastT->active)
			{
				app->audio->PlayMusic(app->BeastT->musBeastPath, 1.0f);
			}
			if (app->hTerrors->active)
			{
				app->audio->PlayMusic(app->hTerrors->musHauntedPath, 1.0f);
			}
			if (pause_B)
			{
				pSettings = pPause->pSettings;
				pSettings->settings_B = !pSettings->settings_B;
			}
		}
		LOG("CLOSE PAUSE");
	}
	else if (settings_B == true && app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN)
	{
		for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NORMAL;
		}
		pSettings->CloseSettings();
		settings_B = false;
	}
	app->guiManager->Draw();
	return true;
}

bool Menus::PostUpdate()
{	
	if (app->iScene->active || app->sceneWin_Lose->active || menuOn || app->combat->active || pause_B)
	{
		app->input->HandleGamepadMouse(app->input->mouseX, app->input->mouseY, app->input->mouseSpeed_F, deltaTime);
		app->input->RenderMouse();
	}

	if (exit_B) return false;

	return true;
}

bool Menus::CleanUp()
{
	if (pSettings != nullptr)
	{
		pSettings->CleanUp();
	}
	if (pPause != nullptr)
	{
		pPause->CleanUp();
	}
	app->guiManager->CleanUp();

	return true;
}

bool Menus::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
		// Pause
	case 701:
		LOG("Button Close pause click");
		pause_B = false;

		if (app->scene->active)
		{
			app->audio->PlayMusic(app->scene->lobby_music, 1.0f);
		}

		if (app->practiceTent->active)
		{
			app->audio->PlayMusic(app->practiceTent->practicePath, 1.0f);
		}

		if (app->BeastT->active)
		{
			app->audio->PlayMusic(app->BeastT->musBeastPath, 1.0f);
		}
		if (app->hTerrors->active)
		{
			app->audio->PlayMusic(app->hTerrors->musHauntedPath, 1.0f);
		}
		break;

	case 702:
		LOG("Button Resume click");
		pause_B = false;		
		
		if (app->scene->active)
		{
			app->audio->PlayMusic(app->scene->lobby_music, 1.0f);
		}
		if (app->practiceTent->active)
		{
			app->audio->PlayMusic(app->practiceTent->practicePath, 1.0f);
		}
		if (app->BeastT->active)
		{
			app->audio->PlayMusic(app->BeastT->musBeastPath, 1.0f);
		}
		if (app->hTerrors->active)
		{
			app->audio->PlayMusic(app->hTerrors->musHauntedPath, 1.0f);
		}
		break;

	case 703:
		LOG("Button Return to title click");
		app->puzzleManager->CleanUp();
		app->puzzleManager->active = false;
		pause_B = false;

		if (app->scene->active)
		{
			app->fade->FadingToBlack(app->scene, (Module*)app->iScene, 90);
		}
		if (app->practiceTent->active)
		{
			app->fade->FadingToBlack(app->practiceTent, (Module*)app->iScene, 90);
		}
		if (app->BeastT->active)
		{
			app->fade->FadingToBlack(app->BeastT, (Module*)app->iScene, 90);
		}
		if (app->hTerrors->active)
		{
			app->fade->FadingToBlack(app->hTerrors, (Module*)app->iScene, 90);
		}
		break;
	case 704:
		LOG("Button Settings click");
		for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::DISABLED;
		}
		settings_B = true;
		break;
	case 705:
		LOG("Button Exit click");
		exit_B = true;
		break;
		// Settings
	case 801:
		LOG("Button Close settings click");

		if (app->iScene->active)
		{
			for (ListItem<GuiButton*>* i = app->iScene->listButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}
		}
		else
		{
			for (ListItem<GuiButton*>* i = pPause->listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}
		}

		settings_B = false;
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

		pause_B = false;
		settings_B = false;

		if (app->scene->active)
		{
			app->fade->FadingToBlack(app->scene, (Module*)app->iScene, 90);
		}
		if (app->practiceTent->active)
		{
			app->fade->FadingToBlack(app->practiceTent, (Module*)app->iScene, 90);
		}
		if (app->BeastT->active)
		{
			app->fade->FadingToBlack(app->BeastT, (Module*)app->iScene, 90);
		}
		if (app->hTerrors->active)
		{
			app->fade->FadingToBlack(app->hTerrors, (Module*)app->iScene, 90);
		}
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
	return false;
}
