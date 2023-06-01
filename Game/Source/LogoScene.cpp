#include "LogoScene.h"

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


LogoScene::LogoScene() : Module()
{
	name.Create("logoScene");
}

// Destructor
LogoScene::~LogoScene()
{}

// Called before render is available
bool LogoScene::Awake(pugi::xml_node& config)
{
	LOG("Loading LogoScene");
	bool ret = true;

	musicLogo = config.attribute("audioLogoPath").as_string();
	logoPath = config.attribute("background").as_string();
	logoCitmPath = config.attribute("collaborator").as_string();

	animationLogo.Set();
	animationLogo.AddTween(100, 80, BACK_OUT);

	animationCitm.Set();
	animationCitm.AddTween(100, 120, BOUNCE_IN_OUT);

	return ret;
}

// Called before the first frame
bool LogoScene::Start()
{
	/*SString title("UPC - CITM");
	app->win->SetTitle(title.GetString());*/

	logoTexture = app->tex->Load(logoPath);
	logoCitmTexture = app->tex->Load(logoCitmPath);

	app->audio->PlayMusic(musicLogo, 1.0f);

	transition__B = false;
	
	return true;
}

// Called each loop iteration
bool LogoScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LogoScene::Update(float dt)
{	
	if (transition__B)
	{
		animationLogo.Backward();
		animationCitm.Backward();
	}
	
	else
	{
		animationLogo.Foward();
		animationCitm.Foward();
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN) {
		transition__B = true;
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->combat->firstCombat_B = false;
		app->input->coso = false;
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);
	}
		

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->iScene, 5);

	animationLogo.Step(1, false);
	animationCitm.Step(1, false);


	float point = animationLogo.GetPoint();
	int offset = -750;

	app->render->DrawTexture(logoTexture, 200, offset + point * (0 - offset));

	
	point = animationCitm.GetPoint();
	app->render->DrawTexture(logoCitmTexture, 850, offset + point * (550 - offset));

	return true;
}

// Called each loop iteration
bool LogoScene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	
	return ret;
}

// Called before quitting
bool LogoScene::CleanUp()
{
	LOG("Freeing LogoScene");

	//app->audio->PauseMusic();
	app->tex->UnLoad(logoTexture);
	app->tex->UnLoad(logoCitmTexture);

	return true;
}
