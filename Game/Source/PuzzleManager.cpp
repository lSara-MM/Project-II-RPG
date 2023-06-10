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
#include "Scene.h"
#include "HouseOfTerrors.h"
#include "BeastTent.h"

#include "FadeToBlack.h"
#include "GuiManager.h"
#include "DialogueSystem.h"
#include "ItemManager.h"
#include "QuestManager.h"
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

	if (app->hTerrors->active) 
	{
		Dun1Awake(config);
	}

	if (app->BeastT->active)
	{
		Dun2Awake(config);
	}
	
	return ret;
}

bool PuzzleManager::Start()
{
	if (app->hTerrors->active) 
	{
		Dun1Start();
	}

	if (app->BeastT->active)
	{
		Dun2Start();
	}
	
	return true;
}

bool PuzzleManager::PreUpdate()
{
	return true;
}

bool PuzzleManager::Update(float dt)
{
	if(app->hTerrors->active)
	{
		Dun1Update();
	}

	if(app->BeastT->active)
	{
		Dun2Update();
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

	if(app->hTerrors->active)
	{
		Dun1CleanUp();
	}
	
	if(app->BeastT->active)
	{
		Dun2CleanUp();
	}

	return true;
}

bool PuzzleManager::Dun1Awake(pugi::xml_node& config)
{
	texturepathDoor = config.child("Door").attribute("texturepathDoor").as_string();
	texturepathPalanca = config.child("Palanca").attribute("texturepathPalanca").as_string();
	texturepathNotas = config.child("Notas").attribute("texturepathNotas").as_string();
	texturepathDoorEscape = config.child("DoorEscape").attribute("texturepathDoorEscape").as_string();
	texturepathBossDeath = config.child("Boss").attribute("texturepathBossDeath").as_string();
	texturepathBoss = config.child("Boss").attribute("texturepathBoss").as_string();
	texturepathLoset = config.child("Loset").attribute("texturepathLoset").as_string();
	texturepathFireGuy = config.child("FireGuy").attribute("texturepathFireGuy").as_string();
	texturepathWintext = config.child("WinDungeon").attribute("texturepathWin").as_string();

	posNotas1.x = config.child("Notas").attribute("x1").as_int();
	posNotas1.y = config.child("Notas").attribute("y1").as_int();

	posNotas2.x = config.child("Notas").attribute("x2").as_int();
	posNotas2.y = config.child("Notas").attribute("y2").as_int();

	posNotas3.x = config.child("Notas").attribute("x3").as_int();
	posNotas3.y = config.child("Notas").attribute("y3").as_int();

	posDoor1.x = config.child("Door").attribute("x").as_int();
	posDoor1.y = config.child("Door").attribute("y").as_int();

	/*posDoor2.x = config.child("Door").attribute("x1").as_int();
	posDoor2.y = config.child("Door").attribute("y1").as_int();*/

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

	widthLoset = config.child("Boss").attribute("width").as_int();
	heightLoset = config.child("Boss").attribute("height").as_int();

	//fx
	palancaPath = "Assets/Audio/Fx/palanca.wav";
	palancafx = app->audio->LoadFx(palancaPath);

	confirmPath = "Assets/Audio/Fx/confirm_interaction.wav";
	confirmInteractfx = app->audio->LoadFx(confirmPath);

	solvedpath = "Assets/Audio/Fx/puzzle_solved.wav";
	solvedfx = app->audio->LoadFx(solvedpath);

	return true;
}

bool PuzzleManager::Dun2Awake(pugi::xml_node& config) 
{
	texturepathPuzzleDungeon2 = config.child("GeneralTexture").attribute("texturepath").as_string();
	texturepathBossDun2 = config.child("BossDun2").attribute("texturepathBossDun2").as_string();

	widthDoorKeys = config.child("DoorDun2").attribute("width").as_int();
	widthKey = config.child("Key").attribute("width").as_int();
	widthChicken = config.child("Chicken").attribute("width").as_int();
	widthBomb = config.child("Bomb").attribute("width").as_int();
	widthBarricade1 = config.child("Barricade").attribute("width").as_int();
	widthBarricade2 = config.child("Barricade").attribute("width1").as_int();
	widthRelic = config.child("Relics").attribute("width").as_int();
	widthRelicColumn = config.child("RelicsColumns").attribute("width").as_int();	
	widthBoss2 = config.child("BossDun2").attribute("width").as_int();
	
	heightDoorKeys = config.child("DoorDun2").attribute("height").as_int();
	heightKey = config.child("Key").attribute("height").as_int();
	heightChicken = config.child("Chicken").attribute("height").as_int();
	heightBomb = config.child("Bomb").attribute("height").as_int();
	heightBarricade1 = config.child("Barricade").attribute("height").as_int();
	heightBarricade2 = config.child("Barricade").attribute("height1").as_int();
	heightRelic = config.child("Relics").attribute("height").as_int();
	heightRelicColumn = config.child("RelicsColumns").attribute("height").as_int();
	heightBoss2 = config.child("BossDun2").attribute("height").as_int();

	 posDoorkey1.x = config.child("DoorDun2").attribute("x").as_int();
	 posDoorkey1.y = config.child("DoorDun2").attribute("y").as_int();
	 posDoorkey2.x = config.child("DoorDun2").attribute("x1").as_int();
	 posDoorkey2.y = config.child("DoorDun2").attribute("y1").as_int();
	 posDoorBoss.x = config.child("DoorDun2").attribute("xBoss").as_int();
	 posDoorBoss.y = config.child("DoorDun2").attribute("yBoss").as_int();
	 posKey.x = config.child("Key").attribute("x").as_int();
	 posKey.y = config.child("Key").attribute("y").as_int();
	 posChicken1.x = config.child("Chicken").attribute("x").as_int();
	 posChicken1.y = config.child("Chicken").attribute("y").as_int();
	 posChicken2.x = config.child("Chicken").attribute("x1").as_int();
	 posChicken2.y = config.child("Chicken").attribute("y1").as_int();
	 posBomb1.x = config.child("Bomb").attribute("x").as_int();
	 posBomb1.y = config.child("Bomb").attribute("y").as_int();
	 posBomb2.x = config.child("Bomb").attribute("x1").as_int();
	 posBomb2.y = config.child("Bomb").attribute("y1").as_int();
	 posBarricade1.x = config.child("Barricade").attribute("x").as_int();
	 posBarricade1.y = config.child("Barricade").attribute("y").as_int();
	 posBarricade2.x = config.child("Barricade").attribute("x1").as_int();
	 posBarricade2.y = config.child("Barricade").attribute("y1").as_int();
	 posBarricade3.x = config.child("Barricade").attribute("x2").as_int();
	 posBarricade3.y = config.child("Barricade").attribute("y2").as_int();
	 posBarricade4.x = config.child("Barricade").attribute("x3").as_int();
	 posBarricade4.y = config.child("Barricade").attribute("y3").as_int();
	 posBarricade5.x = config.child("Barricade").attribute("x4").as_int();
	 posBarricade5.y = config.child("Barricade").attribute("y4").as_int();
	 posRelic1.x = config.child("Relics").attribute("x").as_int();
	 posRelic1.y = config.child("Relics").attribute("y").as_int();
	 posRelic2.x = config.child("Relics").attribute("x1").as_int();
	 posRelic2.y = config.child("Relics").attribute("y1").as_int();
	 posRelic3.x = config.child("Relics").attribute("x2").as_int();
	 posRelic3.y = config.child("Relics").attribute("y2").as_int();
	 posRelicColumn1.x = config.child("RelicsColumns").attribute("x").as_int();
	 posRelicColumn1.y = config.child("RelicsColumns").attribute("y").as_int();
	 posRelicColumn2.x = config.child("RelicsColumns").attribute("x1").as_int();
	 posRelicColumn2.y = config.child("RelicsColumns").attribute("y1").as_int();
	 posRelicColumn3.x = config.child("RelicsColumns").attribute("x2").as_int();
	 posRelicColumn3.y = config.child("RelicsColumns").attribute("y2").as_int();
	 posBoss2.x = config.child("BossDun2").attribute("x").as_int();
	 posBoss2.y = config.child("BossDun2").attribute("y").as_int();

	 confirmPath = "Assets/Audio/Fx/confirm_interaction.wav";
	 confirmInteractfx = app->audio->LoadFx(confirmPath);

	 solvedpath = "Assets/Audio/Fx/puzzle_solved.wav";
	 solvedfx = app->audio->LoadFx(solvedpath);

	return true;
}

bool PuzzleManager::Dun1Start() 
{
	palancas = false;
	escape = false;
	rescue = false;
	teamMate = false;

	fightBoss1 = false;

	//Notas false
	esc1 = false;
	esc2 = false;
	esc3 = false;

	codeActive = false;
	bossSpectreActive_B = false;
	bossIsDead = false;
	losetActive = false;
	bossInvent = false;
	intoCode = false;
	saveFireGuy = false;

	palancasActive = false;

	CurrentDarkFade = false;

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

		Door1 = app->physics->CreateRectangle(posDoor1.x, posDoor1.y - heightHoritzontal / 2, widthHoritzontal, heightHoritzontal, bodyType::STATIC);
		Door1->body->SetFixedRotation(true);

		PalancaSensor = app->physics->CreateRectangleSensor(posPalancas.x - widthPalancaSens / 2, posPalancas.y - heightPalancaSens / 2, widthPalancaSens, heightPalancaSens, bodyType::STATIC);
		PalancaSensor->body->SetFixedRotation(true);
		PalancaSensor->ctype = ColliderType::PALANCA;
	}

	if (rescue == false)
	{
		if (!bossIsDead)
		{
			boss = app->tex->Load(texturepathBoss);

			door = app->tex->Load(texturepathDoor);

			Door3 = app->physics->CreateRectangle(posDoor3.x - widthVertical / 2, posDoor3.y - heightVertical / 2, widthVertical, heightVertical, bodyType::STATIC);
			Door3->body->SetFixedRotation(true);
		}

		if (!bossInvent)
		{
			Boss = app->physics->CreateRectangle(posBoss.x - widthBoss / 2, posBoss.y - heightBoss / 2, widthBoss, heightBoss, bodyType::STATIC);
			Boss->body->SetFixedRotation(true);
			Boss->ctype = ColliderType::BOSSDEAD;
			Boss->id = 0;
		}

		Loset = app->physics->CreateRectangleSensor(posLoset.x - widthLoset / 2, posLoset.y - heightLoset / 2, widthLoset - 32, heightLoset - 32, bodyType::STATIC);
		Loset->body->SetFixedRotation(true);
		Loset->ctype = ColliderType::LOSET;

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
	bossDeath = app->tex->Load(texturepathBossDeath);

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


	bosDeath = { 3, 1, 47, 53 };

	//animation win
	winAnimation.Set();
	winAnimation.AddTween(100, 120, BOUNCE_OUT);
	winText = app->tex->Load(texturepathWintext);
	posYanimation_I = 0;
	completeDungeon_B = false;

	return true;
}

bool PuzzleManager::Dun2Start()
{
	GeneralTextureDungeon2 = app->tex->Load(texturepathPuzzleDungeon2);
	Boss2Texture = app->tex->Load(texturepathBossDun2);
	textureE = app->tex->Load("Assets/GUI/UI_E.png");

	keyDoors = false;
	chickenBoom = false;
	relics = false;

	fightBoss2 = false;

	DoorContact1 = false;
	DoorContact2 = false;
	DoorKey1Opened = false;
	DoorKey2Opened = false;
	keyInvent = false;
	keySens = false;
	BarricadeContact1 = false;
	BarricadeContact2 = false;
	BarricadeContact3 = false;
	BarricadeContact4 = false;
	BarricadeContact5 = false;
	BarricadeExplote1 = false;
	BarricadeExplote2 = false;
	BarricadeExplote3 = false;
	BarricadeExplote4 = false;
	BarricadeExplote5 = false;
	BarricadeHasExploted1 = false;
	BarricadeHasExploted2 = false;
	BarricadeHasExploted3 = false;
	BarricadeHasExploted4 = false;
	BarricadeHasExploted5 = false;
	BombContact1 = false;
	BombContact2 = false;
	BombCarryOn1 = false;
	BombCarryOn2 = false;
	BombPlant1 = false;
	BombPlant2 = false;

	RelicContact1 = false;
	RelicContact2 = false;
	RelicContact3 = false;
	Relic1Invent = false;
	Relic2Invent = false;
	Relic3Invent = false;
	RelicColumnContact1 = false;
	RelicColumnContact2 = false;
	RelicColumnContact3 = false;
	RelicInColumn1 = false;
	RelicInColumn2 = false;
	RelicInColumn3 = false;

	bossIsDeadDun2 = false;
	bossDun2Contact = false;

	DoorsOpened = 0;
	RelicsCompleted = 0;
	BarricadesExplote = 0;

	RestartTimer();

	app->questManager->LoadState();

	KeyD1 = { 64, 128, 64, 64};
	KeyD2 = { 384, 0, 64, 256 };
	BossD = { 320, 0, 64, 256 };
	Ky = { 256, 128, 64, 64 };
	Chick = { 64, 64, 64, 64 };
	Bmb = { 0, 64, 64, 64 };
	Barr1 = { 0, 0, 64, 64 };
	Barr2 = { 128, 64, 128, 128 };
	Rel1 = { 256, 64, 64, 64 };
	Rel3 = { 0, 128, 64, 64 };
	Rel2 = { 256, 0, 64, 64 };

	if(!keyDoors)
	{
		if(!DoorKey1Opened)
		{
			DoorKey1 = app->physics->CreateRectangle(posDoorkey1.x - widthDoorKeys / 2, posDoorkey1.y - heightDoorKeys / 2, widthDoorKeys, heightDoorKeys, bodyType::STATIC);
			DoorKey1->body->SetFixedRotation(true);
			DoorKey1->ctype = ColliderType::KEYDOOR;
			DoorKey1->id = 1;
		}

		if (!DoorKey2Opened)
		{
			DoorKey2 = app->physics->CreateRectangle(posDoorkey2.x - heightDoorKeys / 2 + 32, posDoorkey2.y - widthDoorKeys / 2 + 32, heightDoorKeys - 48, widthDoorKeys * 2, bodyType::STATIC);
			DoorKey2->body->SetFixedRotation(true);
			DoorKey2->ctype = ColliderType::KEYDOOR;
			DoorKey2->id = 2;

			keySensor = app->physics->CreateRectangleSensor(posKey.x - widthKey / 2, posKey.y - heightKey / 2, widthKey, heightKey, bodyType::STATIC);
			keySensor->body->SetFixedRotation(true);
			keySensor->ctype = ColliderType::KEY;
		}
	}

	if (!chickenBoom) 
	{
		Bomb1 = app->physics->CreateRectangleSensor(posBomb1.x - widthBomb / 2, posBomb1.y - heightBomb / 2, widthBomb, heightBomb, bodyType::STATIC);
		Bomb1->body->SetFixedRotation(true);
		Bomb1->ctype = ColliderType::BOMB;
		Bomb1->id = 1;

		Bomb2 = app->physics->CreateRectangleSensor(posBomb2.x - widthBomb / 2, posBomb2.y - heightBomb / 2, widthBomb, heightBomb, bodyType::STATIC);
		Bomb2->body->SetFixedRotation(true);
		Bomb2->ctype = ColliderType::BOMB;
		Bomb2->id = 2;

		//Barricada corta

		if (!BarricadeHasExploted1)
		{
			Barricade1 = app->physics->CreateRectangle(posBarricade1.x - widthBarricade1 / 2, posBarricade1.y - heightBarricade1 / 2, widthBarricade1, heightBarricade1, bodyType::STATIC);
			Barricade1->body->SetFixedRotation(true);
			Barricade1->ctype = ColliderType::BARRICADE;
			Barricade1->id = 1;
		}

		if (!BarricadeHasExploted2)
		{
			Barricade2 = app->physics->CreateRectangle(posBarricade2.x - widthBarricade1 / 2, posBarricade2.y - heightBarricade1 / 2, widthBarricade1, heightBarricade1, bodyType::STATIC);
			Barricade2->body->SetFixedRotation(true);
			Barricade2->ctype = ColliderType::BARRICADE;
			Barricade2->id = 2;
		}

		if (!BarricadeHasExploted3)
		{
			Barricade3 = app->physics->CreateRectangle(posBarricade3.x - widthBarricade1 / 2, posBarricade3.y - heightBarricade1 / 2, widthBarricade1, heightBarricade1, bodyType::STATIC);
			Barricade3->body->SetFixedRotation(true);
			Barricade3->ctype = ColliderType::BARRICADE;
			Barricade3->id = 3;
		}

		//Barricada larga
		if (!BarricadeHasExploted4)
		{
			Barricade4 = app->physics->CreateRectangle(posBarricade4.x, posBarricade4.y, widthBarricade2, heightBarricade2, bodyType::STATIC);
			Barricade4->body->SetFixedRotation(true);
			Barricade4->ctype = ColliderType::BARRICADE;
			Barricade4->id = 4;
		}

		if (!BarricadeHasExploted5)
		{
			Barricade5 = app->physics->CreateRectangle(posBarricade5.x - widthBarricade1 / 2, posBarricade5.y - heightBarricade1 / 2, widthBarricade1, heightBarricade1, bodyType::STATIC);
			Barricade5->body->SetFixedRotation(true);
			Barricade5->ctype = ColliderType::BARRICADE;
			Barricade5->id = 5;
		}

		if (BarricadeHasExploted1)
		{
			BarricadesExplote += 1;
		}

		if (BarricadeHasExploted2)
		{
			BarricadesExplote += 1;
		}

		if (BarricadeHasExploted3)
		{
			BarricadesExplote += 1;
		}

		if (BarricadeHasExploted4)
		{
			BarricadesExplote += 1;
		}

		if (BarricadeHasExploted5)
		{
			BarricadesExplote += 1;
		}
	}

	if (!relics) 
	{
		DoorBossP = app->physics->CreateRectangle(posDoorBoss.x - heightDoorKeys / 2 - 32, posDoorBoss.y - widthDoorKeys / 2 + 32, heightDoorKeys - 48, widthDoorKeys * 2, bodyType::STATIC);
		DoorBossP->body->SetFixedRotation(true);

		if (!Relic1Invent && !RelicInColumn1)
		{
			relic1 = app->physics->CreateRectangleSensor(posRelic1.x - widthRelic / 2, posRelic1.y - heightRelic / 2, widthRelic, heightRelic, bodyType::STATIC);
			relic1->body->SetFixedRotation(true);
			relic1->ctype = ColliderType::RELIC;
			relic1->id = 1; 
		}

		if (!Relic2Invent && !RelicInColumn2)
		{
			relic2 = app->physics->CreateRectangleSensor(posRelic2.x - widthRelic / 2, posRelic2.y - heightRelic / 2, widthRelic, heightRelic, bodyType::STATIC);
			relic2->body->SetFixedRotation(true);
			relic2->ctype = ColliderType::RELIC;
			relic2->id = 2;
		}

		if (!Relic3Invent && !RelicInColumn3)
		{
			relic3 = app->physics->CreateRectangleSensor(posRelic3.x - widthRelic / 2, posRelic3.y - heightRelic / 2, widthRelic, heightRelic, bodyType::STATIC);
			relic3->body->SetFixedRotation(true);
			relic3->ctype = ColliderType::RELIC;
			relic3->id = 3;
		}
	}

	if(!bossIsDeadDun2)
	{
		boss2 = app->physics->CreateRectangle(posBoss2.x - widthBoss2 / 2, posBoss2.y - heightBoss2 / 2, widthBoss2, heightBoss2, bodyType::STATIC);
		boss2->body->SetFixedRotation(true);
		boss2->ctype = ColliderType::BOSSDEAD;
		boss2->id = 1;
	}

	//Colliders que siempre deben estar aunque se les complete el puzzle
	Chicken1 = app->physics->CreateRectangle(posChicken1.x - widthChicken / 2, posChicken1.y - heightChicken / 2, widthChicken, heightChicken, bodyType::STATIC);
	Chicken1->body->SetFixedRotation(true);

	Chicken2 = app->physics->CreateRectangle(posChicken2.x - widthChicken / 2, posChicken2.y - heightChicken / 2, widthChicken, heightChicken, bodyType::STATIC);
	Chicken2->body->SetFixedRotation(true);

	relicColumn1 = app->physics->CreateRectangle(posRelicColumn1.x - widthRelicColumn / 2, posRelicColumn1.y - heightRelicColumn / 2, widthRelicColumn, heightRelicColumn, bodyType::STATIC);
	relicColumn1->body->SetFixedRotation(true);
	relicColumn1->ctype = ColliderType::RELICOLUMN;
	relicColumn1->id = 1;

	relicColumn2 = app->physics->CreateRectangle(posRelicColumn2.x - widthRelicColumn / 2, posRelicColumn2.y - heightRelicColumn / 2, widthRelicColumn, heightRelicColumn, bodyType::STATIC);
	relicColumn2->body->SetFixedRotation(true);
	relicColumn2->ctype = ColliderType::RELICOLUMN;
	relicColumn2->id = 2;

	relicColumn3 = app->physics->CreateRectangle(posRelicColumn3.x - widthRelicColumn / 2, posRelicColumn3.y - heightRelicColumn / 2, widthRelicColumn, heightRelicColumn, bodyType::STATIC);
	relicColumn3->body->SetFixedRotation(true);
	relicColumn3->ctype = ColliderType::RELICOLUMN;
	relicColumn3->id = 3;

	return true;
}

bool PuzzleManager::Dun1Update()
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
		app->render->DrawTexture(loset, posLoset.x - widthLoset + 16, posLoset.y - heightLoset + 16, &los);
	}
	else
	{
		app->render->DrawTexture(loset, posLoset.x - widthLoset + 16, posLoset.y - heightLoset + 16, &los);
		app->render->DrawTexture(bossDeath, posLoset.x - widthBoss + 25, posLoset.y - heightBoss + 22, &bosDeath);
	}

	if (!teamMate)
	{
		TeamMate();
	}

	if (esc1)
	{
		app->render->DrawTexture(textureE, posNotas1.x - 32, posNotas1.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			//Abrir UI nota 1
			app->audio->PlayFx(confirmInteractfx);
			app->dialogueSystem->Enable();
			vector<int> id = { 100 }; // ID DEL DIALOGUES.XML DIALOGUE_TREE
			app->dialogueSystem->PerformDialogue(id);
			app->hTerrors->player->lockMovement = true;
			app->hTerrors->player->currentAnimation = &app->hTerrors->player->currentAnim;
			esc1 = false;
			id.clear();
		}
	}

	if (esc2)
	{
		app->render->DrawTexture(textureE, posNotas2.x - 32, posNotas2.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			//Abrir UI nota 2
			app->audio->PlayFx(confirmInteractfx);
			app->dialogueSystem->Enable();
			vector<int> id = { 101 }; // ID DEL DIALOGUES.XML DIALOGUE_TREE
			app->dialogueSystem->PerformDialogue(id);
			app->hTerrors->player->lockMovement = true;
			app->hTerrors->player->currentAnimation = &app->hTerrors->player->currentAnim;
			esc2 = false;
			id.clear();
		}
	}

	if (esc3)
	{
		app->render->DrawTexture(textureE, posNotas3.x - 32, posNotas3.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			//Abrir UI nota 3
			app->audio->PlayFx(confirmInteractfx);
			app->dialogueSystem->Enable();
			vector<int> id = { 102 }; // ID DEL DIALOGUES.XML DIALOGUE_TREE
			app->dialogueSystem->PerformDialogue(id);
			app->hTerrors->player->lockMovement = true;
			app->hTerrors->player->currentAnimation = &app->hTerrors->player->currentAnim;
			esc3 = false;
			id.clear();
		}
	}

	if (app->dialogueSystem->hasEnded)
	{
		app->dialogueSystem->Disable();
		app->hTerrors->player->lockMovement = false;
		app->dialogueSystem->hasEnded = false;
	}

	if (app->input->godMode_B)
	{
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			bossIsDead = !bossIsDead;
		}

		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			saveFireGuy = true;
		}
	}

	SDL_Rect palan = { 361, 75, 29, 46 };

	app->render->DrawTexture(palanca, posPalancas.x - widthPalanca / 2, posPalancas.y - heightPalanca * 2, &palan);

	SDL_Rect not = { 268, 76, 51, 45 };

	app->render->DrawTexture(notas, posNotas1.x - widthNotas, posNotas1.y - heightNotas, &not);
	app->render->DrawTexture(notas, posNotas2.x - widthNotas, posNotas2.y - heightNotas, &not);
	app->render->DrawTexture(notas, posNotas3.x - widthNotas, posNotas3.y - heightNotas, &not);

	SDL_Rect bos = { 0, 0, 0, 0 };

	app->render->DrawTexture(boss, posBoss.x - widthBoss, posBoss.y - heightBoss, &bos);

	if (!bossIsDead)
	{
		currentDark = app->hTerrors->DarkestDungeon;
		//app->render->DrawTexture(app->hTerrors->DarkestDungeon, app->hTerrors->player->position.x - app->win->GetWidth() / 2, app->hTerrors->player->position.y - app->win->GetHeight() / 2);
	}
	else
	{
		if (!CurrentDarkFade)
		{
			if (currentDark != nullptr)
				app->fade->FadingToBlackImages(currentDark, nullptr, 90);

			CurrentDarkFade = true;
			app->questManager->SaveState();
		}
		else
		{
			currentDark = nullptr;
		}
	}

	if (currentDark != nullptr)
		app->render->DrawTexture(currentDark, -app->render->camera.x - app->render->camera.w / 2 + app->win->GetWidth() / 2, -app->render->camera.y + app->render->camera.h / 2 - app->win->GetHeight() / 2);


	if (app->input->getInput_B)
	{
		iPoint pos = { app->win->GetWidth() / 4, 650 };
		app->input->RenderTempText("Code:  %%", app->input->temp.c_str(), pos, 40, Font::TEXT, { 255, 255, 255 });
	}

	if (completeDungeon_B)
	{
		winAnimation.Step(1, false);
		float point = winAnimation.GetPoint();
		int offset = -750;
		posYanimation_I = offset + point * (0 - offset);
		app->render->DrawTexture(winText, 0 - app->render->camera.x, offset + point * (0 - offset) - app->render->camera.y);
		if (posYanimation_I == 0)//cuando animacion termina, hace el fade2black
		{
			teamMate = true;
			app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->scene, 45);

		}
	}

	return true;
}

