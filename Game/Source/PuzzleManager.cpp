#include "Scene.h"
#include "PuzzleManager.h"
#include "QuestManager.h"
#include "HouseOfTerrors.h"
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
	texturepathFireGuy = config.child("FireGuy").attribute("texturepathFireGuy").as_string();

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
	
	posFireGuy.x = config.child("FireGuy").attribute("x").as_int();
	posFireGuy.y = config.child("FireGuy").attribute("y").as_int();

	widthVertical = config.child("Door").attribute("widthVertical").as_int();
	heightVertical = config.child("Door").attribute("heightVertical").as_int();
	
	widthHoritzontal = config.child("Door").attribute("widthHoritzontal").as_int();
	heightHoritzontal = config.child("Door").attribute("heightHoritzontal").as_int();

	posPalancas.x = config.child("Palanca").attribute("Xpalancas").as_int();
	posPalancas.y = config.child("Palanca").attribute("Ypalancas").as_int();

	widthPalanca = config.child("Palanca").attribute("widthPalanca").as_int();
	heightPalanca = config.child("Palanca").attribute("heightPalanca").as_int();

	widthPalancaSens = config.child("PalancaSens").attribute("widthPalancaSens").as_int();
	heightPalancaSens = config.child("PalancaSens").attribute("heightPalancaSens").as_int();
		
	widthNotas = config.child("Notas").attribute("widthNotas").as_int();
	heightNotas = config.child("Notas").attribute("heightNotas").as_int();
	
	widthFireGuy = config.child("FireGuy").attribute("width").as_int();
	heightFireGuy = config.child("FireGuy").attribute("height").as_int();

	posDoorEscape.x = config.child("DoorEscape").attribute("x").as_int();
	posDoorEscape.y = config.child("DoorEscape").attribute("y").as_int();

	widthDoorEscape = config.child("DoorEscape").attribute("width").as_int();
	heightDoorEscape = config.child("DoorEscape").attribute("height").as_int();
	
	widthBoss = config.child("Boss").attribute("width").as_int();
	heightBoss = config.child("Boss").attribute("height").as_int();
	
	widthLoset= config.child("Boss").attribute("width").as_int();
	heightLoset = config.child("Boss").attribute("height").as_int();

	//fx
	palancaPath = "Assets/Audio/Fx/palanca.wav";
	palancafx = app->audio->LoadFx(palancaPath);

	confirmPath = "Assets/Audio/Fx/confirm_interaction.wav";
	confirmInteractfx = app->audio->LoadFx(confirmPath);
	
	return ret;
}

