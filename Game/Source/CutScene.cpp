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

	pugiNode = config.first_child();

	for (int i = 0; pugiNode != NULL; i++)
	{
		NextImg.Add(pugiNode.attribute("texturepath").as_string());

		pugiNode = pugiNode.next_sibling("Img");
	}

	return ret;
}

// Called before the first frame
bool CutScene::Start()
{
	passImg = 0;

	RestartTimer();

	for(int i = 0; i <= NextImg.Count() - 1; i++)
	{
		ImgToPrint.Add(app->tex->Load(NextImg.At(i)->data));
	}

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

	if (passImg >= ImgToPrint.Count() - 1)
	{
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
	}

	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
	{
		passImg++;

		if(ImgToPrint.At(passImg + 1) != NULL)
			app->fade->FadingToBlackImages(ImgToPrint.At(passImg)->data, ImgToPrint.At(passImg + 1)->data, 40);

		RestartTimer();
	}

	if (ImgToPrint.At(passImg) != NULL)
		app->render->DrawTexture(ImgToPrint.At(passImg)->data, 0, 0);

	if (DeltaTime >= 1)
	{
		app->render->TextDraw("PRESS SPACE", app->win->GetWidth() / 2 - 200, app->win->GetHeight() - 100, 50, Font::UI, { 255, 255, 255 });
	}
	if (DeltaTime >= 2)
	{
		RestartTimer();
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
		passImg = ImgToPrint.Count() - 1;
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
	}

	return ret;
}

// Called before quitting
bool CutScene::CleanUp()
{
	LOG("Freeing LogoScene");

	for (int i = 0; i <= ImgToPrint.Count() - 1; i++) 
	{
		app->tex->UnLoad(ImgToPrint.At(i)->data);
	}

	ImgToPrint.Clear();

	return true;
}

void CutScene::RestartTimer()
{
	mStartTicks = SDL_GetTicks();
	mTicks = 0;
	DeltaTime = 0.0f;
}