bool PuzzleManager::Dun2Update() 
{
	if(!keyDoors)
	{
		KeyDoorsPuz();
	}

	if(!chickenBoom)
	{
		ChickenBoomPuz();
	}

	if (!relics)
	{
		RelicsPuz();
	}

	app->render->DrawTexture(GeneralTextureDungeon2, posChicken1.x - widthChicken, posChicken1.y - heightChicken, &Chick);
	app->render->DrawTexture(GeneralTextureDungeon2, posChicken2.x - widthChicken, posChicken2.y - heightChicken, &Chick);

	if (RelicInColumn1)
	{
		app->render->DrawTexture(GeneralTextureDungeon2, posRelicColumn1.x - widthRelicColumn + 3, posRelicColumn1.y - heightRelicColumn - 20, &Rel1);
	}

	if (RelicInColumn2)
	{
		app->render->DrawTexture(GeneralTextureDungeon2, posRelicColumn2.x - widthRelicColumn, posRelicColumn2.y - heightRelicColumn - 12, &Rel2);
	}

	if (RelicInColumn3)
	{
		app->render->DrawTexture(GeneralTextureDungeon2, posRelicColumn3.x - widthRelicColumn + 3, posRelicColumn3.y - heightRelicColumn - 12, &Rel3);
	}

	if (!bossIsDeadDun2) 
	{
		if(bossDun2Contact)
		{
			app->render->DrawTexture(textureE, posBoss2.x - widthBoss2 - 20, posBoss2.y - heightBoss2);

			if(app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				//Eric: Aquí la transición a combate y el cleanUp de este puzzle
				//Cuando mates al boss no te olvides de poner esta variable a true y hace un save: bossIsDeadDun2

				LOG("Combat");
				app->SaveGameRequest();
				app->audio->PlayFx(app->hTerrors->combatfx);
				app->combat->PreLoadCombat(app->BeastT->name, 21, 20); //Boss doble
				fightBoss2 = true;
				app->fade->FadingToBlack((Module*)app->BeastT, (Module*)app->combat, 45);
				app->questManager->SaveState();
				app->puzzleManager->Disable();
				app->BeastT->steps_I = 0;
			}
		}

		app->render->DrawTexture(Boss2Texture, posBoss2.x - widthBoss2, posBoss2.y - heightBoss2);
	}

	if (BombPlant1 || BombPlant2 || BombCarryOn1 || BombCarryOn2)
	{
		mTicks = SDL_GetTicks() - mStartTicks;
		DeltaTime = mTicks * 0.001f;
	}

	return true;
}

