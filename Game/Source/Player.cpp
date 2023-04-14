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

	currentAnim.PushBack({ 64, 0, 64, 64 });

	downAnim.PushBack({ 0, 0, 64, 64 });
	downAnim.PushBack({ 64, 0, 64, 64 });
	downAnim.PushBack({ 128, 0, 64, 64 });
	downAnim.PushBack({ 128, 0, 64, 64 });

	idleDownAnim.PushBack({ 64, 0, 64, 64 });

	upAnim.PushBack({ 0, 64, 64, 64 });
	upAnim.PushBack({ 64, 64, 64, 64 });
	upAnim.PushBack({ 128, 64, 64, 64 });
	upAnim.PushBack({ 64, 64, 64, 64 });	

	idleUpAnim.PushBack({ 128, 64, 64, 64 });
	
	leftAnim.PushBack({ 0, 128, 64, 64 });
	leftAnim.PushBack({ 64, 128, 64, 64 });
	leftAnim.PushBack({ 128, 128, 64, 64 });
	leftAnim.PushBack({ 64, 128, 64, 64 });

	idleLeftAnim.PushBack({ 128, 128, 64, 64 });

	rigthAnim.PushBack({ 0, 192, 64, 64 });
	rigthAnim.PushBack({ 64, 192, 64, 64 });
	rigthAnim.PushBack({ 128, 192, 64, 64 });
	rigthAnim.PushBack({ 64, 192, 64, 64 });

	idleRigthAnim.PushBack({ 128, 192, 64, 64 });

	downAnim.speed = 0.1f;
	upAnim.speed = 0.1f;
	rigthAnim.speed = 0.1f;
	leftAnim.speed = 0.1f;

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
	currentAnimation = &currentAnim;
	
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
	if (!keyLockDown && !keyLockLeft && !keyLockRigth)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			keyLockUp = true;
			currentAnimation = &upAnim;
			vel.y = -125;
			vel.x = 0;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			keyLockUp = false;
			currentAnimation = &idleUpAnim;
		}
	}
	if (!keyLockUp && !keyLockLeft && !keyLockRigth)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			keyLockDown = true;
			currentAnimation = &downAnim;
			vel.y = 125;
			vel.x = 0;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			keyLockDown = false;
			currentAnimation = &idleDownAnim;
		}
	}
	if (!keyLockDown && !keyLockUp && !keyLockRigth)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			keyLockLeft = true;
			currentAnimation = &leftAnim;
			vel.x = -125;
			vel.y = 0;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			keyLockLeft = false;
			currentAnimation = &idleLeftAnim;
		}
	}
	if (!keyLockDown && !keyLockLeft && !keyLockUp)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			keyLockRigth = true;
			currentAnimation = &rigthAnim;
			vel.x = 125;
			vel.y = 0;
		}
		if(app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			keyLockRigth = false;
			currentAnimation = &idleRigthAnim;
		}
	}
}
