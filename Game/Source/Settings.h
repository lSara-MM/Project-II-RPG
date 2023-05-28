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

#include "Tweening.h"

#include "SDL/include/SDL.h"

class HandleSettings
{
public:

	HandleSettings() {};
	~HandleSettings() {};

	void HandleGampeadGUI(List<GuiButton*>* listButtons)
	{
		for (int i = 0; i < listButtons->Count(); i++)
		{
			if (listButtons->At(i)->data->state == GuiControlState::FOCUSED)
			{
				if (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN)
				{
					if (listButtons->At(i)->next == NULL)
					{
						listButtons->At(i)->data->state = GuiControlState::NORMAL;
						listButtons->start->data->state = GuiControlState::FOCUSED;
					}

					else
					{
						listButtons->At(i)->data->state = GuiControlState::NORMAL;
						listButtons->At(i)->next->data->state = GuiControlState::FOCUSED;
					}
				}
				else if (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN)
				{
					if (listButtons->At(i)->prev == NULL)
					{
						listButtons->At(i)->data->state = GuiControlState::NORMAL;
						listButtons->end->data->state = GuiControlState::FOCUSED;
					}

					else
					{
						listButtons->At(i)->data->state = GuiControlState::NORMAL;
						listButtons->At(i)->prev->data->state = GuiControlState::FOCUSED;
					}
				}
				break;
			}

		}

	}

	int speedAnimButton = 3;//velocidad animacion botones
	int speedAnimNotButton = 3;//velocidad animacion cosas no botones

};


class GameSettings : public HandleSettings
{
public:

	GameSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		game_B = true;
		open_game_B = false;

