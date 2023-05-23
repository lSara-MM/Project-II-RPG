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
#include "PracticeTent.h"
#include"Circus.h"
#include"PuzzleManager.h"
#include"QuestManager.h"

#include "FadeToBlack.h"
#include "EntityManager.h"
#include "DialogueSystem.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"

#include "External/SDL/include/SDL_gamecontroller.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	//name.Create("player");

	currentAnim.PushBack({ 96, 0, 96, 96 });

	downAnim.PushBack({ 0, 0, 96, 96 });
	downAnim.PushBack({ 96, 0, 96, 96 });
	downAnim.PushBack({ 192, 0, 96, 96 });
	downAnim.PushBack({ 96, 0, 96, 96 });

	idleDownAnim.PushBack({96, 0, 96, 96 });

	upAnim.PushBack({ 0, 96, 96, 96 });
	upAnim.PushBack({ 96, 96, 96, 96 });
	upAnim.PushBack({ 192, 96, 96, 96 });
	upAnim.PushBack({ 96, 96, 96, 96 });

	idleUpAnim.PushBack({96, 96, 96, 96 });
	
	leftAnim.PushBack({ 0, 192, 96, 96 });
	leftAnim.PushBack({ 96, 192, 96,96 });
	leftAnim.PushBack({ 192, 192, 96, 96 });
	leftAnim.PushBack({ 96, 192, 96, 96 });

	idleLeftAnim.PushBack({ 96, 192, 96, 96 });

	rigthAnim.PushBack({ 0,292, 96,96 });
	rigthAnim.PushBack({ 96, 292, 96, 96 });
	rigthAnim.PushBack({ 192, 292, 96, 96 });
	rigthAnim.PushBack({ 96, 292, 96, 96 });

	idleRigthAnim.PushBack({ 96, 292, 96, 96 });

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

	//fx enter zone
	enterPath = "Assets/Audio/Fx/entrar_sala.wav";
	enterZone = app->audio->LoadFx(enterPath);

	//fx enter interaction
	interactionPath = "Assets/Audio/Fx/Npc_interaction_sensor.wav";
	interactionfx = app->audio->LoadFx(interactionPath);
	interactionTest = false;

	//fx confirm interaction
	confirmPath = "Assets/Audio/Fx/confirm_interaction.wav";
	confirmInteractfx = app->audio->LoadFx(confirmPath);

	return true;
}

