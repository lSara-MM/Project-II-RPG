#include "ECHouseTank.h"

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

HouseTank::HouseTank() : CombatEnemy()
{
	name.Create("PCProtagonist");

	active = true;
}

HouseTank::~HouseTank() {

}

bool HouseTank::Awake() {

	this->name = parameters.attribute("name").as_string();
	this->maxHp = parameters.attribute("maxHp").as_int();
	this->currentHp = parameters.attribute("currentHp").as_int();
	this->attack = parameters.attribute("attack").as_int();
	this->armor = parameters.attribute("armor").as_int();
	this->speed = parameters.attribute("speed").as_int();
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool HouseTank::Start() {

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);

	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::ENEMY_TANK_HOUSE;
	this->charaType_I = CharacterType::ENEMY;

	this->positionCombat_I = 1;
	this->alive = true;

	return true;
}

bool HouseTank::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	//Health Bar
	int auxhp = ((currentHp * 100) / maxHp) * 0.90;
	app->render->DrawRectangle({ 628 + 127 * positionCombat_I, 250, 90, 20 }, 1, 1, 1, 255, true);
	app->render->DrawRectangle({ 628 + 127 * positionCombat_I, 250, auxhp, 20 }, 255, 0, 0, 255, true);

	// Modify Health Bar
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		currentHp--;
	}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 608 + 128 * positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/); //PrintBueno
	
	if (onTurn)
	{
		app->combat->NextTurn();
		onTurn = false;
	}

	return true;
}

bool HouseTank::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);

	return true;
}