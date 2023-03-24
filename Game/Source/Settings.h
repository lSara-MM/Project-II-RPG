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

struct Settings
{
public:

	Settings* CreateSettings(Module* mod)
	{
		Settings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings = false;
		open = false;

		// close
		GUI_id++;
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 1050, 200, 26, 28 }, ButtonType::SMALL, "x", 10);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// game
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 290, 300, 26, 28 }, ButtonType::SMALL, "Game", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// controls
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 290, 375, 26, 28 }, ButtonType::SMALL, "Controls", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// graphics
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 290, 450, 26, 28 }, ButtonType::SMALL, "Graphics", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// audio
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 290, 525, 26, 28 }, ButtonType::SMALL, "Audio", 20);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		return set;
	}

	bool OpenSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 290, 203, 730, 463 }, 163, 163, 163, 200, true);
		//if (!app->render->DrawTexture(settingsTexture, 150, 70, &rect)) { app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110}); }
		app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110 });

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open)
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

			open = true;
		}

		return true;
	}

	bool CloseSettings()
	{
		settings = false;
		open = false;
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
		CloseSettings();
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

	GuiSliderBar* music, *fx;

	SDL_Texture* settingsTexture;
	const char* settingsPath;
	bool settings;
	bool open;
};

struct GameSettings
{
public:

	GameSettings* CreateGameSettings(Module* mod)
	{
		GameSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		gameSettings = false;
		open = false;

		return set;
	}

	bool OpenGameSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 68, 165, 1095, 694 }, 163, 163, 163, 200, true);
		//if (!app->render->DrawTexture(gameSettingsTexture, 150, 70, &rect)) { app->render->TextDraw("gameSettings", 180, 100, 21, { 107, 0, 110}); }
		//app->render->TextDraw("gameSettings", 180, 100, 21, { 107, 0, 110 });

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Language:", x, y + offset, 12);
		app->render->TextDraw("Text Speed:", x, y + offset * 2, 12);
		app->render->TextDraw("Return to Title:", x, y + offset * 3, 12);
		app->render->TextDraw("Exit Game:", x, y + offset * 4, 12);

		if (!open)
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
		gameSettings = false;
		open = false;
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

	GuiSliderBar* music, * fx;

	SDL_Texture* gameSettingsTexture;
	const char* gameSettingsPath;
	bool gameSettings;
	bool open;
};

struct ControlSettings
{
public:

	ControlSettings* CreateSettings(Module* mod)
	{
		ControlSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings = false;
		open = false;

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
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod,{ 330, 280, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		return set;
	}

	bool OpenSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 150, 70, 226, 261 }, 206, 167, 240, 230, true);
		//if (!app->render->DrawTexture(settingsTexture, 150, 70, &rect)) { app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110}); }
		app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110 });

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open)
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

			music->sliderBounds.x = music->bounds.x + app->audio->volumeM * 60 / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volumeM;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volumeF * 60 / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volumeF;


			open = true;
		}

		if (app->audio->volumeM != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volumeF != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		return true;
	}

	bool CloseSettings()
	{
		settings = false;
		open = false;
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
		CloseSettings();
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
	bool settings;
	bool open;
};

struct GraphicsSettings
{
public:

	GraphicsSettings* CreateSettings(Module* mod)
	{
		GraphicsSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings = false;
		open = false;

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
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod,{ 330, 280, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		return set;
	}

	bool OpenSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 150, 70, 226, 261 }, 206, 167, 240, 230, true);
		//if (!app->render->DrawTexture(settingsTexture, 150, 70, &rect)) { app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110}); }
		app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110 });

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open)
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

			music->sliderBounds.x = music->bounds.x + app->audio->volumeM * 60 / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volumeM;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volumeF * 60 / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volumeF;


			open = true;
		}

		if (app->audio->volumeM != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volumeF != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		return true;
	}

	bool CloseSettings()
	{
		settings = false;
		open = false;
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
		CloseSettings();
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
	bool settings;
	bool open;
};

struct AudioSettings
{
public:

	AudioSettings* CreateSettings(Module* mod)
	{
		AudioSettings* set = this;

		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings = false;
		open = false;

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
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod,{ 330, 280, 30, 30 });
		checkbox->state = GuiControlState::NONE;
		listCheckbox.Add(checkbox);

		return set;
	}

	bool OpenSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

		app->render->DrawRectangle({ 150, 70, 226, 261 }, 206, 167, 240, 230, true);
		//if (!app->render->DrawTexture(settingsTexture, 150, 70, &rect)) { app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110}); }
		app->render->TextDraw("Settings", 180, 100, 21, { 107, 0, 110 });

		int x = 170; int y = 130; int offset = 40;
		app->render->TextDraw("Music:", x, y + offset, 12);
		app->render->TextDraw("Fx:", x, y + offset * 2, 12);
		app->render->TextDraw("Fullscreen:", x, y + offset * 3, 12);
		app->render->TextDraw("Vsync:", x, y + offset * 4, 12);

		if (!open)
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

			music->sliderBounds.x = music->bounds.x + app->audio->volumeM * 60 / SDL_MIX_MAXVOLUME;
			music->volume100 = app->audio->volumeM;

			fx->sliderBounds.x = fx->bounds.x + app->audio->volumeF * 60 / SDL_MIX_MAXVOLUME;
			fx->volume100 = app->audio->volumeF;


			open = true;
		}

		if (app->audio->volumeM != music->volume100)app->audio->ChangeMusicVolume(music->volume100);
		if (app->audio->volumeF != fx->volume100)app->audio->ChangeFxVolume(fx->volume100);
		return true;
	}

	bool CloseSettings()
	{
		settings = false;
		open = false;
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
		CloseSettings();
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
	bool settings;
	bool open;
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
		app->render->TextDraw("Pause", 210, 90, 21, { 107, 0, 110 });

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
		app->render->TextDraw("Credits", 195, 90, 21, { 107, 0, 110 });


		app->render->TextDraw("Game by: Pikum", 160, 130, 11, { 107, 0, 110 });

		int posX = 180; int posY = 150; int offset = 20;

		app->render->TextDraw("This project is", posX, posY + offset, 10, { 107, 0, 110 });
		app->render->TextDraw("licensed under an", posX, posY + offset * 2, 10, { 107, 0, 110 });
		app->render->TextDraw("unmodified MIT", posX, posY + offset * 3, 10, { 107, 0, 110 });
		app->render->TextDraw("license", posX, posY + offset * 4, 10, { 107, 0, 110 });

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