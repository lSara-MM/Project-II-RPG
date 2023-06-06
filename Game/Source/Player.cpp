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
#include "BeastTent.h"
#include "PracticeTent.h"
#include"Circus.h"
#include"PuzzleManager.h"
#include"QuestManager.h"
#include"LootManager.h"

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

	texturePathMiniMap = parameters.attribute("MiniMap").as_string();

	posMiniMap.x = parameters.attribute("xMap").as_int();
	posMiniMap.y = parameters.attribute("yMap").as_int();

	widthMap = parameters.attribute("widthMap").as_int();
	heightMap = parameters.attribute("heightMap").as_int();
	
	texturePathDotPlayer = parameters.attribute("PlayerMiniMap").as_string();

	widthDotPlayer = parameters.attribute("widthDot").as_int();
	heightDotPlayer = parameters.attribute("heightDot").as_int();

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

	//fx confirm interaction
	chestFxPath = "Assets/Audio/Fx/new_item.wav";
	chestfx = app->audio->LoadFx(chestFxPath);

	//animation minimap
	mapAnimation.Set();
	mapAnimation.AddTween(100, 100, BOUNCE_IN_OUT);

	return true;
}

bool Player::Start() 
{
	OpenMap = false;

	miniMap = app->tex->Load(texturePathMiniMap);
	dotPlayer = app->tex->Load(texturePathDotPlayer);
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

	transitionMap_B = true;

	return true;
}

