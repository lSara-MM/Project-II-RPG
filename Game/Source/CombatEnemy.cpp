#include "CombatEnemy.h"

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

CombatEnemy::CombatEnemy() : Characther()
{
	name.Create("Characther");

	active = true;
}

CombatEnemy::CombatEnemy(Characther* chara){}

CombatEnemy::~CombatEnemy() {

}

bool CombatEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool CombatEnemy::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	
	return true;
}

bool CombatEnemy::Update(float dt)
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

bool CombatEnemy::Render()
{

	return true;
}

bool CombatEnemy::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}





