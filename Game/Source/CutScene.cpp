#include "CutScene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include "IntroScene.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "Combat.h"


CutScene::CutScene() : Module()
{
	name.Create("CutScene");
}

// Destructor
CutScene::~CutScene()
{}

// Called before render is available
bool CutScene::Awake(pugi::xml_node& config)
{
	LOG("Loading CutScene");
	bool ret = true;

	pathFirstImg = config.attribute("firstImage").as_string();
	pathSecondImg = config.attribute("secondImage").as_string();
	pathThirdImg = config.attribute("thirdImage").as_string();
	pathFourthImg = config.attribute("fourthImage").as_string();
	pathFifthImg = config.attribute("fifthImage").as_string();
	pathSixImg = config.attribute("sixImage").as_string();

	return ret;
}

// Called before the first frame
bool CutScene::Start()
{
	passImg = 0;

	RestartTimer();

	FirstImg = app->tex->Load(pathFirstImg);
	SecondtImg = app->tex->Load(pathSecondImg);
	ThirdImg = app->tex->Load(pathThirdImg);
	FourthImg = app->tex->Load(pathFourthImg);
	FifthImg = app->tex->Load(pathFifthImg);
	SixImg = app->tex->Load(pathSixImg);

	return true;
}

// Called each loop iteration
bool CutScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool CutScene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		passImg++;
		RestartTimer();
	}

	if (DeltaTime >= 1) 
	{
		app->render->TextDraw("PRESS SPACE", app->win->GetWidth() / 2, app->win->GetHeight() / 2, 50, Font::UI, { 181, 33, 33 });
	}
	if(DeltaTime >= 2)
	{
		RestartTimer();
	}

	if (passImg == 0) 
	{
		app->render->DrawTexture(FirstImg, 0, 0);
	}
	if (passImg == 1) 
	{
		app->render->DrawTexture(SecondtImg, 0, 0);
	}	
	if (passImg == 2) 
	{
		app->render->DrawTexture(ThirdImg, 0, 0);
	}	
	if (passImg == 3) 
	{
		app->render->DrawTexture(FourthImg, 0, 0);
	}	
	if (passImg == 4) 
	{
		app->render->DrawTexture(FifthImg, 0, 0);
	}	
	if (passImg == 5) 
	{
		app->render->DrawTexture(SixImg, 0, 0);
	}
	if (passImg >= 6) 
	{
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
	}

	mTicks = SDL_GetTicks() - mStartTicks;
	DeltaTime = mTicks * 0.001f;

	return true;
}

// Called each loop iteration
bool CutScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		passImg = 6;
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
	}

	return ret;
}

// Called before quitting
bool CutScene::CleanUp()
{
	LOG("Freeing LogoScene");

	app->tex->UnLoad(FirstImg);
	app->tex->UnLoad(SecondtImg);
	app->tex->UnLoad(ThirdImg);
	app->tex->UnLoad(FourthImg);
	app->tex->UnLoad(FifthImg);
	app->tex->UnLoad(SixImg);

	return true;
}

void CutScene::RestartTimer()
{
	mStartTicks = SDL_GetTicks();
	mTicks = 0;
	DeltaTime = 0.0f;
}