bool PuzzleManager::Dun1CleanUp() 
{
	if (!palancas)
	{
		if (Door1 != nullptr)
			Door1->body->GetWorld()->DestroyBody(Door1->body);

		if (PalancaSensor != nullptr)
			PalancaSensor->body->GetWorld()->DestroyBody(PalancaSensor->body);
	}

	if (!escape)
	{
		if (doorEscape != nullptr)
			app->tex->UnLoad(doorEscape);

		if (DoorEscape != nullptr)
			DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);	
		
		if (DoorEscapeSensor != nullptr)
			DoorEscapeSensor->body->GetWorld()->DestroyBody(DoorEscapeSensor->body);
	}

	if (!rescue)
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

	if (!teamMate)
	{
		if (fireGuy != nullptr)
			app->tex->UnLoad(fireGuy);

		if (FireGuy != nullptr)
			FireGuy->body->GetWorld()->DestroyBody(FireGuy->body);

		delete FireGuy;
		FireGuy = nullptr;
	}
	app->tex->UnLoad(fireGuy);//no entiendo nada, lo pongo aqui
	app->tex->UnLoad(winText);

	if (palanca != nullptr)
		app->tex->UnLoad(palanca);

	if (notas != nullptr)
		app->tex->UnLoad(notas);

	if (loset != nullptr)
		app->tex->UnLoad(loset);

	if (door != nullptr)
		app->tex->UnLoad(door);

	if (textureE != nullptr)
		app->tex->UnLoad(textureE);

	if (bossDeath != nullptr)
		app->tex->UnLoad(bossDeath);

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

