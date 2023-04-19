#include "Player.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Window.h"
#include "Textures.h"

#include "Scene.h"
#include "HouseOfTerrors.h"
#include "Circus.h"
#include "PracticeTent.h"

#include "FadeToBlack.h"
#include "EntityManager.h"
#include "DialogueSystem.h"
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

	grass_path = "Assets/Audio/Fx/Pasos_Hierba.wav";
	walk_grass = app->audio->LoadFx(grass_path);

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(texturePath);
	currentAnimation = &currentAnim;
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);	
	pbody->listener = this; 
	pbody->ctype = ColliderType::PLAYER;
	
	playerName = app->input->playerName.c_str();
	npcInteract = false;

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

	//Sara aquí tienes tu parte, donde cuando el player está dentro de la zona interactuable con el npc
	//if (npcInteract) 
	//{
	//	app->render->DrawRectangle({ npcTalkingTo->position.x, npcTalkingTo->position.y - 60, 24, 24 },
	//		255, 255, 255, 200);
	//	app->render->TextDraw("E", npcTalkingTo->position.x + npcTalkingTo->width / 2, npcTalkingTo->position.y - 57, 16, Font::TEXT);

	//	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) 
	//	{
	//		npcTalkingTo->PerformDialogue();
	//	}
	//}

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

	case ColliderType::PORTAL:
		switch (physB->id)
		{
		case 0:
			if (app->scene->active == true)
			{
				app->map->mapPendingtoDelete = true;
				app->fade->FadingToBlack((Module*)app->scene, (Module*)app->hTerrors, 90);
			}
			if (app->hTerrors->active == true)
			{
				app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->scene, 90);
			}
			if (app->practiceTent->active == true)
			{
				app->fade->FadingToBlack((Module*)app->practiceTent, (Module*)app->scene, 90);
			}
			if (app->circus->active == true)
			{
				app->fade->FadingToBlack((Module*)app->circus, (Module*)app->scene, 90);
			}
			break;
		case 1:
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->circus, 90);
			break;
		case 2:
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->practiceTent, 90);
			break;
		}
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
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				app->audio->PlayFx(walk_grass, 0);

				keyLockUp = true;
				currentAnimation = &upAnim;
				vel.y = -125 * 2;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
				{
					vel.x = 125 * 2;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
				{
					vel.x = -125 * 2;
				}
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
				app->audio->PlayFx(walk_grass, 0);

				keyLockDown = true;
				currentAnimation = &downAnim;
				vel.y = 125 * 2;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
				{
					vel.x = 125 * 2;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
				{
					vel.x = -125 * 2;
				}
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
				app->audio->PlayFx(walk_grass, 0);

				keyLockLeft = true;
				currentAnimation = &leftAnim;
				vel.x = -125 * 2;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
				{
					vel.y = 125 * 2;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
				{
					vel.y = -125 * 2;
				}
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
				app->audio->PlayFx(walk_grass, 0);

				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125 * 2;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
				{
					vel.y = 125 * 2;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
				{
					vel.y = -125 * 2;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
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
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				app->audio->PlayFx(walk_grass, 0);

				app->render->camera.y += 125 * dtP;
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
				app->audio->PlayFx(walk_grass, 0);

				app->render->camera.y += -125 * dtP;
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
				app->audio->PlayFx(walk_grass, 0);

				app->render->camera.x += 125 * dtP;
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
				app->audio->PlayFx(walk_grass, 0);

				app->render->camera.x += -125 * dtP;
				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125;
				vel.y = 0;
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
			{
				keyLockRigth = false;
				currentAnimation = &idleRigthAnim;
			}
		}
	}
}