bool Player::Start() 
{
	texture = app->tex->Load(texturePath);
	textureE = app->tex->Load("Assets/GUI/UI_E.png");

	currentAnimation = &currentAnim;

	pbody = app->physics->CreateCircle(position.x - width / 2, position.y - height / 2, 16*1.8, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	flipType = SDL_FLIP_NONE;

	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	name = app->input->playerName->input.c_str();
	npcInteract = false;

	pbody->body->SetGravityScale(0);

	PadLock = false;
	lockMovement = false;
	pauseEnabled_B = true;

	//Setear camara al player al crearlo cada vez
	app->render->camera.y = -position.y + 360 - height;
	app->render->camera.x = -position.x + 640 - width;

	return true;
}

bool Player::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		app->map->mapPendingtoDelete = true;		
		app->fade->FadingToBlack((Module*)app->scene, (Module*)app->hTerrors, 0);
	}

	/*Hasta aqu� PuzzleManager*/

	if (app->scene->pause_B || app->hTerrors->pause_B || app->circus->pause_B || app->practiceTent->pause_B)
	{
		dtP = 0;
	}
	else if (!app->scene->pause_B || !app->hTerrors->pause_B || !app->circus->pause_B || !app->practiceTent->pause_B)
	{
		dtP = dt / 1000;
	}

	if (pbody != nullptr)
	{
		if (app->scene->active==true) //Como tiene una forma tan rara la escena normal hay que hacerle una camara especial
		{
			if (position.y<1760) //Zona de arriba "pasillo" hacia la tienda
			{
				//Desplazar camara con suavidad (estaria bien un speed up en funcion distancia) hacia el centro
				if (app->render->camera.x > -3450 + 635 - width) { app->render->camera.x -= 4; }
				if (app->render->camera.x < -3450 + 645 - width) { app->render->camera.x += 4; }
				app->render->camera.y = -position.y + 360 - height;
			}
			else
			{
				//Control de X del mapa
				if (position.x > 3500) //Borde Derecho del mapa
				{
					//Llevar la camara a la izquerda para alejarla del lado
					if (app->render->camera.x < -3725 + 635 - width) { app->render->camera.x += 8; }
					/*app->render->camera.y = -position.y + 360 - height;*/
				}
				else
				{
					//Setear camara al player (con suavidad)
					if (app->render->camera.x > -position.x + 635 - width) { app->render->camera.x += vel.x * dtP; } //X va al reves de Y 
					if (app->render->camera.x < -position.x + 645 - width) { app->render->camera.x -= vel.x * dtP; }
				}
			
				if (app->render->camera.y > -position.y + 355 - height) { app->render->camera.y -= vel.y * dtP; }
				if (app->render->camera.y < -position.y + 365 - height) { app->render->camera.y += vel.y * dtP; }
				/*app->render->camera.y = -position.y + 360 - height;
				app->render->camera.x = -position.x + 640 - width;*/
			}
		}
		else
		{
			//Setear camara al player
			app->render->camera.y = -position.y + 360 - height;
			app->render->camera.x = -position.x + 640 - width;
		}
		
		



		vel = b2Vec2(vel.x * dtP, vel.y * dtP);
		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width / 2;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height / 2;

		currentAnimation->Update();

		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - width * 2, position.y - height * 2, &rect, 1.0f, NULL, NULL, NULL, flipType);

		if (npcInteract)
		{
			app->render->DrawTexture(textureE, npcTalkingTo->position.x + npcTalkingTo->width / 2 - 12, npcTalkingTo->position.y - 60);

			if (interactionTest == false)
			{
				app->audio->PlayFx(interactionfx);
				interactionTest = true;
			}

			if (!lockMovement)
			{
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_Y) == BUTTON_DOWN)
				{
					app->audio->PlayFx(confirmInteractfx);
					app->dialogueSystem->hasEnded = false;
					lockMovement = true;
					pauseEnabled_B = false;
					app->dialogueSystem->Enable();
					app->dialogueSystem->PerformDialogue(npcTalkingTo->dialoguesID);
;					
					if (keyLockUp)
					{
						keyLockUp = false;
						currentAnimation = &idleUpAnim;
					}

					if (keyLockDown)
					{
						keyLockDown = false;
						currentAnimation = &idleDownAnim;
					}

					if (keyLockLeft)
					{
						keyLockLeft = false;
						currentAnimation = &idleLeftAnim;
					}

					if (keyLockRigth)
					{
						keyLockRigth = false;
						currentAnimation = &idleRigthAnim;
					}
				}
			}

			if (app->dialogueSystem->hasEnded) 
			{
				lockMovement = false; 
				pauseEnabled_B = true;
				app->dialogueSystem->Disable();
			}
		}

		if (!lockMovement)
		{
			Controller(dtP);
		}

		return true;
	}
}

bool Player::CleanUp()
{
	if(texture != nullptr)
		app->tex->UnLoad(texture);

	if(textureE != nullptr)
		app->tex->UnLoad(textureE);

	if (pbody != nullptr)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

	return true;
}


