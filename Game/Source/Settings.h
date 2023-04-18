#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "App.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Audio.h"
#include "Module.h"
#include "Render.h"
#include "Window.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

class GameSettings
{
public:

	GameSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		game_B = true;
		open_game_B = false;

		// Language
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 858, 290, 133, 33 }, ButtonType::SETTINGS, "English", 16);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);

		// Text Speed
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 858, 367, 133, 33 }, ButtonType::SETTINGS, "Medium", 16);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);

		// Return to Title
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 556, 444, 133, 33 }, ButtonType::SETTINGS, "Return to Title", 16);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);

		// Exit
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 556, 521, 133, 33 }, ButtonType::SETTINGS, "Exit", 16);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);
	}

	bool OpenGameSettings()
	{
		int x = 556; int y = 290; int offset = 77;
		app->render->TextDraw("Language", x, y, 16);
		app->render->TextDraw("Text Speed", x, y + offset, 16);

		if (!open_game_B)
		{
			for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}
		}
		return true;
	}

	bool CloseGameSettings()
	{
		game_B = false;
		open_game_B = false;

		for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
		}

		listGameButtons.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 806;
	List<GuiButton*> listGameButtons;

	GuiSliderBar* music_P, * fx_P;

	SDL_Texture* gameSettingsTexture;
	const char* gameSettingsPath;
	bool game_B;
	bool open_game_B;
};

class ControlSettings
{
public:

	ControlSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		control_B = false;
		open_control_B = false;

		GuiButton* button;

		// buttons
		int x = 630; int y = 0;
		for (int i = 0; buttons[i] != "\n"; i++)
		{
			if (i == 5) { x = 900; y = 0; }

			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { x, 280 + 77 * y++, 34, 34 }, ButtonType::CONTROL_SETTINGS, buttons[i], 16);
			button->state = GuiControlState::NONE;
			listControlButtons.Add(button);
		}
	}

	bool OpenControlSettings()
	{
		int x = 530; int y = 290; int offset = 77;
		app->render->TextDraw("Move up", x, y, 16);
		app->render->TextDraw("Move left", x, y + offset, 16);
		app->render->TextDraw("Move right", x, y + offset * 2, 16);
		app->render->TextDraw("Move down", x, y + offset * 3, 16);
		app->render->TextDraw("Interact", x, y + offset * 4, 16);

		x = 800;
		app->render->TextDraw("Inventory", x, y, 16);
		app->render->TextDraw("Party", x, y + offset, 16);
		app->render->TextDraw("Quests", x, y + offset * 2, 16);
		app->render->TextDraw("Map", x, y + offset * 3, 16);
		app->render->TextDraw("Settings", x, y + offset * 4, 16);

		if (!open_control_B)
		{
			for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}
	
			open_control_B = true;
		}

		return true;
	}

	bool CloseControlSettings()
	{
		control_B = false;
		open_control_B = false;

		for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
		}

		listControlButtons.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 810;
	List<GuiButton*> listControlButtons;

	const char* buttons[21] = {"W", "A", "D", "S", "E", "I", "R", "J", "M", "Esc", "\n"};

	bool control_B;
	bool open_control_B;
};

class GraphicsSettings
{
public:

	GraphicsSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		graphics_B = false;
		open_graphics_B = false;

		// Window Size
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 871, 280, 133, 33 }, ButtonType::SETTINGS, "Window Size", 16);
		button->state = GuiControlState::NONE;
		listGraphicsButtons.Add(button);

		// Fullscreen	
		GUI_id++;
		GuiCheckBox* checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 871, 357, 46, 46 });
		checkbox->state = GuiControlState::NONE;
		listGraphicsCheckbox.Add(checkbox);

		// Vsync
		GUI_id++;
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 871, 434, 46, 46 });
		checkbox->state = GuiControlState::NONE;
		listGraphicsCheckbox.Add(checkbox);

		// Max fps
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 871, 511, 133, 33 }, ButtonType::SETTINGS, "Max fps", 10);
		button->state = GuiControlState::NONE;
		listGraphicsButtons.Add(button);
	}

	bool OpenGraphics()
	{
		int x = 556; int y = 290; int offset = 77;
		app->render->TextDraw("Windows Size", x, y, 16);
		app->render->TextDraw("Fullscreen", x, y + offset, 16);
		app->render->TextDraw("Vsync", x, y + offset * 2, 16);
		app->render->TextDraw("Max fps", x, y + offset * 3, 16);

		if (!open_graphics_B)
		{
			for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
			{
				if (app->win->fullscreen || app->render->vSync_B)	// to change later (if as default is on, it should be shown activated 
				{
					i->data->state = GuiControlState::SELECTED;
				}
				else
				{
					i->data->state = GuiControlState::NORMAL;
				}
			}

			open_graphics_B = true;
		}

		return true;
	}

	bool CloseGraphics()
	{
		graphics_B = false;
		open_graphics_B = false;

		for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
		}

		for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
		}

		listGraphicsButtons.Clear();
		listGraphicsCheckbox.Clear();

		return true;
	}