		// Language
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 858, 290, 133, 33 }, ButtonType::IN_SETTINGS, "English", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN,AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);

		// Text Speed
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 858, 367, 133, 33 }, ButtonType::IN_SETTINGS, "Medium", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN,AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);

		// Return to Title
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 556, 444, 133, 33 }, ButtonType::IN_SETTINGS, "Return to Title", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);

		// Exit
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 556, 521, 133, 33 }, ButtonType::IN_SETTINGS, "Exit", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		listGameButtons.Add(button);
	}

	bool OpenGameSettings()
	{
	/*	int x = 556; int y = 290; int offset = 77;
		app->render->TextDraw("Language", x, y, 16);
		app->render->TextDraw("Text Speed", x, y + offset, 16);*/

		if (!open_game_B)
		{
			for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				i->data->state = GuiControlState::NORMAL;
			}
		}
		//if (app->input->gamepadGUI_B == false && (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN
		//	|| app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN))
		//{
		//	app->input->gamepadGUI_B = true;

		//}
		//else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		//{
		//	app->input->gamepadGUI_B = false;
		//}

		//HandleGampeadGUI(&listGameButtons);

		return true;
	}

	bool CloseGameSettings()
	{
		game_B = false;
		open_game_B = false;

		for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
			//i->data->state = GuiControlState::NONE;
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

class ControlSettings : public HandleSettings
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
	/*	int x = 630; int y = 0;
		for (int i = 0; buttons[i] != "\n"; i++)
		{
			if (i == 5) { x = 900; y = 0; }

			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { x, 280 + 77 * y++, 34, 34 }, ButtonType::CONTROL_SETTINGS, buttons[i], 16, Font::UI, { 0,0,0,0 }, 5, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
			button->state = GuiControlState::NONE;
			listControlButtons.Add(button);
		}*/

		//hacer array de buttons con estos textos queda feo, lo hago hardcode

		/*int x = 580; int y = 250; int offset = 50;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 720, y, 0, 0}, ButtonType::SETTINGS_TEXT, "Game by Pikum Studios: ", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y+offset, 0, 0 }, ButtonType::SETTINGS_TEXT, "Sara Martinez Mauri", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y+offset * 2, 0, 0 }, ButtonType::SETTINGS_TEXT, "Jose Antonio Tur Izquierdo", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 3, 0, 0 }, ButtonType::SETTINGS_TEXT, "Sonia Cristina Ojeda Lanz", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 4, 0, 0 }, ButtonType::SETTINGS_TEXT, "Alberto Hidalgo Garcia", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 5, 0, 0 }, ButtonType::SETTINGS_TEXT, "Pau Garriga Brotons", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 718, y + offset * 6, 0, 0 }, ButtonType::SETTINGS_TEXT, "This project is licensed under an unmodified MIT license", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 7, 0, 0 }, ButtonType::SETTINGS_TEXT, "Copyright(c) 2020 Ray", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		x = 850;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset, 0, 0 }, ButtonType::SETTINGS_TEXT, "Andreu Nosas Soler", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 2, 0, 0 }, ButtonType::SETTINGS_TEXT, "Andreu Miro Sabate", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 3, 0, 0 }, ButtonType::SETTINGS_TEXT, "Eric Segovia Baena", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { x, y + offset * 4, 0, 0 }, ButtonType::SETTINGS_TEXT, "Guillem Pol Aixut Anderiz", 16, Font::UI, { 0,0,0,0 }, 5, Easings::BOUNCE_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::DISABLED;
		listControlButtons.Add(button);*/

	}

	bool OpenControlSettings()
	{
		//int x = 530; int y = 290; int offset = 77;
		//app->render->TextDraw("Move up", x, y, 16);
		//app->render->TextDraw("Move left", x, y + offset, 16);
		//app->render->TextDraw("Move right", x, y + offset * 2, 16);
		//app->render->TextDraw("Move down", x, y + offset * 3, 16);
		//app->render->TextDraw("Interact", x, y + offset * 4, 16);

		//x = 800;
		//app->render->TextDraw("Inventory", x, y, 16);
		//app->render->TextDraw("Party", x, y + offset, 16);
		//app->render->TextDraw("Quests", x, y + offset * 2, 16);
		//app->render->TextDraw("Map", x, y + offset * 3, 16);
		//app->render->TextDraw("Pause", x, y + offset * 4, 16);

		if (!open_control_B)
		{
			for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				//i->data->state = GuiControlState::NORMAL;
			}
	
			open_control_B = true;
		}
	/*	if (app->input->gamepadGUI_B == false && (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN
			|| app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN))
		{
			app->input->gamepadGUI_B = true;

		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		{
			app->input->gamepadGUI_B = false;
		}*/

		//HandleGampeadGUI(&listControlButtons);

		return true;
	}

	bool CloseControlSettings()
	{
		control_B = false;
		open_control_B = false;

		for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
			//i->data->state = GuiControlState::NONE;
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
	int GUI_id = 900;//no quiero que se interactue con ellos, pongo valor random
	List<GuiButton*> listControlButtons;

	const char* buttons[21] = {"W", "A", "D", "S", "E", "I", "R", "J", "M", "Esc", "\n"};

	bool control_B;
	bool open_control_B;

	int speedAnim = 3;
};

class GraphicsSettings : public HandleSettings
{
public:

	GraphicsSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		graphics_B = false;
		open_graphics_B = false;

