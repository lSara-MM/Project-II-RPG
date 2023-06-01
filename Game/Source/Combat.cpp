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
#include "HouseOfTerrors.h"

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

	//mouse_Speed = config.attribute("mouseSpeed").as_float();

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
	app->inventory->Disable();

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

	//Load, modificar currentHP, hacer luego de cargar allies
	if (!firstCombat_B)
	{
		LoadCombat();
	}

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

	//Quest Manager desactivate

	if (app->questManager->active)
	{
		app->questManager->active = false;
	}

	//Load inventory
	app->inventory->Enable();

	return true;
}

bool Combat::PreUpdate()
{
	//TODO : Guarrada maxima
	if (charaInTurn >= listInitiative.Count()) 
	{
		charaInTurn = listInitiative.Count() - 1;
	}
	if (!listInitiative.At(charaInTurn)->data->onTurn)
	{
		NextTurn();
	}

	return true;
}

bool Combat::Update(float dt)
{
	Debug();

	app->input->GetMousePosition(mouseX_combat, mouseY_combat);

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
	
	if (listInitiative.Count() > charaInTurn)
	{
		//Printar recuadro info
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

		if (lastPressedAbility_I ==-1 && listInitiative.At(charaInTurn)->data->charaType==CharacterType::ALLY)
		{
			listInitiative.At(charaInTurn);

			int xText1 = 60;//origen texto stat izq
			int xNumber1 = 200;//origen numeor stat izq
			int xText2 = 400;//origen numeor stat dch
			int xNumber2 = 546;//origen numeor stat dch
			int offsetY = 35;//offset distancia entre linias
			int fontSize = 25;
			int posY = 535;//y botones

			string maxHP_C = std::to_string(listInitiative.At(charaInTurn)->data->maxHp);
			const char* c_hp = maxHP_C.c_str();
			string attack_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::ATTACK));
			const char* c_attack = attack_C.c_str();
			string critR_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::CRIT_RATE));
			const char* c_critR = critR_C.c_str();
			string critD_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::CRIT_DMG));
			const char* c_critD = critD_C.c_str();
			string accuracy_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::ACCURACY));
			const char* c_accuracy = accuracy_C.c_str();
			string position_C = std::to_string(listInitiative.At(charaInTurn)->data->positionCombat_I);
			const char* c_position = position_C.c_str();
			string armor_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::ARMOR));
			const char* c_armor = armor_C.c_str();
			string dodge_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::DODGE));
			const char* c_dodge = dodge_C.c_str();
			string resist_C = std::to_string(listInitiative.At(charaInTurn)->data->GetStat(EffectType::RES));
			const char* c_resist = resist_C.c_str();
			string speed_C = std::to_string(listInitiative.At(charaInTurn)->data->speed);
			const char* c_speed = speed_C.c_str();


			app->render->TextDraw("MaxHP", xText1, posY, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_hp, xNumber1, posY, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Attack", xText1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_attack, xNumber1, posY + offsetY * 1, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Rate", xText1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critR, xNumber1, posY + offsetY * 2, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("Crit. Dmg", xText1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_critD, xNumber1, posY + offsetY * 3, fontSize, Font::UI, { 0, 0, 0 });

			app->render->TextDraw("accuracy", xText1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });
			app->render->TextDraw(c_accuracy, xNumber1, posY + offsetY * 4, fontSize, Font::UI, { 0, 0, 0 });

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
		}
		else
		{
			if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ALLY)
			{
				Skill* skillPoint = listInitiative.At(charaInTurn)->data->listSkills.At(lastPressedAbility_I)->data;
				//Name
				app->render->TextDraw(skillPoint->name.GetString(), 48, 545, 25);

				//Stats to get
				string DMG_C;
				string Pre_C;
				string CrRate_C;
				string CrDMG_C;

				//Damage or Heal
				if (skillPoint->multiplierDmg > 0) //Heal
				{
					app->render->TextDraw("Healing:", 260, 550, 18);
					DMG_C = to_string((int)(skillPoint->multiplierDmg * listInitiative.At(charaInTurn)->data->maxHp / 5));
					const char* ch_DMG = DMG_C.c_str();
					app->render->TextDraw(ch_DMG, 335, 550, 18);
				}
				else
				{
					app->render->TextDraw("Damage:", 260, 537, 18);
					DMG_C = to_string((int)(skillPoint->multiplierDmg * listInitiative.At(charaInTurn)->data->GetStat(EffectType::ATTACK)));
					const char* ch_DMG = DMG_C.c_str();
					app->render->TextDraw(ch_DMG, 335, 537, 18);

					//Stats ataques
					Pre_C = to_string((int)listInitiative.At(charaInTurn)->data->GetStatModifier(EffectType::ACCURACY) * (skillPoint->bonusAccuracy + listInitiative.At(charaInTurn)->data->accuracy));
					CrRate_C = to_string((int)listInitiative.At(charaInTurn)->data->GetStatModifier(EffectType::CRIT_RATE) * (skillPoint->bonusCritRate + listInitiative.At(charaInTurn)->data->critRate));
					CrDMG_C = to_string((int)listInitiative.At(charaInTurn)->data->GetStatModifier(EffectType::CRIT_DMG) * (skillPoint->bonusCritRate + listInitiative.At(charaInTurn)->data->critDamage));

					//Printar stats
					app->render->TextDraw("Accuracy.", 260, 560, 15);
					const char* ch_Pre = Pre_C.c_str();
					app->render->TextDraw(ch_Pre, 330, 560, 15);

					app->render->TextDraw("Cr.Rate.", 260, 575, 15);
					const char* ch_CrRate = CrRate_C.c_str();
					app->render->TextDraw(ch_CrRate, 330, 575, 15);

					app->render->TextDraw("Cr.Dmg.", 260, 590, 15);
					const char* ch_CrDMG = CrDMG_C.c_str();
					app->render->TextDraw(ch_CrDMG, 330, 590, 15);
				}

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
					else { effecto_C = "Debuff Crit Rate"; }
					break;
				case EffectType::CRIT_DMG:
					if (skillPoint->positiveEffect) { effecto_C = "BuffCritDMG"; }
					else { effecto_C = "Debuff Crit DMG"; }
					break;
				case EffectType::ACCURACY:
					if (skillPoint->positiveEffect) { effecto_C = "Buff accuracy"; }
					else { effecto_C = "Debuff accuracy"; }
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
				app->render->RenderTrimmedText(55, 620, 5, skillPoint->description.GetString(), &auxTexts, 15, 90);
			}
		}
	}
	
	//God Mode Info
	if (app->input->godMode_B)
	{
		app->render->TextDraw("Press F3 instant win", 10, 20, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press F4 instant lose", 10, 40, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 1 to destroy first ally", 10, 60, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 2 to destroy first enemy", 10, 80, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 3 next turn", 10, 100, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 4 enemies button handle", 10, 120, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 5 allies button handle", 10, 140, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 6 fully heal party", 1100, 20, 12, Font::UI, { 255, 255, 255 });
		app->render->TextDraw("Press 7 become inmortal", 1100, 40, 12, Font::UI, { 255, 255, 255 });
	}
	
	app->input->HandleGamepadMouse(mouseX_combat, mouseY_combat, app->input->mouseSpeed_F, dt);

	return true;
}

