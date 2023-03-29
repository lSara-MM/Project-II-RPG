#include "Scene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"


#include "IntroScene.h"
#include "LoseScene.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
using namespace std;
#include <sstream>

Scene::Scene() : Module()
{
	name.Create("scene");
}

Scene::~Scene()
{}

bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	sceneNode = config;

	return ret;
}

bool Scene::Start()
{
	//pause menu
	pause_B = false;

	// Settings
	pSettings->GUI_id = 0;
	pSettings->CreateSettings(this);

	// Pause 
	pPause->GUI_id = pSettings->GUI_id;
	pPause->CreatePause(this);

	InitEntities();
	app->entityManager->Enable();

	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{

	Debug();

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
	if (pSettings->settings_B) { pSettings->OpenSettings(); }
	if (pPause->pause) { pPause->OpenPause(); }
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	player->Disable();

	app->entityManager->Disable();

	pSettings->CleanUp();
	pPause->CleanUp();
	app->guiManager->CleanUp();
	return true;
}

void Scene::Debug()
{

}

bool Scene::InitEntities()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = sceneNode.child("player");
	player->Awake();

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	//app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 801:
		LOG("Button Close settings click");
		pPause->OpenPause();
		pSettings->CloseSettings();
		break;
	case 802:
		LOG("Slider music click");
		//app->audio->ChangeMusicVolume(pSettings->music->volume100);
		break;
	case 803:
		LOG("Slider fx click");
		//app->audio->ChangeFxVolume(pSettings->fx->volume100);
		break;
	case 804:
		LOG("Checkbox Fullscreen click");
		app->win->changeScreen = !app->win->changeScreen;
		app->win->ResizeWin();
		break;
	case 805:
		LOG("Checkbox Vsync click");
		(control->state == GuiControlState::NORMAL) ? app->render->flags = SDL_RENDERER_ACCELERATED : app->render->flags |= SDL_RENDERER_PRESENTVSYNC;
		break;
	case 806:
		LOG("Button Close pause click");
		pPause->ClosePause(); 
		break;
	case 807:
		LOG("Button Resume click");
		pPause->ClosePause();
		break;
	case 808:
		LOG("Button Return to Title click");
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);
		break;
	case 809:
		LOG("Button settings click");
		pPause->ClosePause();

		pSettings->settings_B = !pSettings->settings_B;
		if (!pSettings->settings_B) { pSettings->CloseSettings();}
		break;
	case 10:
		LOG("Button Exit game click");
		exit_B = true;
		break;
	}

	return true;
}