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
#include "QuestManager.h"
#include "ItemManager.h"

#include "EntityManager.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Inventory.h"

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
	PathlastSelectedSkill = config.attribute("lastSkillTexture").as_string();

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

	//Load inventory
	app->inventory->Enable();

	//Music combat
	app->audio->PlayMusic(musCombat, 1.0);

	//Cargar texturas
	textureBackground = app->tex->Load(texturePathBackground);
	LOG("LOAD COMBAT TEXTURE");
	textureTargetButton = app->tex->Load(texturePathTargetButton);
	textureTurnsBar = app->tex->Load(texturePathTurnsBar);

	profileTex = app->tex->Load("Assets/Textures/CombatUISuperiorPart.png");
	skillTex = app->tex->Load("Assets/Textures/CombatUIHabInfoPart.png");

	textureLastSelectedSkill = app->tex->Load(PathlastSelectedSkill);

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
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, this, { 311 + 0, 433, 47, 47 }, ButtonType::SKILL_1, "", 12, Font::UI, { 0,0,0,0 }, 2, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
	listButtons.Add(button);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10 + 1, this, { 311 + 1 * 65, 433, 47, 47 }, ButtonType::SKILL_2, "", 12, Font::UI, { 0,0,0,0 }, 2, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
	listButtons.Add(button);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10 + 2, this, { 311 + 2 * 65, 433, 47, 47 }, ButtonType::SKILL_3, "", 12, Font::UI, { 0,0,0,0 }, 2, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
	listButtons.Add(button);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10 + 3, this, { 311 + 3 * 65, 433, 47, 47 }, ButtonType::SKILL_4, "", 12, Font::UI, { 0,0,0,0 }, 2, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
	listButtons.Add(button);
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10 + 4, this, { 311 + 4 * 65, 433, 47, 47 }, ButtonType::CHANGE_POSITION, "", 12, Font::UI, { 0,0,0,0 }, 2, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
	listButtons.Add(button);


	// Skip button
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, this, { 542, 100, 129, 43}, ButtonType::SKIPPY, "", 12, Font::UI, { 0,0,0,0 }, 2, Easings::CUBIC_IN);
	listButtons.Add(button);

	button = nullptr;

	//Load, modificar currentHP, hacer luego de cargar allies
	if (!firstCombat_B)
	{
		LoadCombat();
	}

	//Quest Manager desactivate

	if (app->questManager->active)
	{
		app->questManager->active = false;
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

		int x = 640 - ((int)listInitiative.Count()) * 50 + i * 110;

		//El calculo largo es para que la barra este centrada siempre aprox
		SDL_Color color;
		if (listInitiative.At(j)->data->charaType == CharacterType::ALLY) { color = { 33, 180, 33, 170 }; }
		if (listInitiative.At(j)->data->charaType == CharacterType::ENEMY) { color = { 180, 33, 33, 170 }; }
		app->render->DrawRectangle({ x, 20, 76, 76 }, color.r, color.g, color.b, color.a);

		
		app->render->DrawTexture(listInitiative.At(j)->data->texture, x + 1, 21, &listInitiative.At(j)->data->texSection);
		j++;
	}

	//GUI chara in turn
	SDL_Rect rect = { 0, 0, 588, 179 };
	SDL_Rect rect2 = { 0, 0, 588, 90 };
	
	switch (listInitiative.At(charaInTurn)->data->id)
	{
	case 0:
		app->render->DrawTexture(skillTex, 36, 527, &rect);
		app->render->DrawTexture(profileTex, 38, 407, &rect2);
		break;
	case 1:
		rect.y = 179;
		rect2.y = 90;
		app->render->DrawTexture(skillTex, 36, 527, &rect);
		app->render->DrawTexture(profileTex, 38, 407, &rect2);
		break;
	case 2:
		rect.y = 179 * 2;
		rect2.y = 90 * 2;
		app->render->DrawTexture(skillTex, 36, 527, &rect);
		app->render->DrawTexture(profileTex, 38, 407, &rect2);
		break;
	case 3:
		rect.y = 179 * 3;
		rect2.y = 90 * 3;
		app->render->DrawTexture(skillTex, 36, 527, &rect);
		app->render->DrawTexture(profileTex, 38, 407, &rect2);
		break;
	default:
		break;
	}
	
	//God Mode Info
	if (app->input->godMode_B)
	{
		app->render->TextDraw("Press 1 to destroy first ally", 10, 40, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 2 to destroy first enemy", 10, 60, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 3 next turn", 10, 80, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 4 enemies button handle", 10, 100, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 5 allies button handle", 10, 120, 12, Font::UI, { 255, 255, 255 });
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

	for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
	{

		SDL_Rect rect = { 0, 0, 53, 53 };
		int offset = 3;

		switch (i->data->buttonType)
		{

		case ButtonType::SKILL_1:
			if (lastPressedAbility_I==0)
			{
				app->render->DrawTexture(textureLastSelectedSkill, i->data->bounds.x- offset, i->data->bounds.y- offset, &rect);
			}
			break;
		case ButtonType::SKILL_2:
			if (lastPressedAbility_I == 1)
			{
				app->render->DrawTexture(textureLastSelectedSkill, i->data->bounds.x - offset, i->data->bounds.y- offset, &rect);
			}
			break;
		case ButtonType::SKILL_3:
			if (lastPressedAbility_I == 2)
			{
				app->render->DrawTexture(textureLastSelectedSkill, i->data->bounds.x - offset, i->data->bounds.y- offset, &rect);
			}
			break;
		case ButtonType::SKILL_4:
			if (lastPressedAbility_I == 3)
			{
				app->render->DrawTexture(textureLastSelectedSkill, i->data->bounds.x - offset, i->data->bounds.y- offset, &rect);
			}
			break;

		case ButtonType::COMBAT_TARGET:
			
			rect = { 0,0,i->data->bounds.w,i->data->bounds.w };

			app->render->DrawTexture(textureLastSelectedSkill, i->data->bounds.x, i->data->bounds.y, &rect);
			break;
		default:
			break;
		}
	}

	return ret;
}