bool Combat::PostUpdate()
{
	bool ret = true;

	//if (exit_B) return false;	

	//if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;
	//
	app->guiManager->Draw();

	//Poner en reborde blanco la habilidad seleccionada? Ahora mismo no parece hacer nada
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

	
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
	{
		app->render->DrawTexture(app->input->cursorPressedTex, mouseX_combat, mouseY_combat);
	}

	else
	{
		app->render->DrawTexture(app->input->cursorIdleTex, mouseX_combat, mouseY_combat);
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

		for (int i = 0; i < vecAllies.size(); i++)
		{
			// TO DO: WHEN A CHARA HAS HP = 0, IT SHOULD DIE

			// If es el mismo chara, cambiale el hp, sino se queda igual por lo que el chara muerto revive a 5 hp
			if (strcmp(app->itemManager->arrParty.at(i)->name.GetString(), vecAllies.at(i)->name.GetString()) == 0)
			{
				app->itemManager->arrParty.at(i)->currentHp = vecAllies.at(i)->currentHp;
			}
			else
			{
				app->itemManager->arrParty.at(i)->currentHp = 5;
			}
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

	//Para load de player en hTerror
	app->hTerrors->combatEnd = true;

	//Unload inventory
	app->inventory->Disable();


	app->input->coso = true;

	return true;
}

void Combat::Debug()
{	
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->input->godMode_B = !app->input->godMode_B;
	}

	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			app->fade->FadingToBlack(this, (Module*)app->scene, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			app->sceneWin_Lose->win = true;
			app->fade->FadingToBlack(this, (Module*)app->sceneWin_Lose, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		{
			app->sceneWin_Lose->win = false;
			app->fade->FadingToBlack(this, (Module*)app->sceneWin_Lose, 0);
		}


		// num
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			vecAllies.at(0)->ModifyHP(-99999);
		}

		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			vecEnemies.at(0)->ModifyHP(-99999);
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
		if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) {
			LOG("Button handle");

			for (int i = 0; i < vecAllies.size(); i++)
			{
				vecAllies.at(i)->ModifyHP(99999);
			}
		}
	}
}

