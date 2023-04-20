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

	this->skills_C[0] = "Skill 1 Prota";
	this->skills_C[1] = "Skill 2 Prota";
	this->skills_C[2] = "Skill 3 Prota";
	this->skills_C[3] = "Skill 4 Prota";

	return true;
}

bool Protagonist::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);

	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::PC_PROTAGONIST;
	this->charaType_I = CharacterType::ALLY;
	//this->name = "Protagonista"; //Hay que poner el nombre assigando al principio del juego
	
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

	if (this->currentHp <= 0)
	{
		this->alive = false;
	}
	else { this->alive = true; }

	if (this->alive == false)
	{
		app->combat->EliminateCombatant(this);
	}
	
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 544 - 128*positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/);

	if (onTurn )
	{
		app->render->DrawCircle(544 - 128 * positionCombat_I + (126 / 2), 220, 20, 0, 255, 255);

		app->combat->DisableTargetButton(0);
		app->combat->DisableTargetButton(1);
		app->combat->DisableTargetButton(2);
		app->combat->DisableTargetButton(3);
		app->combat->DisableTargetButton(4);
		app->combat->DisableTargetButton(5);
		app->combat->DisableTargetButton(6);
		app->combat->DisableTargetButton(7);

		if (app->combat->enemies[1] == nullptr && app->combat->enemies[2] == nullptr) 
		{
			app->combat->DisableSkillButton(1);
		}
		if (app->combat->enemies[2] == nullptr && app->combat->enemies[3] == nullptr)
		{
			app->combat->DisableSkillButton(4);
		}

		/*if (this->currentHp <= 0)
		{
			this->alive = false;
		}
		else { this->alive = true; }

		if (this->alive == false)
		{
			app->combat->EliminateCombatant(this);
		}

		else*/
		{
			if (app->combat->lastPressedAbility_I == 1) //Only allows targeting 2 and 3
			{
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

					if (!app->input->godMode_B)
					{
						float damage = app->combat->targeted_Character->CalculateDamage(attack);
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
			if (app->combat->lastPressedAbility_I == 2)
			{
				if (app->combat->enemies[1] != nullptr && app->combat->enemies[1]->alive == true)
				{
					app->combat->EnableTargetButton(5);
				}
				if (app->combat->enemies[0] != nullptr && app->combat->enemies[0]->alive == true)
				{
					app->combat->EnableTargetButton(4);
				}

				if (app->combat->targeted_Character == app->combat->enemies[0] || app->combat->targeted_Character == app->combat->enemies[1])
				{
					if (app->combat->targeted_Character != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->targeted_Character->CalculateDamage(attack);
							app->combat->targeted_Character->ModifyHP(-damage);
						}
						else
						{
							app->combat->targeted_Character->ModifyHP(-99999);
						}

						onTurn = false;
						app->combat->NextTurn();
					}
					
				}

			}
			if (app->combat->lastPressedAbility_I == 3)
			{

				if (app->combat->enemies[1] != nullptr && app->combat->enemies[1]->alive == true)
				{
					app->combat->EnableTargetButton(5);
				}
				if (app->combat->enemies[0] != nullptr && app->combat->enemies[0]->alive == true)
				{
					app->combat->EnableTargetButton(4);
				}

				if (app->combat->targeted_Character == app->combat->enemies[0] || app->combat->targeted_Character == app->combat->enemies[1]) {
					if (app->combat->enemies[0] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[0]->CalculateDamage(attack * 0.65);
							app->combat->enemies[0]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[0]->ModifyHP(-99999);
						}

					}
					if (app->combat->enemies[1] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[1]->CalculateDamage(attack * 0.65);
							app->combat->enemies[1]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[1]->ModifyHP(-99999);
						}
					}
					
					app->combat->NextTurn();
					onTurn = false;
				}

			}
			if (app->combat->lastPressedAbility_I == 4)
			{
				if (app->combat->enemies[2] != nullptr && app->combat->enemies[2]->alive == true)
				{
					app->combat->EnableTargetButton(6);
				}

				if (app->combat->enemies[3] != nullptr && app->combat->enemies[3]->alive == true)
				{
					app->combat->EnableTargetButton(7);
				}


				if (app->combat->targeted_Character == app->combat->enemies[2] || app->combat->targeted_Character == app->combat->enemies[3] && app->combat->targeted_Character != nullptr) {
					if (app->combat->enemies[2] != nullptr) {
						if (!app->input->godMode_B)
						{
							float damage = app->combat->enemies[2]->CalculateDamage(attack * 0.6);
							app->combat->enemies[2]->ModifyHP(-damage);
						}
						else
						{
							app->combat->enemies[2]->ModifyHP(-99999);
						}

					}
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
					app->combat->NextTurn();
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
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}




