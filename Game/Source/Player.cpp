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
#include "DialogueSystem.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"

#include "External/SDL/include/SDL_gamecontroller.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	currentAnim.PushBack({ 64, 0, 64, 64 });

	downAnim.PushBack({ 0, 0, 64, 64 });
	downAnim.PushBack({ 64, 0, 64, 64 });
	downAnim.PushBack({ 128, 0, 64, 64 });
	downAnim.PushBack({ 64, 0, 64, 64 });

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

	downAnim.speed = 0.08f;
	upAnim.speed = 0.08f;
	rigthAnim.speed = 0.08f;
	leftAnim.speed = 0.08f;

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
	
	pbody = app->physics->CreateRectangle(position.x - width / 2, position.y - height / 2, width + 5, height + 5, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 
	pbody->ctype = ColliderType::PLAYER;
	
	playerName = app->input->playerName.c_str();
	npcInteract = false;

	return true;
}

bool Player::Update(float dt)
{
	app->render->camera.y = -position.y + 360 - height;
	app->render->camera.x = -position.x + 640 - width;

	pbody->body->SetGravityScale(0);

	if (app->scene->pause_B)
	{
		dtP = 0;
	}
	else if (!app->scene->pause_B)
	{
		dtP = dt / 1000;
	}

	vel = b2Vec2(vel.x * dtP, vel.y * dtP);
	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width / 2;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height / 2;

	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - width - 2, position.y - height, &rect, 1.0f, NULL, NULL, NULL, flipType);

	//Sara aqu� tienes tu parte, donde cuando el player est� dentro de la zona interactuable con el npc
	if (npcInteract) 
	{
		app->render->DrawRectangle({ npcTalkingTo->position.x, npcTalkingTo->position.y - 60, 24, 24 },
			255, 255, 255, 200);
		app->render->TextDraw("E", npcTalkingTo->position.x + npcTalkingTo->width / 2, npcTalkingTo->position.y - 57, 16, Font::TEXT);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->controller.A != 0)
		{
			lockMovement = true;
			npcTalkingTo->PerformDialogue();
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || app->input->controller.j1_y > 0)
		{
			keyLockUp = false;
			currentAnimation = &idleUpAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP || app->input->controller.j1_y < 0)
		{
			keyLockDown = false;
			currentAnimation = &idleDownAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || app->input->controller.j1_x < 0)
		{
			keyLockLeft = false;
			currentAnimation = &idleLeftAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || app->input->controller.j1_x > 0)
		{
			keyLockRigth = false;
			currentAnimation = &idleRigthAnim;
		}
	}
	else 
	{
		lockMovement = false;
	}

	if(!lockMovement)
	{
		Controller(dtP);
	}

	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}


void Player::OnCollision(PhysBody* physA, PhysBody* physB) 
{
	ListItem<Npc*>* i;

	switch (physB->ctype)
	{
	case ColliderType::NPC:

		i = app->scene->listNpc.start;

		for (i; i != NULL; i = i->next)
		{
			if (i->data->pSensor->id == physB->id)
			{
				npcTalkingTo = i->data;
				break;
			}
		}
		npcInteract = true;
		break;
	default:
		break;
	}
}
void Player::EndContact(PhysBody* physA, PhysBody* physB) 
{
	switch (physB->ctype)
	{
	case ColliderType::NPC:
		npcInteract = false;
		break;
	default:
		break;
	}
}

void Player::Controller(float dt)
{
	if (app->input->godMode_B) 
	{
		if (!keyLockDown && !keyLockLeft && !keyLockRigth)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || app->input->controller.j1_y > 0)
			{
				keyLockUp = true;
				currentAnimation = &upAnim;
				currentAnim.speed = currentAnim.speed * 4;
				vel.y = -125 * 3;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || app->input->controller.j1_x > 0)
				{
					vel.x = 125 * 3;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || app->input->controller.j1_x < 0)
				{
					vel.x = -125 * 3;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || app->input->controller.j1_y > 0)
			{
				keyLockUp = false;
				currentAnimation = &idleUpAnim;
			}
		}
		if (!keyLockUp && !keyLockLeft && !keyLockRigth)
		{
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || app->input->controller.j1_y < 0)
			{
				keyLockDown = true;
				currentAnimation = &downAnim;
				currentAnim.speed = currentAnim.speed * 4;
				vel.y = 125 * 3;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || app->input->controller.j1_x > 0)
				{
					vel.x = 125 * 3;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || app->input->controller.j1_x < 0)
				{
					vel.x = -125 * 3;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP || app->input->controller.j1_y < 0)
			{
				keyLockDown = false;
				currentAnimation = &idleDownAnim;
			}
		}
		if (!keyLockDown && !keyLockUp && !keyLockRigth)
		{
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || app->input->controller.j1_x < 0)
			{
				keyLockLeft = true;
				currentAnimation = &leftAnim;
				currentAnim.speed = currentAnim.speed * 4;
				vel.x = -125 * 3;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || app->input->controller.j1_y < 0)
				{
					vel.y = 125 * 3;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || app->input->controller.j1_y > 0)
				{
					vel.y = -125 * 3;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || app->input->controller.j1_x < 0)
			{
				keyLockLeft = false;
				currentAnimation = &idleLeftAnim;
			}
		}
		if (!keyLockDown && !keyLockLeft && !keyLockUp)
		{
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || app->input->controller.j1_x > 0)
			{
				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				currentAnim.speed = currentAnim.speed * 4;
				vel.x = 125 * 3;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || app->input->controller.j1_y < 0)
				{
					vel.y = 125 * 3;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || app->input->controller.j1_y > 0)
				{
					vel.y = -125 * 3;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || app->input->controller.j1_x > 0)
			{
				keyLockRigth = false;
				currentAnimation = &idleRigthAnim;
			}
		}
	}
	else
	{
		if (!keyLockDown && !keyLockLeft && !keyLockRigth)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || app->input->controller.j1_y > 0)
			{
				keyLockUp = true;
				currentAnimation = &upAnim;
				vel.y = -125;
				vel.x = 0;
			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || app->input->controller.j1_y > 0)
			{
				keyLockUp = false;
				currentAnimation = &idleUpAnim;
			}
		}
		if (!keyLockUp && !keyLockLeft && !keyLockRigth)
		{
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || app->input->controller.j1_y < 0)
			{
				keyLockDown = true;
				currentAnimation = &downAnim;
				vel.y = 125;
				vel.x = 0;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP || app->input->controller.j1_y < 0)
			{
				keyLockDown = false;
				currentAnimation = &idleDownAnim;
			}
		}
		if (!keyLockDown && !keyLockUp && !keyLockRigth)
		{
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || app->input->controller.j1_x < 0)
			{
				keyLockLeft = true;
				currentAnimation = &leftAnim;
				vel.x = -125;
				vel.y = 0;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || app->input->controller.j1_x < 0)
			{
				keyLockLeft = false;
				currentAnimation = &idleLeftAnim;
			}
		}
		if (!keyLockDown && !keyLockLeft && !keyLockUp)
		{
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || app->input->controller.j1_x > 0)
			{
				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125;
				vel.y = 0;
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || app->input->controller.j1_x > 0)
			{
				keyLockRigth = false;
				currentAnimation = &idleRigthAnim;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			vel.x = vel.x * 2;
			vel.y = vel.y * 2;
			currentAnim.speed = currentAnim.speed * 3;
		}

	}
}