// Start Combat
bool Combat::PreLoadCombat(SString n, int boss)
{
	srand(time(NULL));

	int randSize = rand() % 3 + 2;
	int randId;

	if (boss != -1)
	{
		arrSetEnemies.push_back(boss);
	}
	else
	{
		for (int i = 0; i < randSize; i++)
		{
			randId = rand() % 3;
			arrSetEnemies.push_back(randId);
		}
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

		// TO DO, guarrada provisional
		vecAllies.at(i)->maxHp = app->itemManager->arrParty.at(i)->maxHp;
		vecAllies.at(i)->currentHp = app->itemManager->arrParty.at(i)->currentHp;
		vecAllies.at(i)->attack = app->itemManager->arrParty.at(i)->attack;
		vecAllies.at(i)->critRate = app->itemManager->arrParty.at(i)->critRate;
		vecAllies.at(i)->critDamage = app->itemManager->arrParty.at(i)->critDamage;
		vecAllies.at(i)->accuracy = app->itemManager->arrParty.at(i)->accuracy;
		vecAllies.at(i)->armor = app->itemManager->arrParty.at(i)->armor;
		vecAllies.at(i)->dodge = app->itemManager->arrParty.at(i)->dodge;
		vecAllies.at(i)->res = app->itemManager->arrParty.at(i)->res;
		vecAllies.at(i)->speed = app->itemManager->arrParty.at(i)->speed;

		//RELEASE(party.at(i));
	}


	//delete &chara;
	//chara = nullptr;

	/*for (int i = 0; i < party.size(); i++)
	{
		if (party.at(i) == nullptr) { break; }

		party.at(i)->isCombatant = true;
		party.at(i)->positionCombat_I = i;
		vecAllies.push_back(party.at(i));
		vecAllies.at(i)->Start();
	}*/

	return true;
}

bool Combat::StartCombat()
{
	InitAllies(app->itemManager->arrParty);
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

//Turn Managment
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

	//TO TEST, new method ERIC
	//Quit turn previous chara 
	if (charaInTurn>= listInitiative.Count())
	{
		charaInTurn = listInitiative.Count() - 1;
	}
	listInitiative.At(charaInTurn)->data->onTurn = false;
	
	//Change turn
	charaInTurn++;
	if (charaInTurn >= listInitiative.Count())
	{
		charaInTurn = 0;
	}
	listInitiative.At(charaInTurn)->data->onTurn = true;

	//Control botones
	if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ENEMY)
	{
		listButtons.end->prev->data->state = GuiControlState::DISABLED;	// change position
		listButtons.end->data->state = GuiControlState::DISABLED;	// skip
		HandleSkillsButtons(listInitiative.At(charaInTurn)->data);
	}
	else
	{
		listButtons.end->prev->data->state = GuiControlState::NORMAL;
		listButtons.end->data->state = GuiControlState::NORMAL;
	}

	//Status effects
	int DoT = listInitiative.At(charaInTurn)->data->GetStat(EffectType::CURRENT_HP);
	listInitiative.At(charaInTurn)->data->ModifyHP(DoT);
	listInitiative.At(charaInTurn)->data->ReduceCountStatusEffects();

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
	
	//Si se pasa ponerlo en el limite
	if (newPos < 0) { newPos = 0; }
	if (newPos > arr->size()) { newPos = arr->size()-1; }

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
	listButtons.Del(listButtons.At(listButtons.Find(chara->button)));

	// Delete from its type vector
	arr->erase(arr->begin() + chara->positionCombat_I);

	// Delete from initiative list
	int del = listInitiative.Find(chara);
	listInitiative.Del(listInitiative.At(del));

	// Delete from entity manager list
	app->entityManager->DestroyEntity(chara);

	if (arr->empty())
	{
		(chara->charaType == CharacterType::ALLY) ? app->sceneWin_Lose->win = false : app->sceneWin_Lose->win = true;

		if (app->sceneWin_Lose->win)
		{
			int coins = rand() % 21;

			app->itemManager->coins += coins;

			/*int loot = rand() % 101;

			if (loot >= 50)
			{
				app->itemManager->AddQuantity(94, 1);
				app->itemManager->AddQuantity(93, 2);
				app->itemManager->AddQuantity(99, 2);
				app->itemManager->AddQuantity(99, 2);
				app->itemManager->AddQuantity(100, 2);
			}
			else if (loot >= 70)
			{
				app->itemManager->AddQuantity(93, 2);
				app->itemManager->AddQuantity(99, 2);
				app->itemManager->AddQuantity(99, 2);
				app->itemManager->AddQuantity(100, 2);
			}
			else if (loot >= 80)
			{
				app->itemManager->AddQuantity(93, 2);
				app->itemManager->AddQuantity(100, 2);
			}*/

		}

		app->fade->FadingToBlack(this, (Module*)app->sceneWin_Lose, 0);
	}
	else
	{
		UpdatePositions(arr, chara->positionCombat_I);
	}

	delete chara;
	chara = nullptr;
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

