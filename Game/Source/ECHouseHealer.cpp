#include "ECHouseHealer.h"

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

HouseHealer::HouseHealer() : CombatEnemy()
{
	name.Create("PCProtagonist");

	active = true;
}

HouseHealer::~HouseHealer() {

}

bool HouseHealer::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();


	//texturePath = parameters.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/Enemy1.png";

	return true;
}

bool HouseHealer::Start() {

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);

	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::ENEMY_HEALER_HOUSE;
	this->charaType_I = CharatherType::ENEMY;
	this->maxHp = 80;
	this->currentHp = 80;
	this->attack = 12;
	this->armor = 30;
	this->speed = 2;

	this->positionCombat_I = 1;

	return true;
}

bool HouseHealer::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 300 + 70 * positionCombat_I, 120/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/);

	if (onTurn)
	{
		
		int randomNum = std::rand() % 3 + 1;

		if (randomNum == 1)
		{

			float damage = app->combat->allies[0]->CalculateDamage(attack);
			app->combat->enemies[0]->ModifyHP(-damage);
			app->combat->NextTurn();
		}
		if (randomNum == 2)
		{

			float damage = app->combat->allies[1]->CalculateDamage(attack);
			app->combat->enemies[1]->ModifyHP(-damage);
			app->combat->NextTurn();
		}
		if (randomNum == 3)
		{

			float damage = app->combat->allies[0]->CalculateDamage(attack * 0.5);
			app->combat->enemies[0]->ModifyHP(-damage);
			app->combat->enemies[1]->ModifyHP(-damage);
			app->combat->NextTurn();
		}
		//render barra de habilidades
		// Para seleccionar app->input->GetMousePosition o 
		app->combat->NextTurn();
		onTurn = false;

	}


	return true;
}

bool HouseHealer::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);

	return true;
}