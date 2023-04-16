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

#include "Characther.h"

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
	//pSettings->GUI_id = 0;
	//pSettings->CreateSettings(this);

	// Pause 
	//pPause->CreatePause(this);

	return true;
}

bool Combat::PreUpdate()
{
	return true;
}

bool Combat::Update(float dt)
{
	Debug();

	//Pruebas de mover positiones

	for (int i=1;initiative.Count()>=i;i++) 
	{
		initiative.At(i-1)->data->Update(dt);
	}



	return true;
}

bool Combat::PostUpdate()
{
	bool ret = true;

	if (exit) return false;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
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
	//pPause->CleanUp();
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

	
	return true;
}

bool Combat::AddCombatant(Character* pChara, int mod)
{
	pChara->Awake(); //Sino peta porque no se carga la textura
	pChara->Enable();
	pChara->Start();
	
	//We add a Characther to the initiative list and we sort it by speed (fasters first, slowers last)
	initiative.Add(pChara);
	pChara->speed += mod; //Aumentar o disminuir la speed de ese especifico (es para enemigos que usan la misma template)

	//Add to the formation list position
	switch (pChara->charaType_I)
	{
	case pChara->ALLY:
		//Asignar en su posicion en el array, si esta vacio 
		if (allies[pChara->positionCombat_I-1]==nullptr) //Los valores son del 1 al 4 por eso le restamos 1
		{
			allies[pChara->positionCombat_I-1] = pChara;
		}
		else
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (allies[i] == nullptr) //Los valores son del 1 al 4 por eso le restamos 1
				{
					allies[i] = pChara;
					pChara->positionCombat_I = (i + 1);
					break;
				}
			}
		}

		break;
	case pChara->ENEMY:
		//Asignar en su posicion en el array, si esta vacio 
		if (enemies[pChara->positionCombat_I - 1] == nullptr) //Los valores son del 1 al 4 por eso le restamos 1
		{
			enemies[pChara->positionCombat_I - 1] = pChara;
		}
		else
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (enemies[i] == nullptr) //Los valores son del 1 al 4 por eso le restamos 1
				{
					enemies[i] = pChara;
					pChara->positionCombat_I = (i + 1);
					break;
				}
			}
		}

		break;
	default:
		break;
	}

	return true;
}

bool Combat::OrderBySpeed()
{

	//Order by initiative
	int n = initiative.Count();
	initiative.At(2)->data->speed;

	for (int i = 0; i < n - 1; i++)
		for (int j = 0; j < n - i - 1; j++)
			if (initiative.At(j)->data->speed > initiative.At(j + 1)->data->speed)
			{
				//SWAP WIP
				ListItem<Character*>* aux = nullptr; /*new ListItem<Characther*>*/; //Esta petando el switchhh
				aux = initiative.At(j);
				aux->data = initiative.At(j)->data;
				initiative.At(j)->data = initiative.At(j + 1)->data;
				initiative.At(j + 1)->data = aux->data;
			}

	return true;
}


bool Combat::NextTurn()
{
	if (initiative.Count() <= charaInTurn) { charaInTurn = 1; }
	else { ++charaInTurn; }
	initiative.At(charaInTurn)->data->onTurn = true;

	return true;
}

bool Combat::MoveAllies(int charaPosition_I, int newPosition_I)
{
	//DUDA: Esto solo es para evitar accesos de acceso , no se si quitarlo porque teoricamente no se deberia poder poner esos valores.
	if (charaPosition_I>4||charaPosition_I<0)
	{
		return false;
	}
	if (newPosition_I > 4 || newPosition_I < 0)
	{
		return false;
	}

	//Guardar las referencias a cosas
	Character* aux = new Character;
	aux = allies[charaPosition_I - 1]; //Ally que queremos mover.

	//En caso de avanzar los desplaza hacia atras. (los otros characthers)
	if (charaPosition_I > newPosition_I) //Avanzar hacia la frontline
	{	
		for (size_t i = charaPosition_I-1; i > newPosition_I-1; i--)//Desplazar hacia atras a los demas
		{
			allies[i] = allies[i - 1];
			allies[i]->positionCombat_I = i + 1;
		}
	}
	//En caso de retroceder los avanza hacia adelante. (los otros characthers)
	if (charaPosition_I < newPosition_I) //Retroceder a la backline
	{
		for (size_t i = charaPosition_I - 1; i < newPosition_I - 1; i++)//Desplazar hacia atras a los demas
		{
			allies[i] = allies[i + 1];
			if(allies[i]!=nullptr)
			{
			allies[i]->positionCombat_I = i - 1;
			}
		}

	}

	allies[newPosition_I - 1]=aux;//Colocamos el alliado en la posicion objetivo

	return true;
}
