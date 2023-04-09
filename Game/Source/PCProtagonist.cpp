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
	this->maxHp = 50;
	this->currentHp = 37;
	this->attack = 30;
	this->armor = 20;
	this->speed = 5;

	this->positionCombat_I = 1;

	return true;
}

bool Protagonist::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	rect = { 0,0,258,496 };
	//Numeros no exactos pero los allies van mas cerca de 0 en la pantalla cuanto mas atras esten en la formación
	app->render->DrawTexture(texture, 300 - 70*positionCombat_I, 120/* ,&rect, 1.0f, NULL, NULL, NULL, flipType*/);

	if (onTurn)
	{
		//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		//{
		//	int x, y;
		//	app->input->GetMousePosition(x, y);

		//	List<Entity*> targets = app->entityManager->GetEntitiesByType(ENEMY);

		//	for (List<Entity*> node = targets->start; node != nullptr; node = node->next)
		//	{
		//		Character* enemy = node->data;
		//		if (enemy->pbody->Contains(x, y))
		//		{
		//			// Calculate damage and apply it to the enemy
		//			int damage = CalculateDamage(attack);
		//			enemy->ModifyHP(-damage);

		//			// Exit the loop since we've found our target
		//			break;
		//		}
		//	}
		//}
	}


	return true;
}

bool Protagonist::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}




