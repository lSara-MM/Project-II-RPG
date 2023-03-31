#include "PartyMember.h"

#include "Entity.h"
#include "Characther.h"

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

PartyMember::PartyMember() : Characther()
{
	name.Create("Characther");

	active = true;
}

PartyMember::PartyMember(Characther* chara){}

PartyMember::~PartyMember() {

}

bool PartyMember::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool PartyMember::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	
	return true;
}

bool PartyMember::Update(float dt)
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

bool PartyMember::Render()
{

	return true;
}

bool PartyMember::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}





