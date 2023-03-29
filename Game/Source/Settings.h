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

struct GameSettings
{
public:

	GameSettings* CreateGameSettings(Module* mod)
	{
		GameSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		game_B = false;
		open_game_B = false;

		return set;
	}

	bool OpenGameSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Language:", x, y + offset, 12);
		app->render->TextDraw("Text Speed:", x, y + offset * 2, 12);
		app->render->TextDraw("Return to Title:", x, y + offset * 3, 12);
		app->render->TextDraw("Exit Game:", x, y + offset * 4, 12);

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
		CloseGameSettings();
		//app->tex->UnLoad(gameSettingsTexture);
		listGameButtons.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 0;
	List<GuiButton*> listGameButtons;

	GuiSliderBar* music_P, * fx_P;

	SDL_Texture* gameSettingsTexture;
	const char* gameSettingsPath;
	bool game_B;
	bool open_game_B;
};

struct ControlSettings
{
public:

	ControlSettings* CreateControlSettings(Module* mod)
	{
		ControlSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		control_B = false;
		open_control_B = false;

		// close
		GUI_id++;
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 137, 56, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// music
		GUI_id++;
		GuiSliderBar* sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 250, 170, 63, 10 }, ButtonType::NONE, "music", 10, { 310, 165, 14, 16 });
		sliderBar->state = GuiControlState::NONE;
		music = sliderBar;
		listSliderBars.Add(sliderBar);

		// fx
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 250, 210, 63, 10 }, ButtonType::NONE, "fx", 10, { 310, 205, 14, 16 });
		sliderBar->state = GuiControlState::NONE;
		fx = sliderBar;
		listSliderBars.Add(sliderBar);


		// fullscreen	
		GUI_id++;
		GuiCheckBox* checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 330, 240, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		// vsync
		GUI_id++;
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 330, 280, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		return set;
	}

	bool OpenControlSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open_control_B)
		{
			for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}


			for (ListItem<GuiCheckBox*>* i = listCheckbox.start; i != nullptr; i = i->next)
			{
				if (i->data->id == 4 && app->win->changeScreen)
				{
					i->data->state = GuiControlState::SELECTED;
				}
				else
				{
					i->data->state = GuiControlState::NORMAL;
				}
			}

			for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			music->sliderBounds.x = music->bounds.x + app->audio->volume_music * 60 / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volume_music;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volume_fx * 60 / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volume_fx;


			open_control_B = true;
		}

		if (app->audio->volume_music != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volume_fx != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		return true;
	}

	bool CloseControlSettings()
	{
		control_B = false;
		open_control_B = false;
		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiCheckBox*>* i = listCheckbox.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		CloseControlSettings();
		//app->tex->UnLoad(settingsTexture);
		listSettingsButtons.Clear();
		listCheckbox.Clear();
		listSliderBars.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 0;
	List<GuiButton*> listSettingsButtons;
	List<GuiSliderBar*> listSliderBars;
	List<GuiCheckBox*> listCheckbox;

	GuiSliderBar* music, * fx;

	SDL_Texture* settingsTexture;
	const char* settingsPath;
	bool control_B;
	bool open_control_B;
};

struct GraphicsSettings
{
public:

	GraphicsSettings* CreateGraphicsSettings(Module* mod)
	{
		GraphicsSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		graphics_B = false;
		open_graphics_B = false;

		// close
		GUI_id++;
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 137, 56, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// music
		GUI_id++;
		GuiSliderBar* sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 250, 170, 63, 10 }, ButtonType::NONE, "music", 10, { 310, 165, 14, 16 });
		sliderBar->state = GuiControlState::NONE;
		music = sliderBar;
		listSliderBars.Add(sliderBar);

		// fx
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 250, 210, 63, 10 }, ButtonType::NONE, "fx", 10, { 310, 205, 14, 16 });
		sliderBar->state = GuiControlState::NONE;
		fx = sliderBar;
		listSliderBars.Add(sliderBar);


		// fullscreen	
		GUI_id++;
		GuiCheckBox* checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 330, 240, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		// vsync
		GUI_id++;
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 330, 280, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		return set;
	}

	bool OpenGraphics()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open_graphics_B)
		{
			for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}


			for (ListItem<GuiCheckBox*>* i = listCheckbox.start; i != nullptr; i = i->next)
			{
				if (i->data->id == 4 && app->win->changeScreen)
				{
					i->data->state = GuiControlState::SELECTED;
				}
				else
				{
					i->data->state = GuiControlState::NORMAL;
				}
			}

			for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			music->sliderBounds.x = music->bounds.x + app->audio->volume_music * 60 / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volume_music;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volume_fx * 60 / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volume_fx;


			open_graphics_B = true;
		}

		if (app->audio->volume_music != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volume_fx != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		return true;
	}

	bool CloseGraphics()
	{
		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiCheckBox*>* i = listCheckbox.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		CloseGraphics();
		//app->tex->UnLoad(settingsTexture);
		listSettingsButtons.Clear();
		listCheckbox.Clear();
		listSliderBars.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 0;
	List<GuiButton*> listSettingsButtons;
	List<GuiSliderBar*> listSliderBars;
	List<GuiCheckBox*> listCheckbox;

	GuiSliderBar* music, * fx;

	SDL_Texture* settingsTexture;
	const char* settingsPath;
	bool graphics_B;
	bool open_graphics_B;
};

