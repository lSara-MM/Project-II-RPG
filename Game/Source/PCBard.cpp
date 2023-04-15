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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	//texturePath = parameters.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/PCA.png";

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
	this->maxHp = 1000;
	this->currentHp = 1000;
	this->attack = 200;
	this->armor = 8;
	this->speed = 6;
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
	app->render->DrawRectangle({ 477 - 107 * positionCombat_I, 280, 90, 20 }, 255, 0, 255, 255, true);
	app->render->DrawRectangle({ 477 - 107 * positionCombat_I, 280, auxhp, 20 }, 255, 255, 255, 255, true);

	// Modify Health Bar
	/*if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		currentHp--;
	}*/

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 416 - 128 * positionCombat_I, 280/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/); //Print bueno

	if (onTurn)
	{
		app->render->DrawCircle( 300 - 70 * positionCombat_I,15, 20,0,255,255);
		//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		//{
		//	int x, y;
		//	app->input->GetMousePosition(x, y);

		//	//List<Entity*> targets = app->entityManager->GetEntitiesByType(/*type of the enemy*/);

		//	for (int i = 0; i < 3; i++)
		//	{
		//		if (app->combat->enemies[i]->pbody->Contains(x, y))
		//		{
		//			// Calculate damage and apply it to the enemy
		//			int damage = app->combat->enemies[i]->CalculateDamage(attack);
		//			app->combat->enemies[i]->ModifyHP(-damage);

		//			// Exit the loop since we've found the target
		//			break;
		//		}
		//	}
		//}
		
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_UP)
		{

				float damage = app->combat->enemies[0]->CalculateDamage(attack);

				//Si no hay godmode va normal, si lo hay la vida del enemigo se reduce a 0
				if (!app->input->godMode_B)
				{
					app->combat->enemies[0]->ModifyHP(-damage);
				}

				else
				{
					app->combat->enemies[0]->currentHp = 0;
				}
		
				SDL_Delay(500);
				onTurn = false;
				app->combat->NextTurn();
				
		}
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{

			float damage = app->combat->enemies[1]->CalculateDamage(attack);
			app->combat->enemies[0]->ModifyHP(-damage);
			onTurn = false;
			app->combat->NextTurn();
			
		}
		if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{

			float damage = app->combat->enemies[2]->CalculateDamage(attack * 0.75);
			app->combat->enemies[0]->ModifyHP(-damage);
			app->combat->NextTurn();
			onTurn = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{

			float damage = app->combat->enemies[3]->CalculateDamage(attack * 0.75);
			app->combat->enemies[0]->ModifyHP(-damage);
			app->combat->NextTurn();
			onTurn = false;
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




