#include "PCProtagonist.h"

#include "Characther.h"
#include "PartyMember.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Window.h"

#include "Scene.h"

#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"
#include "Combat.h"

Protagonist::Protagonist() : PartyMember()
{
	name.Create("PCProtagonist");

	active = true;
}

Protagonist::~Protagonist() {

}

bool Protagonist::Awake() {

	this->name = parameters.attribute("name").as_string();
	this->maxHp = parameters.attribute("maxHp").as_int();
	if (app->input->currentHP_Protagonist!=0)
	{
		this->currentHp = app->input->currentHP_Protagonist;
	}
	else
	{
		this->currentHp = parameters.attribute("currentHp").as_int();
	}
	this->attack = parameters.attribute("attack").as_int();
	this->armor = parameters.attribute("armor").as_int();
	this->speed = parameters.attribute("speed").as_int();
	texturePath = parameters.attribute("texturePath").as_string();

	this->skills_C[0] = "Damage X1 to 2 OR 3";
	this->skills_C[1] = "Self Heal X0.6";
	this->skills_C[2] = "Damage X 0.75 to 1 AND 2";
	this->skills_C[3] = "Damage X0.6 to 2 AND 4";

	return true;
}

bool Protagonist::Start() {

	texture = app->tex->Load(texturePath);
	
	this->type = EntityType::PC_PROTAGONIST;
	this->charaType_I = CharacterType::ALLY;
	this->name = "Protagonista"; //Hay que poner el nombre assigando al principio del juego
	
	this->onTurn = false;

	this->positionCombat_I = 1;

	return true;
}