// Called before quitting
bool Combat::CleanUp()
{
	LOG("Freeing scene");
	//Save al terminar
	if (app->sceneWin_Lose->win == true)
	{
		if (firstCombat_B)
		{
			firstCombat_B = false;
		}

		SaveCombat();
	}

	if (skillTex != nullptr)
		app->tex->UnLoad(skillTex);
	if (profileTex != nullptr)
		app->tex->UnLoad(profileTex);
	
	listButtons.Clear();
	app->guiManager->CleanUp();

	//pSettings->CleanUp();

	vecAllies.clear();
	vecAllies.shrink_to_fit();
	vecEnemies.clear();
	vecEnemies.shrink_to_fit();
	arrSetEnemies.clear();
	arrSetEnemies.shrink_to_fit();

	app->entityManager->entities.Clear();
	app->entityManager->Disable();

	listInitiative.Clear();

	//Reactivar physics
	app->physics->Enable();

	//QuestManager reactivate
	if (!app->questManager->active)
	{
		app->questManager->active = true;
	}

	//Unload inventory
	app->inventory->Disable();

	return true;
}

void Combat::Debug()
{
	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) { vecAllies.at(0)->ModifyHP(-99999); }
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) { vecEnemies.at(0)->ModifyHP(-99999); }
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 
	{
		app->fade->FadingToBlack(this, (Module*)app->scene, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->input->godMode_B = !app->input->godMode_B;
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

	if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ENEMY) 
	{ 
		listButtons.end->prev->data->state = GuiControlState::DISABLED;	// change position
		listButtons.end->data->state = GuiControlState::DISABLED;	// skip
		HandleSkillsButtons(listInitiative.At(charaInTurn)->data);
	}
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

bool Combat::HandleSkillsButtons(Character* chara)
{
	int offset = vecAllies.size() + vecEnemies.size();

	for (int i = 0; i < chara->listSkills.Count(); i++)
	{
		if (chara->listSkills.At(i)->data->posToUseStart_I<= chara->positionCombat_I && chara->positionCombat_I <= chara->listSkills.At(i)->data->posToUseEnd_I && chara->charaType==CharacterType::ALLY)
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

	return true;
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
	int posInVec = SearchInSkills(vecAllies, listInitiative.At(charaInTurn)->data);

	// target allies
	if (control->id < 5)
	{
		posInVec = SearchInVec(vecAllies, control->id);
		targeted_Character = vecAllies.at(posInVec);

		LOG("%s chara", vecAllies.at(posInVec)->name.GetString());

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
		//Busca el target ya que al moverse las posiciones del vector cambia
		posInVec = SearchInVec(vecEnemies, control->id);
		targeted_Character = vecEnemies.at(posInVec); 
		
		LOG("%s chara", vecEnemies.at(posInVec)->name.GetString());
	} 
	// skills buttons
	else if (control->id >= 10 && control->id < 14)
	{
		isMoving = false;

		if (lastPressedAbility_I == control->id - 10)
		{
			lastPressedAbility_I = -1;
		} // Si already clicked deseleccionar
		lastPressedAbility_I = control->id - 10;

		posStart = vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->posToTargetStart_I;
		posEnd = vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->posToTargetEnd_I;
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
		if (vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->autoTarget == true)
		{
			HandleCharaButtons(&vecAllies, vecAllies.at(posInVec)->positionCombat_I, vecAllies.at(posInVec)->positionCombat_I);
		}
		else
		{
			if (vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->targetFriend == true)
			{
				HandleCharaButtons(&vecAllies, posStart, posEnd);
			}
			else
			{
				HandleCharaButtons(&vecEnemies, posStart, posEnd);
			}
		}
	}
	if (lastPressedAbility_I == -1 && !isMoving)
	{
		for (int i = 0; i < listInitiative.Count(); i++)
		{
			HandleCharaButtons(&vecAllies);
		}
	}

	return true;
}

bool Combat::OnGuiMouseHoverEvent(GuiControl* control)
{
	//GUI character
	SDL_Rect rect = { 0,0,588,179 };
	SDL_Rect rect2 = { 0,0,588,90 };

	int xText1 = 60;//origen texto stat izq
	int xNumber1 = 200;//origen numeor stat izq
	int xText2 = 400;//origen numeor stat dch
	int xNumber2 = 546;//origen numeor stat dch
	int offsetY = 35;//offset distancia entre linias
	int fontSize = 25;
	int posY = 535;//y botones

	if (control->id<=4)
	{
		string maxHP_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->maxHp);
		const char* c_hp = maxHP_C.c_str();
		string attack_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->attack);
		const char* c_attack = attack_C.c_str();
		string critR_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->critRate);
		const char* c_critR = critR_C.c_str();
		string critD_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->critDamage);
		const char* c_critD = critD_C.c_str();
		string precision_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->precision);
		const char* c_precision = precision_C.c_str();
		string position_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->positionCombat_I);
		const char* c_position = position_C.c_str();
		string armor_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->armor);
		const char* c_armor = armor_C.c_str();
		string dodge_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->dodge);
		const char* c_dodge = dodge_C.c_str();
		string resist_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->res);
		const char* c_resist = resist_C.c_str();
		string speed_C = std::to_string(vecAllies.at(SearchInVec(vecAllies, control->id))->speed);
		const char* c_speed = speed_C.c_str();

		switch (control->id)
		{
		case 0:
			app->render->DrawTexture(skillTex, 36, 527, &rect);
			app->render->DrawTexture(profileTex, 38, 407, &rect2);

			app->render->TextDraw("MaxHP", xText1, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_hp, xNumber1, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Attack", xText1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_attack, xNumber1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Rate", xText1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critR, xNumber1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Dmg", xText1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critD, xNumber1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Precision", xText1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_precision, xNumber1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Position", xText2, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_position, xNumber2, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Armor", xText2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_armor, xNumber2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Dodge", xText2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_dodge, xNumber2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Res", xText2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_resist, xNumber2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Speed", xText2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_speed, xNumber2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });

			break;
		case 1:
			rect.y = 179;
			rect2.y = 90;
			app->render->DrawTexture(skillTex, 36, 527, &rect);
			app->render->DrawTexture(profileTex, 38, 407, &rect2);

			app->render->TextDraw("MaxHP", xText1, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_hp, xNumber1, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Attack", xText1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_attack, xNumber1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Rate", xText1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critR, xNumber1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Dmg", xText1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critD, xNumber1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Precision", xText1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_precision, xNumber1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Position", xText2, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_position, xNumber2, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Armor", xText2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_armor, xNumber2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Dodge", xText2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_dodge, xNumber2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Res", xText2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_resist, xNumber2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Speed", xText2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_speed, xNumber2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			break;
		case 2:
			rect.y = 179 * 2;
			rect2.y = 90 * 2;
			app->render->DrawTexture(skillTex, 36, 527, &rect);
			app->render->DrawTexture(profileTex, 38, 407, &rect2);

			app->render->TextDraw("MaxHP", xText1, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_hp, xNumber1, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Attack", xText1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_attack, xNumber1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Rate", xText1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critR, xNumber1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Dmg", xText1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critD, xNumber1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Precision", xText1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_precision, xNumber1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Position", xText2, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_position, xNumber2, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Armor", xText2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_armor, xNumber2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Dodge", xText2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_dodge, xNumber2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Res", xText2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_resist, xNumber2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Speed", xText2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_speed, xNumber2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			break;
		case 3:
			rect.y = 179 * 3;
			rect2.y = 90 * 3;
			app->render->DrawTexture(skillTex, 36, 527, &rect);
			app->render->DrawTexture(profileTex, 38, 407, &rect2);

			app->render->TextDraw("MaxHP", xText1, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_hp, xNumber1, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Attack", xText1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_attack, xNumber1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Rate", xText1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critR, xNumber1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Dmg", xText1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critD, xNumber1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Precision", xText1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_precision, xNumber1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Position", xText2, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_position, xNumber2, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Armor", xText2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_armor, xNumber2, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Dodge", xText2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_dodge, xNumber2, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Res", xText2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_resist, xNumber2, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Speed", xText2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_speed, xNumber2, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			break;
		default:
			break;
		}
	}
	

	if (control->id >= 10 && control->id < 14)
	{
		if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ALLY)
		{
			Skill* skillPoint = listInitiative.At(charaInTurn)->data->listSkills.At(control->id - 10)->data;
			//Name
			app->render->TextDraw(skillPoint->name.GetString(), 48, 545, 25);

			//Damage or Heal
			string DMG_C;
			if (skillPoint->multiplierDmg > 0) //Heal
			{
				DMG_C = std::to_string((int)(skillPoint->multiplierDmg * listInitiative.At(charaInTurn)->data->maxHp / 5));
			}
			else { DMG_C = std::to_string((int)(skillPoint->multiplierDmg * listInitiative.At(charaInTurn)->data->GetStat(EffectType::ATTACK))); }
			const char* ch_DMG = DMG_C.c_str();
			app->render->TextDraw(ch_DMG, 335, 550, 18);

			//Efect
			SString effecto_C;
			switch ((EffectType)skillPoint->status)
			{
			case EffectType::NONE:
				effecto_C = "None";
				break;
			case EffectType::CURRENT_HP:
				if (skillPoint->positiveEffect) { effecto_C = "Regeneration"; }
				else { effecto_C = "Burn"; }
				break;
			case EffectType::ATTACK:
				if (skillPoint->positiveEffect) { effecto_C = "Buff ATK"; }
				else { effecto_C = "Debuff ATK"; }
				break;
			case EffectType::CRIT_RATE:
				if (skillPoint->positiveEffect) { effecto_C = "Buff Crit Rate"; }
				else { effecto_C = "DebuffCritRate"; }
				break;
			case EffectType::CRIT_DMG:
				if (skillPoint->positiveEffect) { effecto_C = "BuffCritDMG"; }
				else { effecto_C = "Debuff Crit DMG"; }
				break;
			case EffectType::PRECISION:
				if (skillPoint->positiveEffect) { effecto_C = "Buff Precision"; }
				else { effecto_C = "Debuff Precision"; }
				break;
			case EffectType::ARMOR:
				if (skillPoint->positiveEffect) { effecto_C = "Buff Armor"; }
				else { effecto_C = "Debuff Armor"; }
				break;
			case EffectType::DODGE:
				if (skillPoint->positiveEffect) { effecto_C = "Buff Dodge"; }
				else { effecto_C = "Debuff Dodge"; }
				break;
			case EffectType::RES:
				if (skillPoint->positiveEffect) { effecto_C = "Buff Resistance"; }
				else { effecto_C = "Debuff Resistance"; }
				break;
			default:
				break;
			}
			app->render->TextDraw(effecto_C.GetString(), 480, 550, 18);
			
			//Description
			app->render->TextDraw(skillPoint->description.GetString(), 55, 620, 15);
		}
	}

	return false;
}

int Combat::SearchInSkills(vector<Character*> arr, Character* chara)
{
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr.at(i) == chara) { return i; }
	}

	return -1;
}

// Busqueda mediante ID para onGuiClickEvent
int Combat::SearchInVec(vector<Character*> arr, int id)
{
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr.at(i)->button->id == id) { return i; }
	}

	return -1;
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
			if (i < app->itemManager->vecPC.size())
			{
				break;
			}
			app->itemManager->vecPC.at(i)->currentHp = itemNode.attribute("currentHp").as_int();
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