bool Player::Update(float dt)
{
	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			app->map->mapPendingtoDelete = true;
			OpenMap = false;
			lockMovement = false;
			//transitionMap_B = false;
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->hTerrors, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			app->map->mapPendingtoDelete = true;
			OpenMap = false;
			lockMovement = false;
			//transitionMap_B = false;
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->BeastT, 0);
		}
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

	//Camera movement
	if (pbody != nullptr)
	{
		if (app->scene->active == true) //Como tiene una forma tan rara la escena normal hay que hacerle una camara especial
		{
			if (position.y < 1800) //Zona de arriba "pasillo" hacia la tienda
			{
				//Desplazar camara con suavidad (estaria bien un speed up en funcion distancia) hacia el centro
				if (app->render->camera.x > -3450 + 625 - width) { app->render->camera.x -= 2; }
				if (app->render->camera.x < -3450 + 655 - width) { app->render->camera.x += 2; }
				//app->render->camera.x = -3450 + 640 - width;
				if (app->render->camera.y > -position.y + 345 - height) { app->render->camera.y -= 12; }
				if (app->render->camera.y < -position.y + 375 - height) { app->render->camera.y += 12; }
			}
			else
			{
				//Control de X del mapa
				if (position.x > 3875) //Borde Derecho del mapa
				{
					//Llevar la camara a la izquerda para alejarla del lado
					app->render->camera.x = -3875 + 635 - width;
					/*app->render->camera.y = -position.y + 360 - height;*/
				}
				else if (position.x < 2000) //Borde izquierdo mapa
				{
					app->render->camera.x = -2000 + 635 - width;
				}
				else
				{
					//Setear camara al player (con suavidad)

					if (app->render->camera.x > -position.x + 650 - width) { app->render->camera.x -= 8; }
					if (app->render->camera.x < -position.x + 630 - width) { app->render->camera.x += 8; }
					else { app->render->camera.x = -position.x + 640 - width; }
				}

				//Control de Y del mapa
				if (position.y < 2070 && position.y > 1400) //Borde arriba (zona vacia por el pasillo a la carpa)
				{
					//Setear poco a poco
					if (app->render->camera.y > -position.y + 350 - height)
					{ app->render->camera.y -= 4; }
					else if (app->render->camera.y < -position.y + 370 - height)
					{ app->render->camera.y += 4; }
					else { app->render->camera.y = -position.y + 365 - height; }
				}
				else if (position.y > 4240) //Borde de abajo (dungeon horrores)
				{
					app->render->camera.y = -4250 + 365 - height;
				}
				else
				{

					if (app->render->camera.y > -position.y + 385 - height) { app->render->camera.y -= 15; }
					else if (app->render->camera.y < -position.y + 335 - height) { app->render->camera.y += 15; }
					else { app->render->camera.y = -position.y + 360 - height; }
				}

				/*app->render->camera.y = -position.y + 360 - height;
				app->render->camera.x = -position.x + 640 - width;*/
			}
		}
		else if (app->practiceTent->active == true) //La tienda es tan pequeña que necesita de 
		{
			app->render->camera.x = 0;
			app->render->camera.y = 0;
		}
		else
		{
			//Setear camara al player
			app->render->camera.y = -position.y + 360 - height;
			app->render->camera.x = -position.x + 640 - width;

			//Bloquear camara en bordes
			int hMap = app->map->mapData.height * app->map->mapData.tileHeight;
			int wMap = app->map->mapData.width * app->map->mapData.tileWidth;
			if (position.x < 640)
			{
				app->render->camera.x = 0;
			}
			if (position.y < 360)
			{
				app->render->camera.y = 0;
			}
			if (position.x > wMap - 640)
			{
				app->render->camera.x = -(wMap - app->render->camera.w);
			}
			if (position.y > hMap - 360)
			{
				app->render->camera.y = -(hMap - app->render->camera.h);
			}
		}
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

		/*MiniMapa*/
		posMiniMap.x = -app->render->camera.x + app->render->camera.w / 2 - widthMap / 2;
		posMiniMap.y = -app->render->camera.y + app->render->camera.h / 2 - heightMap / 2;

		posMiniPlayer.x = posMiniMap.x + position.x / 10;
		posMiniPlayer.y = posMiniMap.y + position.y / 10;

		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_X) == ButtonState::BUTTON_DOWN)
		{
			MiniMap();
		}

		/*if (OpenMap)
		{*/
			if (transitionMap_B)
			{
				mapAnimation.Backward();
			}
			else
			{
				mapAnimation.Foward();
			}
			mapAnimation.Step(1, false);

			float point = mapAnimation.GetPoint();
			int offset = -1300;
			app->render->DrawTexture(miniMap, posMiniMap.x, offset + point * (posMiniMap.y - offset));

				if (app->scene->active)
				{
					//2659 4907
					app->render->DrawTexture(dotPlayer, posMiniPlayer.x - 23, offset + point * (posMiniPlayer.y - 30 - offset));
				}
				if (app->hTerrors->active)
				{
					app->render->DrawTexture(dotPlayer, posMiniMap.x + 2259 / 10, offset + point * (posMiniMap.y + 4507 / 10 - offset));
				}
				if (app->practiceTent->active)
				{
					//4360 4385
					app->render->DrawTexture(dotPlayer, posMiniMap.x + 4460 / 10, offset + point * (posMiniMap.y + 3585 / 10 - offset));
				}
				if (app->circus->active)
				{
					//3899 1027
					app->render->DrawTexture(dotPlayer, posMiniMap.x + 3250 / 10, offset + point * (posMiniMap.y + 350 / 10 - offset));
				}
			
		//}
		/*MiniMapa*/

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
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
				{
					app->audio->PlayFx(confirmInteractfx);
					app->dialogueSystem->hasEnded = false;
					lockMovement = true;
					pauseEnabled_B = false;
					app->dialogueSystem->Enable();
					app->dialogueSystem->PerformDialogue(npcTalkingTo->dialoguesID);
					
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

			if (app->dialogueSystem->hasEnded && !app->store->active) 
			{
				lockMovement = false; 
				pauseEnabled_B = true;
				app->dialogueSystem->Disable();
			}

		}

		if (Chest_contact)
		{
			app->render->DrawTexture(textureE, app->lootManager->chests[Chest_ID]->x + 40, app->lootManager->chests[Chest_ID]->y - 20);

			if (interactionTest == false)
			{
				app->audio->PlayFx(interactionfx);
				interactionTest = true;
			}

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				Chest_contact = false;
				interactionTest = false;
				app->lootManager->chests[Chest_ID]->UseChest();
				app->audio->PlayFx(chestfx);
			}
		}

		if (!lockMovement)
		{
			Controller(dtP);
		}

		return true;
}


