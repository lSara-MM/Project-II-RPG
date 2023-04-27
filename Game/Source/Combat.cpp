#include "Combat.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Scene.h"
#include "IntroScene.h"
#include "LoseScene.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include "Skills.h"
#include "Character.h"

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

	texturePathBackground = "Assets/Textures/combat_background_placeholder.png"; //FondoActual (habra que cambiarlo por los de la dungeon actual)
	texturePathTargetButton = "Assets/GUI/UI_button_charactherSelection.png"; //De momento lo he puesto aqui para ver como se ve pero quiza haya que borrarlos
	mouse_Speed = config.attribute("mouseSpeed").as_float();

	combatNode = config;

	return ret;
}

bool Combat::Start()
{
	//Cargar texturas
	textureBackground = app->tex->Load(texturePathBackground);
	textureTargetButton = app->tex->Load(texturePathTargetButton);

	//Poner la camara en su lugar
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	
	//Desactivar physics
	app->physics->Disable();
	//Activar entityManager que es lo que controlara que enemy  
	app->entityManager->Enable();

	app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);


	StartCombat();

	return true;
}

bool Combat::PreUpdate()
{


	return true;
}

bool Combat::Update(float dt)
{
	Debug();

	//Printar barra de Turnos Cutre momentaria
	app->render->DrawTexture(textureBackground, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->input->godMode_B = !app->input->godMode_B;
	}

	app->input->HandleGamepadMouse(mouseX_combat, mouseY_combat, mouse_Speed, dt);

	return true;
}

bool Combat::PostUpdate()
{
	bool ret = true;

	//if (exit_B) return false;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool Combat::CleanUp()
{
	LOG("Freeing scene");

	listButtons.Clear();

	//pSettings->CleanUp();
	
	app->guiManager->CleanUp();



	app->entityManager->entities.Clear();
	app->entityManager->Disable();

	listInitiative.Clear();

	//Reactivar physics
	app->physics->Enable();

	return true;
}

void Combat::Debug()
{

}

bool Combat::InitEntities()
{
	for (pugi::xml_node itemNode = combatNode.child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		Character* chara = (Character*)app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);
		chara->parameters = itemNode;
		chara->Awake();

		if (chara->charaType == CharacterType::ALLY)
		{
			alliesList.push_back(chara);
		}
		else if (chara->charaType == CharacterType::ENEMY)
		{
			enemiesList.push_back(chara);
		}
		
	}
	return true;
}

bool Combat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	return true;
}

bool Combat::AddCombatant(int id)
{
	

	return true;
}

bool Combat::OrderBySpeed()
{
	//Order by initiative
	int n = listInitiative.Count();
	listInitiative.At(2)->data->speed;

	for (int i = 0; i < n - 1; i++) {
		
		for (int j = 0; j < n - i - 1; j++) {

			if (listInitiative.At(j)->data->speed > listInitiative.At(j + 1)->data->speed)
			{
				//SWAP WIP
				ListItem<Character*>* aux = new ListItem<Character*>(listInitiative.At(j)->data); /*new ListItem<Characther*>*/; //Esta petando el switchhh
				/*aux = listInitiative.At(j);
				aux->data = listInitiative.At(j)->data;*/
				listInitiative.At(j)->data = listInitiative.At(j + 1)->data;
				listInitiative.At(j + 1)->data = aux->data;
				delete aux;
			}
		}
		
	}
		
	return true;
}

bool Combat::EliminateCombatant(Character* chara)
{
	if (chara==nullptr)
	{
		return false;
	}

	////Mover chara al fondo antes de matarlo para ordenar los arrays
	//switch (chara->charaType_I)
	//{
	//case chara->CharacterType::ALLY:
	//	for (int i = 3; i >= 0; i--)
	//	{
	//		if (allies[i] != nullptr)
	//		{
	//			MoveAllies(chara->positionCombat_I, i+1);
	//			delete allies[i];
	//			allies[i]=nullptr;
	//			break;
	//		}
	//	}
	//	
	//	
	//	break;

	//case chara->CharacterType::ENEMY:
	//	for (int i = 3; i >= 0; i--)
	//	{
	//		if (enemies[i] != nullptr)
	//		{
	//			MoveEnemies(chara->positionCombat_I, i+1);
	//			enemies[i] = nullptr;
	//			break;
	//		}
	//	}
	//	break;

	//case chara->CharacterType::NONE:
	//	break;

	//default:
	//	break;
	//}
	//app->entityManager->DestroyEntity(chara);
	////chara->Disable();
	//listInitiative.Del(listInitiative.At(listInitiative.Find(chara)));

	return true;
}

bool Combat::StartCombat()
{
	OrderBySpeed();

	lastPressedAbility_I = 0;
	targeted_Character = nullptr;
	for (int i = 0; i < 7; i++)
	{
		EnableTargetButton(i);
		EnableSkillButton(i); //Es quiza una guarrada pero no deberia haber problema
	}
	//listInitiative.start->data->onTurn = true;
	charaInTurn = 0;
	
	return true;
}

//bool Combat::NextTurn()
//{
//	//Resetear los botones targeteados
//	lastPressedAbility_I = 0;
//	targeted_Character = nullptr;
//
//	//Reactivar todos los posibles targets, los vacios desactivarlos
//
//	for (int i = 0; i < 7; i++)
//	{
//		EnableTargetButton(i);
//	}
//	for (int i = 1; i < 4; i++)
//	{
//		EnableSkillButton(i);
//	}
//
//	//Si algo esta vacio desactivarlo
//	for (int i = 0; i <= 3; i++)
//	{
//		if (enemies[i] == nullptr) { DisableTargetButton(4 + i); }
//	}
//	for (int i = 0; i <= 3; i++)
//	{
//		if (allies[3-i] == nullptr) { DisableTargetButton(i); }
//	}
//	
//	
//
//
//
//	if (listInitiative.Count()-1 <= charaInTurn) { charaInTurn = 0; }
//	else
//	{
//		listInitiative.At(charaInTurn)->data->onTurn = false;
//		++charaInTurn; 
//	}
//	listInitiative.At(charaInTurn)->data->onTurn = true;
//
//	return true;
//}