public:

	// GUI
	int GUI_id = 828;
	List<GuiButton*> listGraphicsButtons;
	List<GuiCheckBox*> listGraphicsCheckbox;

	bool graphics_B;
	bool open_graphics_B;
};

class AudioSettings
{
public:

	AudioSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		audio_B = false;
		open_audio_B = false;

		// General volume
		GuiSliderBar* sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 743, 298, 219, 39 }, ButtonType::NONE, "general", 10, Font::UI, { 310, 298, 28, 39 });
		sliderBar->state = GuiControlState::NONE;
		general = sliderBar;
		listSliderBars.Add(sliderBar);

		// music
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 743, 375, 219, 39 }, ButtonType::NONE, "music", 10, Font::UI, { 310, 375, 28, 39 });
		sliderBar->state = GuiControlState::NONE;
		music = sliderBar;
		listSliderBars.Add(sliderBar);

		// fx
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 743, 452, 219, 39 }, ButtonType::NONE, "fx", 10, Font::UI, { 310, 452, 28, 39 });
		sliderBar->state = GuiControlState::NONE;
		fx = sliderBar;
		listSliderBars.Add(sliderBar);
	}

	bool OpenAudioSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		int x = 556; int y = 290; int offset = 77;
		app->render->TextDraw("General volume", x, y, 16);
		app->render->TextDraw("Music volume", x, y + offset, 16);
		app->render->TextDraw("Fx volume", x, y + offset * 2, 16);
	
		if (!open_audio_B)
		{
			for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			general->sliderBounds.x = general->bounds.x + app->audio->volume_general * general->bounds.w / SDL_MIX_MAXVOLUME;
			general->volume100 = app->audio->volume_general;

			music->sliderBounds.x = music->bounds.x + app->audio->volume_music * music->bounds.w / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volume_music;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volume_fx * fx->bounds.w / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volume_fx;


			open_audio_B = true;
		}

		if (app->audio->volume_general != general->volume100)app->audio->ChangeGeneralVolume(general->volume100);
		if (app->audio->volume_music != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volume_fx != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		
		return true;
	}

	bool CloseAudioSettings()
	{
		audio_B = false;
		open_audio_B = false;

		for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
		}
		
		listSliderBars.Clear();

		return true;
	}

public:

	// GUI
	int GUI_id = 832;
	List<GuiSliderBar*> listSliderBars;

	GuiSliderBar* general, * music, * fx;

	bool audio_B;
	bool open_audio_B;
};

class Settings
{
public:

	Settings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings_B = false;
		open_settings_B = false;

