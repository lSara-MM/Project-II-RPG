#include "PCBard.h"

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

Bard::Bard() : PartyMember()
{
	name.Create("PCBard");

	active = true;
}

Bard::~Bard() {

}

bool Bard::Awake() {

	this->maxHp = parameters.attribute("maxHp").as_int();
	this->currentHp = parameters.attribute("currentHp").as_int();
	this->attack = parameters.attribute("attack").as_int();
	this->armor = parameters.attribute("armor").as_int();
	this->speed = parameters.attribute("speed").as_int();
	
	//texturePath = parameters.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/AllyBard.png";

	return true;
}

bool Bard::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::PC_BARD;
	this->charaType_I = CharatherType::ALLY;
	this->name = "Bard";

	this->onTurn = false;

	this->positionCombat_I = 2;

	return true;
}

bool Bard::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	//Health Bar
	int auxhp = ((currentHp * 100) / maxHp) * 0.90;
	app->render->DrawRectangle({ 560 - 127 * positionCombat_I, 250, 90, 20 }, 255, 0, 255, 255, true);
	app->render->DrawRectangle({ 560 - 127 * positionCombat_I, 250, auxhp, 20 }, 255, 255, 255, 255, true);

	// Modify Health Bar
	/*if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		currentHp--;
	}*/

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 544 - 128 * positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/); //Print bueno

	if (onTurn)
	{
		app->render->DrawCircle(544 - 128 * positionCombat_I + (126 / 2), 220, 20, 0, 255, 255);

		if (app->combat->lastPressedAbility_I == 1)
		{				
				if(app->combat->targeted_Character != nullptr)
				{
					float damage = app->combat->enemies[0]->CalculateDamage(attack * 0.6);
					//Si no hay godmode va normal, si lo hay la vida del enemigo se reduce a 0
					if (!app->input->godMode_B)
					{
						app->combat->targeted_Character->ModifyHP(+damage);
					}
					else
					{
						app->combat->enemies[0]->currentHp = 0;
					}

					SDL_Delay(500);
					onTurn = false;
					app->combat->NextTurn();
				}
				
		}
		if (app->combat->lastPressedAbility_I == 2)
		{
			if (app->combat->targeted_Character == app->combat->enemies[1] || app->combat->targeted_Character == app->combat->enemies[2] || app->combat->targeted_Character == app->combat->enemies[3])
			{
				float damage = app->combat->enemies[1]->CalculateDamage(attack);
				app->combat->enemies[0]->ModifyHP(-damage);
				onTurn = false;
				app->combat->NextTurn();
			}
			
		}
		if (app->combat->lastPressedAbility_I == 3)
		{
			if (app->combat->targeted_Character == app->combat->enemies[0])
			{
				float damage = app->combat->enemies[0]->CalculateDamage(attack * 0.85);
				app->combat->enemies[0]->ModifyHP(-damage);
				app->combat->NextTurn();
				onTurn = false;
			}
		}
		if (app->combat->lastPressedAbility_I == 4)
		{
			if (app->combat->targeted_Character != nullptr)
			{
				app->combat->enemies[0]->ModifyHP(+(attack * 0.25));
				app->combat->NextTurn();
				onTurn = false;
			}
		}
	}


	return true;
}

bool Bard::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}




