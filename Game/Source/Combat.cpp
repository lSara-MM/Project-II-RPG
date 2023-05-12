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
	
	texturePathBackground = config.attribute("bgTexture").as_string(); //FondoActual (habra que cambiarlo por los de la dungeon actual)
	texturePathTargetButton = config.attribute("targetTexture").as_string();
	texturePathTurnsBar = config.attribute("turnBarTexture").as_string();

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
	textureTurnsBar = app->tex->Load(texturePathTurnsBar);

	//Poner la camara en su lugar
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->entityManager->Enable();
	app->physics->Disable();

	GuiButton* button;
	for (int i = 0; i < 5; i++)
	{
		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + 10, this, { 40 + i * 100, 470, 80, 80 });
		listButtons.Add(button);
	}

	StartCombat();

	//Load, modificar currentHP, hacer luego de cargar allies
	if (!firstCombat_B)
	{
		LoadCombat();
	}

	return true;
}

bool Combat::PreUpdate()
{


	return true;
}

bool Combat::Update(float dt)
{
	Debug();

	app->render->DrawTexture(textureBackground, 0, 0);

	// Printar Barra Turnos (UI WORK)
	int j = charaInTurn;
	for (int i = 0; i < listInitiative.Count(); i++)
	{
		if (listInitiative.At(j) == nullptr) { j = 0; }

		//El calculo largo es para que la barra este centrada siempre aprox
		SDL_Color color;
		if (listInitiative.At(j)->data->charaType == CharacterType::ALLY) { color = { 0, 255, 100, 200 }; }
		if (listInitiative.At(j)->data->charaType == CharacterType::ENEMY) { color = { 255, 0, 100, 200 }; }
		app->render->DrawRectangle({ 640 - ((int)listInitiative.Count()) * 50 + i * 110, 20, 90, 90 }, color.r, color.g, color.b, color.a);

		//El nombre es temporal, luego ira la head del character
		app->render->TextDraw(listInitiative.At(j)->data->name.GetString(), 640 - ((int)listInitiative.Count()) * 50 + i * 110, 30, 11);
		j++;
	}

	//Barra skills + name
	//app->render->DrawRectangle({ 20,450,500,120 }, 220, 220, 220);
	
	//God Mode Info
	if (app->input->godMode_B)
	{
		app->render->TextDraw("Press D to destroy first ally", 10, 80, 12);
		app->render->TextDraw("Press A to destroy first enemy", 10, 100, 12);
		app->render->TextDraw("Press 1 to move enemy[1] two positions (may crash)", 10, 120, 12);
		app->render->TextDraw("Press 2 to destroy enemy[1]", 10, 140, 12);
		app->render->TextDraw("Press 3 next turn", 10, 160, 12);
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
	//if (win)
	//{
	//	if (firstCombat_B)
	//	{
	//		firstCombat_B = false;
	//	}

	//	SaveCombat();
	//}
	
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
	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) { RemoveCharacter(&vecAllies, vecAllies.at(0)); }
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) { RemoveCharacter(&vecEnemies, vecEnemies.at(0)); }
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->input->godMode_B = !app->input->godMode_B;
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		LOG("Change chara");

		MoveCharacter(&vecEnemies, vecEnemies.at(0), 2);
	}
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		LOG("Remove chara");

		RemoveCharacter(&vecEnemies, vecEnemies.at(1));
	}
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		LOG("Next turn");

		NextTurn();
	}
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
	bool ret = true;
	// TO TEST
	for (int i = 0; i < party.size(); i++)
	{
		if (party.at(i) == nullptr) { return true; }
		vecAllies.push_back(party.at(i));
	}
	return ret;
}

bool Combat::RestartCombatData()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_stats");

	pugi::xml_node protagonist = node.append_child("protagonist");

	//todo

	pugi::xml_node bard = node.append_child("bard");

	//todo

	ret = saveDoc->save_file("save_dialogue.xml");

	return ret;
}

bool Combat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);
	
	app->audio->PlayFx(control->fxControl);

	// enemies so far start from 10.
	// line 159
	//vecAllies.insert(vecAllies.end(), begin(party), begin(party) + j);

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
		vecEnemies.at(i)->button->id = 5 + i;
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
	for (int i = 0; i < vecAllies.size(); i++)
	{
		listInitiative.Add(vecAllies.at(i));
	}

	for (int i = 0; i < vecEnemies.size(); i++)
	{
		listInitiative.Add(vecEnemies.at(i));
	}

	int n = listInitiative.Count();

	for (int i = 0; i < n - 1; i++) 
	{
		for (int j = 0; j < n - i - 1; j++) 
		{
			if (listInitiative.At(j)->data->speed < listInitiative.At(j + 1)->data->speed)
			{
				ListItem<Character*>* aux = new ListItem<Character*>(listInitiative.At(j)->data);
				listInitiative.At(j)->data = listInitiative.At(j + 1)->data;
				listInitiative.At(j + 1)->data = aux->data;
				delete aux;
				aux = nullptr;
			}
		}
	}

	return true;
}

