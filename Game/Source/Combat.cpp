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

	texturePath = "Assets/Textures/combat_background_placeholder.png";

	return ret;
}

bool Combat::Start()
{
	texture = app->tex->Load(texturePath);
	//Zona aliados
	for (int i = 0; i < 4; i++)
	{
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i, this, { 77 + i * 107, 160, 90, 180 }, ButtonType::LARGE));
	}
	//Zona enemigos, tiene un espaciado
	for (int i = 0; i < 4; i++)
	{
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4 + i, this, { 827 + i * 107, 160, 90, 180 }, ButtonType::LARGE));
	}
	//Ambos de los botones de arriba tendrian que ser tipo combat target y estos tener la textura correspondiente de "UI_button_charactherSelection"

	//Botones Acciones Turno Player
	listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, this, { 107, 510, 140, 50 }, ButtonType::START, actions[0], 20));
	listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, this, { 107 + 167, 510, 140, 50 }, ButtonType::START, actions[1], 20));

	listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, this, { 107, 600, 140, 50 }, ButtonType::START, actions[2], 20));
	listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, this, { 107 + 167, 600, 140, 50 }, ButtonType::START, actions[3], 20));

	return true;
}

bool Combat::PreUpdate()
{
	return true;
}

bool Combat::Update(float dt)
{
	Debug();

	app->render->DrawTexture(texture, 0, 0);

	//Pruebas de mover positiones
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		listInitiative.start->data->onTurn = true;
	}

	for (int i=1;listInitiative.Count()>=i;i++) 
	{
		listInitiative.At(i-1)->data->Update(dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;

	//Rectangulo donde va la Info abajo derecha {x,y,w,h} r, g, b, opacity(0 = 100% & 255 = 0%)
	app->render->DrawRectangle({ 430, 470, 730, 220 }, 255, 255, 255, 250, true);


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

	//player->Disable();

	app->entityManager->Disable();

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

	switch (control->id)
	{
	//Target 
	case 0:
		LOG("Allies Slot 1 click");
		targeted_Character = allies[3];
		break;
	case 1:
		LOG("Allies Slot 2	 click");
		targeted_Character = allies[2];
		break;
	case 2:
		LOG("Allies Slot 2 click");
		targeted_Character = allies[1];
		break;
	case 3:
		LOG("Allies Slot 3 click");
		targeted_Character = allies[0];
		break;
	case 4:
		LOG("Enemies Slot 1 click");
		targeted_Character = enemies[0];
		break;
	case 5:
		LOG("Enemies Slot 2 click");
		targeted_Character = enemies[1];
		break;
	case 6:
		LOG("Enemies Slot 3 click");
		targeted_Character = enemies[2];
		break;
	case 7:
		LOG("Enemies Slot 4 click");
		targeted_Character = enemies[3];
		break;

	//PLayer OnTurn Action Buttons
	
	case 8:
		LOG("Attack 1");
		break;

	case 9:
		LOG("Attack 2");
		break;

	case 10:
		LOG("Attack 3");
		break;

	case 11:
		LOG("Attack 4");
		break;
	//Target

		//Otros botones

		//Otros botones


	default:
		break;
	}

	app->audio->PlayFx(control->fxControl);
	return true;
}

bool Combat::AddCombatant(Character* pChara, int mod)
{
	pChara->Awake(); //Sino peta porque no se carga la textura
	pChara->Enable();
	pChara->Start();
	
	//We add a Characther to the initiative list and we sort it by speed (fasters first, slowers last)
	listInitiative.Add(pChara);
	pChara->speed += mod; //Aumentar o disminuir la speed de ese especifico (es para enemigos que usan la misma template)

	//Add to the formation list position
	switch (pChara->charaType_I)
	{
	case pChara->ALLY:
		//Asignar en su posicion en el array, si esta vacio 
		if (allies[pChara->positionCombat_I - 1] == nullptr) //Los valores son del 1 al 4 por eso le restamos 1
		{
			allies[pChara->positionCombat_I - 1] = pChara;
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
	int n = listInitiative.Count();
	listInitiative.At(2)->data->speed;

	for (int i = 0; i < n - 1; i++)
		for (int j = 0; j < n - i - 1; j++)
			if (listInitiative.At(j)->data->speed > listInitiative.At(j + 1)->data->speed)
			{
				//SWAP WIP
				ListItem<Character*>* aux = nullptr; /*new ListItem<Characther*>*/; //Esta petando el switchhh
				aux = listInitiative.At(j);
				aux->data = listInitiative.At(j)->data;
				listInitiative.At(j)->data = listInitiative.At(j + 1)->data;
				listInitiative.At(j + 1)->data = aux->data;
			}

	return true;
}


bool Combat::NextTurn()
{

	if (listInitiative.Count()-1 <= charaInTurn) { charaInTurn = 1; }
	else
	{
		listInitiative.At(charaInTurn)->data->onTurn = false;
		++charaInTurn; 
	}
	listInitiative.At(charaInTurn)->data->onTurn = true;

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

bool Combat::StartCombat()
{
	//OrderBySpeed();

	/*listInitiative.start->data->onTurn = true;*/
	//NextTurn();
	return false;
}
