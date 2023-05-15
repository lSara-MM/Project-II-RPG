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
#include "SceneWin_Lose.h"

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

	musCombat = config.attribute("music").as_string();

	swapPath = "Assets/Audio/Fx/change_position.wav";
	swapPositionfx = app->audio->LoadFx(swapPath);

	return ret;
}

bool Combat::Start()
{
	//Load
	//LoadCombat();

	//Music combat
	app->audio->PlayMusic(musCombat, 1.0);

	//Cargar texturas
	textureBackground = app->tex->Load(texturePathBackground);
	LOG("LOAD COMBAT TEXTURE");
	textureTargetButton = app->tex->Load(texturePathTargetButton);
	textureTurnsBar = app->tex->Load(texturePathTurnsBar);

	//Poner la camara en su lugar
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	
	app->physics->Disable();
	app->entityManager->active = true;

	StartCombat();
	
	// Skill button
	GuiButton* button;	int j = 10;
	//for (int i = 0; i < 5; i++)
	//{
	//	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + 10, this, { 40 + i * 100, 470, 80, 80 }, ButtonType::COMBAT_TARGET);
	//	listButtons.Add(button);
	//	j++;
	//}
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, this, { 40 + 0, 470, 47, 47 }, ButtonType::SKILL_1);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10+1, this, { 40 + 1*100, 470, 47, 47 }, ButtonType::SKILL_2);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10+2, this, { 40 + 2*100, 470, 47, 47 }, ButtonType::SKILL_3);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10+3, this, { 40 + 3*100, 470, 47, 47 }, ButtonType::SKILL_4);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10+4, this, { 40 + 4*100, 470, 47, 47 }, ButtonType::CHANGE_POSITION);

	// Skip button
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, j, this, { 576, 85, 134, 40});
	listButtons.Add(button);

	button = nullptr;

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
	app->guiManager->CleanUp();

	//pSettings->CleanUp();

	vecAllies.clear();
	vecEnemies.clear();
	arrSetEnemies.clear();

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
	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) {
		LOG("Button handle");

		HandleCharaButtons(&vecEnemies, 0, 2);
	}
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {
		LOG("Button handle");

		HandleCharaButtons(&vecAllies, 1, 2);
	}
}

// Start Combat
bool Combat::PreLoadCombat(array<Character*, 4> arrParty_, SString n)
{
	arrParty = arrParty_;
	srand(time(NULL));

	int randSize = rand() % 3 + 2;
	int randId;

	for (int i = 0; i < randSize; i++)
	{
		randId = rand() % 3;
		arrSetEnemies.push_back(randId);
	}

	sceneFromName = n;
	return true;
}

