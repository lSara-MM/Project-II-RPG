#include "IntroScene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Scene.h"

#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

IntroScene::IntroScene() : Module()
{
	name.Create("introScene");
}

// Destructor
IntroScene::~IntroScene()
{}

// Called before render is available
bool IntroScene::Awake(pugi::xml_node& config)
{
	LOG("Loading IntroScene");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool IntroScene::Start()
{
	SString title("Twisted Tent: width- %d, height- %d", app->win->GetWidth(), app->win->GetHeight());

	app->win->SetTitle(title.GetString());


	// buttons
	for (int i = 0; buttons[i] != "\n"; i++)
	{
		bNum = i + 6;
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, bNum, buttons[i], { 25, 180 + 33 * i, 90, 27 }, 10, this, ButtonType::LARGE));
	}

	listButtons.start->next->data->state = GuiControlState::DISABLED;

	pSettings->CreateSettings(this);
	listButtons.Add(pSettings->listSettingsButtons.start->data);

	pCredits->CreateCredits(this, bNum);

	exit = false;

	return true;
}

// Called each loop iteration
bool IntroScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool IntroScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);

	return true;
}

// Called each loop iteration
bool IntroScene::PostUpdate()
{
	bool ret = true;

	if (exit) return false;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	if (pSettings->settings) { pSettings->OpenSettings(); }
	if (pCredits->credits) { pCredits->OpenCredits(); }

	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool IntroScene::CleanUp()
{
	LOG("Freeing IntroScene");
	
	listButtons.Clear();
	pSettings->CleanUp();
	pCredits->CleanUp();

	app->guiManager->CleanUp();
	return true;
}

bool IntroScene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button Close settings click");
		pSettings->CloseSettings();
		break;
	case 2:
		LOG("Slider music click");
		app->audio->ChangeMusicVolume(pSettings->music->volume100);
		break;
	case 3:
		LOG("Slider fx click");
		app->audio->ChangeFxVolume(pSettings->fx->volume100);
		break;
	case 4:
		LOG("Checkbox Fullscreen click");
		app->win->changeScreen = !app->win->changeScreen;
		app->win->ResizeWin();
		break;
	case 5:
		LOG("Checkbox Vsync click");
		(control->state == GuiControlState::NORMAL) ? app->render->flags = SDL_RENDERER_ACCELERATED : app->render->flags |= SDL_RENDERER_PRESENTVSYNC;
		break;
	case 6:
		LOG("Button start click");
		app->fade->FadingToBlack(this, (Module*)app->scene, 90);
		break;
	case 7:
		LOG("Button continue click");
		break;
	case 8:
		LOG("Button settings click");
		pSettings->settings = !pSettings->settings;
		if (!pSettings->settings)
		{
			pSettings->CloseSettings();
		}
		break;
	case 9:
		LOG("Button Credits click");
		pCredits->credits = !pCredits->credits;
		if (!pCredits->credits) { pCredits->CloseCredits(); }
		break;

	case 10:
		LOG("Button Exit game click");
		exit = true;
		break;

	case 11:
		LOG("Button Close credits");
		pCredits->CloseCredits();
		break;
	}

	return true;
}