		// close
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 1038, 175, 57, 57 }, ButtonType::CLOSE);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// buttons
		for (int i = 0; buttons[i] != "\n"; i++)
		{
			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { 290, 300 + 75 * i, 136, 33 }, ButtonType::SETTINGS, buttons[i], 32);
			button->state = GuiControlState::NONE;
			listSettingsButtons.Add(button);
		}

		pGame = new GameSettings(mod);
		pControl = new ControlSettings(mod);
		pGraphics = new GraphicsSettings(mod);
		pAudio = new AudioSettings(mod);
	}

	bool OpenSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 290, 203, 730, 463 }, 163, 163, 163, 200, true);
		//if (!app->render->DrawTexture(settingsTexture, 150, 70, &rect)) { app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110}); }

		app->render->DrawLine(490, 250, 490, 600, 0, 0, 0);
		app->render->TextDraw("Settings", 600, 121, 40, Font::UI, { 255, 255, 255 });


		if (pGame->game_B) { pGame->OpenGameSettings(); }
		if (pControl->control_B) { pControl->OpenControlSettings(); }
		if (pGraphics->graphics_B) { pGraphics->OpenGraphics(); }
		if (pAudio->audio_B) { pAudio->OpenAudioSettings(); }

		if (!open_settings_B)
		{
			for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			open_settings_B = true;
		}

		return true;
	}

	bool CloseSettings()
	{
		settings_B = false;
		open_settings_B = false;

		pGame->CloseGameSettings();
		pControl->CloseControlSettings();
		pGraphics->CloseGraphics();
		pAudio->CloseAudioSettings();

		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		//app->tex->UnLoad(settingsTexture);

		listSettingsButtons.Clear();
		settings_B = false;
		
		// no li agraden els clean ups aquests
		/*pGame->CleanUp();
		pControl->CleanUp();
		pGraphics->CleanUp();
		pAudio->CleanUp();*/

		delete pGame;
		pGame = nullptr;

		delete pControl;
		pControl = nullptr;

		delete pGraphics;
		pGraphics = nullptr;

		delete pAudio;
		pAudio = nullptr;

		return true;
	}

public:

	// buttons
	int GUI_id = 801;
	List<GuiButton*> listSettingsButtons;
	const char* buttons[5] = { "Game", "Controls", "Graphics", "Audio", "\n" };

	SDL_Texture* settingsTexture;
	const char* settingsPath;
	bool settings_B;
	bool open_settings_B;

	
	GameSettings* pGame;
	ControlSettings* pControl;
	GraphicsSettings* pGraphics;
	AudioSettings* pAudio;
};


class Pause
{
public:

	Pause(Module* mod)
	{
		//PauseTexture = app->tex->Load(PausePath);

		// Pause buttons
		pause_B = false;
		open_pause_B = false;

		// close
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 137, 56, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listPauseButtons.Add(button);

		// buttons
		for (int i = 0; buttons[i] != "\n"; i++)
		{
			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { 180, 130 + 50 * i, 172, 40 }, ButtonType::EXTRA_LARGE, buttons[i], 10);
			button->state = GuiControlState::NONE;
			listPauseButtons.Add(button);
		}

		// Settings
		pSettings = new Settings(mod);
	}

	bool OpenPause()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 150, 70, 226, 261 }, 206, 167, 240, 230, true);
		//if (!app->render->DrawTexture(PauseTexture, 150, 70, &rect)) { app->render->TextDraw("Pause", 210, 90, 21, { 107, 0, 110 }); }
		app->render->TextDraw("Pause", 210, 90, 21, Font::UI, { 107, 0, 110 });

		if (!open_pause_B)
		{
			for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			open_pause_B = true;
		}

		return true;
	}

	bool ClosePause()
	{
		pause_B = false;
		open_pause_B = false;
		for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		ClosePause();
		//app->tex->UnLoad(PauseTexture);
		listPauseButtons.Clear();
		
		return true;
	}

public:

	// buttons
	int GUI_id = 701;
	List<GuiButton*> listPauseButtons;
	const char* buttons[5] = { "Resume", "Return to Title", "Settings", "Exit", "\n" };

	SDL_Texture* PauseTexture;
	const char* PausePath;
	bool pause_B;
	bool open_pause_B;

	Settings* pSettings;
};

#endif // __SETTINGS_H__



// Settings gui manager template

/*

	switch (a)
	{

		// Settings
	case 801:
		LOG("Button Close settings click");
		pause_P->OpenPause();
		settings_P->CloseSettings();
		break;

	case 802:
		LOG("Game settings click");

		break;

	case 803:
		LOG("Controls settings click");

		break;

	case 804:
		LOG("Graphics settings click");

		break;

	case 805:
		LOG("Audio settings click");

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

		break;

	case 830:
		LOG("Checkbox Vsync check");

		break;

	case 831:
		LOG("Button Max fps");

		break;


		// Audio settings
	case 832:
		LOG("Slider bar General volume");

		break;

	case 833:
		LOG("Slider bar Music volume");

		break;

	case 834:
		LOG("Slider bar Fx volume");

		break;
	}

*/