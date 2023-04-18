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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();


	//texturePath = parameters.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/EnemyCursedPuppet.png";

	return true;
}

bool HouseDPS::Start() {

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);

	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::ENEMY_DPS_HOUSE;
	this->charaType_I = CharatherType::ENEMY;
	this->name = "Cursed Puppet";
	this->maxHp = 600;
	this->currentHp = 600;
	this->attack = 200;
	this->armor = 0;
	this->speed = 5;

	this->positionCombat_I = 1;

	return true;
}

bool HouseDPS::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	//Health Bar
	int auxhp = ((currentHp * 100) / maxHp) * 0.90;
	app->render->DrawRectangle({ 628 + 127 * positionCombat_I, 250, 90, 20 }, 255, 0, 255, 255, true);
	app->render->DrawRectangle({ 628 + 127 * positionCombat_I, 250, auxhp, 20 }, 255, 255, 255, 255, true);

	// Modify Health Bar
	/*if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		currentHp--;
	}*/

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 608 + 128 * positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/); //PrintBueno

	if (onTurn)
	{
		SDL_Delay(2000);
		
		int randomNum = std::rand() % 3 + 1;
		//Mejor con un switch
		if (randomNum == 1)
		{
			if (!(app->combat->allies[0]==nullptr))
			{
			float damage = app->combat->allies[0]->CalculateDamage(attack);
			if (!app->input->godMode_B)//Hace daño si no hay godmode
			{
				app->combat->allies[0]->ModifyHP(-damage);
			}
			}
			

			app->combat->NextTurn();
		}
		if (randomNum == 2)
		{
			if (!(app->combat->allies[1] == nullptr))
			{
			float damage = app->combat->allies[1]->CalculateDamage(attack);
			app->combat->allies[1]->ModifyHP(-damage);
			app->combat->NextTurn();
			}
			
		}
		if (randomNum == 3)
		{
			if (!(app->combat->allies[0] == nullptr))
			{
			float damage = app->combat->allies[0]->CalculateDamage(attack * 0.5);
			app->combat->allies[0]->ModifyHP(-damage);
			}
			
			if (!(app->combat->allies[1] == nullptr))
			{
				float damage = app->combat->allies[1]->CalculateDamage(attack * 0.5);
				app->combat->allies[1]->ModifyHP(-damage);
			}
			app->combat->NextTurn();
		}
		//render barra de habilidades
		// Para seleccionar app->input->GetMousePosition o 
		//app->combat->NextTurn();
		onTurn = false;

	}


	return true;
}

bool HouseDPS::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);

	return true;
}