bool PuzzleManager::Dun2CleanUp()
{
	if (!keyDoors) 
	{
		if (DoorKey1 != nullptr)
			DoorKey1->body->GetWorld()->DestroyBody(DoorKey1->body);
		
		if (DoorKey2 != nullptr)
			DoorKey2->body->GetWorld()->DestroyBody(DoorKey2->body);		
		
		if (keySensor != nullptr)
			keySensor->body->GetWorld()->DestroyBody(keySensor->body);
	}

	delete DoorKey1;
	DoorKey1 = nullptr;
	
	delete DoorKey2;
	DoorKey2 = nullptr;
	
	delete keySensor;
	keySensor = nullptr;

	if (!chickenBoom)
	{
		if (Bomb1 != nullptr)
			Bomb1->body->GetWorld()->DestroyBody(Bomb1->body);
		
		if (Bomb2 != nullptr)
			Bomb2->body->GetWorld()->DestroyBody(Bomb2->body);
		
		if (Barricade1 != nullptr)
			Barricade1->body->GetWorld()->DestroyBody(Barricade1->body);	
		
		if (Barricade2 != nullptr)
			Barricade2->body->GetWorld()->DestroyBody(Barricade2->body);
		
		if (Barricade3 != nullptr)
			Barricade3->body->GetWorld()->DestroyBody(Barricade3->body);
		
		if (Barricade4 != nullptr)
			Barricade4->body->GetWorld()->DestroyBody(Barricade4->body);	
		
		if (Barricade5 != nullptr)
			Barricade5->body->GetWorld()->DestroyBody(Barricade5->body);
	}

	delete Bomb1;
	Bomb1 = nullptr;	
	
	delete Bomb2;
	Bomb2 = nullptr;	
	
	delete Barricade1;
	Barricade1 = nullptr;	
	
	delete Barricade2;
	Barricade2 = nullptr;
	
	delete Barricade3;
	Barricade3 = nullptr;
	
	delete Barricade4;
	Barricade4 = nullptr;
	
	delete Barricade5;
	Barricade5 = nullptr;

	if (!relics) 
	{
		if (DoorBossP != nullptr)
			DoorBossP->body->GetWorld()->DestroyBody(DoorBossP->body);
		
		if (relic1 != nullptr)
			relic1->body->GetWorld()->DestroyBody(relic1->body);
		
		if (relic2 != nullptr)
			relic2->body->GetWorld()->DestroyBody(relic2->body);	
		
		if (relic3 != nullptr)
			relic3->body->GetWorld()->DestroyBody(relic3->body);
	}

	delete DoorBossP;
	DoorBossP = nullptr;	
	
	delete relic1;
	relic1 = nullptr;	
	
	delete relic2;
	relic2 = nullptr;	
	
	delete relic3;
	relic3 = nullptr;

	if (GeneralTextureDungeon2 != nullptr)
		app->tex->UnLoad(GeneralTextureDungeon2);	
	
	if (textureE != nullptr)
		app->tex->UnLoad(textureE);

	if (Boss2Texture != nullptr)
		app->tex->UnLoad(Boss2Texture);

	if (Chicken1 != nullptr)
		Chicken1->body->GetWorld()->DestroyBody(Chicken1->body);	
	
	if (Chicken2 != nullptr)
		Chicken2->body->GetWorld()->DestroyBody(Chicken2->body);

	if (relicColumn1 != nullptr)
		relicColumn1->body->GetWorld()->DestroyBody(relicColumn1->body);

	if (relicColumn2 != nullptr)
		relicColumn2->body->GetWorld()->DestroyBody(relicColumn2->body);

	if (relicColumn3 != nullptr)
		relicColumn3->body->GetWorld()->DestroyBody(relicColumn3->body);

	if (boss2 != nullptr)
		boss2->body->GetWorld()->DestroyBody(boss2->body);

	delete boss2;
	boss2 = nullptr;
	
	delete Chicken1;
	Chicken1 = nullptr;	
	
	delete Chicken2;
	Chicken2 = nullptr;

	delete relicColumn1;
	relicColumn1 = nullptr;	
	
	delete relicColumn2;
	relicColumn2 = nullptr;

	delete relicColumn3;
	relicColumn3 = nullptr;

	return true;
}