bool PuzzleManager::Start()
{
	palancas = false;
	escape = false;
	rescue = false;
	teamMate = false;

	//Notas false
	esc1 = false;
	esc2 = false;
	esc3 = false;

	codeActive = false;
	bossActive = false;
	losetActive = false;
	bossInvent = false;
	intoCode = false;

	palancasActive = false;

	realCode = "344";

	Door1 = nullptr;
	Door2 = nullptr;
	Door3 = nullptr;
	DoorEscape = nullptr;
	Palanca = nullptr;
	PalancaSensor = nullptr;
	nota1 = nullptr;
	nota2 = nullptr;
	nota3 = nullptr;
	DoorEscapeSensor = nullptr;
	Boss = nullptr;
	Loset = nullptr;
	FireGuy = nullptr;

	los = { 131, 66, 64, 64 };

	app->questManager->active = true;
	app->questManager->LoadState();

	if (palancas == false) 
	{
		door = app->tex->Load(texturepathDoor);

		Door1 = app->physics->CreateRectangle(posDoor1.x, posDoor1.y - heightHoritzontal / 2, widthHoritzontal * 2, heightHoritzontal, bodyType::STATIC);
		Door1->body->SetFixedRotation(true);

		Door2 = app->physics->CreateRectangle(posDoor2.x - widthHoritzontal / 2, posDoor2.y - heightHoritzontal / 2, widthHoritzontal, heightHoritzontal, bodyType::STATIC);
		Door2->body->SetFixedRotation(true);

		PalancaSensor = app->physics->CreateRectangleSensor(posPalancas.x - widthPalancaSens / 2, posPalancas.y - heightPalancaSens / 2, widthPalancaSens, heightPalancaSens, bodyType::STATIC);
		PalancaSensor->body->SetFixedRotation(true);
		PalancaSensor->ctype = ColliderType::PALANCA;
	}

	if (rescue == false) 
	{
		door = app->tex->Load(texturepathDoor);
		boss = app->tex->Load(texturepathBoss);
		
		Boss = app->physics->CreateRectangleSensor(posBoss.x - widthBoss / 2, posBoss.y - heightBoss / 2, widthBoss, heightBoss, bodyType::STATIC);
		Boss->body->SetFixedRotation(true);
		Boss->ctype = ColliderType::BOSSDEAD;

		Loset = app->physics->CreateRectangleSensor(posLoset.x - widthLoset / 2, posLoset.y - heightLoset / 2, widthLoset - 32, heightLoset - 32, bodyType::STATIC);
		Loset->body->SetFixedRotation(true);
		Loset->ctype = ColliderType::LOSET;

		Door3 = app->physics->CreateRectangle(posDoor3.x - widthVertical / 2, posDoor3.y - heightVertical / 2, widthVertical, heightVertical, bodyType::STATIC);
		Door3->body->SetFixedRotation(true);
	}
	else
	{
		los = { 196, 66, 64, 64 };
	}

	if (escape == false)
	{
		doorEscape = app->tex->Load(texturepathDoorEscape);

		DoorEscape = app->physics->CreateRectangle(posDoorEscape.x - widthDoorEscape / 2, posDoorEscape.y - heightDoorEscape / 2, widthDoorEscape, heightDoorEscape, bodyType::STATIC);
		DoorEscape->body->SetFixedRotation(true);

		DoorEscapeSensor = app->physics->CreateRectangleSensor(posDoorEscape.x - widthDoorEscape / 2, posDoorEscape.y - heightDoorEscape / 2, widthDoorEscape, heightDoorEscape, bodyType::STATIC);
		DoorEscapeSensor->body->SetFixedRotation(true);
		DoorEscapeSensor->ctype = ColliderType::DOORCODE;
	}

	if (teamMate == false) 
	{
		fireGuy = app->tex->Load(texturepathFireGuy);

		FireGuy = app->physics->CreateRectangleSensor(posFireGuy.x - widthFireGuy / 2, posFireGuy.y - heightFireGuy / 2, widthFireGuy, heightFireGuy * 2, bodyType::STATIC);
		FireGuy->body->SetFixedRotation(true);
		FireGuy->ctype = ColliderType::FIREGUY;
	}

	textureE = app->tex->Load("Assets/GUI/UI_E.png");
	notas = app->tex->Load(texturepathNotas);
	loset = app->tex->Load(texturepathLoset);
	palanca = app->tex->Load(texturepathPalanca);

	Palanca = app->physics->CreateRectangle(posPalancas.x - widthPalanca / 2, posPalancas.y - heightPalanca, widthPalanca, heightPalanca, bodyType::STATIC);
	Palanca->body->SetFixedRotation(true);

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

	numCode = new PlayerInput("", 3, false);

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
		Palancas();
	}

	if (!escape) 
	{
		Escape();
	}

	if (!rescue)
	{
		Rescue();
	}

	if (!teamMate) 
	{
		TeamMate();
	}

	if (esc1)
	{
		app->render->DrawTexture(textureE, posNotas1.x - 32, posNotas1.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			//Abrir UI nota 1
			app->audio->PlayFx(confirmInteractfx);
			app->dialogueSystem->Enable();
			vector<int> id = { 100 }; // ID DEL DIALOGUES.XML DIALOGUE_TREE
			app->dialogueSystem->PerformDialogue(id);
			app->hTerrors->player->lockMovement = true;
			esc1 = false;
		}
	}

	if (esc2)
	{
		app->render->DrawTexture(textureE, posNotas2.x - 32, posNotas2.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			//Abrir UI nota 2
			app->audio->PlayFx(confirmInteractfx);
			app->dialogueSystem->Enable();
			vector<int> id = { 101 }; // ID DEL DIALOGUES.XML DIALOGUE_TREE
			app->dialogueSystem->PerformDialogue(id);
			app->hTerrors->player->lockMovement = true;
			esc2 = false;
		}
	}

	if (esc3)
	{
		app->render->DrawTexture(textureE, posNotas3.x - 32, posNotas3.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			//Abrir UI nota 3
			app->audio->PlayFx(confirmInteractfx);
			app->dialogueSystem->Enable();
			vector<int> id = { 102 }; // ID DEL DIALOGUES.XML DIALOGUE_TREE
			app->dialogueSystem->PerformDialogue(id);
			app->hTerrors->player->lockMovement = true;
			esc3 = false;
		}
	}

	if (app->dialogueSystem->hasEnded)
	{
		app->dialogueSystem->Disable();
		app->hTerrors->player->lockMovement = false;
		app->dialogueSystem->hasEnded = false;
	}

	SDL_Rect palan = { 361, 75, 29, 46 };

	app->render->DrawTexture(palanca, posPalancas.x - widthPalanca / 2, posPalancas.y - heightPalanca, &palan);

	SDL_Rect not = { 268, 76, 51, 45 };

	app->render->DrawTexture(notas, posNotas1.x - widthNotas, posNotas1.y - heightNotas, &not);
	app->render->DrawTexture(notas, posNotas2.x - widthNotas, posNotas2.y - heightNotas, &not);
	app->render->DrawTexture(notas, posNotas3.x - widthNotas, posNotas3.y - heightNotas, &not);

	SDL_Rect bos = { 0, 0, 0, 0 };

	app->render->DrawTexture(boss, posBoss.x - widthBoss, posBoss.y - heightBoss, &bos);

	app->render->DrawTexture(loset, posLoset.x - widthLoset + 16, posLoset.y - heightLoset + 16, &los);

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

	if (palancas) 
	{
		if (Door1 != nullptr)
			Door1->body->GetWorld()->DestroyBody(Door1->body);

		if (Door2 != nullptr)
			Door2->body->GetWorld()->DestroyBody(Door2->body);

		if (PalancaSensor != nullptr)
			PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);
	}

	if (escape) 
	{

		if (doorEscape != nullptr)
			app->tex->UnLoad(doorEscape);

		if (DoorEscape != nullptr)
			DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);
	}

	if (rescue) 
	{
		if (boss != nullptr)
			app->tex->UnLoad(boss);

		if (Boss != nullptr)
			Boss->body->GetWorld()->DestroyBody(Boss->body);

		if (Door3 != nullptr)
			Door3->body->GetWorld()->DestroyBody(Door3->body);

		if (Loset != nullptr)
			Loset->body->GetWorld()->DestroyBody(Loset->body);
	}

	if (teamMate) 
	{
		if (fireGuy != nullptr)
			app->tex->UnLoad(fireGuy);

		if (FireGuy != nullptr)
			FireGuy->body->GetWorld()->DestroyBody(FireGuy->body);

		delete FireGuy;
		FireGuy = nullptr;
	}

	if (palanca != nullptr)
		app->tex->UnLoad(palanca);

	if (notas != nullptr)
		app->tex->UnLoad(notas);

	if (loset != nullptr)
		app->tex->UnLoad(loset);

	if(door != nullptr)
		app->tex->UnLoad(door);
	
	if(textureE != nullptr)
		app->tex->UnLoad(textureE);

	if (Palanca != nullptr)
		Palanca->body->GetWorld()->DestroyBody(Palanca->body);

	if (nota1 != nullptr)
		nota1->body->GetWorld()->DestroyBody(nota1->body);

	if (nota2 != nullptr)
		nota2->body->GetWorld()->DestroyBody(nota2->body);

	if (nota3 != nullptr)
		nota3->body->GetWorld()->DestroyBody(nota3->body);

	delete Door1;
	Door1 = nullptr;

	delete Door2;
	Door2 = nullptr;

	delete Door3;
	Door3 = nullptr;

	delete DoorEscape;
	DoorEscape = nullptr;

	delete Palanca;
	Palanca = nullptr;

	delete PalancaSensor;
	PalancaSensor = nullptr;

	delete nota1;
	nota1 = nullptr;

	delete nota2;
	nota2 = nullptr;

	delete nota3;
	nota3 = nullptr;

	delete DoorEscapeSensor;
	DoorEscapeSensor = nullptr;

	delete Boss;
	Boss = nullptr;

	delete Loset;
	Loset = nullptr;

	RELEASE(numCode);

	return true;
}

