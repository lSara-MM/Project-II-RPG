#include "Scene.h"
#include "SceneWin_Lose.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Combat.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
using namespace std;
#include <sstream>

SceneWin_Lose::SceneWin_Lose() : Module()
{
	name.Create("SceneWin_Lose");
}

SceneWin_Lose::~SceneWin_Lose()
{}

bool SceneWin_Lose::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneWin_Lose");
	bool ret = true;

	texturepathWin = config.child("win").attribute("texturepath").as_string();
	texturepathLose = config.child("lose").attribute("texturepath").as_string();

	return ret;
}

bool SceneWin_Lose::Start()
{
	Win = app->tex->Load(texturepathWin);
	Lose = app->tex->Load(texturepathLose);

	win = false;
	lose = false;


	return true;
}

bool SceneWin_Lose::PreUpdate()
{
	return true;
}

bool SceneWin_Lose::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{

		app->render->camera.x = 0;
		app->render->camera.y = 0;
		lose = false;
		win = true;
	}
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) 
	{

		app->render->camera.x = 0;
		app->render->camera.y = 0;
		win = false;
		lose = true;
	}

	if (win)
	{
		app->render->DrawTexture(Win, 0, 0);
	}
	if (lose)
	{
		app->render->DrawTexture(Lose, 0, 0);
	}

	/*if (app->combat->win) 
	{
		app->render->DrawTexture(Win, 0, 0);
	}
	if (app->combat->lose) 
	{
		app->render->DrawTexture(Lose, 0, 0);
	}*/

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);

	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;*/

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

	return true;
}

void SceneWin_Lose::Debug()
{
	// Start again level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 0);

	// Return Title
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		app->fade->FadingToBlack(this, (Module*)app->iScene, 0);
	}

	// Load / Save - keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	// Show Gui 
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {

		//app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
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