bool PuzzleManager::Palancas() 
{
	SDL_Rect dor = { 66, 1, 14, 64 };

	app->render->DrawTexture(door, posDoor1.x - widthHoritzontal / 2, posDoor1.y - heightHoritzontal, &dor);
	//app->render->DrawTexture(door, posDoor1.x, posDoor1.y - heightHoritzontal / 2, &dor);
	//app->render->DrawTexture(door, posDoor2.x - widthHoritzontal, posDoor2.y - heightHoritzontal / 2, &dor);

	if (palancasActive)
	{
		app->render->DrawTexture(textureE, posPalancas.x - 30, posPalancas.y - 10);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
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
			app->audio->PlayFx(solvedfx);
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

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			app->hTerrors->player->lockMovement = true;
			app->input->ActiveGetInput(numCode);
			app->audio->PlayFx(confirmInteractfx);
		}
	}

	if (numCode->input_entered)
	{
		if (strcmp(numCode->input.c_str(), realCode.c_str()) == 0)
		{
			if (doorEscape != nullptr)
				app->tex->UnLoad(doorEscape);

			if (DoorEscape != nullptr)
				DoorEscape->body->GetWorld()->DestroyBody(DoorEscape->body);
			
			if (DoorEscapeSensor != nullptr)
				DoorEscapeSensor->body->GetWorld()->DestroyBody(DoorEscapeSensor->body);

			delete DoorEscape;
			DoorEscape = nullptr;
			
			delete DoorEscapeSensor;
			DoorEscapeSensor = nullptr;

			codeActive = false;
			escape = true;
			app->audio->PlayFx(solvedfx);
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
	
	if (bossIsDead) 
	{
		if (!bossInvent)
		{
			app->render->DrawTexture(bossDeath, posBoss.x - widthBoss, posBoss.y - heightBoss, &bosDeath);
		}

		if (bossSpectreActive_B)
		{
			app->render->DrawTexture(textureE, posBoss.x - 64, posBoss.y - 145);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				bossInvent = true;
				app->audio->PlayFx(confirmInteractfx);

				app->itemManager->AddQuantity(5, 1);

				if (Boss != nullptr)
					Boss->body->GetWorld()->DestroyBody(Boss->body);

				delete Boss;
				Boss = nullptr;

				bossSpectreActive_B = false;
			}
		}

		if (bossInvent)
		{
			if (losetActive)
			{
				app->render->DrawTexture(textureE, posLoset.x - 64, posLoset.y - 120);

				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
				{
					for (int i=0; i< app->itemManager->nodeList.size(); i++)
					{
						if (app->itemManager->nodeList[i]->ID == 5)
						{
							app->itemManager->MinusQuantity(app->itemManager->nodeList[i]);
						}
					}

					if (door != nullptr)
						app->tex->UnLoad(door);

					if (Door3 != nullptr)
						Door3->body->GetWorld()->DestroyBody(Door3->body);

					if (Loset != nullptr)
						Loset->body->GetWorld()->DestroyBody(Loset->body);

					delete Loset;
					Loset = nullptr;

					delete Door3;
					Door3 = nullptr;

					losetActive = false;
					bossInvent = false;
					rescue = true;
					app->audio->PlayFx(solvedfx);
					app->questManager->SaveState();
				}
			}
		}
	}
	else 
	{
		SDL_Rect bos = { 0, 0, 126, 178 };
		app->render->DrawTexture(boss, posBoss.x - 126, posBoss.y - 178, &bos, 1.0f, NULL, NULL, NULL, SDL_FLIP_HORIZONTAL);

		if (bossSpectreActive_B)
		{
			app->render->DrawTexture(textureE, posBoss.x, posBoss.y - 145);
			//pasar a house of terrors
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				LOG("Combat");
				app->SaveGameRequest();
				app->audio->PlayFx(app->hTerrors->combatfx);
				app->combat->PreLoadCombat(app->hTerrors->name, 20);
				fightBoss1 = true; 
				app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->combat, 45);
				app->questManager->SaveState();
				app->puzzleManager->Disable();
				app->hTerrors->steps_I = 0;
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

		if(app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			if (fireGuy != nullptr)
				app->tex->UnLoad(fireGuy);

			if (FireGuy != nullptr)
				FireGuy->body->GetWorld()->DestroyBody(FireGuy->body);

			delete FireGuy;
			FireGuy = nullptr;

			teamMate = true;
			
			app->itemManager->AddCharaToParty(3);

			app->questManager->quest1->active = true;
			app->questManager->quest1->complete = true;

			app->tex->UnLoad(app->hTerrors->player->texture);

			app->questManager->SaveState();

			app->entityManager->tpID = 0;
			//app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->scene, 90);

			completeDungeon_B = true;
		}
	
	}
	return true;
}

