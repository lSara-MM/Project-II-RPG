#include "Player.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Window.h"
#include "Textures.h"

#include "Scene.h"

#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	idleAnim.PushBack({ 8, 39, 17, 28 });
	idleAnim.PushBack({ 40, 39, 17, 28 });
	idleAnim.PushBack({ 71, 38, 18, 29 });
	idleAnim.PushBack({ 102, 38, 20, 29 });

	idleAnim.speed = 0.1f;

	active = true;
}

Player::~Player() {

}

bool Player::Awake() {


	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

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
	pbody->body->SetGravityScale(0);

	if (app->scene->pause_B)
	{
		dtP = 0;
	}
	else if (!app->scene->pause_B)
	{
		dtP = dt / 1000;
	}

	Controller(dtP);

	vel = b2Vec2(vel.x * dtP, vel.y * dtP);
	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width / 2;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height / 2;

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
	if(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT|| app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		vel.y = -125;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		vel.y = 125;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT  || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		vel.x = -125;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		vel.x = 125;
	}
}