		// Window Size
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 871, 280, 133, 33 }, ButtonType::IN_SETTINGS, "Window Size", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		listGraphicsButtons.Add(button);

		// Fullscreen	
		GUI_id++;
		GuiCheckBox* checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 871, 357, 46, 46 },ButtonType::NONE, "", 12, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		listGraphicsCheckbox.Add(checkbox);

		// Vsync
		GUI_id++;
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 871, 434, 46, 46 },ButtonType::NONE, "", 12, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
		checkbox->state = GuiControlState::NONE;
		listGraphicsCheckbox.Add(checkbox);

		// Max fps
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 871, 511, 133, 33 }, ButtonType::IN_SETTINGS, "Max fps", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		listGraphicsButtons.Add(button);
	}

	bool OpenGraphics()
	{
		/*int x = 556; int y = 290; int offset = 77;
		app->render->TextDraw("Windows Size", x, y, 16);
		app->render->TextDraw("Fullscreen", x, y + offset, 16);
		app->render->TextDraw("Vsync", x, y + offset * 2, 16);
		app->render->TextDraw("Max fps", x, y + offset * 3, 16);*/

		if (!open_graphics_B)
		{
			for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				i->data->state = GuiControlState::NORMAL;
			}

			for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				if (i->data->id == 829)
				{
					if (app->win->fullscreen)	// to change later (if as default is on, it should be shown activated 
					{
						i->data->state = GuiControlState::SELECTED;
					}
					else
					{
						i->data->state = GuiControlState::NORMAL;
					}
				}
				else if (i->data->id == 830)
				{
					if (app->render->vSync_B)	// to change later (if as default is on, it should be shown activated 
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

		/*	if (app->input->gamepadGUI_B == false && (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN
				|| app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN))
			{
				app->input->gamepadGUI_B = true;

			}
			else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
			{
				app->input->gamepadGUI_B = false;
			}*/

			//HandleGampeadGUI(&listGraphicsButtons);

			return true;
		}
	}

	bool CloseGraphics()
	{
		graphics_B = false;
		open_graphics_B = false;

		for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
			//i->data->state = GuiControlState::NONE;
		}

		for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
			//i->data->state = GuiControlState::NONE;
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

class AudioSettings : public HandleSettings
{
public:

	AudioSettings(Module* mod)
	{
		//settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		audio_B = false;
		open_audio_B = false;

		//animacion sliders en eje x no va bien
		// General volume
		GuiSliderBar* sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 743, 298, 219, 39 }, ButtonType::NONE, "general", 10, Font::UI, { 310, 298, 28, 39 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::UP_Y);
		sliderBar->state = GuiControlState::NONE;
		general = sliderBar;
		listSliderBars.Add(sliderBar);
		
		// music
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 743, 375, 219, 39 }, ButtonType::NONE, "music", 10, Font::UI, { 310, 375, 28, 39 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::UP_Y);
		sliderBar->state = GuiControlState::NONE;
		music = sliderBar;
		listSliderBars.Add(sliderBar);

		// fx
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 743, 452, 219, 39 }, ButtonType::NONE, "fx", 10, Font::UI, { 310, 452, 28, 39 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::UP_Y);
		sliderBar->state = GuiControlState::NONE;
		fx = sliderBar;
		listSliderBars.Add(sliderBar);
	}

	bool OpenAudioSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };

	/*	int x = 556; int y = 290; int offset = 77;
		app->render->TextDraw("General volume", x, y, 16);
		app->render->TextDraw("Music volume", x, y + offset, 16);
		app->render->TextDraw("Fx volume", x, y + offset * 2, 16);*/
	
		if (!open_audio_B)
		{
			for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
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
			i->data->isForward_B = false;
			//i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		/*for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
		}*/
		
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

class Settings : public HandleSettings
{
public:

	Settings(Module* mod)
	{
		settingsTexture = app->tex->Load(settingsPath);

		// settings buttons
		settings_B = false;
		open_settings_B = false;

		// close
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 1038, 175, 57, 57 }, ButtonType::CLOSE, "", 12, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		listSettingsButtons.Add(button);

		// buttons
		for (int i = 0; buttons[i] != "\n"; i++)
		{
			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { 290, 300 + 75 * i, 136, 50 }, ButtonType::SETTINGS, buttons[i], 32, Font::UI, { 0,0,0,0 }, speedAnimButton,Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
			button->state = GuiControlState::NONE;
			listSettingsButtons.Add(button);
		}

		pGame = new GameSettings(mod);
		pControl = new ControlSettings(mod);
		pGraphics = new GraphicsSettings(mod);
		pAudio = new AudioSettings(mod);

