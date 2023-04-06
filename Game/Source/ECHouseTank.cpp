#include "ECHouseTank.h"

#include "Characther.h"
#include "EnemyCombat.h"

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

HouseTank::HouseTank() : EnemyCombat()
{
	name.Create("PCProtagonist");

	active = true;
}

HouseTank::~HouseTank() {

}

bool HouseTank::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();


	//texturePath = parameters.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/EC1.png";

	return true;
}

bool HouseTank::Start() {

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);

	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::ENEMY_TANK_HOUSE;
	this->charaType_I = CharatherType::ENEMY;
	this->maxHp = 80;
	this->currentHp = 70;
	this->attack = 12;
	this->armor = 30;
	this->speed = 3;

	this->positionCombat_I = 1;

	return true;
}

bool HouseTank::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 600 + 70 * positionCombat_I, 120/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/);

	if (onTurn)
	{
		//render barra de habilidades
		// Para seleccionar app->input->GetMousePosition o 

	}


	return true;
}

bool HouseTank::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);

	return true;