bool Combat::NextTurn()
{
	lastPressedAbility_I = 0;
	targeted_Character = nullptr;
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
	//	if (listInitiative.Count()-1 <= charaInTurn) { charaInTurn = 0; }
	//	else
	//	{
	//		listInitiative.At(charaInTurn)->data->onTurn = false;
	//		++charaInTurn; 
	//	}
	//	listInitiative.At(charaInTurn)->data->onTurn = true;


	listInitiative.At(charaInTurn++)->data->onTurn = false;
	
	if (listInitiative.Count() == charaInTurn) { charaInTurn = 0; }
	listInitiative.At(charaInTurn)->data->onTurn = true;

	LOG("%s turn - num %d", listInitiative.At(charaInTurn)->data->name.GetString(), charaInTurn);

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
	if (skillNum < 0 || skillNum > 3)
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



// Combat mechanics
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
	UpdatePositions(arr, chara->positionCombat_I);
}

void Combat::RemoveCharacter(vector<Character*>* arr, Character* chara)
{
	// Delete from its type vector
	arr->erase(arr->begin() + chara->positionCombat_I);
	
	// Delete & free memory (should work?)

	// Delete from initiative list
	int del = listInitiative.Find(chara);
	delete listInitiative.At(del)->data;
	listInitiative.At(del)->data = nullptr;
	listInitiative.Del(listInitiative.At(del));

	// Delete from entity manager list
	app->entityManager->DestroyEntity(chara);
	UpdatePositions(arr, chara->positionCombat_I);	
}

void Combat::UpdatePositions(vector<Character*>* arr, int pos)
{
	// Update combat and buttons position
	for (int i = pos; i < arr->size(); i++)
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


// Settings
//bool Combat::OnGuiMouseClickEvent(GuiControl* control)
//{
//	LOG("Event by %d ", control->id);
//
//	app->audio->PlayFx(control->fxControl);
//
//	if (control->id >= 5)
//	{
//		LOG("%s chara", vecEnemies.at(control->id - 5)->name.GetString());
//	}
//	else
//	{
//		LOG("%s chara", vecAllies.at(control->id)->name.GetString());
//	}
//
//	// enemies so far start from 10.
//	// line 159
//	return true;
//}

// Save/Load
bool Combat::SaveCombat()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_stats");

	//for (c = app->scene->listAllies.start; c != NULL; c = c->next) {

	//	pugi::xml_node character = node.append_child("CombatCharacter");
	//	character.append_attribute("currentHp") = c->currentHP;//hacerlo para todas las stats

	//}

	for (int i = 0; i < vecAllies.size(); i++)
	{
		pugi::xml_node character = node.append_child("CombatCharacter");
		character.append_attribute("currentHp") = vecAllies[i]->currentHp;
		
	}

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
		int i = 0;

		for (pugi::xml_node itemNode = node.child("CombatCharacter"); itemNode != NULL; itemNode = itemNode.next_sibling("CombatCharacter"))
		{
			if (i<app->scene->player->listPC.size())
			{
				break;
			}
			app->scene->player->listPC[i]->currentHp = itemNode.attribute("currentHp").as_int();
			i++;
		}

	}

	return ret;
}

//bool Combat::RestartCombatData()
//{
//	bool ret = true;
//	//cargar config.xml
//	pugi::xml_document gameStateFile;
//	pugi::xml_parse_result result = gameStateFile.load_file("config.xml");
//	pugi::xml_node nodeConfig = gameStateFile.child("CombatCharacter");
//
//	if (result == NULL)
//	{
//		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
//		ret = false;
//	}
//
//	else
//	{
//
//		pugi::xml_document* saveDoc = new pugi::xml_document();
//		pugi::xml_node nodeCombat = saveDoc->append_child("save_stats");
//
//		for (pugi::xml_node itemNode = nodeConfig.child("CombatCharacter"); itemNode != NULL; itemNode = itemNode.next_sibling("CombatCharacter")) {
//
//			pugi::xml_node character = nodeCombat.append_child("CombatCharacter");
//			character.append_attribute("currentHp") = itemNode.attribute("currentHp").as_int();
//
//		}
//
//		ret = saveDoc->save_file("save_combat.xml");
//	}
//
//	return ret;
//}