bool PuzzleManager::KeyDoorsPuz()
{
	if (keySens) 
	{
		app->render->DrawTexture(textureE, posKey.x - 32, posKey.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			if (keySensor != nullptr)
				keySensor->body->GetWorld()->DestroyBody(keySensor->body);

			delete keySensor;
			keySensor = nullptr;

			keyInvent = true;
			keySens = false;

			app->questManager->SaveState();
		}
	}

	if (keyInvent) 
	{
		if(DoorContact1)
		{
			app->render->DrawTexture(textureE, posDoorkey1.x - 32, posDoorkey1.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				if (DoorKey1 != nullptr)
					DoorKey1->body->GetWorld()->DestroyBody(DoorKey1->body);

				delete DoorKey1;
				DoorKey1 = nullptr;

				DoorsOpened += 1;
				DoorContact1 = false;
				DoorKey1Opened = true;

				app->questManager->SaveState();
			}
		}

		if (DoorContact2)
		{
			app->render->DrawTexture(textureE, posDoorkey2.x - 32, posDoorkey2.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				if (DoorKey2 != nullptr)
					DoorKey2->body->GetWorld()->DestroyBody(DoorKey2->body);

				delete DoorKey2;
				DoorKey2 = nullptr;

				DoorsOpened += 1;
				DoorContact2 = false;
				DoorKey2Opened = true;

				app->questManager->SaveState();
			}
		}
	}
	else
	{
		app->render->DrawTexture(GeneralTextureDungeon2, posKey.x - widthKey, posKey.y - heightKey, &Ky);
	}

	if (DoorKey1 != nullptr)
		app->render->DrawTexture(GeneralTextureDungeon2, posDoorkey1.x - widthDoorKeys, posDoorkey1.y - heightDoorKeys, &KeyD1);
	
	if (DoorKey2 != nullptr)
		app->render->DrawTexture(GeneralTextureDungeon2, posDoorkey2.x - widthDoorKeys + 32, posDoorkey2.y - heightDoorKeys - 64, &KeyD2);

	if (DoorsOpened >= 2)
	{
		keyDoors = true;

		keySens = false;
		keyInvent = false;
		DoorsOpened = 0;

		app->audio->PlayFx(solvedfx);

		app->questManager->SaveState();
	}

	return true;
}