		animationMenu.Set();
		animationMenu.AddTween(100, 80, BACK_OUT);
		animationAudio.Set();
		animationAudio.AddTween(100, 80, BACK_OUT);
		animationControl.Set();
		animationControl.AddTween(100, 80, BACK_OUT);
		animationGame.Set();
		animationGame.AddTween(100, 80, BACK_OUT);
		animationGraphics.Set();
		animationGraphics.AddTween(100, 80, BACK_OUT);
	}


	bool OpenSettings()
	{
		//animaciones cosas menu
		animationMenu.Foward();
		animationMenu.Step(speedAnimNotButton, false);
		float point = animationMenu.GetPoint();
		int offsetAnimation = -750;
		//formula int(offsetAnimation + point * (0 - offsetAnimation))

		SDL_Rect rect = { 0, 0, 90, 93 };

		app->render->DrawRectangle({ 290 - app->render->camera.x, int(offsetAnimation + point * (203 - app->render->camera.y - offsetAnimation)), 730, 463 }, 163, 163, 163, 200, true);
		
		app->render->DrawTexture(settingsTexture, 450 - app->render->camera.x, int(offsetAnimation + point * (100 - app->render->camera.y - offsetAnimation)), &rect);
		rect = { 91, 0, 249, 93 };
		app->render->DrawTexture(settingsTexture, 540 - app->render->camera.x, int(offsetAnimation + point * (100 - app->render->camera.y - offsetAnimation)), &rect);
		rect = { 341, 0, 90, 93 };
		app->render->DrawTexture(settingsTexture, 789 - app->render->camera.x, int(offsetAnimation + point * (100 - app->render->camera.y - offsetAnimation)), &rect);

		//app->render->DrawLine(490 - app->render->camera.x, int(offsetAnimation + point * (250 - app->render->camera.y - offsetAnimation)), 490 - app->render->camera.x, 600 - app->render->camera.y , 0, 0, 0);
		app->render->TextDraw("Settings", 600, int(offsetAnimation + point * (121 - offsetAnimation)), 40, Font::UI, { 255, 255, 255 });


		if (pGame->game_B) { 
			pGame->OpenGameSettings();
			animationGame.Foward();
			animationGame.Step(speedAnimNotButton, false);
			float point2 = animationGame.GetPoint();
			
			int x = 556; int y = 290; int offset = 77;
			app->render->TextDraw("Language", x, int(offsetAnimation + point2 * (y - offsetAnimation)), 16);
			app->render->TextDraw("Text Speed", x, int(offsetAnimation + point2 * (y + offset - offsetAnimation)), 16);
		}

		if (pControl->control_B) { 
			pControl->OpenControlSettings();
			animationControl.Foward();
			animationControl.Step(speedAnimNotButton, false);
			float point2 = animationControl.GetPoint();

			int x = 530; int y = 250; int offset = 50;

			app->render->TextDraw("Game by Pikum Studios: ", 655, int(offsetAnimation + point2 * (y - offsetAnimation)), 16);

			app->render->TextDraw("Sara Martinez Mauri", x, int(offsetAnimation + point2 * ((y + offset) - offsetAnimation)), 16);
			app->render->TextDraw("Jose Antonio Tur Izquierdo", x, int(offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation)), 16);
			app->render->TextDraw("Sonia Cristina Ojeda Lanz", x, int(offsetAnimation + point2 * ((y + offset * 3) - offsetAnimation)), 16);
			app->render->TextDraw("Alberto Hidalgo Garcia", x, int(offsetAnimation + point2 * ((y + offset * 4) - offsetAnimation)), 16);
			app->render->TextDraw("Pau Garriga Brotons", x, int(offsetAnimation + point2 * ((y + offset * 5) - offsetAnimation)), 16);

			app->render->TextDraw("This project is licensed under an unmodified MIT license", x, int(offsetAnimation + point2 * ((y + offset * 6) - offsetAnimation)), 16);
			app->render->TextDraw("Copyright(c) 2020 Ray", x, int(offsetAnimation + point2 * ((y + offset * 7) - offsetAnimation)), 16);

			x = 800;
			app->render->TextDraw("Andreu Nosas Soler", x, int(offsetAnimation + point2 * ((y + offset) - offsetAnimation)), 16);
			app->render->TextDraw("Andreu Miro Sabate", x, int(offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation)), 16);
			app->render->TextDraw("Eric Segovia Baena", x, int(offsetAnimation + point2 * ((y + offset * 3) - offsetAnimation)), 16);
			app->render->TextDraw("Guillem Pol Aixut Anderiz", x, int(offsetAnimation + point2 * ((y + offset * 4) - offsetAnimation)), 16);
		}
		if (pGraphics->graphics_B) { 
			pGraphics->OpenGraphics();
			animationGraphics.Foward();
			animationGraphics.Step(speedAnimNotButton, false);
			float point2 = animationGraphics.GetPoint();

			int x = 556; int y = 290; int offset = 77;
			app->render->TextDraw("Windows Size", x, int(offsetAnimation + point2 * (y - offsetAnimation)), 16);
			app->render->TextDraw("Fullscreen", x, int(offsetAnimation + point2 * ((y + offset) - offsetAnimation)), 16);
			app->render->TextDraw("Vsync", x, int(offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation)), 16);
			app->render->TextDraw("Max fps", x, int(offsetAnimation + point2 * ((y + offset * 3) - offsetAnimation)), 16);
		}
		if (pAudio->audio_B) { 
			pAudio->OpenAudioSettings();
			animationAudio.Foward();
			animationAudio.Step(speedAnimNotButton, false);
			float point2 = animationAudio.GetPoint();
			int x = 556; int y = 290; int offset = 77;
			app->render->TextDraw("General volume", x, int(offsetAnimation + point2 * (y - offsetAnimation)), 16);
			app->render->TextDraw("Music volume", x, int(offsetAnimation + point2 * ((y + offset) - offsetAnimation)), 16);
			app->render->TextDraw("Fx volume", x, int(offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation)), 16);
		}


		if (!open_settings_B)
		{
			for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				i->data->state = GuiControlState::NORMAL;
			}

			open_settings_B = true;
		}

	/*	if (app->input->gamepadGUI_B == false && (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN
			|| app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN))
		{
			app->input->gamepadGUI_B = true;

		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		{
			app->input->gamepadGUI_B = false;
		}*/

		//HandleGampeadGUI(&listSettingsButtons);

		return true;
	}

	bool CloseSettings()
	{
		//animaciones cosas menu
		animationMenu.Backward();
		animationMenu.Step(speedAnimNotButton, false);
		float point = animationMenu.GetPoint();
		int offsetAnimation = -750;
		//formula int(offsetAnimation + point * (0 - offsetAnimation))

		SDL_Rect rect = { 0, 0, 90, 93 };

		app->render->DrawRectangle({ 290 - app->render->camera.x, int(offsetAnimation + point * (203 - app->render->camera.y - offsetAnimation)), 730, 463 }, 163, 163, 163, 200, true);

		app->render->DrawTexture(settingsTexture, 450 - app->render->camera.x, int(offsetAnimation + point * (100 - app->render->camera.y - offsetAnimation)), &rect);
		rect = { 91, 0, 249, 93 };
		app->render->DrawTexture(settingsTexture, 540 - app->render->camera.x, int(offsetAnimation + point * (100 - app->render->camera.y - offsetAnimation)), &rect);
		rect = { 341, 0, 90, 93 };
		app->render->DrawTexture(settingsTexture, 789 - app->render->camera.x, int(offsetAnimation + point * (100 - app->render->camera.y - offsetAnimation)), &rect);

		//app->render->DrawLine(490 - app->render->camera.x, int(offsetAnimation + point * (250 - app->render->camera.y - offsetAnimation)), 490 - app->render->camera.x, 600 - app->render->camera.y, 0, 0, 0);
		app->render->TextDraw("Settings", 600, int(offsetAnimation + point * (121 - offsetAnimation)), 40, Font::UI, { 255, 255, 255 });

		settings_B = false;
		open_settings_B = false;

		pGame->CloseGameSettings();
		pControl->CloseControlSettings();
		pGraphics->CloseGraphics();
		pAudio->CloseAudioSettings();

		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
			//i->data->state = GuiControlState::NONE;
		}

		return true;
	}

	bool CleanUp()
	{
		app->tex->UnLoad(settingsTexture);

		listSettingsButtons.Clear();
		settings_B = false;
		
		// no li agraden els clean ups aquests
		pGame->CleanUp();
		pControl->CleanUp();
		pGraphics->CleanUp();
		pAudio->CleanUp();

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
	const char* buttons[5] = { "Game", "Graphics", "Audio", "Credits", "\n" };

	SDL_Texture* settingsTexture;
	const char* settingsPath = "Assets/GUI/UI_Marker_L.png";
	bool settings_B;
	bool open_settings_B;

	
	GameSettings* pGame;
	ControlSettings* pControl;
	GraphicsSettings* pGraphics;
	AudioSettings* pAudio;

	Tween animationControl;
	Tween animationGraphics;
	Tween animationAudio;
	Tween animationGame;
	Tween animationMenu;
};