struct AudioSettings
{
public:

	AudioSettings* CreateAudioSettings(Module* mod)
	{
		AudioSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		audio_B = false;
		open_audio_B = false;

		// close
		GUI_id++;
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 137, 56, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// music
		GUI_id++;
		GuiSliderBar* sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 250, 170, 63, 10 }, ButtonType::NONE, "music", 10, { 310, 165, 14, 16 });
		sliderBar->state = GuiControlState::NONE;
		music = sliderBar;
		listSliderBars.Add(sliderBar);

		// fx
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 250, 210, 63, 10 }, ButtonType::NONE, "fx", 10, { 310, 205, 14, 16 });
		sliderBar->state = GuiControlState::NONE;
		fx = sliderBar;
		listSliderBars.Add(sliderBar);
		
		return set;
	}

	bool OpenAudioSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open_audio_B)
		{
			for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			music->sliderBounds.x = music->bounds.x + app->audio->volume_music * 60 / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volume_music;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volume_fx * 60 / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volume_fx;


			open_audio_B = true;
		}

		if (app->audio->volume_music != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volume_fx != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		return true;
	}

	bool CloseAudioSettings()
	{
		audio_B = false;
		open_audio_B = false;

		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		CloseAudioSettings();
		//app->tex->UnLoad(settingsTexture);
		listSettingsButtons.Clear();
		listSliderBars.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 0;
	List<GuiButton*> listSettingsButtons;
	List<GuiSliderBar*> listSliderBars;

	GuiSliderBar* music, * fx;

	SDL_Texture* settingsTexture;
	const char* settingsPath;
	bool audio_B;
	bool open_audio_B;
};

struct Settings
{
public:

	Settings* CreateSettings(Module* mod)
	{
		Settings* set = this;
		module_P = mod;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings_B = false;
		open_settings_B = false;

		// close
		GUI_id++;
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, module_P, { 1050, 200, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// game
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, module_P, { 290, 300, 26, 28 }, ButtonType::EXTRA_LARGE, "Game", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// controls
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, module_P, { 290, 375, 26, 28 }, ButtonType::EXTRA_LARGE, "Controls", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// graphics
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, module_P, { 290, 450, 26, 28 }, ButtonType::EXTRA_LARGE, "Graphics", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// audio
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, module_P, { 290, 525, 26, 28 }, ButtonType::EXTRA_LARGE, "Audio", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		return set;
	}

	bool OpenSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 290, 203, 730, 463 }, 163, 163, 163, 200, true);
		//if (!app->render->DrawTexture(settingsTexture, 150, 70, &rect)) { app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110}); }

		app->render->DrawLine(490, 250, 490, 600, 0, 0, 0);
		app->render->TextDraw("Settings", 500, 121, 40, FONT::UI, { 255, 255, 255 });


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
		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		CloseSettings();
		//app->tex->UnLoad(settingsTexture);
		listSettingsButtons.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 0;
	List<GuiButton*> listSettingsButtons;

	SDL_Texture* settingsTexture;
	const char* settingsPath;
	bool settings_B;
	bool open_settings_B;

	Module* module_P;

	GameSettings* pGame;
	ControlSettings* pControl;
	GraphicsSettings* pGraphics;
	AudioSettings* pAudio;
};


struct Pause
{
public:

	Pause* CreatePause(Module* mod)
	{
		Pause* pause = this;

		//PauseTexture = app->tex->Load(PausePath);

		// Pause buttons
		pause = false;
		open = false;

		// close
		GUI_id++;
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

		return pause;
	}