//bool Combat::MoveAllies(int charaPosition_I, int newPosition_I)
//{
//	//DUDA: Esto solo es para evitar errores de acceso , no se si quitarlo porque teoricamente no se deberia poder poner esos valores.
//	if (charaPosition_I>4||charaPosition_I<1) {return false;}
//	//Si te fueras a salir del array te empuja hacia el limite (4 backline, 1 frontline) pero no te deja sobrepasarte
//	if (newPosition_I > 4) { newPosition_I = 4; } 
//	if (newPosition_I < 1) { newPosition_I = 1; }
//	//Evitar que se acceda a un nullptr
//	if (allies[newPosition_I-1] == nullptr || allies[charaPosition_I-1] == nullptr) {return false;}
//
//	//Guardar las referencias a cosas
//	Character* aux = new Character;
//	aux = allies[charaPosition_I - 1]; //Ally que queremos mover.
//
//	//En caso de avanzar los desplaza hacia atras. (los otros characthers)
//	if (charaPosition_I > newPosition_I) //Avanzar hacia la frontline
//	{	
//		for (size_t i = charaPosition_I-1; i > newPosition_I-1; i--)//Desplazar hacia atras a los demas
//		{
//			allies[i] = allies[i - 1];
//			allies[i]->positionCombat_I = i + 1;
//		}
//	}
//	//En caso de retroceder los avanza hacia adelante. (los otros characthers)
//	if (charaPosition_I < newPosition_I) //Retroceder a la backline
//	{
//		for (int i = charaPosition_I - 1; i < newPosition_I - 1; i++)//Desplazar hacia atras a los demas
//		{
//			allies[i] = allies[i + 1];
//			if(allies[i]!=nullptr)
//			{
//			allies[i]->positionCombat_I = i + 1;
//			}
//		}
//
//	}
//
//	allies[newPosition_I - 1]=aux;//Colocamos el alliado en la posicion objetivo
//	allies[newPosition_I - 1]->positionCombat_I = newPosition_I;
//
//	return true;
//}
//
//bool Combat::MoveEnemies(int charaPosition_I, int newPosition_I)
//{
//	//DUDA: Esto solo es para evitar errores de acceso , no se si quitarlo porque teoricamente no se deberia poder poner esos valores.
//	if (charaPosition_I > 4 || charaPosition_I < 0) { return false; }
//	if (newPosition_I > 4 || newPosition_I < 0) { return false; }
//	//Evitar que se acceda a un nullptr
//	if (enemies[newPosition_I - 1] == nullptr || enemies[charaPosition_I - 1] == nullptr) { return false; }
//
//	//Guardar las referencias a cosas
//	Character* aux = new Character;
//	aux = enemies[charaPosition_I - 1]; //Ally que queremos mover.
//
//	//En caso de avanzar los desplaza hacia atras. (los otros characthers)
//	if (charaPosition_I > newPosition_I) //Avanzar hacia la frontline
//	{
//		for (size_t i = charaPosition_I - 1; i > newPosition_I - 1; i--)//Desplazar hacia atras a los demas
//		{
//			enemies[i] = enemies[i - 1];
//			enemies[i]->positionCombat_I = i + 1;
//		}
//	}
//	//En caso de retroceder los avanza hacia adelante. (los otros characthers)
//	if (charaPosition_I < newPosition_I) //Retroceder a la backline
//	{
//		for (int i = charaPosition_I - 1; i < newPosition_I - 1; i++)//Desplazar hacia atras a los demas
//		{
//			enemies[i] = enemies[i + 1];
//			if (allies[i] != nullptr)
//			{
//				enemies[i]->positionCombat_I = i + 1;
//			}
//		}
//
//	}
//
//	enemies[newPosition_I - 1] = aux;//Colocamos el alliado en la posicion objetivo
//	enemies[newPosition_I - 1]->positionCombat_I = newPosition_I;
//
//	return true;
//}



bool Combat::DisableTargetButton(int id)
{
	//Evitar que pete o acceder a botones que no deberia 
	if (id<0 || id > 7)
	{
		return false;
	}
	//if (id > 7)
	//{
	//	return false;
	//}

	//listButtons.At(id)->data->state = GuiControlState::DISABLED;


	return true;
}

bool Combat::EnableTargetButton(int id)
{
	//Evitar que pete o acceder a botones que no deberia 
	if (id < 0 || id > 7)
	{
		return false;
	}
	/*if (id > 7)
	{
		return false;
	}*/

	//listButtons.At(id)->data->state = GuiControlState::NORMAL;


	return true;
}

bool Combat::EnableSkillButton(int skillNum)
{
	//Evitar que pete o acceder a botones que no deberia 
	if (skillNum < 1 || skillNum > 4)
	{
		return false;
	}
	//if (skillNum > 4)
	//{
	//	return false;
	//}

	//listButtons.At(7 + skillNum)->data->state = GuiControlState::NORMAL;

	return true;
}

bool Combat::DisableSkillButton(int skillNum)
{
	//Evitar que pete o acceder a botones que no deberia 
	if (skillNum < 1 || skillNum > 4)
	{
		return false;
	}
	//if (skillNum > 4)
	//{
	//	return false;
	//}

	//listButtons.At(7 + skillNum)->data->state = GuiControlState::DISABLED;

	return true;
}
