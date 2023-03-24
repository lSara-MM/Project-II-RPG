#include "Player.h"

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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	active = true;
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	
	return true;
}

bool Player::Update(float dt)
{
	Controller(dt);

	currentAnimation = &idleAnim;
	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}


void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	}
}

void Player::Controller(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_W) || app->input->GetKey(SDL_SCANCODE_UP))
	{
		pbody->body->SetLinearVelocity({ 0, 5 * dt});
	}
	if (app->input->GetKey(SDL_SCANCODE_S) || app->input->GetKey(SDL_SCANCODE_DOWN))
	{
		pbody->body->SetLinearVelocity({ 0, -5 * dt});
	}
	if (app->input->GetKey(SDL_SCANCODE_A) || app->input->GetKey(SDL_SCANCODE_LEFT))
	{
		pbody->body->SetLinearVelocity({ -5 * dt , 0});
	}
	if (app->input->GetKey(SDL_SCANCODE_D) || app->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		pbody->body->SetLinearVelocity({ 5 * dt , 0 });
	}
}