void Player::OnCollision(PhysBody* physA, PhysBody* physB) 
{
	ListItem<Npc*>* i;

	switch (physB->ctype)
	{
	case ColliderType::NPC:
		LOG("NPC Interact");

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

	case ColliderType::DUMMY:
		app->practiceTent->DummySensor = true;
		break;
	case ColliderType::PALANCA:
		app->puzzleManager->palancasActive = true;
		break;
	case ColliderType::BOSSDEAD:
		app->puzzleManager->bossActive = true;
		break;
	case ColliderType::LOSET:
		app->puzzleManager->losetActive = true;
		break;
	case ColliderType::DOORCODE:
		app->puzzleManager->intoCode = true;
		break;
	case ColliderType::FIREGUY:
		app->puzzleManager->saveFireGuy = true;
		break;
	case ColliderType::NOTA:
		switch (physB->id) //Abrir Nota + sumar puntos a keyScape
		{
		case 0:
			if (!app->puzzleManager->esc1) 
			{
				app->puzzleManager->esc1 = true; //Este bool es para evitar sumar puntos al recoger la misma nota
			}
			break;
		case 1:
			if (!app->puzzleManager->esc2) 
			{
				app->puzzleManager->esc2 = true; //Este bool es para evitar sumar puntos al recoger la misma nota
			}
			break;
		case 2:
			if (!app->puzzleManager->esc3) 
			{
				app->puzzleManager->esc3 = true; //Este bool es para evitar sumar puntos al recoger la misma nota
			}
			break;
		}
		break;
	case ColliderType::PORTAL:
		int fadeTime = 45;
		switch (physB->id)
		{
		case 0:
			if (app->hTerrors->active == true)
			{
				app->entityManager->tpID = 0;
				app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->scene, fadeTime);
			}
			if (app->scene->active == true)
			{
				app->map->mapPendingtoDelete = true;
				app->fade->FadingToBlack((Module*)app->scene, (Module*)app->hTerrors, fadeTime);
			}
			if (app->practiceTent->active == true)
			{
				app->entityManager->tpID = 1;
				app->fade->FadingToBlack((Module*)app->practiceTent, (Module*)app->scene, fadeTime);
			}
			if (app->circus->active == true)
			{
				app->questManager->SaveState();
				app->entityManager->tpID = 21;
				app->fade->FadingToBlack((Module*)app->circus, (Module*)app->scene, fadeTime);
			}
			break;
		case 1:
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->circus, fadeTime);
			break;
		case 2:
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->practiceTent, fadeTime);
			break;
		}
		break;
	}
}

void Player::EndContact(PhysBody* physA, PhysBody* physB) 
{
	switch (physB->ctype)
	{
	case ColliderType::NPC:
		npcInteract = false;
		interactionTest = false;
		break; 
	case ColliderType::DUMMY:
		app->practiceTent->DummySensor = false;
		break;
	case ColliderType::PALANCA:
		app->puzzleManager->palancasActive = false;
		break;
	case ColliderType::BOSSDEAD:
		app->puzzleManager->bossActive = false;
		break;
	case ColliderType::LOSET:
		app->puzzleManager->losetActive = false;
		break;
	case ColliderType::DOORCODE:
		app->puzzleManager->intoCode = false;
		break;
	case ColliderType::FIREGUY:
		app->puzzleManager->saveFireGuy = false;
		break;
	case ColliderType::PORTAL:
		app->audio->PlayFx(enterZone, 0);
	case ColliderType::NOTA:
		//Apagar Dialogos
		app->puzzleManager->esc1 = false;
		app->puzzleManager->esc2 = false;
		app->puzzleManager->esc3 = false;
		break;
	default:
		break;
	}
}