bool Combat::InitEnemies(vector<int> arr)
{
	int cPos = 0;

	for (pugi::xml_node sceneNode = combatNode.child("scenes"); sceneNode; sceneNode = sceneNode.next_sibling("scenes"))
	{
		if (strcmp(sceneNode.attribute("name").as_string(), sceneFromName.GetString()) == 0)
		{
			for (pugi::xml_node itemNode = sceneNode.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
			{
				for (int i = 0; i < arr.size(); i++)
				{
					if (itemNode.attribute("id").as_int() == arr.at(i))
					{
						Character* chara = (Character*)app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);
						chara->parameters = itemNode;
						chara->Awake();
						
						chara->positionCombat_I = cPos++;
						chara->Start();

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
	int cPos = 0;

	// TO TEST
	Character* chara;
	for (int i = 0; i < party.size(); i++)
	{
		if (party.at(i) == nullptr) { return true; }
		chara = (Character*)app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);
		chara->parameters = party.at(i)->parameters;
		chara->Awake();

		chara->positionCombat_I = cPos++;

		chara->Start();
		chara->button->id = i;

		vecAllies.push_back(chara);
		//RELEASE(party.at(i));
	}

	//delete &chara;
	//chara = nullptr;
	return true;
}

bool Combat::StartCombat()
{
	InitAllies(arrParty);
	InitEnemies(arrSetEnemies);
	
	for (int i = 0; i < vecEnemies.size(); i++) 
	{ vecEnemies.at(i)->button->id = 5 + i; }

	OrderBySpeed();

	lastPressedAbility_I = -1;
	targeted_Character = nullptr;
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
	lastPressedAbility_I = -1;
	targeted_Character = nullptr;

	// Disable all buttons
	HandleCharaButtons(&app->combat->vecEnemies);

	listInitiative.At(charaInTurn++)->data->onTurn = false;
	
	if (listInitiative.Count() == charaInTurn) { charaInTurn = 0; }
	listInitiative.At(charaInTurn)->data->onTurn = true;
	listInitiative.At(charaInTurn)->data->ModifyHP(listInitiative.At(charaInTurn)->data->GetStat(EffectType::CURRENT_HP));

	if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ENEMY) { listButtons.end->data->state = GuiControlState::DISABLED; }
	else { listButtons.end->data->state = GuiControlState::NORMAL; }

	LOG("%s turn - num %d", listInitiative.At(charaInTurn)->data->name.GetString(), charaInTurn);

	return true;
}


// Handle buttons
void Combat::HandleCharaButtons(vector<Character*>* arr, int pos1, int pos2)
{
	for (int i = 0; i < vecAllies.size(); i++)
	{
		vecAllies.at(i)->button->state = GuiControlState::SELECTED;
		vecAllies.at(i)->button->isSelected = true;
		//LOG("disabled %d", vecAllies.at(i)->button->id);
	}

	for (int i = 0; i < vecEnemies.size(); i++)
	{
		vecEnemies.at(i)->button->state = GuiControlState::SELECTED;
		vecEnemies.at(i)->button->isSelected = true;
		//LOG("disabled %d", vecEnemies.at(i)->button->id);
	}

	if (pos1 != -1 || pos2 != -1) // If -1, disable all
	{
		for (int i = 0; i < arr->size(); i++)
		{
			if (i >= pos1 && i <= pos2)
			{
				arr->at(i)->button->state = GuiControlState::NORMAL;
				arr->at(i)->button->isSelected = false;
				//LOG("enabled %d", arr->at(i)->button->id);
			}
		}
	}
}

// TO TEST
void Combat::HandleSkillsButtons(List<Skill*> listSkills_)
{
	int offset = vecAllies.size() + vecEnemies.size();
	for (int i = 0; i < listSkills_.Count(); i++)
	{
		// If character in turn is in position to use skill, enable button;
		// skill > can be used funtion of charater in turn 
		if(listSkills_.start!=nullptr)
		{
			if (listSkills_.At(i)->data->PosCanBeUsed(listInitiative.At(charaInTurn)->data->positionCombat_I))
			{
				listButtons.At(offset + i)->data->state = GuiControlState::NORMAL;
				listButtons.At(offset + i)->data->isSelected = false;
			}
			else
			{
				listButtons.At(offset + i)->data->state = GuiControlState::SELECTED;
				listButtons.At(offset + i)->data->isSelected = true;
			}
		}
	}
}


// Combat mechanics
void Combat::MoveCharacter(vector<Character*>* arr, Character* chara, int movement_I)
{
	//swap(arr.at(currentPosition_I), arr.at(newPosition_I));
	
	int newPos = chara->positionCombat_I + movement_I;	
	
	//Evitar que se pase de posicion.
	if (arr->size() > newPos)
	{
		arr->erase(arr->begin() + chara->positionCombat_I);

		//Insertar en nueva posicion
		arr->insert(arr->begin() + newPos, chara);
		UpdatePositions(arr);
		app->audio->PlayFx(swapPositionfx);
	}
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
	if (arr->empty())
	{
		(chara->charaType == CharacterType::ALLY) ? app->sceneWin_Lose->win = false : app->sceneWin_Lose->win = true;
		// TO TEST: VA TO LENTO EL FADE, SE QUEDA COMO UN MONTON DE RATO EN EL COMBATE PARALIZADO Y LUEGO SE RALENTIZA TODO? LA ANIMACION DEL 
		// FONDO DE LOSE VA LENTO PERO EL DE WIN NO a veces XD?
		app->fade->FadingToBlack(this, (Module*)app->sceneWin_Lose, 0);
	}
	else
	{
		UpdatePositions(arr, chara->positionCombat_I);
	}
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
bool Combat::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	//Gestion Skills
	int posStart = 0, posEnd = 0;

	// target allies
	if (control->id < 5)
	{
		LOG("%s chara", vecAllies.at(control->id)->name.GetString());

		//Busca el target ya que al moverse las posiciones del vector cambia
		for (int i = 0; i < vecAllies.size(); i++)
		{
			if (vecAllies.at(i)->button->id == control->id)
			{
				targeted_Character = vecAllies.at(i);
				break;
			}
		}		
		
		if (isMoving)
		{
			int a = targeted_Character->positionCombat_I - listInitiative.At(charaInTurn)->data->positionCombat_I;
			MoveCharacter(&vecAllies, listInitiative.At(charaInTurn)->data, a);

			isMoving = false;
			NextTurn();
		}
	}
	// target enemies
	else if (control->id >= 5 && control->id < 10)
	{
		LOG("%s chara", vecEnemies.at(control->id - 5)->name.GetString());
		
		//Busca el target ya que al moverse las posiciones del vector cambia
		for (int i = 0; i < vecEnemies.size(); i++)
		{
			if (vecEnemies.at(i)->button->id == control->id)
			{
				targeted_Character = vecEnemies.at(i);
				break;
			}
		}
	} 
	// skills buttons
	else if(control->id >= 10 && control->id < 14)
	{
		isMoving = false;
		if (lastPressedAbility_I == control->id - 10) { lastPressedAbility_I = -1; } // Si already clicked deseleccionar
		lastPressedAbility_I = control->id - 10;

		posStart = listInitiative.At(charaInTurn)->data->listSkills.At(lastPressedAbility_I)->data->posToTargetStart_I;
		posEnd = listInitiative.At(charaInTurn)->data->listSkills.At(lastPressedAbility_I)->data->posToTargetEnd_I;
	}
	// move character
	else if (control->id == 14)
	{
		HandleCharaButtons(&vecAllies, 0, vecAllies.size()); 
		listInitiative.At(charaInTurn)->data->button->state = GuiControlState::SELECTED;
		isMoving = true;
	}
	else if (control->id == 15)
	{
		HandleCharaButtons(&vecAllies, 0, vecAllies.size());
		NextTurn();
	}

	//
	if (lastPressedAbility_I != -1 && !isMoving)
	{
		if (vecAllies.at(charaInTurn)->listSkills.At(lastPressedAbility_I)->data->autoTarget == true)
		{
			HandleCharaButtons(&vecAllies, vecAllies.at(charaInTurn)->positionCombat_I, vecAllies.at(charaInTurn)->positionCombat_I);
		}
		else
		{
			if (vecAllies.at(charaInTurn)->listSkills.At(lastPressedAbility_I)->data->targetFriend==true)
			{
				HandleCharaButtons(&vecAllies, posStart, posEnd);
			}
			else
			{
				HandleCharaButtons(&vecEnemies, posStart, posEnd);
			}
		}
		
	}

	return true;
}

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

		for (pugi::xml_node itemNode = nodeConfig.child("CombatCharacter"); itemNode != NULL; itemNode = itemNode.next_sibling("CombatCharacter")) {

			pugi::xml_node character = nodeCombat.append_child("CombatCharacter");
			character.append_attribute("currentHp") = itemNode.attribute("currentHp").as_int();

		}

		ret = saveDoc->save_file("save_combat.xml");
	}

	return ret;
}