bool PuzzleManager::Palancas() 
{
	SDL_Rect dor = { 1, 1, 64, 14 };

	app->render->DrawTexture(door, posDoor1.x - widthHoritzontal, posDoor1.y - heightHoritzontal / 2, &dor);
	app->render->DrawTexture(door, posDoor1.x, posDoor1.y - heightHoritzontal / 2, &dor);
	app->render->DrawTexture(door, posDoor2.x - widthHoritzontal, posDoor2.y - heightHoritzontal / 2, &dor);

	if (palancasActive)
	{
		app->render->DrawTexture(textureE, posPalancas.x - 30, posPalancas.y - 60);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			if (Door1 != nullptr)
				Door1->body->GetWorld()->DestroyBody(Door1->body);

			if (Door2 != nullptr)
				Door2->body->GetWorld()->DestroyBody(Door2->body);

			if (PalancaSensor != nullptr)
				PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);

			delete Door1;
			Door1 = nullptr;

			delete Door2;
			Door2 = nullptr;

			delete PalancaSensor;
			PalancaSensor = nullptr;

			palancas = true;
			app->audio->PlayFx(palancafx);
			app->questManager->SaveState();
		}
	}

	return true;
}

bool PuzzleManager::Escape() 
{
	SDL_Rect dorEsc = { 130, 1, 14, 64 };

	app->render->DrawTexture(doorEscape, posDoorEscape.x - widthDoorEscape, posDoorEscape.y - heightDoorEscape, &dorEsc);

	if (intoCode == true) 
	{
		app->render->DrawTexture(textureE, posDoorEscape.x + 20, posDoorEscape.y + 10);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			app->hTerrors->player->lockMovement = true;
			app->input->ActiveGetInput(numCode);
		}
	}

	if (app->input->getInput_B)
	{
		iPoint pos = { app->win->GetWidth() / 4, 650 };
		app->input->RenderTempText("Code:  %%", app->input->temp.c_str(), pos, 40, Font::TEXT, { 255, 255, 255 });
	}

	if (numCode->input_entered)
	{
		if (strcmp(numCode->input.c_str(), realCode.c_str()) == 0)
		{
			if (doorEscape != nullptr)
				app->tex->UnLoad(doorEscape);

			if (DoorEscape != nullptr)
				DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);

			delete DoorEscape;
			DoorEscape = nullptr;

			codeActive = false;
			escape = true;
			app->questManager->SaveState();
		}

		app->hTerrors->player->lockMovement = false;
	}

	return true;
}

