#include "ECHouseDPS.h"

#include "Characther.h"
#include "CombatEnemy.h"

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
#include"Combat.h"

HouseDPS::HouseDPS() : CombatEnemy()
{
	name.Create("PCProtagonist");

	active = true;
}

HouseDPS::~HouseDPS() {

}

bool HouseDPS::Awake() {

	this->name = parameters.attribute("name").as_string();
	this->maxHp = parameters.attribute("maxHp").as_int();
	this->currentHp = parameters.attribute("currentHp").as_int();
	this->attack = parameters.attribute("attack").as_int();
	this->armor = parameters.attribute("armor").as_int();
	this->speed = parameters.attribute("speed").as_int();
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool HouseDPS::Start() {

	texture = app->tex->Load(texturePath);

	this->type = EntityType::ENEMY_DPS_HOUSE;
	this->charaType_I = CharacterType::ENEMY;

	this->positionCombat_I = 1;
	this->alive = true;

	return true;
}

bool HouseDPS::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	//Health Bar
	int auxhp = ((currentHp * 100) / maxHp) * 0.90;
	app->render->DrawRectangle({ 628 + 127 * positionCombat_I, 250, 90, 20 }, 1, 1, 1, 255, true);
	app->render->DrawRectangle({ 628 + 127 * positionCombat_I, 250, auxhp, 20 }, 255, 0, 0, 255, true);

	string s_hp = std::to_string(this->currentHp);
	const char* ch_hp = s_hp.c_str();
	app->render->TextDraw(ch_hp, 628 + 127 * positionCombat_I, 220, 20, UI, { 125,0,0 });
	//Si vida es 0 o menor, eliminar enemy
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
	
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formaci�n

	app->render->DrawTexture(texture, 608 + 128 * positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/); //PrintBueno
	
	
	if (onTurn)
	{
		{
			//Elegir numero random de 1 al 3
			int randomNum = std::rand() % 3 + 1;
			//Si es 1, hace daño al 1r aliado
			if (randomNum == 1 )
			{
				if (!(app->combat->allies[0] == nullptr))
				{
					
					if (!app->input->godMode_B)//Hace da�o si no hay godmode
					{
						float damage = app->combat->allies[0]->CalculateDamage(attack);
						app->combat->allies[0]->ModifyHP(-damage);
					}
					app->combat->NextTurn();
				}
			}
			//Si es 2, hace daño a 2ndo aliado
			if (randomNum == 2)
			{
				if (!(app->combat->allies[1] == nullptr))
				{
					
					if (!app->input->godMode_B)
					{
						float damage = app->combat->allies[1]->CalculateDamage(attack);
						app->combat->allies[1]->ModifyHP(-damage);
					}
				
					
				}
				app->combat->NextTurn();
			}
			//Si es 3, hace daño a 1r y 2ndo aliado
			if (randomNum == 3)
			{
				//Daño al 1r aliado
				if (!(app->combat->allies[0] == nullptr))
				{
					
					if (!app->input->godMode_B)
					{
						float damage = app->combat->allies[0]->CalculateDamage(attack * 0.5);
						app->combat->allies[0]->ModifyHP(-damage);
					}
				
				}
				//Daño al 2ndo aliado
				if (!(app->combat->allies[1] == nullptr))
				{
					
					if (!app->input->godMode_B)
					{
						float damage = app->combat->allies[1]->CalculateDamage(attack * 0.5);
						app->combat->allies[1]->ModifyHP(-damage);
					}
				}
				app->combat->NextTurn();
			}

			onTurn = false;
		}
	}


	return true;
}

bool HouseDPS::CleanUp()
{
	app->tex->UnLoad(texture);
	

	return true;
}