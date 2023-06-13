#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "App.h"
#include "DialogueSystem.h"
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
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 750, 290, 133, 33 }, ButtonType::IN_SETTINGS, "English", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN,AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		button->color = { 255, 246, 240 };
		listGameButtons.Add(button);

		// Text Speed
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 750, 367, 133, 33 }, ButtonType::IN_SETTINGS, "Medium", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN,AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		button->color = { 255, 246, 240 };
		listGameButtons.Add(button);

		// Return to Title
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 486, 444, 133, 33 }, ButtonType::IN_SETTINGS, "Return to Title", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		button->color = { 255, 246, 240 };
		listGameButtons.Add(button);

		// Exit
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 486, 521, 133, 33 }, ButtonType::IN_SETTINGS, "Exit", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		button->color = { 255, 246, 240 };
		listGameButtons.Add(button);
	}

	bool OpenGameSettings()
	{
		if (!open_game_B)
		{
			for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
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
			i->data->isForward_B = false;
		}
		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiButton*>* i = listGameButtons.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
			app->guiManager->DestroyGuiControl(i->data);
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
		// settings buttons
		control_B = false;
		open_control_B = false;

		GuiButton* button;
	}

	bool OpenControlSettings()
	{
		if (!open_control_B)
		{
			for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
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
			i->data->isForward_B = false;
		}
		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiButton*>* i = listControlButtons.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
			app->guiManager->DestroyGuiControl(i->data);
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
		// settings buttons
		graphics_B = false;
		open_graphics_B = false;

		// Window Size
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 750, 320, 133, 33 }, ButtonType::IN_SETTINGS, "Window Size", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		button->color = { 255, 246, 240 };
		listGraphicsButtons.Add(button);

		// Fullscreen	
		GUI_id++;
		GuiCheckBox* checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 781, 377, 46, 46 },ButtonType::NONE, "", 12, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
		button->state = GuiControlState::NONE;
		listGraphicsCheckbox.Add(checkbox);

		// Vsync
		GUI_id++;
		checkbox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, GUI_id, mod, { 781, 454, 46, 46 },ButtonType::NONE, "", 12, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
		checkbox->state = GuiControlState::NONE;
		listGraphicsCheckbox.Add(checkbox);

		// Max fps
		GUI_id++;
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 750, 551, 133, 33 }, ButtonType::IN_SETTINGS, "Max fps", 16, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
		button->state = GuiControlState::NONE;
		button->color = { 255, 246, 240 };
		listGraphicsButtons.Add(button);
	}

	bool OpenGraphics()
	{
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
		}
		return true;
	}

	bool CloseGraphics()
	{
		graphics_B = false;
		open_graphics_B = false;

		for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}
		for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}
		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiButton*>* i = listGraphicsButtons.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
			app->guiManager->DestroyGuiControl(i->data);
		}

		for (ListItem<GuiCheckBox*>* i = listGraphicsCheckbox.start; i != nullptr; i = i->next)
		{
			delete i->data;
			i->data = nullptr;
			app->guiManager->DestroyGuiControl(i->data);
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
		// settings buttons
		audio_B = false;
		open_audio_B = false;

		//animacion sliders en eje x no va bien
		// General volume
		GuiSliderBar* sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 693, 348, 219, 39 }, ButtonType::NONE, "general", 10, Font::UI, { 310, 348, 28, 39 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::UP_Y);
		sliderBar->state = GuiControlState::NONE;
		general = sliderBar;
		listSliderBars.Add(sliderBar);
		
		// music
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 693, 425, 219, 39 }, ButtonType::NONE, "music", 10, Font::UI, { 310, 425, 28, 39 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::UP_Y);
		sliderBar->state = GuiControlState::NONE;
		music = sliderBar;
		listSliderBars.Add(sliderBar);

		// fx
		GUI_id++;
		sliderBar = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, GUI_id, mod, { 693, 502, 219, 39 }, ButtonType::NONE, "fx", 10, Font::UI, { 310, 502, 28, 39 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::UP_Y);
		sliderBar->state = GuiControlState::NONE;
		fx = sliderBar;
		listSliderBars.Add(sliderBar);
	}

	bool OpenAudioSettings()
	{
		SDL_Rect rect = { 0, 0, 226, 261 };	
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
		}
		return true;
	}

	bool CleanUp()
	{
		for (ListItem<GuiSliderBar*>* i = listSliderBars.start; i != nullptr; i = i->next)
		{
			app->guiManager->DestroyGuiControl(i->data);
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

class Settings : public HandleSettings
{
public:

	Settings(Module* mod)
	{
		cintaTexture = app->tex->Load(cintaPath);
		cartelTexture = app->tex->Load(cartelPath);
		sombraDchTexture = app->tex->Load(sombraDchPath);
		sombraIzqTexture = app->tex->Load(sombraIzqPath);

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
			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { 290, 300 + 75 * i, 155, 67 }, ButtonType::SETTINGS, buttons[i], 30, Font::UI, { 0,0,0,0 }, speedAnimButton,Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
			button->state = GuiControlState::NONE;
			button->color = { 255, 246, 240 };
			listSettingsButtons.Add(button);
		}

		pGame = new GameSettings(mod);
		pControl = new ControlSettings(mod);
		pGraphics = new GraphicsSettings(mod);
		pAudio = new AudioSettings(mod);

		animationMenu.Set();
		animationMenu.AddTween(100, 80, BACK_IN_OUT);
		animationAudio.Set();
		animationAudio.AddTween(100, 100, CIRCULAR_OUT);
		animationControl.Set();
		animationControl.AddTween(100, 100, CIRCULAR_OUT);
		animationGame.Set();
		animationGame.AddTween(100, 100, CIRCULAR_OUT);
		animationGraphics.Set();
		animationGraphics.AddTween(100, 100, CIRCULAR_OUT);
		animationShadow.Set();
		animationShadow.AddTween(100, 80, EXPONENTIAL_IN);
	}

	bool OpenSettings()
	{
		//animaciones cosas menu
		animationMenu.Foward();
		animationMenu.Step(speedAnimNotButton, false);
		animationShadow.Foward();
		animationShadow.Step(speedAnimNotButton, false);
		float point = animationMenu.GetPoint();
		int offsetAnimation = -750;
		//formula int(offsetAnimation + point * (0 - offsetAnimation))
	
		app->render->DrawTexture(cintaTexture, 114 - app->render->camera.x, offsetAnimation + point * (85  - offsetAnimation) - app->render->camera.y);
		app->render->DrawTexture(cartelTexture, 274 - app->render->camera.x, offsetAnimation + point * (210 - offsetAnimation) - app->render->camera.y);
		app->render->TextDraw("Settings", 565, int(offsetAnimation + point * (107 - offsetAnimation)), 40, Font::UI, { 255, 246, 240 });

		float point69 = animationShadow.GetPoint();
		int offsetAnimationDch = 1300;
		int offsetAnimationIzq = -1300;
		app->render->DrawTexture(sombraDchTexture, offsetAnimationDch + point69 * (0 - offsetAnimationDch) - app->render->camera.x, 0 - app->render->camera.y);
		app->render->DrawTexture(sombraIzqTexture, offsetAnimationIzq + point69 * (0 - offsetAnimationIzq) - app->render->camera.x, 0 - app->render->camera.y);

		if (pGame->game_B) { 
			pGame->OpenGameSettings();
			animationGame.Foward();
			animationGame.Step(speedAnimNotButton, false);
			float point2 = animationGame.GetPoint();

			int x = 506; int y = 290+10; int offset = 77;
			app->render->TextDraw("Language", x, offsetAnimation + point2 * (y - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Text Speed", x, offsetAnimation + point2 * (y + offset - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
		}

		if (pControl->control_B) { //se queda aqui el texto, hacerlo botones el juego no aguanta
			pControl->OpenControlSettings();
			animationControl.Foward();
			animationControl.Step(5, false);
			float point2 = animationControl.GetPoint();

			int x = 480; int y = 250; int offset = 50;

			app->render->TextDraw("Game by Pikum Studios: ", 605, (offsetAnimation + point2 * (y - offsetAnimation)), 16, Font::TEXT, { 255, 246, 240 });

			app->render->TextDraw("Sara Qiao Martinez Mauri", x, offsetAnimation + point2 * ((y + offset) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Jose Antonio Tur Izquierdo", x, offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Sonia Cristina Ojeda Lanz", x, offsetAnimation + point2 * ((y + offset * 3) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Alberto Hidalgo Garcia", x, offsetAnimation + point2 * ((y + offset * 4) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Pau Garriga Brotons", x, offsetAnimation + point2 * ((y + offset * 5) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });

			app->render->TextDraw("This project is licensed under an unmodified MIT license", x, offsetAnimation + point2 * ((y + offset * 6) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Copyright(c) 2020 Ray", x, offsetAnimation + point2 * ((y + offset * 7) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });

			x = 750;
			app->render->TextDraw("Andreu Nosas Soler", x, offsetAnimation + point2 * ((y + offset) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Andreu Miro Sabate", x, offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Eric Segovia Baena", x, offsetAnimation + point2 * ((y + offset * 3) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Guillem Pol Aixut Anderiz", x, offsetAnimation + point2 * ((y + offset * 4) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
		}

		if (pGraphics->graphics_B) { 
			pGraphics->OpenGraphics();
			animationGraphics.Foward();
			animationGraphics.Step(speedAnimNotButton, false);
			float point2 = animationGraphics.GetPoint();

			int x = 506; int y = 320; int offset = 77;
			app->render->TextDraw("Windows Size", x, offsetAnimation + point2 * (y - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Fullscreen", x, offsetAnimation + point2 * ((y + offset) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Vsync", x, offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Max fps", x, offsetAnimation + point2 * ((y + offset * 3) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });

		}
		if (pAudio->audio_B) { 
			pAudio->OpenAudioSettings();
			animationAudio.Foward();
			animationAudio.Step(speedAnimNotButton, false);
			float point2 = animationAudio.GetPoint();
			int x = 506; int y = 340; int offset = 77;
			app->render->TextDraw("General volume", x, offsetAnimation + point2 * (y - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Music volume", x, offsetAnimation + point2 * ((y + offset) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw("Fx volume", x, offsetAnimation + point2 * ((y + offset * 2) - offsetAnimation), 16, Font::TEXT, { 255, 246, 240 });
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

		return true;
	}

	bool CloseSettings()
	{
		//animaciones cosas menu
		animationMenu.Backward();
		animationMenu.Step(speedAnimNotButton, false);
		animationShadow.Backward();
		animationShadow.Step(speedAnimNotButton, false);
		float point = animationMenu.GetPoint();
		int offsetAnimation = -750;

		app->render->DrawTexture(cartelTexture, 274 - app->render->camera.x, offsetAnimation + point * (210 - offsetAnimation) - app->render->camera.y);
		app->render->DrawTexture(cintaTexture, 114 - app->render->camera.x, offsetAnimation + point * (85  - offsetAnimation)- app->render->camera.y);

		app->render->TextDraw("Settings", 565, offsetAnimation + point * (107 - offsetAnimation), 40, Font::UI, { 255, 246, 240 });
		int offsetAnimationDch = 1300;
		int offsetAnimationIzq = -1300;
		float point69 = animationShadow.GetPoint();
		app->render->DrawTexture(sombraDchTexture, offsetAnimationDch + point69 * (0 - offsetAnimationDch) - app->render->camera.x, 0 - app->render->camera.y);
		app->render->DrawTexture(sombraIzqTexture, offsetAnimationIzq + point69 * (0 - offsetAnimationIzq) - app->render->camera.x, 0 - app->render->camera.y);

		settings_B = false;
		open_settings_B = false;

		pGame->CloseGameSettings();
		pControl->CloseControlSettings();
		pGraphics->CloseGraphics();
		pAudio->CloseAudioSettings();

		for (ListItem<GuiButton*>* i = listSettingsButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}

		return true;
	}

	bool CleanUp()
	{
		app->tex->UnLoad(cintaTexture);

		ListItem<GuiButton*>* buttons;

		for (buttons = listSettingsButtons.start; buttons != NULL; buttons = buttons->next)
		{
			app->guiManager->DestroyGuiControl(buttons->data);
		}

		listSettingsButtons.Clear();
		settings_B = false;
	
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

	SDL_Texture* cintaTexture;
	const char* cintaPath = "Assets/GUI/CintaTitulo_Settings.png";
	SDL_Texture* cartelTexture;
	const char* cartelPath = "Assets/GUI/Cartel_Settings.png";
	SDL_Texture* sombraDchTexture;
	const char* sombraDchPath = "Assets/GUI/sombraDerecha.png";
	SDL_Texture* sombraIzqTexture;
	const char* sombraIzqPath = "Assets/GUI/sombraIzquierda.png";

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
	Tween animationShadow;
};

class Pause : public HandleSettings
{
public:

	Pause(Module* mod)
	{
		cartelpauseTexture = app->tex->Load(cartelpausePath);
		fondoTexture = app->tex->Load(fondoPath);

		// Pause buttons
		pause_B = false;
		open_pause_B = false;

		// close
		GuiButton* button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, GUI_id, mod, { 1200, 57, 57, 57 }, ButtonType::CLOSE,"", 20, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::DOWN_Y);
		button->state = GuiControlState::NONE;
		listPauseButtons.Add(button);

		for (int i = 0; buttons[i] != "\n"; i++)
		{
			button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id + 1, mod, { 50, 280 + 77 * i, 155, 67 }, ButtonType::SETTINGS, buttons[i], 17, Font::UI, { 0,0,0,0 }, speedAnimButton, Easings::CUBIC_IN, AnimationAxis::RIGHT_X);
			button->state = GuiControlState::NONE;
			listPauseButtons.Add(button);
		}

		pauseAnimation.Set();
		pauseAnimation.AddTween(100, 80, BACK_OUT);
	}
	bool OpenPause()
	{
		//animaciones cosas menu
		pauseAnimation.Foward();
		pauseAnimation.Step(speedAnimNotButton, false);
		float point = pauseAnimation.GetPoint();
		int offsetAnimation = -750;

		app->audio->lowerBgMusic();

		app->render->DrawTexture(fondoTexture, 0 - app->render->camera.x, offsetAnimation + point * (0  - offsetAnimation)- app->render->camera.y);
		app->render->DrawTexture(cartelpauseTexture, 0 - app->render->camera.x, offsetAnimation + point * (250 - offsetAnimation)- app->render->camera.y );

		app->render->TextDraw("Pause", 600, offsetAnimation + point * (121 - offsetAnimation), 40, Font::UI, { 255, 246, 240 });
		if (!open_pause_B)
		{
			for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
			{
				i->data->isForward_B = true;
				i->data->state = GuiControlState::NORMAL;
			}
			app->input->stepSound_B = true;
			open_pause_B = true;
		}
		return true;
	}

	bool ClosePause()
	{
		app->audio->upperBgMusic();

		pauseAnimation.Backward();
		pauseAnimation.Step(speedAnimNotButton, false);
		float point = pauseAnimation.GetPoint();
		int offsetAnimation = -750;

		app->audio->lowerBgMusic();
		app->render->DrawTexture(fondoTexture, 0 - app->render->camera.x, offsetAnimation + point * (0  - offsetAnimation)- app->render->camera.y);
		app->render->DrawTexture(cartelpauseTexture, 0 - app->render->camera.x, offsetAnimation + point * (250  - offsetAnimation)- app->render->camera.y);

		app->render->TextDraw("Pause", 600, offsetAnimation + point * (121 - offsetAnimation), 40, Font::UI, {255, 255, 255});

		pause_B = false;
		open_pause_B = false;
		app->input->stepSound_B = false;
		for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}
		return true;
	}

	bool CleanUp()
	{
		ClosePause();
		for (ListItem<GuiButton*>* i = listPauseButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
			i->data->state = GuiControlState::NONE;
			app->guiManager->DestroyGuiControl(i->data);
		}
		listPauseButtons.Clear();
		return true;
	}

public:

	// buttons
	int GUI_id = 701;
	List<GuiButton*> listPauseButtons;
	const char* buttons[5] = { "Resume", "Return to Title", "Settings", "Exit", "\n" };

	SDL_Texture* fondoTexture;
	const char* fondoPath = "Assets/GUI/fondo_Pause.png";
	SDL_Texture* cartelpauseTexture;
	const char* cartelpausePath = "Assets/GUI/CartelMarron_Pause.png";

	bool pause_B;
	bool open_pause_B;

	Settings* pSettings;
	Tween pauseAnimation;
};

#endif // __SETTINGS_H__