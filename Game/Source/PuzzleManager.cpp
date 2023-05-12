#include "Scene.h"
#include "PuzzleManager.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Player.h"

#include "Combat.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "DialogueSystem.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include "Point.h"

#include <iostream>
using namespace std;
#include <sstream>

PuzzleManager::PuzzleManager() : Module()
{
	name.Create("PuzzleManager");
}

PuzzleManager::~PuzzleManager()
{

}

bool PuzzleManager::Awake(pugi::xml_node& config)
{
	LOG("Loading PuzzleManager");
	bool ret = true;

	texturepathDoor = config.child("Door").attribute("texturepathDoor").as_string();
	texturepathPalanca = config.child("Palanca").attribute("texturepathPalanca").as_string();
	texturepathNotas = config.child("Notas").attribute("texturepathNotas").as_string();
	texturepathDoorEscape = config.child("DoorEscape").attribute("texturepathDoorEscape").as_string();
	texturepathBoss = config.child("Boss").attribute("texturepathBoss").as_string();
	texturepathLoset = config.child("Loset").attribute("texturepathLoset").as_string();

	posNotas1.x = config.child("Notas").attribute("x1").as_int();
	posNotas1.y = config.child("Notas").attribute("y1").as_int();	
	
	posNotas2.x = config.child("Notas").attribute("x2").as_int();
	posNotas2.y = config.child("Notas").attribute("y2").as_int();	
	
	posNotas3.x = config.child("Notas").attribute("x3").as_int();
	posNotas3.y = config.child("Notas").attribute("y3").as_int();

	posDoor1.x = config.child("Door").attribute("x").as_int();
	posDoor1.y = config.child("Door").attribute("y").as_int();

	posDoor2.x = config.child("Door").attribute("x1").as_int();
	posDoor2.y = config.child("Door").attribute("y1").as_int();
	
	posDoor3.x = config.child("Door").attribute("x2").as_int();
	posDoor3.y = config.child("Door").attribute("y2").as_int();
	
	posBoss.x = config.child("Boss").attribute("x").as_int();
	posBoss.y = config.child("Boss").attribute("y").as_int();	
	
	posLoset.x = config.child("Loset").attribute("x").as_int();
	posLoset.y = config.child("Loset").attribute("y").as_int();

	widthDoor = config.child("Door").attribute("width").as_int();
	heightDoor = config.child("Door").attribute("height").as_int();

	posPalancas.x = config.child("Palanca").attribute("Xpalancas").as_int();
	posPalancas.y = config.child("Palanca").attribute("Ypalancas").as_int();

	widthPalanca = config.child("Palanca").attribute("widthPalanca").as_int();
	heightPalanca = config.child("Palanca").attribute("heightPalanca").as_int();

	widthPalancaSens = config.child("PalancaSens").attribute("widthPalancaSens").as_int();
	heightPalancaSens = config.child("PalancaSens").attribute("heightPalancaSens").as_int();
		
	widthNotas = config.child("Notas").attribute("widthNotas").as_int();
	heightNotas = config.child("Notas").attribute("heightNotas").as_int();

	posDoorEscape.x = config.child("DoorEscape").attribute("x").as_int();
	posDoorEscape.y = config.child("DoorEscape").attribute("y").as_int();

	widthDoorEscape = config.child("DoorEscape").attribute("width").as_int();
	heightDoorEscape = config.child("DoorEscape").attribute("height").as_int();
	
	widthBoss = config.child("Boss").attribute("width").as_int();
	heightBoss = config.child("Boss").attribute("height").as_int();
	
	widthLoset= config.child("Boss").attribute("width").as_int();
	heightLoset = config.child("Boss").attribute("height").as_int();
	
	return ret;
}

