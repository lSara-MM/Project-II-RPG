#include "SceneWin_Lose.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Scene.h"
#include "Combat.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "PuzzleManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
using namespace std;
#include <sstream>

SceneWin_Lose::SceneWin_Lose() : Module()
{
	name.Create("SceneWin_Lose");
	win = false;
}

SceneWin_Lose::~SceneWin_Lose()
{}

bool SceneWin_Lose::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneWin_Lose");
	bool ret = true;

	texturepathWin = config.child("win").attribute("texturepath").as_string();
	texturepathLose = config.child("lose").attribute("texturepath").as_string();

	looseMusicPath = config.child("lose").attribute("musicl").as_string();
	winMusicPath = config.child("win").attribute("musicw").as_string();

	confirmPath = "Assets/Audio/Fx/confirm_interaction.wav";
	confirmInteractfx = app->audio->LoadFx(confirmPath);

	backgroundAnimation.Set();
	backgroundAnimation.AddTween(100, 80, BOUNCE_OUT);
	posYani_I = 0;
	return ret;
}

bool SceneWin_Lose::Start()
{
	Win = app->tex->Load(texturepathWin);
	Lose = app->tex->Load(texturepathLose);

	transition_B = false;//para animacion

	app->questManager->active = false;

	if (win)
	{
		//app->audio->PlayMusic(winMusicPath, 1.0f);
	
	}
	else
	{
		app->audio->PlayMusic(looseMusicPath, 1.0f);
	}
	//provisional
	if (!win)
	{
		continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1100, this, { 160, 130, 153, 50 }, ButtonType::IN_SETTINGS, "Continue", 16, Font::UI, { 0, 0, 0, 0 }, 1, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);
		returnButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1101, this, { 967, 130, 153, 50 }, ButtonType::IN_SETTINGS, "Return to title", 16, Font::UI, { 0, 0, 0, 0 }, 1, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);
	}
	return true;
}

bool SceneWin_Lose::PreUpdate()
{
	return true;
}

bool SceneWin_Lose::Update(float dt)
{
	//app->audio->PlayMusic(winMusicPath);
	if (transition_B)
	{
		backgroundAnimation.Backward();
		if (posYani_I==-750)
		{
			if (!win)
			{
				if (continueButton->state == GuiControlState::DISABLED)
				{
					app->fade->FadingToBlack(this, (Module*)app->iScene, 90);
				}
				else
				{
					app->LoadFromFile(this);
				}
			}
			else
			{
				app->LoadFromFile(this);
			}
			
		}
	}
	else
	{
		backgroundAnimation.Foward();
	}

	backgroundAnimation.Step(1, false);
	float point = backgroundAnimation.GetPoint();
	int offset = -750;
	posYani_I = offset + point * (0 - offset);

	if (win)
	{
		app->render->DrawTexture(Win, 0, offset + point * (0 - offset));
		app->render->TextDraw("VICTORY", 280, offset + point * (40 - offset), 150, Font::TITLE, { 255,255,255 });
		if (app->puzzleManager->fightBoss)
		{
			app->puzzleManager->bossIsDead = true;
			app->questManager->SaveState();
		}

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			app->audio->PlayFx(confirmInteractfx);
			transition_B = true;
		}
		app->render->TextDraw("Press Enter to continue", 1000, 670, 20, Font::UI, { 255, 255, 255 });
	}
	else
	{
		app->render->DrawTexture(Lose, 0, offset + point * (0 - offset));
		if (app->puzzleManager->fightBoss)
		{
			app->puzzleManager->bossIsDead = false;
			app->questManager->SaveState();
		}
	}

	app->guiManager->Draw();
	app->input->HandleGamepadMouse(app->input->mouseX, app->input->mouseY, app->input->mouseSpeed_F, dt);
	app->input->RenderMouse();

	return true;
}

bool SceneWin_Lose::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneWin_Lose::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(Win);
	app->tex->UnLoad(Lose);
	if (!win)
	{
		app->guiManager->DestroyGuiControl(continueButton);
		app->guiManager->DestroyGuiControl(returnButton);
	}
	app->questManager->active = true;

	return true;
}

void SceneWin_Lose::Debug()
{
	// Start again level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		transition_B = true;
		app->fade->FadingToBlack(this, (Module*)app->scene, 0);
	}
		
	// Return Title
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		transition_B = true;
		app->fade->FadingToBlack(this, (Module*)app->iScene, 0);
	}

	// Load / Save - keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	// Show Gui 
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {

		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
	}

	// Show collisions
	if (app->input->GetKey(SDL_SCANCODE_F8))
	{
		app->physics->collisions = !app->physics->collisions;
	}

	// Enable/Disable Frcap
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		frcap_B = !frcap_B;
		LOG("frame rate: %d", app->physics->frameRate);
	}

	// GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->physics->collisions = !app->physics->collisions;
		app->input->godMode_B = !app->input->godMode_B;
	}

	// Mute / unmute
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {

		mute_B = !mute_B;
		LOG("MUTE");
	}

	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
}

bool SceneWin_Lose::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 1100:
		LOG("Button Continue click");
		app->audio->PlayFx(confirmInteractfx);
		transition_B = true;
		continueButton->isForward_B = false;
		returnButton->isForward_B = false;
		break;
	case 1101:
		LOG("Button Return to Title click");
		transition_B = true;
		returnButton->isForward_B = false;
		continueButton->state = GuiControlState::DISABLED;
		break;
	}

	return true;
}
