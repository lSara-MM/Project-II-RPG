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
	// iterate all objects in the LogoScene
	// Check https://pugixml.org/docs/quickstart.html#access
	
	return ret;
}

// Called before the first frame
bool LogoScene::Start()
{
	/*SString title("UPC - CITM");
	app->win->SetTitle(title.GetString());*/

	logoTexture = app->tex->Load(logoPath);
	logoCitmTexture = app->tex->Load(logoCitmPath);

	app->audio->PlayMusic(musicLogo, 0);
	
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
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->controller.A)
		app->fade->FadingToBlack(this, (Module*)app->iScene, 90);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->iScene, 5);

	return true;
}

// Called each loop iteration
bool LogoScene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(logoTexture, 200, 0);
	app->render->DrawTexture(logoCitmTexture, 850, 550);
	
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
