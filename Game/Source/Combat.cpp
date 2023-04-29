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
	
	//Activar entityManager que es lo que controlara que enemy  
	app->entityManager->Enable();

	app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);

	//Desactivar physics
	app->physics->Disable();
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

bool Combat::InitEnemies(SString scene, vector<int> arr)
{
	// to test
	for (pugi::xml_node sceneNode = combatNode.child("scenes"); sceneNode; sceneNode = sceneNode.next_sibling("scenes"))
	{
		// to test if string compare works
		if (strcmp(sceneNode.attribute("name").as_string(), scene.GetString()) == 0)
		{
			for (pugi::xml_node itemNode = sceneNode.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
			{
				for (int i = 0; i < arr.size(); i++)
				{
					if (itemNode.attribute("id").as_int() == arr[i])
					{
						Character* chara = (Character*)app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);
						chara->parameters = itemNode;
						chara->Awake();

						listEnemies.push_back(chara);					
					}
				}

				// if list enemies full, stop checking pugi
				if (listEnemies.size() == arr.size())	return true;
			}
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

	//Reactivar todos los posibles targets, los vacios desactivarlos

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




void Combat::MoveCharacter(vector<Character*> arr, Character* chara, int movement_I)
{
	//swap(arr.at(currentPosition_I), arr.at(newPosition_I));

	arr.erase(arr.begin() + chara->positionCombat_I);
	int newPos = chara->positionCombat_I + movement_I;
	
	//Evitar que se pase de posicion.
	if (newPos < 0)
	{
		newPos = 0;
	}
	if (newPos > 3)
	{
		newPos = 3;
	}

	//Insertar en nueva posicion
	chara->positionCombat_I = newPos;
	arr.insert(arr.begin() + newPos, chara);
}

void Combat::RemoveCharacter(vector<Character*> arr, Character* chara)
{
	// Delete from its type vector
	arr.erase(arr.begin() + chara->positionCombat_I);

	// Delete & free memory (should work?)

	// Delete from initiative list
	int del = listInitiative.Find(chara);
	delete listInitiative.At(del)->data;
	listInitiative.At(del)->data = nullptr;
	listInitiative.Del(listInitiative.At(del));

	// Delete from entity manager list
	app->entityManager->DestroyEntity(chara);

	/*del = app->entityManager->entities.Find(chara);
	delete app->entityManager->entities.At(del)->data;
	app->entityManager->entities.At(del)->data = nullptr;
	app->entityManager->entities.Del(app->entityManager->entities.At(del));*/
}