void Player::Controller(float dt)
{

	//Velocidad movimiento camara
	float speed = 0.2 * dt;

	if (app->input->godMode_B) //Movimiento libre de GOD MODE
	{

		if (!keyLockDown && !keyLockLeft && !keyLockRigth)
		{
			if (PadLock == false && keyLockUp)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_UP || app->input->controller.j1_y == 0)
				{
					PadLock = true;
					keyLockUp = false;
					currentAnimation = &idleUpAnim;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || 
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT || app->input->controller.j1_y < 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockUp = true;
				currentAnimation = &upAnim;
				vel.y = -125 * 6;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || 
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
				{
					vel.x = 125 * 6;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
				{
					vel.x = -125 * 6;
				}
			}
		}
		if (!keyLockUp && !keyLockLeft && !keyLockRigth)
		{
			if (PadLock == false && keyLockDown)
			{
				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_UP || app->input->controller.j1_y == 0)
				{
					PadLock = true;
					keyLockDown = false;
					currentAnimation = &idleDownAnim;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || 
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockDown = true;
				currentAnimation = &downAnim;
				vel.y = 125 * 6;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
				{
					vel.x = 125 * 6;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
				{
					vel.x = -125 * 6;
				}
			}
		}
		if (!keyLockDown && !keyLockUp && !keyLockRigth)
		{
			if (PadLock == false && keyLockLeft)
			{
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_UP || app->input->controller.j1_x == 0)
				{
					PadLock = true;
					keyLockLeft = false;
					currentAnimation = &idleLeftAnim;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || 
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockLeft = true;
				currentAnimation = &leftAnim;
				vel.x = -125 * 6;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
				{
					vel.y = 125 * 6;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT || app->input->controller.j1_y < 0)
				{
					vel.y = -125 * 6;
				}
			}
		}
		if (!keyLockDown && !keyLockLeft && !keyLockUp)
		{
			if (PadLock == false && keyLockRigth)
			{
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_UP || app->input->controller.j1_x == 0)
				{
					PadLock = true;
					keyLockRigth = false;
					currentAnimation = &idleRigthAnim;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125 * 6;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || 
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
				{
					vel.y = 125 * 6;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || 
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT || app->input->controller.j1_y < 0)
				{
					vel.y = -125 * 6;
				}
			}
		}
	}
	else //Movimiento normal
	{
		if (!keyLockDown && !keyLockLeft && !keyLockRigth)
		{
			if (PadLock == false && keyLockUp)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_UP || app->input->controller.j1_y == 0)
				{
					PadLock = true;
					keyLockUp = false;
					currentAnimation = &idleUpAnim;
					app->hTerrors->steps_I++;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT || app->input->controller.j1_y < 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.y += 125 * dtP; //MoverCamara
				keyLockUp = true;
				currentAnimation = &upAnim;
				vel.y = -125 * 2;
				vel.x = 0;
				app->hTerrors->steps_I++;

				
			}
		}
		if (!keyLockUp && !keyLockLeft && !keyLockRigth)
		{
			if (PadLock == false && keyLockDown)
			{
				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_UP || app->input->controller.j1_y == 0)
				{
					PadLock = true;
					keyLockDown = false;
					currentAnimation = &idleDownAnim;
					app->hTerrors->steps_I++;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.y += -125 * dtP; //Mover camara
				keyLockDown = true;
				currentAnimation = &downAnim;
				vel.y = 125 * 2;
				vel.x = 0;
				app->hTerrors->steps_I++;

				
			}
		}
		if (!keyLockDown && !keyLockUp && !keyLockRigth)
		{
			if (PadLock == false && keyLockLeft)
			{
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_UP || app->input->controller.j1_x == 0)
				{
					PadLock = true;
					keyLockLeft = false;
					currentAnimation = &idleLeftAnim;
					app->hTerrors->steps_I++;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.x += 125 * dtP;
				keyLockLeft = true;
				currentAnimation = &leftAnim;
				vel.x = -125 * 2;
				vel.y = 0;
				app->hTerrors->steps_I++;

				//Mover camara
				app->render->camera.x += ceil(speed);
			}
		}

		//Movimiento derecha
		if (!keyLockDown && !keyLockLeft && !keyLockUp) //Mirar que no haya otra tecla pulsada
		{
			if (PadLock == false && keyLockRigth) //Mirar que esa tecla se puede mover
			{
				//Tecla pulsada primera vez
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_UP || app->input->controller.j1_x == 0)
				{

					PadLock = true;
					keyLockRigth = false;
					currentAnimation = &idleRigthAnim;
					app->hTerrors->steps_I++;
				}
			}
			//Mantenerla pulsada (moverse)
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
			{
				if (!app->scene->pause_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.x += -125 * dtP;
				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125 * 2;
				vel.y = 0;
				app->hTerrors->steps_I++;

				//Mover camara
				app->render->camera.x -= ceil(speed);
			}
		}

		//Shift acelerar la velocidad
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_REPEAT) {
			vel.x = vel.x * 2;
			vel.y = vel.y * 2;
			currentAnim.speed = currentAnim.speed * 3;
		}
	}
	
	PadLock = false;
}