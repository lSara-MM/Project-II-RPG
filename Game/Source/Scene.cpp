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
#include "Combat.h"

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
	//Load Map
	app->map->Load();
	backGround = app->tex->Load("Assets/Maps/TwistedTentMap.png");

	//pause menu
	pause_B = false;

	// Settings
	pSettings->GUI_id = 0;
	pSettings->CreateSettings(this);

	// Pause 
	pPause->GUI_id = pSettings->GUI_id;
	pPause->CreatePause(this);

	//Camera pos
	app->render->camera.x = -2800;
	app->render->camera.y = -800;
	app->physics->CreateRectangle(-2850, -700, 8000, 2, bodyType::STATIC);
	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	//Render background
	app->render->DrawTexture(backGround, 0, 0);

	Debug();

	Entity* prota1 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
	app->entityManager->AddEntity(prota1);

	Entity* prota2 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
	app->entityManager->AddEntity(prota2);

	Entity* prota3 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
	app->entityManager->AddEntity(prota3);

	Entity* prota4 = app->entityManager->CreateEntity(EntityType::PC_PROTAGONIST);
	app->entityManager->AddEntity(prota4);

	Entity* enemy1 = app->entityManager->CreateEntity(EntityType::ENEMY_TANK_HOUSE);
	app->entityManager->AddEntity(enemy1);

	/*Entity* entidad2 = app->entityManager->CreateEntity(EntityType::ENEMY_TANK_HOUSE);
	app->entityManager->AddEntity(entidad2);*/
	
	//ERIC: Prueba que no funciona.
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) 
	{ 
		//!!!PONERLOS ORDENADOS, SI NO, PETA EL CODIGO Y PRINTA MENOS PERSONAJES, QUEDA�S AVISADOS!!!
		app->combat->AddCombatant((Character*)enemy1, 0);
		app->combat->AddCombatant((Character*)prota1, 3);
		app->combat->AddCombatant((Character*)prota2, 4);
		app->combat->AddCombatant((Character*)prota3, 5);
		app->combat->AddCombatant((Character*)prota4, 9);	
	}
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		app->combat->MoveAllies(1,4);
		/*app->combat->AddCombatant((Characther*)prota2, -2);
		app->combat->AddCombatant((Characther*)prota3, 5);*/
	}

	//Borrar
	float speed = 0.2 * dt;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);


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

	//player->Disable();

	app->entityManager->Disable();

	pSettings->CleanUp();
	pPause->CleanUp();
	app->guiManager->CleanUp();
	app->map->CleanUp();
	app->tex->UnLoad(backGround);

	return true;
}

void Scene::Debug()
{
	// Start again level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 0);

	// Load / Save - keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();


	// Show Gui 
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		//app->iScene->buttonDebug = !app->iScene->buttonDebug;
	}

	// Show collisions
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		!app->physics->collisions;	
	}

	// GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		!app->input->godMode_B;
	}

	// Enable/Disable Frcap
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		!frcap_B;
		LOG("frame rate: %d", app->physics->frameRate);
	}

	//pause menu
	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		!pause_B;
		!pSettings->settings_B;
		if (!pSettings->settings_B)
		{
			pSettings->CloseSettings();
		}

		LOG("PAUSE");
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		!pause_B;
		pPause->pause = !pPause->pause;
		if (!pPause->pause)
		{
			pPause->ClosePause();
		}

		LOG("PAUSE");
	}

	// Mute / unmute
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {

		!mute_B;
		LOG("MUTE");
	}
		

	// God mode functions
	if (app->input->godMode_B)
	{
		
	}

	(mute_B) ? app->audio->PauseMusic() : app->audio->ResumeMusic();
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