bool PuzzleManager::ChickenBoomPuz()
{
	if (BombContact1) 
	{
		if (!BombCarryOn2)
		{
			app->render->DrawTexture(textureE, posBomb1.x - 32, posBomb1.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				BombCarryOn1 = true;
				RestartTimer();

				if (Bomb1 != nullptr)
					Bomb1->body->GetWorld()->DestroyBody(Bomb1->body);

				delete Bomb1;
				Bomb1 = nullptr;

				BombContact1 = false;
			}
		}
	}

	if (BombCarryOn1)
	{
		posBomb1.x = app->BeastT->player->position.x + 10;
		posBomb1.y = app->BeastT->player->position.y + 25;

		if (DeltaTime >= 6)
		{
			BombCarryOn1 = false;

			posBomb1.x = posChicken1.x + 64;
			posBomb1.y = posChicken1.y;

			Bomb1 = app->physics->CreateRectangleSensor(posBomb1.x - widthBomb / 2, posBomb1.y - heightBomb / 2, widthBomb, heightBomb, bodyType::STATIC);
			Bomb1->body->SetFixedRotation(true);
			Bomb1->ctype = ColliderType::BOMB;
			Bomb1->id = 1;

			RestartTimer();
		}

		if(BarricadeContact1 || BarricadeContact2 || BarricadeContact3 || BarricadeContact4 || BarricadeContact5)
		{
			if(BarricadeContact1)
			{
				app->render->DrawTexture(textureE, posBarricade1.x - 32, posBarricade1.y - 80);
			}
			if(BarricadeContact2)
			{
				app->render->DrawTexture(textureE, posBarricade2.x - 32, posBarricade2.y - 80);
			}	
			if(BarricadeContact3)
			{
				app->render->DrawTexture(textureE, posBarricade3.x - 32, posBarricade3.y - 80);
			}	
			if(BarricadeContact4)
			{
				app->render->DrawTexture(textureE, posBarricade4.x - 32, posBarricade4.y - 80);
			}
			if(BarricadeContact5)
			{
				app->render->DrawTexture(textureE, posBarricade5.x - 32, posBarricade5.y - 80);
			}

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{

				if (BarricadeContact1) 
				{
					BarricadeExplote1 = true;
				}
				if (BarricadeContact2) 
				{
					BarricadeExplote2 = true;
				}
				if (BarricadeContact3) 
				{
					BarricadeExplote3 = true;
				}
				if (BarricadeContact4) 
				{
					BarricadeExplote4 = true;
				}
				if (BarricadeContact5) 
				{
					BarricadeExplote5 = true;
				}

				BombPlant1 = true;

				BombCarryOn1 = false;

				RestartTimer();
			}
		}
	}

	if (BombPlant1) 
	{
		if (BarricadeExplote1)
		{
			posBomb1.x = posBarricade1.x + widthBarricade1 / 6 - 10;
			posBomb1.y = posBarricade1.y - heightBarricade1 / 2 - 10;
		}

		if (BarricadeExplote2)
		{
			posBomb1.x = posBarricade2.x + widthBarricade1 / 6 - 10;
			posBomb1.y = posBarricade2.y - heightBarricade1 / 2 - 10;
		}

		if (BarricadeExplote3)
		{
			posBomb1.x = posBarricade3.x + widthBarricade1 / 6 - 10;
			posBomb1.y = posBarricade3.y - heightBarricade1 / 2 - 10;
		}

		if (BarricadeExplote4)
		{
			posBomb1.x = posBarricade4.x + widthBarricade2 / 4;
			posBomb1.y = posBarricade4.y + heightBarricade2 - 20;
		}

		if (BarricadeExplote5)
		{
			posBomb1.x = posBarricade5.x + widthBarricade1 / 6 - 10;
			posBomb1.y = posBarricade5.y - heightBarricade1 / 2 - 10;
		}

		if(DeltaTime >= 2)
		{
			posBomb1.x = posChicken1.x + 64;
			posBomb1.y = posChicken1.y;

			if (BarricadeExplote1) 
			{
				BarricadeHasExploted1 = true;
				BarricadeExplote1 = false;

				if (Barricade1 != nullptr)
					Barricade1->body->GetWorld()->DestroyBody(Barricade1->body);

				delete Barricade1;
				Barricade1 = nullptr;
			}
			if (BarricadeExplote2) 
			{
				BarricadeHasExploted2 = true;
				BarricadeExplote2 = false;

				if (Barricade2 != nullptr)
					Barricade2->body->GetWorld()->DestroyBody(Barricade2->body);

				delete Barricade2;
				Barricade2 = nullptr;
			}
			if (BarricadeExplote3) 
			{
				BarricadeHasExploted3 = true;
				BarricadeExplote3 = false;

				if (Barricade3 != nullptr)
					Barricade3->body->GetWorld()->DestroyBody(Barricade3->body);

				delete Barricade3;
				Barricade3 = nullptr;
			}
			if (BarricadeExplote4) 
			{
				BarricadeHasExploted4 = true;
				BarricadeExplote4 = false;

				if (Barricade4 != nullptr)
					Barricade4->body->GetWorld()->DestroyBody(Barricade4->body);

				delete Barricade4;
				Barricade4 = nullptr;
			}
			if (BarricadeExplote5) 
			{
				BarricadeHasExploted5 = true;
				BarricadeExplote5 = false;

				if (Barricade5 != nullptr)
					Barricade5->body->GetWorld()->DestroyBody(Barricade5->body);

				delete Barricade5;
				Barricade5 = nullptr;
			}

			RestartTimer();
			BombPlant1 = false;

			Bomb1 = app->physics->CreateRectangleSensor(posBomb1.x - widthBomb / 2, posBomb1.y - heightBomb / 2, widthBomb, heightBomb, bodyType::STATIC);
			Bomb1->body->SetFixedRotation(true);
			Bomb1->ctype = ColliderType::BOMB;
			Bomb1->id = 1;

			BarricadesExplote += 1;

			app->questManager->SaveState();
		}
	}

	if (BombContact2)
	{
		if (!BombCarryOn1)
		{
			app->render->DrawTexture(textureE, posBomb2.x - 32, posBomb2.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				BombCarryOn2 = true;
				RestartTimer();

				if (Bomb2 != nullptr)
					Bomb2->body->GetWorld()->DestroyBody(Bomb2->body);

				delete Bomb2;
				Bomb2 = nullptr;

				BombContact2 = false;
			}
		}
	}

	if (BombCarryOn2)
	{
		posBomb2.x = app->BeastT->player->position.x + 10;
		posBomb2.y = app->BeastT->player->position.y + 25;

		if (DeltaTime >= 6)
		{
			BombCarryOn2 = false;


			posBomb2.x = posChicken2.x + 64;
			posBomb2.y = posChicken2.y;

			Bomb2 = app->physics->CreateRectangleSensor(posBomb2.x - widthBomb / 2, posBomb2.y - heightBomb / 2, widthBomb, heightBomb, bodyType::STATIC);
			Bomb2->body->SetFixedRotation(true);
			Bomb2->ctype = ColliderType::BOMB;
			Bomb2->id = 2;

			RestartTimer();
		}

		if (BarricadeContact1 || BarricadeContact2 || BarricadeContact3 || BarricadeContact4 || BarricadeContact5)
		{
			if (BarricadeContact1)
			{
				app->render->DrawTexture(textureE, posBarricade1.x - 32, posBarricade1.y - 80);
			}
			if (BarricadeContact2)
			{
				app->render->DrawTexture(textureE, posBarricade2.x - 32, posBarricade2.y - 80);
			}
			if (BarricadeContact3)
			{
				app->render->DrawTexture(textureE, posBarricade3.x - 32, posBarricade3.y - 80);
			}
			if (BarricadeContact4)
			{
				app->render->DrawTexture(textureE, posBarricade4.x - 32, posBarricade4.y - 80);
			}
			if (BarricadeContact5)
			{
				app->render->DrawTexture(textureE, posBarricade5.x - 32, posBarricade5.y - 80);
			}

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{

				if (BarricadeContact1)
				{
					BarricadeExplote1 = true;
				}
				if (BarricadeContact2)
				{
					BarricadeExplote2 = true;
				}
				if (BarricadeContact3)
				{
					BarricadeExplote3 = true;
				}
				if (BarricadeContact4)
				{
					BarricadeExplote4 = true;
				}
				if (BarricadeContact5)
				{
					BarricadeExplote5 = true;
				}

				BombPlant2 = true;

				BombCarryOn2 = false;

				RestartTimer();
			}
		}
	}

	if (BombPlant2)
	{
		if (BarricadeExplote1)
		{
			posBomb2.x = posBarricade1.x + widthBarricade1 / 6 - 10;
			posBomb2.y = posBarricade1.y - heightBarricade1 / 2 - 10;
		}

		if (BarricadeExplote2)
		{
			posBomb2.x = posBarricade2.x + widthBarricade1 / 6 - 10;
			posBomb2.y = posBarricade2.y - heightBarricade1 / 2 - 10;
		}		
		
		if (BarricadeExplote3)
		{
			posBomb2.x = posBarricade3.x + widthBarricade1 / 6 - 10;
			posBomb2.y = posBarricade3.y - heightBarricade1 / 2 - 10;
		}		
		
		if (BarricadeExplote4)
		{
			posBomb2.x = posBarricade4.x + widthBarricade2 / 4;
			posBomb2.y = posBarricade4.y + heightBarricade2 - 20;
		}

		if (BarricadeExplote5)
		{
			posBomb2.x = posBarricade5.x + widthBarricade1 / 6 - 10;
			posBomb2.y = posBarricade5.y - heightBarricade1 / 2 - 10;
		}

		if (DeltaTime >= 2)
		{
			posBomb2.x = posChicken2.x + 64;
			posBomb2.y = posChicken2.y;

			if (BarricadeExplote1)
			{
				BarricadeHasExploted1 = true;
				BarricadeExplote1 = false;

				if (Barricade1 != nullptr)
					Barricade1->body->GetWorld()->DestroyBody(Barricade1->body);

				delete Barricade1;
				Barricade1 = nullptr;
			}
			if (BarricadeExplote2)
			{
				BarricadeHasExploted2 = true;
				BarricadeExplote2 = false;

				if (Barricade2 != nullptr)
					Barricade2->body->GetWorld()->DestroyBody(Barricade2->body);

				delete Barricade2;
				Barricade2 = nullptr;
			}
			if (BarricadeExplote3)
			{
				BarricadeHasExploted3 = true;
				BarricadeExplote3 = false;

				if (Barricade3 != nullptr)
					Barricade3->body->GetWorld()->DestroyBody(Barricade3->body);

				delete Barricade3;
				Barricade3 = nullptr;
			}
			if (BarricadeExplote4)
			{
				BarricadeHasExploted4 = true;
				BarricadeExplote4 = false;

				if (Barricade4 != nullptr)
					Barricade4->body->GetWorld()->DestroyBody(Barricade4->body);

				delete Barricade4;
				Barricade4 = nullptr;
			}
			if (BarricadeExplote5)
			{
				BarricadeHasExploted5 = true;
				BarricadeExplote5 = false;

				if (Barricade5 != nullptr)
					Barricade5->body->GetWorld()->DestroyBody(Barricade5->body);

				delete Barricade5;
				Barricade5 = nullptr;
			}

			RestartTimer();
			BombPlant2 = false;

			Bomb2 = app->physics->CreateRectangleSensor(posBomb2.x - widthBomb / 2, posBomb2.y - heightBomb / 2, widthBomb, heightBomb, bodyType::STATIC);
			Bomb2->body->SetFixedRotation(true);
			Bomb2->ctype = ColliderType::BOMB;
			Bomb2->id = 2;

			BarricadesExplote += 1;

			app->questManager->SaveState();
		}
	}

	if (!BarricadeHasExploted1)
	{
		if (Barricade1 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posBarricade1.x - widthBarricade1, posBarricade1.y - heightBarricade1, &Barr1);
	}

	if (!BarricadeHasExploted2)
	{
		if (Barricade2 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posBarricade2.x - widthBarricade1, posBarricade2.y - heightBarricade1, &Barr1);
	}

	if (!BarricadeHasExploted3)
	{
		if (Barricade3 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posBarricade3.x - widthBarricade1, posBarricade3.y - heightBarricade1, &Barr1);
	}


	if (!BarricadeHasExploted4)
	{
		if (Barricade4 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posBarricade4.x - widthBarricade2 / 2, posBarricade4.y - heightBarricade2 / 2, &Barr2);
	}

	if (!BarricadeHasExploted5)
	{
		if (Barricade5 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posBarricade5.x - widthBarricade1, posBarricade5.y - heightBarricade1, &Barr1);
	}

	if(!BombCarryOn1 && !BombPlant1)
	{
		posBomb1.x = posChicken1.x + 64;
		posBomb1.y = posChicken1.y;
	}	
	if(!BombCarryOn2 && !BombPlant2)
	{
		posBomb2.x = posChicken2.x + 64;
		posBomb2.y = posChicken2.y;
	}

	app->render->DrawTexture(GeneralTextureDungeon2, posBomb1.x - widthBomb, posBomb1.y - heightBomb, &Bmb);
	app->render->DrawTexture(GeneralTextureDungeon2, posBomb2.x - widthBomb, posBomb2.y - heightBomb, &Bmb);

	if(BarricadesExplote >= 5)
	{
		chickenBoom = true;

		BarricadesExplote = 0;

		if (Bomb1!= nullptr)
			Bomb1->body->GetWorld()->DestroyBody(Bomb1->body);

		delete Bomb1;
		Bomb1 = nullptr;

		if (Bomb2 != nullptr)
			Bomb2->body->GetWorld()->DestroyBody(Bomb2->body);

		delete Bomb2;
		Bomb2 = nullptr;

		app->audio->PlayFx(solvedfx);

		app->questManager->SaveState();
	}

	return true;
}

