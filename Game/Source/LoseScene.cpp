#include "LoseScene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "IntroScene.h"
#include "Scene.h"

#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

LoseScene::LoseScene() : Module()
{
	name.Create("loseScene");
}

// Destructor
LoseScene::~LoseScene()
{}

// Called before render is available
bool LoseScene::Awake(pugi::xml_node& config)
{
	LOG("Loading LoseScene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LoseScene::Start()
{
	SString title("You lost");
	app->win->SetTitle(title.GetString());

	// buttons
	for (int i = 0; buttons[i] != "\n"; i++)
	{
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + 1, this, { 30, 200 + 35 * i, 90, 27 }, ButtonType::LARGE, buttons[i], 11));
	}

	return true;
}

// Called each loop iteration
bool LoseScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LoseScene::Update(float dt)
{	
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->iScene, 5);

	return true;
}

// Called each loop iteration
bool LoseScene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool LoseScene::CleanUp()
{
	LOG("Freeing LoseScene");
	app->audio->PauseMusic();

	listButtons.Clear();
	app->guiManager->CleanUp();

	return true;
}

bool LoseScene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button Retry click");
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
		
		break;
	case 2:
		LOG("Button Give up click");
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);
		break;
	case 3:
		LOG("Button Leaderboard click");
		app->fade->FadingToBlack(this, (Module*)app->leadScene, 90);
		break;
	}

	return true;
}