bool PuzzleManager::Start()
{
	palancas = false;
	escape = false;
	rescue = false;

	esc1 = false;
	esc2 = false;
	esc3 = false;

	codeActive = false;
	bossActive = false;
	losetActive = false;
	bossInvent = false;
	intoCode = false;

	keyPalancas = 0;
	keyEscape = 0;
	keyRescue = 0;

	realCode = "1234";

	app->LoadGameRequest();

	if (palancas == false) 
	{
		door = app->tex->Load(texturepathDoor);
		palanca = app->tex->Load(texturepathPalanca);

		Door1 = app->physics->CreateRectangle(posDoor1.x - widthDoor / 2, posDoor1.y - heightDoor / 2, widthDoor, heightDoor, bodyType::STATIC);
		Door1->body->SetFixedRotation(true);

		Door2 = app->physics->CreateRectangle(posDoor2.x - widthDoor / 2, posDoor2.y - heightDoor / 2, widthDoor, heightDoor, bodyType::STATIC);
		Door2->body->SetFixedRotation(true);

		Palanca = app->physics->CreateRectangle(posPalancas.x - widthPalanca / 2, posPalancas.y - heightPalanca / 2, widthPalanca, heightPalanca, bodyType::STATIC);
		Palanca->body->SetFixedRotation(true);

		PalancaSensor = app->physics->CreateRectangle(posPalancas.x - widthPalancaSens / 2, posPalancas.y - heightPalancaSens / 2, widthPalancaSens, heightPalancaSens, bodyType::STATIC);
		PalancaSensor->body->SetFixedRotation(true);
		PalancaSensor->ctype = ColliderType::PALANCA;
	}

	if (rescue == false) 
	{
		door = app->tex->Load(texturepathDoor);
		boss = app->tex->Load(texturepathBoss);
		loset = app->tex->Load(texturepathLoset);
		
		Boss = app->physics->CreateRectangleSensor(posBoss.x - widthBoss / 2, posBoss.y - heightBoss / 2, widthBoss, heightBoss, bodyType::STATIC);
		Boss->body->SetFixedRotation(true);
		Boss->ctype = ColliderType::BOSSDEAD;

		Loset = app->physics->CreateRectangleSensor(posLoset.x - widthLoset / 2, posLoset.y - heightLoset / 2, widthLoset, heightLoset, bodyType::STATIC);
		Loset->body->SetFixedRotation(true);
		Loset->ctype = ColliderType::LOSET;

		Door3 = app->physics->CreateRectangle(posDoor3.x - widthDoor / 2, posDoor3.y - heightDoor / 2, widthDoor, heightDoor, bodyType::STATIC);
		Door3->body->SetFixedRotation(true);
	}

	if (escape == false)
	{
		notas = app->tex->Load(texturepathNotas);
		doorEscape = app->tex->Load(texturepathDoorEscape);

		DoorEscape = app->physics->CreateRectangle(posDoorEscape.x - widthDoorEscape / 2, posDoorEscape.y - heightDoorEscape / 2, widthDoorEscape, heightDoorEscape, bodyType::STATIC);
		DoorEscape->body->SetFixedRotation(true);

		DoorEscapeSensor = app->physics->CreateRectangleSensor(posDoorEscape.x - widthDoorEscape / 2, posDoorEscape.y - heightDoorEscape / 2, widthDoorEscape, heightDoorEscape * 2, bodyType::STATIC);
		DoorEscapeSensor->body->SetFixedRotation(true);
		DoorEscapeSensor->ctype = ColliderType::DOORCODE;

		nota1 = app->physics->CreateRectangleSensor(posNotas1.x - widthNotas / 2, posNotas1.y - heightNotas / 2, widthNotas, heightNotas, bodyType::STATIC);
		nota1->body->SetFixedRotation(true);
		nota1->ctype = ColliderType::NOTA;
		nota1->id = 0;

		nota2 = app->physics->CreateRectangleSensor(posNotas2.x - widthNotas / 2, posNotas2.y - heightNotas / 2, widthNotas, heightNotas, bodyType::STATIC);
		nota2->body->SetFixedRotation(true);
		nota2->ctype = ColliderType::NOTA;
		nota2->id = 1;

		nota3 = app->physics->CreateRectangleSensor(posNotas3.x - widthNotas / 2, posNotas3.y - heightNotas / 2, widthNotas, heightNotas, bodyType::STATIC);
		nota3->body->SetFixedRotation(true);
		nota3->ctype = ColliderType::NOTA;
		nota3->id = 2;
	}



	numCode = new PlayerInput("", 4, false);

	return true;
}

bool PuzzleManager::PreUpdate()
{
	return true;
}

bool PuzzleManager::Update(float dt)
{
	if (!palancas)
	{
		if (Palancas())
		{

		}
	}

	if (!escape) 
	{
		Escape();
	}

	if (!rescue)
	{
		if (Rescue())
		{

		}
	}

	return true;
}

bool PuzzleManager::PostUpdate()
{

	return true;
}

// Called before quitting
bool PuzzleManager::CleanUp()
{
	LOG("Freeing scene");

	if(door != nullptr)
		app->tex->UnLoad(door);

	if (palanca != nullptr)
		app->tex->UnLoad(palanca);
	
	if (notas != nullptr)
		app->tex->UnLoad(notas);
	
	if (doorEscape != nullptr)
		app->tex->UnLoad(doorEscape);
	
	if (boss != nullptr)
		app->tex->UnLoad(boss);	
	
	if (loset != nullptr)
		app->tex->UnLoad(loset);

	if(Door1 != nullptr)
		Door1->body->GetWorld()->DestroyBody(Door1->body);

	if (DoorEscape->body != nullptr)
		DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);
	
	if (Door2 != nullptr)
		Door2->body->GetWorld()->DestroyBody(Door2->body);
	
	if (Door3 != nullptr)
		Door3->body->GetWorld()->DestroyBody(Door3->body);

	if (Palanca != nullptr)
		Palanca->body->GetWorld()->DestroyBody(Palanca->body);

	if (PalancaSensor->body != nullptr)
		PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);
	
	if (nota1 != nullptr)
		nota1->body->GetWorld()->DestroyBody(nota1->body);
	
	if (nota2 != nullptr)
		nota2->body->GetWorld()->DestroyBody(nota2->body);
	
	if (nota3 != nullptr)
		nota3->body->GetWorld()->DestroyBody(nota3->body);

	if (Boss != nullptr)
		Boss->body->GetWorld()->DestroyBody(Boss->body);	
	
	if (Loset != nullptr)
		Loset->body->GetWorld()->DestroyBody(Loset->body);

	RELEASE(numCode);

	return true;
}

