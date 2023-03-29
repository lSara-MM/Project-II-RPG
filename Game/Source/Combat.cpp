#include "Combat.h"

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

Combat::Combat() : Module()
{
	name.Create("combat");
}

Combat::~Combat()
{}

bool Combat::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool Combat::Start()
{
	// Settings
	pSettings->GUI_id = 0;
	pSettings->CreateSettings(this);

	// Pause 
	pPause->GUI_id = pSettings->GUI_id;
	pPause->CreatePause(this);

	return true;
}

bool Combat::PreUpdate()
{
	return true;
}

bool Combat::Update(float dt)
{
	Debug();

	/*for (int i=1;initiative.Count()>=i;i++) 
	{initiative.At(i)->data.}*/

	return true;
}

bool Combat::PostUpdate()
{
	bool ret = true;

	if (exit) return false;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
	if (pSettings->settings) { pSettings->OpenSettings(); }
	if (pPause->pause) { pPause->OpenPause(); }
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Combat::CleanUp()
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

void Combat::Debug()
{

}

bool Combat::InitEntities()
{

	return true;
}

bool Combat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button Close settings click");
		pPause->OpenPause();
		pSettings->CloseSettings();
		break;
	case 2:
		LOG("Slider music click");
		app->audio->ChangeMusicVolume(pSettings->music->volume100);
		break;
	case 3:
		LOG("Slider fx click");
		app->audio->ChangeFxVolume(pSettings->fx->volume100);
		break;
	case 4:
		LOG("Checkbox Fullscreen click");
		app->win->changeScreen = !app->win->changeScreen;
		app->win->ResizeWin();
		break;
	case 5:
		LOG("Checkbox Vsync click");
		(control->state == GuiControlState::NORMAL) ? app->render->flags = SDL_RENDERER_ACCELERATED : app->render->flags |= SDL_RENDERER_PRESENTVSYNC;
		break;
	case 6:
		LOG("Button Close pause click");
		pPause->ClosePause(); 
		break;
	case 7:
		LOG("Button Resume click");
		pPause->ClosePause();
		break;
	case 8:
		LOG("Button Return to Title click");
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);
		break;
	case 9:
		LOG("Button settings click");
		pPause->ClosePause();

		pSettings->settings = !pSettings->settings;
		if (!pSettings->settings) { pSettings->CloseSettings();}
		break;
	case 10:
		LOG("Button Exit game click");
		exit = true;
		break;
	}

	return true;
}

bool Combat::AddCombatant(Characther* chara, int mod)
{
	//We add a Characther to the initiative list and we sort it by speed (fasters first, slowers last)
	initiative.Add(chara);
	chara->speed += mod; //Aumentar o disminuir la speed de ese especifico (es para enemigos que usan la misma template)

	int n= initiative.Count();
	initiative.At(2)->data->speed;

		for (int i = 0; i < n - 1; i++)
			for (int j = 0; j < n - i - 1; j++)
				if (initiative.At(j)->data->speed > initiative.At(j+1)->data->speed)
				{
					//SWAP WIP
					ListItem<Characther*>* aux=nullptr;
					aux->data = initiative.At(j)->data;
					initiative.At(j)->data = initiative.At(j + 1)->data;
					initiative.At(j + 1)->data = aux->data;
				}
					
					
	
	
	//initiative.BubbleSort();
	//if (chara != initiative.start->data) {
	//	//Bubble Sort Method
	//	ListItem<Characther*>* aux;
	//	aux = initiative.end->next;
	//	//aux = new ListItem<Characther*>;
	//	while (chara->speed > aux->data->speed)
	//	{
	//		//ERIC: Hacer el cambio
	//	}
	//}

	return true;
}


bool Combat::NextTurn()
{
	if (initiative.Count() <= charaInTurn) { charaInTurn = 1; }
	else { ++charaInTurn; }
	initiative.At(charaInTurn)->data->onTurn = true;

	return true;
}