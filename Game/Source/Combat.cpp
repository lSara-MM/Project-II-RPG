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
	//Load
	//LoadCombat();

	//Cargar texturas
	textureBackground = app->tex->Load(texturePathBackground);
	textureTargetButton = app->tex->Load(texturePathTargetButton);

	//Poner la camara en su lugar
	app->render->camera.x = 0;
	app->render->camera.y = 0;


	// Set allies ID
	for (int i = 0; i < vecAllies.size(); i++)
	{
		//vecAllies.at(i)->type = EntityType::COMBAT_CHARA;
		vecAllies.at(i)->positionCombat_I = i;
	}


	app->entityManager->Enable();
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

	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		LOG("Change chara");

		MoveCharacter(&vecEnemies, vecEnemies.at(1), 1);
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		LOG("Change chara");

		RemoveCharacter(vecEnemies, vecEnemies.at(1));
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
	//Save al terminar
	//SaveCombat();

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
	int cPos = 0;

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

						// to delete
						//chara->Start();

						chara->charaType = CharacterType::ENEMY;
						chara->positionCombat_I = cPos++;

						vecEnemies.push_back(chara);
					}
				}

				// if list enemies full, stop checking pugi
				if (vecEnemies.size() == arr.size())	return true;
			}
		}
	}

	return true;
}

bool Combat::InitAllies(array<Character*, 4> party)
{
	int j = 0;
	for (int i = 0; i < party.size(); i++)
	{
		if (party.at(i) == nullptr)
		{
			j = i;
			break;
		}
	}

	vecAllies.insert(vecAllies.end(), begin(party), begin(party) + j);
	return true;
}

bool Combat::SaveCombat()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_stats");

	//for (c = app->scene->listAllies.start; c != NULL; c = c->next) {

	//	pugi::xml_node character = node.append_child("CombatCharacter");
	//	character.append_attribute("currentHp") = c->currentHP;//hacerlo para todas las stats

	//}
	
	ret = saveDoc->save_file("save_combat.xml");

	return ret;
}

bool Combat::LoadCombat()
{
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_combat.xml");
	pugi::xml_node node = gameStateFile.child("save_stats");

	bool ret = true;

	if (result == NULL)
	{
		LOG("Could not load xml file save_combat.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		/*for (pugi::xml_node itemNode = node.child("CombatCharacter"); itemNode != NULL; itemNode = itemNode.next_sibling("CombatCharacter"))
		{

			app->scene->listAllies[i] = itemNode.attribute("currentHp").as_int();

		}*/
	
	}

	return ret;
}

bool Combat::RestartCombatData()
{
	bool ret = true;
	//cargar config.xml
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("config.xml");
	pugi::xml_node nodeConfig = gameStateFile.child("CombatCharacter");

	if (result == NULL)
	{
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}

	else
	{

		pugi::xml_document* saveDoc = new pugi::xml_document();
		pugi::xml_node nodeCombat = saveDoc->append_child("save_stats");

		for (pugi::xml_node itemNode = nodeConfig.child("CombatCharacter"); itemNode != NULL; itemNode = itemNode.next_sibling("CombatCharacter")){

				pugi::xml_node character = nodeCombat.append_child("CombatCharacter");
				character.append_attribute("currentHp") = itemNode.attribute("currentHp").as_int();
			 
		}

		ret = saveDoc->save_file("save_combat.xml");
	}

	return ret;
}

bool Combat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);
	
	app->audio->PlayFx(control->fxControl);

	if (control->id >= 10)
	{
		LOG("%s chara",vecEnemies.at(control->id - 10)->name.GetString());
	}
	else
	{
		LOG("%s chara", vecAllies.at(control->id)->name.GetString());
	}

	// enemies so far start from 10.
	// line 159
	return true;
}


bool Combat::StartCombat()
{
	// set buttons ID
	for (int i = 0; i < vecAllies.size(); i++)
	{
		vecAllies.at(i)->button->id = i;
	}

	for (int i = 0; i < vecEnemies.size(); i++)
	{
		vecEnemies.at(i)->button->id = 10 + i;
	}

	OrderBySpeed();

	lastPressedAbility_I = 0;
	targeted_Character = nullptr;
	//for (int i = 0; i < 7; i++)
	//{
	//	EnableTargetButton(i);
	//	EnableSkillButton(i); //Es quiza una guarrada pero no deberia haber problema
	//}
	listInitiative.start->data->onTurn = true;
	charaInTurn = 0;

	return true;
}

bool Combat::OrderBySpeed()
{
	//Order by initiative
	int n = listInitiative.Count(); // ?

	for (int i = 0; i < vecAllies.size(); i++)
	{
		listInitiative.Add(vecAllies.at(i));
	}

	for (int i = 0; i < vecEnemies.size(); i++)
	{
		listInitiative.Add(vecEnemies.at(i));
	}

	//int n = listInitiative.Count();

	for (int i = 0; i < n - 1; i++) 
	{
		for (int j = 0; j < n - i - 1; j++) 
		{
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


bool Combat::NextTurn()
{
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
	return true;
}


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




void Combat::MoveCharacter(vector<Character*>* arr, Character* chara, int movement_I)
{
	//swap(arr.at(currentPosition_I), arr.at(newPosition_I));

	arr->erase(arr->begin() + chara->positionCombat_I);
	int newPos = chara->positionCombat_I + movement_I;
	
	//Evitar que se pase de posicion.
	if (newPos < 0)
	{
		newPos = 0;
	}
	if (newPos > arr->size()-1)
	{
		newPos = arr->size() - 1;
	}

	//Insertar en nueva posicion
	arr->insert(arr->begin() + newPos, chara);

	// Update combat and buttons position
	/*for (int i = 0; i < arr->size(); i++)
	{
		arr->at(i)->positionCombat_I = i;

		if (chara->charaType == CharacterType::ALLY)
		{
			arr->at(i)->button->bounds.x = 400 - 126 * arr->at(i)->positionCombat_I;
			arr->at(i)->position.x = 400 - 126 * arr->at(i)->positionCombat_I;
		}

		if (chara->charaType == CharacterType::ENEMY)
		{
			arr->at(i)->button->bounds.x = 700 + 126 * arr->at(i)->positionCombat_I;
			arr->at(i)->position.x = 700 + 126 * arr->at(i)->positionCombat_I;
		}
	}*/

	UpdatePositions(arr);
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

	UpdatePositions(&arr);
}

void Combat::UpdatePositions(vector<Character*>* arr)
{
	// Update combat and buttons position
	for (int i = 0; i < arr->size(); i++)
	{
		arr->at(i)->positionCombat_I = i;

		if (arr->at(i)->charaType == CharacterType::ALLY)
		{
			arr->at(i)->button->bounds.x = 400 - 126 * arr->at(i)->positionCombat_I;
			arr->at(i)->position.x = 400 - 126 * arr->at(i)->positionCombat_I;
		}

		if (arr->at(i)->charaType == CharacterType::ENEMY)
		{
			arr->at(i)->button->bounds.x = 700 + 126 * arr->at(i)->positionCombat_I;
			arr->at(i)->position.x = 700 + 126 * arr->at(i)->positionCombat_I;
		}
	}
}