bool PuzzleManager::Palancas() 
{
	app->render->DrawTexture(palanca, posPalancas.x, posPalancas.y);
	app->render->DrawTexture(door, posDoor1.x, posDoor1.y);
	app->render->DrawTexture(door, posDoor2.x, posDoor2.y);

	if (keyPalancas == 1) 
	{
		if(palanca != nullptr)
			app->tex->UnLoad(palanca);
		
		if (Door1 != nullptr)
			Door1->body->GetWorld()->DestroyBody(Door1->body);
		
		if(Door2 != nullptr)
			Door2->body->GetWorld()->DestroyBody(Door2->body);

		if (Palanca != nullptr)
			Palanca->body->GetWorld()->DestroyBody(Palanca->body);

		if (PalancaSensor != nullptr)
			PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);

		palancas = true;

		return true;
	}

	return false;
}

bool PuzzleManager::Escape() 
{
	app->render->DrawTexture(doorEscape, posDoorEscape.x, posDoorEscape.y);
	app->render->DrawTexture(notas, posNotas1.x, posNotas1.y);
	app->render->DrawTexture(notas, posNotas2.x, posNotas2.y);
	app->render->DrawTexture(notas, posNotas3.x, posNotas3.y);


	if (intoCode == true) 
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			codeActive = !codeActive;
	}
	else
	{
		if (esc1)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				//Abrir UI nota 1
				int id = 100; // ID DEL DIALOGUES.XML DIALOGUE_TREE!!!!!!!!!!!!!!!!!!!!!!!!!1
				app->dialogueSystem->LoadDialogue(id);
				esc1 = false;
			}
		}

		if (esc2)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				//Abrir UI nota 2
				int id = 101; // ID DEL DIALOGUES.XML DIALOGUE_TREE!!!!!!!!!!!!!!!!!!!!!!!!!1
				app->dialogueSystem->LoadDialogue(id);
				esc2 = false;
			}
		}

		if (esc3)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				//Abrir UI nota 3
				int id = 102; // ID DEL DIALOGUES.XML DIALOGUE_TREE!!!!!!!!!!!!!!!!!!!!!!!!!1
				app->dialogueSystem->LoadDialogue(id);
				esc3 = false;
			}
		}
	}

	if (codeActive)
	{
		if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) && codeToCompare.empty())
		{
			codeToCompare.erase(codeToCompare.length() - 1);
		}

		if (!app->puzzleManager->numCode->input_entered)
		{
			app->input->ActiveGetInput(app->puzzleManager->numCode);

			codeToCompare = app->puzzleManager->numCode->input.c_str();
		}

		if (strcmp(codeToCompare.c_str(), realCode.c_str()) == 0)
		{
			if (app->puzzleManager->doorEscape != nullptr)
				app->tex->UnLoad(app->puzzleManager->doorEscape);

			if (app->puzzleManager->DoorEscape != nullptr)
				app->puzzleManager->DoorEscape->body->GetWorld()->DestroyBody(app->puzzleManager->DoorEscape->body);

			if (app->puzzleManager->notas != nullptr)
				app->tex->UnLoad(app->puzzleManager->notas);

			if (app->puzzleManager->nota1 != nullptr)
				app->puzzleManager->nota1->body->GetWorld()->DestroyBody(app->puzzleManager->nota1->body);

			if (app->puzzleManager->nota2 != nullptr)
				app->puzzleManager->nota2->body->GetWorld()->DestroyBody(app->puzzleManager->nota2->body);

			if (app->puzzleManager->nota3 != nullptr)
				app->puzzleManager->nota3->body->GetWorld()->DestroyBody(app->puzzleManager->nota3->body);

			codeActive = false;
			escape = true;
		}
	}

	return false;
}

bool PuzzleManager::Rescue() 
{
	app->render->DrawTexture(boss, posBoss.x, posBoss.y);
	app->render->DrawTexture(door, posDoor3.x, posDoor3.y);
	app->render->DrawTexture(loset, posLoset.x, posLoset.y);

	if (bossActive) 
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) 
		{
			bossInvent = true;

			if (boss != nullptr)
				app->tex->UnLoad(boss);

			if (Boss->body != nullptr)
				Boss->body->GetWorld()->DestroyBody(Boss->body);

			bossActive = false;
		}
	}

	if (bossInvent) 
	{
		if (losetActive) 
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				if (door != nullptr)
					app->tex->UnLoad(door);

				if (Loset->body != nullptr)
					Loset->body->GetWorld()->DestroyBody(Loset->body);

				if (Door3->body != nullptr)
					Door3->body->GetWorld()->DestroyBody(Door3->body);

				if (loset != nullptr)
					app->tex->UnLoad(loset);

				losetActive = false;
				bossInvent = false;
				rescue = true;
			}
		}
	}

	return false;
}