bool PuzzleManager::RelicsPuz()
{
	if (RelicContact1) 
	{
		app->render->DrawTexture(textureE, posRelic1.x - 32, posRelic1.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			if (relic1 != nullptr)
				relic1->body->GetWorld()->DestroyBody(relic1->body);

			delete relic1;
			relic1 = nullptr;

			Relic1Invent = true;
			RelicContact1 = false;
			app->itemManager->AddQuantity(110, 1);

			app->questManager->SaveState();
		}
	}

	if (RelicContact2) 
	{
		app->render->DrawTexture(textureE, posRelic2.x - 32, posRelic2.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			if (relic2 != nullptr)
				relic2->body->GetWorld()->DestroyBody(relic2->body);

			delete relic2;
			relic2 = nullptr;

			Relic2Invent = true;
			RelicContact2 = false;
			app->itemManager->AddQuantity(111, 1);

			app->questManager->SaveState();
		}
	}
	
	if (RelicContact3) 
	{
		app->render->DrawTexture(textureE, posRelic3.x - 32, posRelic3.y - 80);

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
			if (relic3 != nullptr)
				relic3->body->GetWorld()->DestroyBody(relic3->body);

			delete relic3;
			relic3 = nullptr;

			Relic3Invent = true;
			RelicContact3 = false;
			app->itemManager->AddQuantity(109, 1);

			app->questManager->SaveState();
		}
	}

	if (Relic1Invent) 
	{
		if (RelicColumnContact1) 
		{
			app->render->DrawTexture(textureE, posRelicColumn1.x - 32, posRelicColumn1.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				//Cambiar textura
				//Col1 = { , , , };

				RelicsCompleted += 1;
				RelicColumnContact1 = false;
				Relic1Invent = false;
				RelicInColumn1 = true;
				app->itemManager->MinusQuantity(app->itemManager->nodeList[110]);


				app->questManager->SaveState();
			}
		}
	}
	else 
	{
		if (relic1 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posRelic1.x - widthRelic, posRelic1.y - heightRelic, &Rel1);
	}

	if (Relic2Invent) 
	{
		if (RelicColumnContact2)
		{
			app->render->DrawTexture(textureE, posRelicColumn2.x - 32, posRelicColumn2.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				//Cambiar textura
				//Col2 = { , , , };

				RelicsCompleted += 1;
				RelicColumnContact2 = false;
				Relic2Invent = false;
				RelicInColumn2 = true;
				app->itemManager->MinusQuantity(app->itemManager->nodeList[111]);

				app->questManager->SaveState();
			}
		}
	}
	else 
	{
		if (relic2 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posRelic2.x - widthRelic, posRelic2.y - heightRelic, &Rel2);
	}	

	if (Relic3Invent) 
	{
		if (RelicColumnContact3)
		{
			app->render->DrawTexture(textureE, posRelicColumn3.x - 32, posRelicColumn3.y - 80);

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
			{
				//Cambiar textura
				//Col3 = { , , , };

				RelicsCompleted += 1;
				RelicColumnContact3 = false;
				Relic3Invent = false;
				RelicInColumn3 = true;
				app->itemManager->MinusQuantity(app->itemManager->nodeList[109]);

				app->questManager->SaveState();
			}
		}
	}
	else 
	{
		if(relic3 != nullptr)
			app->render->DrawTexture(GeneralTextureDungeon2, posRelic3.x - widthRelic, posRelic3.y - heightRelic, &Rel3);
	}

	if (RelicsCompleted >= 3) 
	{
		relics = true;

		RelicsCompleted = 0;

		if (DoorBossP != nullptr)
			DoorBossP->body->GetWorld()->DestroyBody(DoorBossP->body);

		delete DoorBossP;
		DoorBossP = nullptr;

		app->audio->PlayFx(solvedfx);

		app->questManager->SaveState();
	}
	else 
	{
		app->render->DrawTexture(GeneralTextureDungeon2, posDoorBoss.x - 101, posDoorBoss.y - 64 * 2, &BossD);
	}

	return true;
}

void PuzzleManager::RestartTimer()
{
	mStartTicks = SDL_GetTicks();
	mTicks = 0;
	DeltaTime = 0.0f;
}
