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
	name.Create("Characther");

	active = true;
}

Protagonist::~Protagonist() {

}

bool Protagonist::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Protagonist::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	this->type = EntityType::PC_PROTAGONIST;
	maxHp = 50;
	currentHp = 37;
	attack = 30;
	armor = 20;
	speed = 5;

	return true;
}

bool Protagonist::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	if (onTurn)
	{
		//render barra de habilidades
		// Para seleccionar app->input->GetMousePosition o 

	}


	return true;
}

bool Protagonist::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}