// Settings / GUI Controls
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
		else
		{	lastPressedAbility_I = control->id - 10; 

			//Evitar petar boton amarillo
			if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ALLY)
			{
				posStart = vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->posToTargetStart_I;
				if (vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->targetFriend) //Target aliado
				{
					posEnd = vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->RangeCanTarget(vecAllies); //Aqui hay poner un get range
				}
				else //Target enemigo
				{
					posEnd = vecAllies.at(posInVec)->listSkills.At(lastPressedAbility_I)->data->RangeCanTarget(vecEnemies); //Aqui hay poner un get range
				}
			
			}
		}
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
	if (lastPressedAbility_I != -1 && !isMoving && listInitiative.At(charaInTurn)->data->charaType==CharacterType::ALLY)
	{
		if (listInitiative.At(charaInTurn)->data->listSkills.At(lastPressedAbility_I)->data->autoTarget == true)
		{
			HandleCharaButtons(&vecAllies, vecAllies.at(posInVec)->positionCombat_I, vecAllies.at(posInVec)->positionCombat_I);
		}
		else
		{
			if (listInitiative.At(charaInTurn)->data->listSkills.At(lastPressedAbility_I)->data->targetFriend == true)
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

	//Printar recuadro info para la info de las skills
	switch (listInitiative.At(charaInTurn)->data->id)
	{
	case 0:
		rect.y = 0;
		rect2.y = 0;
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
	int xText1 = 60;//origen texto stat izq
	int xNumber1 = 200;//origen numeor stat izq
	int xText2 = 400;//origen numeor stat dch
	int xNumber2 = 546;//origen numeor stat dch
	int offsetY = 35;//offset distancia entre linias
	int fontSize = 25;
	int posY = 535;//y botones

	if (control->id <= 4) //Aliados (0-3, 4 de buffer/seguro)
	{
		if ((SearchInVec(vecAllies, control->id))==-1) //Seguro para que no pete
		{
			return false;
		}
		else
		{
			switch (control->id) //Printar cuadro del personaje correcto 
			{
			case 0:
				rect.y = 0;
				rect2.y = 0;
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

			//Printar status effects
			Character* cha = vecAllies.at((SearchInVec(vecAllies, control->id)));
			SString effectToPrint;
			for (size_t i = 0; i < cha->listStatusEffects.Count(); i++)
			{
				switch (cha->listStatusEffects.At(i)->data->type)
				{
				case EffectType::NONE:
					break;
				case EffectType::CURRENT_HP:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Regeneration";
					}
					else
					{
						effectToPrint = "Burn";
					}
					break;
				case EffectType::ATTACK:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Attack Up";
					}
					else
					{
						effectToPrint = "Attack Down";
					}
					break;
				case EffectType::CRIT_RATE:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Crit Rate Up";
					}
					else
					{
						effectToPrint = "Crit Rate Down";
					}
					break;
				case EffectType::CRIT_DMG:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Crit Damage Up";
					}
					else
					{
						effectToPrint = "Crit Damage Down";
					}
					break;
				case EffectType::ACCURACY:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Accuracy Up";
					}
					else
					{
						effectToPrint = "Accuracy Down";
					}
					break;
				case EffectType::ARMOR:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Armor Up";
					}
					else
					{
						effectToPrint = "Armor Down";
					}
					break;
				case EffectType::DODGE:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Dodge Up";
					}
					else
					{
						effectToPrint = "Dodge Down";
					}
					break;
				case EffectType::RES:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Resistance Up";
					}
					else
					{
						effectToPrint = "Resistance Down";
					}
					break;
				default:
					break;
				}
				//Efecto
				app->render->TextDraw("Effect", 75, 530, 20);
				app->render->TextDraw(effectToPrint.GetString(),75,555+i*18,15);
				string numeros = to_string(cha->listStatusEffects.At(i)->data->turnsLeft);
				//Turnos
				app->render->TextDraw("Duration", 190, 530, 20);
				app->render->TextDraw(numeros.c_str(), 220, 555 + i * 18, 15);
				//% intensidad
				app->render->TextDraw("% intensity", 300, 530, 20);
				numeros = to_string(abs(cha->listStatusEffects.At(i)->data->intensity));
				app->render->TextDraw(numeros.c_str(), 335, 555 + i * 18, 15);
			}
			
		}
	}
	
	if (control->id >= 5 && control->id < 10)
	{
		if (SearchInVec(vecEnemies, control->id)==-1) //Seguro para que no pete
		{
			return false;
		}
		Character* enemy = vecEnemies.at(SearchInVec(vecEnemies, control->id));
		for (int i = 0; i < enemy->listSkills.Count(); i++)
		{
			if (enemy->listSkillsHistory.Find(i+1) == -1)
			{
				app->render->TextDraw("???", 70, 540 + 15 * i, 12);
			}
			else
			{
				SString ability;
				ability += enemy->listSkills.At(i)->data->name;
				ability += ": ";
				ability += enemy->listSkills.At(i)->data->description.GetString();
				app->render->TextDraw(ability.GetString(), 70, 540 + 15 * i, 12);
			}

			//Printar status effects
			Character* cha = vecEnemies.at((SearchInVec(vecEnemies, control->id)));
			SString effectToPrint;
			for (size_t i = 0; i < cha->listStatusEffects.Count(); i++)
			{
				switch (cha->listStatusEffects.At(i)->data->type)
				{
				case EffectType::NONE:
					break;
				case EffectType::CURRENT_HP:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Regeneration";
					}
					else
					{
						effectToPrint = "Burn";
					}
					break;
				case EffectType::ATTACK:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Attack Up";
					}
					else
					{
						effectToPrint = "Attack Down";
					}
					break;
				case EffectType::CRIT_RATE:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Crit Rate Up";
					}
					else
					{
						effectToPrint = "Crit Rate Down";
					}
					break;
				case EffectType::CRIT_DMG:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Crit Damage Up";
					}
					else
					{
						effectToPrint = "Crit Damage Down";
					}
					break;
				case EffectType::ACCURACY:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Accuracy Up";
					}
					else
					{
						effectToPrint = "Accuracy Down";
					}
					break;
				case EffectType::ARMOR:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Armor Up";
					}
					else
					{
						effectToPrint = "Armor Down";
					}
					break;
				case EffectType::DODGE:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Dodge Up";
					}
					else
					{
						effectToPrint = "Dodge Down";
					}
					break;
				case EffectType::RES:
					if (cha->listStatusEffects.At(i)->data->isPositive)
					{
						effectToPrint = "Resistance Up";
					}
					else
					{
						effectToPrint = "Resistance Down";
					}
					break;
				default:
					break;
				}
				//Efecto
				app->render->TextDraw("Effect", 75, 580, 20);
				app->render->TextDraw(effectToPrint.GetString(), 75, 600 + i * 18, 15);
				string numeros = to_string(cha->listStatusEffects.At(i)->data->turnsLeft);
				//Turnos
				app->render->TextDraw("Duration", 190, 580, 20);
				app->render->TextDraw(numeros.c_str(), 220, 600 + i * 18, 15);
				//% intensidad
				app->render->TextDraw("% intensity", 300, 580, 20);
				numeros = to_string(abs(cha->listStatusEffects.At(i)->data->intensity));
				app->render->TextDraw(numeros.c_str(), 335, 600 + i * 18, 15);
			}
		}	
	}

	if (control->id >= 10 && control->id < 14)
	{
		if (listInitiative.At(charaInTurn)->data->charaType == CharacterType::ALLY)
		{
			Skill* skillPoint = listInitiative.At(charaInTurn)->data->listSkills.At(control->id - 10)->data;
			//Name
			app->render->TextDraw(skillPoint->name.GetString(), 48, 545, 25);

			//Stats to get
			string DMG_C;
			string Pre_C;
			string CrRate_C;
			string CrDMG_C;

			//Damage or Heal
			if (skillPoint->multiplierDmg > 0) //Heal
			{
				app->render->TextDraw("Healing:", 260, 550, 18);
				DMG_C = to_string((int)(skillPoint->multiplierDmg * listInitiative.At(charaInTurn)->data->maxHp / 5));
				const char* ch_DMG = DMG_C.c_str();
				app->render->TextDraw(ch_DMG, 335, 550, 18);
			}
			else 
			{ 
				app->render->TextDraw("Damage:", 260, 537, 18);
				DMG_C = to_string((int)(abs(skillPoint->multiplierDmg) * listInitiative.At(charaInTurn)->data->GetStat(EffectType::ATTACK)));
				const char* ch_DMG = DMG_C.c_str();
				app->render->TextDraw(ch_DMG, 335, 537, 18);

				//Stats ataques
				Pre_C = to_string((int)listInitiative.At(charaInTurn)->data->GetStatModifier(EffectType::ACCURACY)*(skillPoint->bonusAccuracy + listInitiative.At(charaInTurn)->data->accuracy));
				CrRate_C = to_string((int)listInitiative.At(charaInTurn)->data->GetStatModifier(EffectType::CRIT_RATE)*(skillPoint->bonusCritRate + listInitiative.At(charaInTurn)->data->critRate));
				CrDMG_C = to_string((int)listInitiative.At(charaInTurn)->data->GetStatModifier(EffectType::CRIT_DMG)*(skillPoint->bonusCritDamage + listInitiative.At(charaInTurn)->data->critDamage));
				
				//Printar stats
				app->render->TextDraw("Accuracy.", 260, 560, 15);
				const char* ch_Pre = Pre_C.c_str();
				app->render->TextDraw(ch_Pre, 330, 560, 15);

				app->render->TextDraw("Cr.Rate.", 260, 575, 15);
				const char* ch_CrRate = CrRate_C.c_str();
				app->render->TextDraw(ch_CrRate, 330, 575, 15);

				app->render->TextDraw("Cr.Dmg.", 260, 590, 15);
				const char* ch_CrDMG = CrDMG_C.c_str();
				app->render->TextDraw(ch_CrDMG, 330, 590, 15);
			}

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
				else { effecto_C = "Debuff Crit Rate"; }
				break;
			case EffectType::CRIT_DMG:
				if (skillPoint->positiveEffect) { effecto_C = "BuffCritDMG"; }
				else { effecto_C = "Debuff Crit DMG"; }
				break;
			case EffectType::ACCURACY:
				if (skillPoint->positiveEffect) { effecto_C = "Buff accuracy"; }
				else { effecto_C = "Debuff accuracy"; }
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
			app->render->RenderTrimmedText(55, 620, 5, skillPoint->description.GetString(), &auxTexts, 15, 90);
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

	for (int i = 0; i < app->itemManager->arrParty.size(); i++)
	{
		if (app->itemManager->arrParty.at(i) != nullptr)
		{
			pugi::xml_node character = node.append_child("CombatCharacter");
			character.append_attribute("name") = app->itemManager->arrParty.at(i)->name.GetString();
			character.append_attribute("currentHp") = app->itemManager->arrParty.at(i)->currentHp;
		}
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
			if (i >= app->itemManager->vecPC.size())
			{
				break;
			}

			app->itemManager->arrParty.at(i)->name = itemNode.attribute("name").as_string();
			app->itemManager->arrParty.at(i)->currentHp = itemNode.attribute("currentHp").as_int();

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