bool Protagonist::Update(float dt)
{
	
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	//Health Bar
	int auxhp = ((currentHp * 100) / maxHp)*0.90;
	app->render->DrawRectangle({ 560 - 127 * positionCombat_I, 250, 90, 20 }, 1, 1, 1, 255, true);
	app->render->DrawRectangle({ 560 - 127 * positionCombat_I, 250, auxhp, 20}, 255, 0, 0, 255, true);
	
	string s_hp = std::to_string(this->currentHp);
	const char* ch_hp = s_hp.c_str();
	app->render->TextDraw(ch_hp, 560 - 127 * positionCombat_I, 220, 20, UI, { 125,0,0 });

	//If not alive, destroy entity
	if (currentHp <= 0)
	{
		if (onTurn)
		{
			app->combat->NextTurn();
		}
		app->combat->EliminateCombatant(this);
		return true;
	}
	
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 544 - 128*positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/);

	if (onTurn )
	{
		app->render->DrawCircle(544 - 128 * positionCombat_I + (126 / 2), 220, 20, 0, 255, 255);

		//Disable all Target Buttons
		app->combat->DisableTargetButton(0);
		app->combat->DisableTargetButton(1);
		app->combat->DisableTargetButton(2);
		app->combat->DisableTargetButton(3);
		app->combat->DisableTargetButton(4);
		app->combat->DisableTargetButton(5);
		app->combat->DisableTargetButton(6);
		app->combat->DisableTargetButton(7);

		//Disable skill buttons if targets not alive
		if (app->combat->enemies[1] == nullptr && app->combat->enemies[2] == nullptr) 
		{
			app->combat->DisableSkillButton(1);
		}
		if (app->combat->enemies[2] == nullptr && app->combat->enemies[3] == nullptr)
		{
			app->combat->DisableSkillButton(4);
		}
		if (positionCombat_I == 3)
		{
			app->combat->DisableSkillButton(4);
		}


		{
			//Habilidad 1 Daño a una sola unidad
			if (app->combat->lastPressedAbility_I == 1) //Only allows targeting 2 and 3
			{
				//Enable Buttons if enemies are alive
				if (app->combat->enemies[1] != nullptr && app->combat->enemies[1]->alive == true)
				{
					app->combat->EnableTargetButton(5);
				}

				if (app->combat->enemies[2] != nullptr && app->combat->enemies[2]->alive == true)
				{
					app->combat->EnableTargetButton(6);
				}
				//Si no hay godmode va normal, si lo hay la vida del enemigo se reduce a 0

				if (app->combat->targeted_Character != nullptr)
				{
					//If in godmode, deal massive damage
					if (!app->input->godMode_B)
					{
						float damage = app->combat->targeted_Character->CalculateDamage(1.3*attack);
						app->combat->targeted_Character->ModifyHP(-damage);
					}
					else
					{
						app->combat->targeted_Character->ModifyHP(-99999);
					}
					app->combat->NextTurn();
					onTurn = false;
				}

			}
			//Habilidad 2 Auto Heal
			if (app->combat->lastPressedAbility_I == 2)
			{
				//Enable Buttons if enemies are alive
				app->combat->EnableTargetButton(3);
				if (app->combat->targeted_Character == app->combat->allies[0]) //ERIC:Solo puede usarse en posicion 0? Porque si no esto puede curar a otros personajes PAU: Si
				{
					if (!app->input->godMode_B)
					{
						float heal = attack*0.6;
						app->combat->targeted_Character->ModifyHP(heal);
					}
					else
					{
					app->combat->targeted_Character->ModifyHP(99999);
					}

					onTurn = false;
					app->combat->NextTurn();
				}
					
			}
			//Habilidad 3 daño a 1r y 2ndo enemigo
			if (app->combat->lastPressedAbility_I == 3)
			{
				//Enable Buttons if enemies are alive
				if (app->combat->enemies[1] != nullptr && app->combat->enemies[1]->alive == true)
				{
					app->combat->EnableTargetButton(5);
				}
				if (app->combat->enemies[0] != nullptr && app->combat->enemies[0]->alive == true)
				{
					app->combat->EnableTargetButton(4);
				}

				if (app->combat->targeted_Character == app->combat->enemies[0] || app->combat->targeted_Character == app->combat->enemies[1] && app->combat->targeted_Character != nullptr) {
					//Damage to 1rst enemy
					if (app->combat->enemies[0] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[0]->CalculateDamage(attack * 0.75);
							app->combat->enemies[0]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[0]->ModifyHP(-99999);
						}

					}
					//Damage to 2nd enemy
					if (app->combat->enemies[1] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[1]->CalculateDamage(attack * 0.75);
							app->combat->enemies[1]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[1]->ModifyHP(-99999);
						}
					}
					//Next Turn
					if (app->combat->enemies[0] != nullptr || app->combat->enemies[1] != nullptr) { app->combat->NextTurn(); }
					onTurn = false;
				}

			}
			//Habilidad 4 Daño a 2ndo y 4to enemigo
			if (app->combat->lastPressedAbility_I == 4)
			{
				//Enable Buttons if enemies are alive
				if (app->combat->enemies[1] != nullptr && app->combat->enemies[1]->alive == true)
				{
					app->combat->EnableTargetButton(5);
				}

				if (app->combat->enemies[3] != nullptr && app->combat->enemies[3]->alive == true)
				{
					app->combat->EnableTargetButton(7);
				}


				if (app->combat->targeted_Character == app->combat->enemies[1] || app->combat->targeted_Character == app->combat->enemies[3] && app->combat->targeted_Character != nullptr) {
					//Damage to 2nd enemy
					if (app->combat->enemies[1] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[1]->CalculateDamage(attack * 0.6);
							app->combat->enemies[1]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[1]->ModifyHP(-99999);
						}

					}
					//Damage to 4th enemy
					if (app->combat->enemies[3] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[3]->CalculateDamage(attack * 0.6);
							app->combat->enemies[3]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[1]->ModifyHP(-99999);
						}
					}
					//Next Turn
					if (app->combat->enemies[1] != nullptr || app->combat->enemies[3] != nullptr) { app->combat->NextTurn(); }
					onTurn = false;
				}
			}
		}
	}


	return true;
}

bool Protagonist::CleanUp()
{
	app->tex->UnLoad(texture);
	
	
	return true;
}