bool Player::CleanUp()
{
	if(texture != nullptr)
		app->tex->UnLoad(texture);
	
	if(miniMap != nullptr)
		app->tex->UnLoad(miniMap);

	if(dotPlayer != nullptr)
		app->tex->UnLoad(dotPlayer);

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
	case ColliderType::LOOT:
		LOG("LOOT Interact");

		for (int i = 0; i < app->lootManager->chests.size(); i++)
		{
			if (app->lootManager->chests[i]->sensor != nullptr)
			{
				if (app->lootManager->chests[i]->sensor->id == physB->id)
				{
					Chest_ID = i;
					Chest_contact = true;
					break;
				}
			}
		}
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
	case ColliderType::KEY:
		app->puzzleManager->keySens = true;	
	case ColliderType::KEYDOOR:
		switch (physB->id)
		{
			case 1:
				app->puzzleManager->DoorContact1 = true;
				break;
			case 2:
				app->puzzleManager->DoorContact2 = true;
				break;
		}
		break;
	case ColliderType::BARRICADE:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->BarricadeContact1 = true;
			break;
		case 2:
			app->puzzleManager->BarricadeContact2 = true;
			break;
		case 3:
			app->puzzleManager->BarricadeContact3 = true;
			break;
		case 4:
			app->puzzleManager->BarricadeContact4 = true;
			break;
		case 5:
			app->puzzleManager->BarricadeContact5 = true;
			break;
		}
		break;
	case ColliderType::BOMB:
		switch (physB->id)
		{
			case 1:
				if(!app->puzzleManager->BombCarryOn1)
					app->puzzleManager->BombContact1 = true;
				break;
			case 2:
				if (!app->puzzleManager->BombCarryOn2)
					app->puzzleManager->BombContact2 = true;
				break;
		}
		break;
	case ColliderType::RELIC:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->RelicContact1 = true;
			break;
		case 2:
			app->puzzleManager->RelicContact2 = true;
			break;
		case 3:
			app->puzzleManager->RelicContact3 = true;
			break;
		}
		break;
	case ColliderType::RELICOLUMN:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->RelicColumnContact1 = true;
			break;
		case 2:
			app->puzzleManager->RelicColumnContact2 = true;
			break;
		case 3:
			app->puzzleManager->RelicColumnContact3 = true;
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
				app->fade->FadingToBlack((Module*)app->scene, (Module*)app->BeastT, fadeTime);
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
			if (app->BeastT->active == true)
			{
				app->entityManager->tpID = 22;
				app->fade->FadingToBlack((Module*)app->BeastT, (Module*)app->scene, fadeTime);
			}
			break;
		case 2:
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->circus, fadeTime);
			break;
		case 1:
			app->fade->FadingToBlack((Module*)app->scene, (Module*)app->hTerrors, fadeTime);
			break;
		case 3:
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
	case ColliderType::LOOT:
		Chest_contact = false;
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
	case ColliderType::KEY:
		app->puzzleManager->keySens = false;
	case ColliderType::KEYDOOR:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->DoorContact1 = false;
			break;
		case 2:
			app->puzzleManager->DoorContact2 = false;
			break;
		}
		break;
	case ColliderType::BARRICADE:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->BarricadeContact1 = false;
			break;
		case 2:
			app->puzzleManager->BarricadeContact2 = false;
			break;
		case 3:
			app->puzzleManager->BarricadeContact3 = false;
			break;
		case 4:
			app->puzzleManager->BarricadeContact4 = false;
			break;
		case 5:
			app->puzzleManager->BarricadeContact5 = false;
			break;
		}
		break;
	case ColliderType::BOMB:
		switch (physB->id)
		{
		case 1:
			if(!app->puzzleManager->BombCarryOn1)
				app->puzzleManager->BombContact1 = false;
			break;
		case 2:
			if (!app->puzzleManager->BombCarryOn2)
				app->puzzleManager->BombContact2 = false;
			break;
		}
		break;
	case ColliderType::RELIC:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->RelicContact1 = false;
			break;
		case 2:
			app->puzzleManager->RelicContact2 = false;
			break;
		case 3:
			app->puzzleManager->RelicContact3 = false;
			break;
		}
		break;
	case ColliderType::RELICOLUMN:
		switch (physB->id)
		{
		case 1:
			app->puzzleManager->RelicColumnContact1 = false;
			break;
		case 2:
			app->puzzleManager->RelicColumnContact2 = false;
			break;
		case 3:
			app->puzzleManager->RelicColumnContact3 = false;
			break;
		}
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
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockUp = true;
				currentAnimation = &upAnim;
				vel.y = -125 * 8;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || 
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
				{
					vel.x = 125 * 8;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
				{
					vel.x = -125 * 8;
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
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockDown = true;
				currentAnimation = &downAnim;
				vel.y = 125 * 8;

				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
				{
					vel.x = 125 * 8;
				}
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
				{
					vel.x = -125 * 8;
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
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockLeft = true;
				currentAnimation = &leftAnim;
				vel.x = -125 * 8;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
				{
					vel.y = 125 * 8;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT ||
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT || app->input->controller.j1_y < 0)
				{
					vel.y = -125 * 8;
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
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125 * 8;

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || 
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
				{
					vel.y = 125 * 8;
				}
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || 
					app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT || app->input->controller.j1_y < 0)
				{
					vel.y = -125 * 8;
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
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.y += 125 * dtP; //MoverCamara
				keyLockUp = true;
				currentAnimation = &upAnim;
				vel.y = -125 * 4;
				vel.x = 0;
				app->hTerrors->steps_I++;
				app->BeastT->steps_I++;
				
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
					app->BeastT->steps_I++;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT || app->input->controller.j1_y > 0)
			{
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.y += -125 * dtP; //Mover camara
				keyLockDown = true;
				currentAnimation = &downAnim;
				vel.y = 125 * 4;
				vel.x = 0;
				app->hTerrors->steps_I++;
				app->BeastT->steps_I++;
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
					app->BeastT->steps_I++;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT || app->input->controller.j1_x < 0)
			{
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.x += 125 * dtP;
				keyLockLeft = true;
				currentAnimation = &leftAnim;
				vel.x = -125 * 4;
				vel.y = 0;
				app->hTerrors->steps_I++;
				app->BeastT->steps_I++;
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
					app->BeastT->steps_I++;
				}
			}
			//Mantenerla pulsada (moverse)
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT ||
				app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT || app->input->controller.j1_x > 0)
			{
				if (!app->input->stepSound_B)
				{
					app->audio->PlayFx(walk_grass, 0);
				}

				//app->render->camera.x += -125 * dtP;
				keyLockRigth = true;
				currentAnimation = &rigthAnim;
				vel.x = 125 * 4;
				vel.y = 0;
				app->hTerrors->steps_I++;
				app->BeastT->steps_I++;
				//Mover camara
				app->render->camera.x -= ceil(speed);
			}
		}

		currentAnim.speed = currentAnim.speed * 3;

		//Shift acelerar la velocidad
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_REPEAT) {
			vel.x = vel.x * 1.5;
			vel.y = vel.y * 1.5;
			currentAnim.speed = currentAnim.speed * 6;
		}
	}
	
	PadLock = false;
}

void Player::MiniMap() 
{
	OpenMap = !OpenMap;
	lockMovement = !lockMovement;
	transitionMap_B = !transitionMap_B;
}