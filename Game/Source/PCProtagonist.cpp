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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	//texturePath = parameters.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/PCA.png";

	return true;
}

bool Protagonist::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);

	
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::PC_PROTAGONIST;
	this->charaType_I = CharatherType::ALLY;
	this->name = "Protagonista"; //Hay que poner el nombre assigando al principio del juego
	this->maxHp = 1150;
	this->currentHp = 1150;
	this->attack = 190;
	this->armor = 10;
	this->speed = 5;
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
	app->render->DrawRectangle({ 477 - 107 * positionCombat_I, 250, 90, 20 }, 255, 0, 255, 255, true);
	app->render->DrawRectangle({ 477 - 107 * positionCombat_I, 250, auxhp, 20}, 255, 255, 255, 255, true);
	
	//Modify Health Bar
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		ModifyHP(-1);
	}
	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		ModifyHP(10);
	}

	
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 416 - 128*positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/);

	if (onTurn)
	{
		app->render->DrawCircle( 300 - 70 * positionCombat_I,15, 20,0,255,255);
		if (app->combat->LastPressedAbility == 1)
		{

			float damage = app->combat->enemies[0]->CalculateDamage(attack);
			//Si no hay godmode va normal, si lo hay la vida del enemigo se reduce a 0

			if (app->combat->targeted_Character == app->combat->enemies[2] || app->combat->targeted_Character == app->combat->enemies[3])
			{
				if (!app->input->godMode_B)
				{
					app->combat->targeted_Character->ModifyHP(-damage);
				}
				else
				{
					app->combat->enemies[0]->currentHp = 0;
				}

				SDL_Delay(500);
				app->combat->NextTurn();
				onTurn = false;
			}
				
		}
		if (app->combat->LastPressedAbility == 2)
		{
			if (app->combat->targeted_Character == app->combat->enemies[0] || app->combat->targeted_Character == app->combat->enemies[1]) 
			{
				float damage = app->combat->targeted_Character->CalculateDamage(attack);
				app->combat->targeted_Character->ModifyHP(-damage);
				onTurn = false;
				app->combat->NextTurn();
			}
			
		}
		if (app->combat->LastPressedAbility == 3)
		{

			if (app->combat->targeted_Character == app->combat->enemies[0] || app->combat->targeted_Character == app->combat->enemies[1]) {
				float damage = app->combat->enemies[2]->CalculateDamage(attack * 0.55);
				app->combat->enemies[0]->ModifyHP(-damage);
				app->combat->enemies[1]->ModifyHP(-damage);
				app->combat->NextTurn();
				onTurn = false;
			}
			
		}
		if (app->combat->LastPressedAbility == 4)
		{

			if (app->combat->targeted_Character == app->combat->enemies[2] || app->combat->targeted_Character == app->combat->enemies[3]) {
				float damage = app->combat->enemies[2]->CalculateDamage(attack * 0.55);
				app->combat->enemies[2]->ModifyHP(-damage);
				app->combat->enemies[3]->ModifyHP(-damage);
				app->combat->NextTurn();
				onTurn = false;
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