class Pause : public HandleSettings
{
public:

	Pause(Module* mod)
	{
		//PauseTexture = app->tex->Load(PausePath);

		// Pause buttons
		pause_B = false;
		open_pause_B = false;

		// close
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 1200, 57, 57, 57 }, ButtonType::CLOSE);
		button->state = GuiControlState::NONE;
		listPauseButtons.Add(button);

		for (int i = 0; buttons[i] != "\n"; i++)
		{
			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { 50, 300 + 77 * i, 155, 52 }, ButtonType::EXTRA_LARGE, buttons[i], 20, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
			button->state = GuiControlState::NONE;
			listPauseButtons.Add(button);
		}

		pauseAnimation.Set();
		pauseAnimation.AddTween(100, 80, BACK_OUT);

		// Settings
		//pSettings = new Settings(mod);
	}

	bool OpenPause()
	{
		//animaciones cosas menu
		pauseAnimation.Foward();
		pauseAnimation.Step(speedAnimNotButton, false);
		float point = pauseAnimation.GetPoint();
		int offsetAnimation = -750;
		//formula int(offsetAnimation + point * (0 - offsetAnimation))

		//SDL_Rect rect = { 0, 0, 226, 261 };

		app->audio->lowerBgMusic();

		app->render->DrawRectangle({ 0 - app->render->camera.x,int(offsetAnimation + point * (0 - app->render->camera.y-offsetAnimation)), app->win->GetWidth(), app->win->GetHeight()}, 64, 58, 57);
		//if (!app->render->DrawTexture(PauseTexture, 150, 70, &rect)) { app->render->TextDraw("Pause", 210, 90, 21, { 107, 0, 110 }); }
		app->render->TextDraw("Pause", 600, int(offsetAnimation + point * (121 - offsetAnimation)), 40, Font::UI);
		if (!open_pause_B)
		{
			for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				i->data->state = GuiControlState::NORMAL;
			}

			open_pause_B = true;
		}

	/*	if (app->input->gamepadGUI_B == false && (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN
			|| app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN))
		{
			app->input->gamepadGUI_B = true;

		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		{
			app->input->gamepadGUI_B = false;
		}*/

		//HandleGampeadGUI(&listPauseButtons);

		return true;
	}

	bool ClosePause()
	{
		app->audio->upperBgMusic();

		pauseAnimation.Backward();
		pauseAnimation.Step(speedAnimNotButton, false);
		float point = pauseAnimation.GetPoint();
		int offsetAnimation = -750;
		//formula int(offsetAnimation + point * (0 - offsetAnimation))

		//SDL_Rect rect = { 0, 0, 226, 261 };

		app->audio->lowerBgMusic();

		app->render->DrawRectangle({ 0 - app->render->camera.x,int(offsetAnimation + point * (0 - app->render->camera.y - offsetAnimation)), app->win->GetWidth(), app->win->GetHeight() }, 64, 58, 57);
		//if (!app->render->DrawTexture(PauseTexture, 150, 70, &rect)) { app->render->TextDraw("Pause", 210, 90, 21, { 107, 0, 110 }); }
		app->render->TextDraw("Pause", 600, int(offsetAnimation + point * (121 - offsetAnimation)), 40, Font::UI);

		
		pause_B = false;
		open_pause_B = false;
		for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
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
	Tween pauseAnimation;

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