bool PuzzleManager::Rescue() 
{
	SDL_Rect dor = { 377, 1, 14, 64 };

	app->render->DrawTexture(door, posDoor3.x - widthVertical, posDoor3.y - heightVertical, &dor);

	if (bossActive) 
	{
		app->render->DrawTexture(textureE, posBoss.x - 64, posBoss.y - 145);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) 
		{
			bossInvent = true;

			if (boss != nullptr)
				app->tex->UnLoad(boss);

			if (Boss->body != nullptr)
				Boss->body->GetWorld()->DestroyBody(Boss->body);

			delete Boss;
			Boss = nullptr;

			bossActive = false;
		}
	}

	if (bossInvent) 
	{
		if (losetActive) 
		{
			app->render->DrawTexture(textureE, posLoset.x - 64, posLoset.y - 120);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				if (door != nullptr)
					app->tex->UnLoad(door);

				if (Door3->body != nullptr)
					Door3->body->GetWorld()->DestroyBody(Door3->body);
				
				if (Loset->body != nullptr)
					Loset->body->GetWorld()->DestroyBody(Loset->body);

				delete Loset;
				Loset = nullptr;
				
				delete Door3;
				Door3 = nullptr;

				losetActive = false;
				bossInvent = false;
				rescue = true;
				app->questManager->SaveState();
			}
		}
	}

	return true;
}

bool PuzzleManager::TeamMate() 
{
	app->render->DrawTexture(fireGuy, posFireGuy.x - widthFireGuy * 2, posFireGuy.y - heightFireGuy * 2);

	if (saveFireGuy) 
	{
		app->render->DrawTexture(textureE, posFireGuy.x - 32, posFireGuy.y + 30);

		if(app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			if (fireGuy != nullptr)
				app->tex->UnLoad(fireGuy);

			if (FireGuy != nullptr)
				FireGuy->body->GetWorld()->DestroyBody(FireGuy->body);

			delete FireGuy;
			FireGuy = nullptr;

			teamMate = true;

			if (app->questManager->quest1->active) 
			{
				app->questManager->quest1->complete = true;
			}

			app->questManager->SaveState();

			app->entityManager->tpID = 0;
			app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->scene, 90);
		}
	}

	return true;
}