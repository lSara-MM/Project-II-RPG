#include "Scene.h"
#include "PuzzleManager.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Combat.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
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
	texturepathPalancaSens = config.child("PalancaSens").attribute("texturepathSensor").as_string();
	texturepathNotas = config.child("Notas").attribute("texturepathNotas").as_string();
	texturepathDoorEscape = config.child("DoorEscape").attribute("texturepathDoorEscape").as_string();

	posNotas1.x = config.child("Notas").attribute("x1").as_int();
	posNotas1.y = config.child("Notas").attribute("y1").as_int();	
	
	posNotas2.x = config.child("Notas").attribute("x2").as_int();
	posNotas2.y = config.child("Notas").attribute("y2").as_int();	
	
	posNotas3.x = config.child("Notas").attribute("x3").as_int();
	posNotas3.y = config.child("Notas").attribute("y3").as_int();

	posDoor1.x = config.child("Door").attribute("x").as_int();
	posDoor1.y = config.child("Door").attribute("y").as_int();

	posDoor2.x = config.child("Door").attribute("x1").as_int();
	posDoor2.y = config.child("Door").attribute("y2").as_int();

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
	
	return ret;
}

bool PuzzleManager::Start()
{
	palanc = false;
	escape = false;

	esc1 = false;
	esc2 = false;
	esc3 = false;

	codeActive = false;

	keyPalancas = 0;
	keyEscape = 0;
	keyRescue = 0;

	realCode = "1234";

	Door = app->tex->Load(texturepathDoor);
	palanca = app->tex->Load(texturepathPalanca);
	palancaSens = app->tex->Load(texturepathPalancaSens);
	notas = app->tex->Load(texturepathNotas);
	doorEscape = app->tex->Load(texturepathDoorEscape);

	Door1 = app->physics->CreateRectangle(posDoor1.x - widthDoor / 2, posDoor1.y - heightDoor / 2, widthDoor, heightDoor, bodyType::STATIC);
	Door1->body->SetFixedRotation(true);

	Door2 = app->physics->CreateRectangle(posDoor2.x - widthDoor / 2, posDoor2.y - heightDoor / 2, widthDoor, heightDoor, bodyType::STATIC);
	Door2->body->SetFixedRotation(true);

	DoorEscape = app->physics->CreateRectangle(posDoorEscape.x - widthDoorEscape / 2, posDoorEscape.y - heightDoorEscape / 2, widthDoorEscape, heightDoorEscape, bodyType::STATIC);
	DoorEscape->body->SetFixedRotation(true);

	Palanca = app->physics->CreateRectangle(posPalancas.x - widthPalanca / 2, posPalancas.y - heightPalanca / 2, widthPalanca, heightPalanca, bodyType::STATIC);
	Palanca->body->SetFixedRotation(true);

	PalancaSensor = app->physics->CreateRectangle(posPalancas.x - widthPalancaSens / 2, posPalancas.y - heightPalancaSens / 2, widthPalancaSens, heightPalancaSens, bodyType::STATIC);
	PalancaSensor->body->SetFixedRotation(true);
	PalancaSensor->ctype = ColliderType::PALANCA;

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

	return true;
}

bool PuzzleManager::PreUpdate()
{
	return true;
}

bool PuzzleManager::Update(float dt)
{
	if (!palanc)
	{
		if (Palancas())
		{
			LOG("PALANCAS TRUE");
		}
	}

	if (!escape) 
	{
		if(Escape())
		{
			//Introduce c�digo, si acierta destruir puerta

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
				codeActive = !codeActive;

			LOG("ESCAPE TRUE");
		}
	}

	if (codeActive) 
	{
		codeToCompare = CodeInput();

		if (strcmp(codeToCompare.GetString(), realCode.GetString()) == 0) 
		{
			if(doorEscape != nullptr)
				app->tex->UnLoad(doorEscape);

			if(DoorEscape != nullptr)
				DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);

			if (notas != nullptr)
				app->tex->UnLoad(notas);

			if (nota1 != nullptr)
				nota1->body->GetWorld()->DestroyBody(nota1->body);

			if (nota2 != nullptr)
				nota2->body->GetWorld()->DestroyBody(nota2->body);

			if (nota3 != nullptr)
				nota3->body->GetWorld()->DestroyBody(nota3->body);

			codeActive = false;

			LOG("YOU ESCAPE!");
		}
	}

	if (esc1) 
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) 
		{
			//Abrir UI nota 1

		}
	}
	
	if (esc2) 
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) 
		{
			//Abrir UI nota 2

		}
	}
	
	if (esc3) 
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) 
		{
			//Abrir UI nota 3

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

	if(Door != nullptr)
		app->tex->UnLoad(Door);

	if (palanca != nullptr)
		app->tex->UnLoad(palanca);

	if (palancaSens != nullptr)
		app->tex->UnLoad(palancaSens);
	
	if (notas != nullptr)
		app->tex->UnLoad(notas);
	
	if (doorEscape != nullptr)
		app->tex->UnLoad(doorEscape);

	if(Door1->body != nullptr)
		Door1->body->GetWorld()->DestroyBody(Door1->body);

	if (DoorEscape->body != nullptr)
		DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);
	
	if (Door2->body != nullptr)
		Door2->body->GetWorld()->DestroyBody(Door2->body);

	if (Palanca->body != nullptr)
		Palanca->body->GetWorld()->DestroyBody(Palanca->body);

	if (PalancaSensor->body != nullptr)
		PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);
	
	if (nota1->body != nullptr)
		nota1->body->GetWorld()->DestroyBody(nota1->body);
	
	if (nota2->body != nullptr)
		nota2->body->GetWorld()->DestroyBody(nota2->body);
	
	if (nota3->body != nullptr)
		nota3->body->GetWorld()->DestroyBody(nota3->body);

	return true;
}

bool PuzzleManager::Palancas() 
{
	if (keyPalancas >= 1) 
	{
		if (Door != nullptr)
			app->tex->UnLoad(Door);

		if(palanca != nullptr)
			app->tex->UnLoad(palanca);

		if(palancaSens != nullptr)
			app->tex->UnLoad(palancaSens);
		
		if (Door1 != nullptr)
			Door1->body->GetWorld()->DestroyBody(Door1->body);
		
		if(Door2 != nullptr)
			Door2->body->GetWorld()->DestroyBody(Door2->body);

		if (Palanca != nullptr)
			Palanca->body->GetWorld()->DestroyBody(Palanca->body);

		if (PalancaSensor != nullptr)
			PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);

		palanc = true;

		return true;
	}

	return false;
}

bool PuzzleManager::Escape() 
{
	if (keyEscape >= 3) 
	{
		escape = true;

		return true;
	}

	return false;
}

bool PuzzleManager::Rescue() 
{
	
	return false;
}

SString PuzzleManager::CodeInput()
{
	SString temp;

	temp = "Code:  %%";
	temp.Substitute("%", code.GetString());
	app->render->TextDraw(temp.GetString(), app->win->GetWidth() / 4, 650, 40, Font::TEXT, { 255, 255, 255 });

	return code;
}