	bool OpenPause()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 150, 70, 226, 261 }, 206, 167, 240, 230, true);
		//if (!app->render->DrawTexture(PauseTexture, 150, 70, &rect)) { app->render->TextDraw("Pause", 210, 90, 21, { 107, 0, 110 }); }
		app->render->TextDraw("Pause", 210, 90, 21, FONT::UI, { 107, 0, 110 });

		if (!open)
		{
			for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			open = true;
		}

		return true;
	}

	bool ClosePause()
	{
		pause = false;
		open = false;
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
	int GUI_id = 0;
	List<GuiButton*> listPauseButtons;
	const char* buttons[5] = { "Resume", "Return to Title", "Settings", "Exit", "\n" };

	SDL_Texture* PauseTexture;
	const char* PausePath;
	bool pause;
	bool open;
};


struct Credits
{
public:

	Credits* CreateCredits(Module* mod, int num)
	{
		Credits* Credits = this;

		//CreditsTexture = app->tex->Load(CreditsPath);

		GUI_id = num;

		// Credits buttons
		credits = false;
		open = false;

		// close
		GUI_id++;
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 137, 56, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listCreditsButtons.Add(button);

		return Credits;
	}

	bool OpenCredits()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 150, 70, 226, 261 }, 206, 167, 240, 230, true);
		//if (!app->render->DrawTexture(CreditsTexture, 150, 70, &rect)) { app->render->TextDraw("Credits", 210, 90, 21, { 107, 0, 110 }); }
		app->render->TextDraw("Credits", 195, 90, 21, FONT::UI, { 107, 0, 110 });


		app->render->TextDraw("Game by: Pikum", 160, 130, 11, FONT::UI, { 107, 0, 110 });

		int posX = 180; int posY = 150; int offset = 20;

		app->render->TextDraw("This project is", posX, posY + offset, 10, FONT::UI, { 107, 0, 110 });
		app->render->TextDraw("licensed under an", posX, posY + offset * 2, 10, FONT::UI, { 107, 0, 110 });
		app->render->TextDraw("unmodified MIT", posX, posY + offset * 3, 10, FONT::UI, { 107, 0, 110 });
		app->render->TextDraw("license", posX, posY + offset * 4, 10, FONT::UI, { 107, 0, 110 });

		if (!open)
		{
			for (ListItem<GuiButton*>* i = listCreditsButtons.start; i != nullptr; i = i->next)
			{
				i->data->state = GuiControlState::NORMAL;
			}

			open = true;
		}

		return true;
	}

	bool CloseCredits()
	{
		credits = false;
		open = false;
		for (ListItem<GuiButton*>* i = listCreditsButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		CloseCredits();
		//app->tex->UnLoad(CreditsTexture);
		listCreditsButtons.Clear();

		return true;
	}

public:

	// buttons
	int GUI_id = 0;
	List<GuiButton*> listCreditsButtons;
	const char* buttons[2] = { "x", "\n" };

	SDL_Texture* creditsTexture;
	const char* creditsPath;
	bool credits;
	bool open;
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
		LOG("Checkbox Move Up keyboard check");

		break;

	case 811:
		LOG("Checkbox Move Up gamepad check");

		break;

	case 812:
		LOG("Checkbox Move Left keyboard check");

		break;

	case 813:
		LOG("Checkbox Move Left gamepad check");

		break;

	case 814:
		LOG("Checkbox Move Right keyboard check");

		break;

	case 815:
		LOG("Checkbox Move Right gamepad check");

		break;

	case 816:
		LOG("Checkbox Move Down keyboard check");

		break;

	case 817:
		LOG("Checkbox Move Down gamepad check");

		break;

	case 818:
		LOG("Checkbox Interact keyboard check");

		break;

	case 819:
		LOG("Checkbox Interact gamepad check");

		break;

	case 820:
		LOG("Checkbox Inventory keyboard check");

		break;

	case 821:
		LOG("Checkbox Party gamepad check");

		break;

	case 822:
		LOG("Checkbox Quests keyboard check");

		break;

	case 823:
		LOG("Checkbox Quests gamepad check");

		break;

	case 824:
		LOG("Checkbox Map keyboard check");

		break;

	case 825:
		LOG("Checkbox Map gamepad check");

		break;

	case 826:
		LOG("Checkbox Settings keyboard check");

		break;

	case 827:
		LOG("Checkbox Settings gamepad check");

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