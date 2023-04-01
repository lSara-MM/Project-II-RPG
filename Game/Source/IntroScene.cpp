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
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, bNum, this, { 25, 180 + 77 * i, 136, 33 }, ButtonType::LARGE, buttons[i], 20));
	}

	listButtons.start->next->data->state = GuiControlState::DISABLED;

	pSettings->CreateSettings(this);
	listButtons.Add(pSettings->listSettingsButtons.start->data);

	exit_B = false;

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

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;

	return true;
}

// Called each loop iteration
bool IntroScene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	if (pSettings->settings_B) { pSettings->OpenSettings(); }

	app->guiManager->Draw();

	return ret;
}

// Called before quitting
bool IntroScene::CleanUp()
{
	LOG("Freeing IntroScene");
	
	listButtons.Clear();
	pSettings->CleanUp();

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
		//app->audio->ChangeMusicVolume(pSettings->music->volume100);
		break;
	case 3:
		LOG("Slider fx click");
		//app->audio->ChangeFxVolume(pSettings->fx->volume100);
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
		pSettings->settings_B = !pSettings->settings_B;
		if (!pSettings->settings_B)
		{
			pSettings->CloseSettings();
		}
		break;
	case 9:
		LOG("Button Credits click");
		break;

	case 10:
		LOG("Button Exit game click");
		exit_B = true;
		break;

	case 11:
		LOG("Button Close credits");
		break;


		// Settings
	case 801:
		LOG("Button Close settings click");
		pSettings->CloseSettings();
		break;

	case 802:
		LOG("Game settings click");
		pSettings->pGame->game_B = true;
		break;

	case 803:
		LOG("Controls settings click");
		pSettings->pControl->control_B = true;
		break;

	case 804:
		LOG("Graphics settings click");
		pSettings->pGraphics->graphics_B = true;
		break;

	case 805:
		LOG("Audio settings click");
		pSettings->pAudio->audio_B = true;
		break;


		// Game settings
	case 806:
		LOG("Button Language click");

		break;

	case 807:
		LOG("Button Text Speed click");

		break;
	case 808:
		LOG("Button Return to Title click");

		break;

	case 809:
		LOG("Button Exit Game click");

		break;


		// Control settings
	case 810:
		LOG("Button Move Up keyboard check");

		break;

	case 811:
		LOG("Button Move Up gamepad check");

		break;

	case 812:
		LOG("Button Move Left keyboard check");

		break;

	case 813:
		LOG("Button Move Left gamepad check");

		break;

	case 814:
		LOG("Button Move Right keyboard check");

		break;

	case 815:
		LOG("Button Move Right gamepad check");

		break;

	case 816:
		LOG("Button Move Down keyboard check");

		break;

	case 817:
		LOG("Button Move Down gamepad check");

		break;

	case 818:
		LOG("Button Interact keyboard check");

		break;

	case 819:
		LOG("Button Interact gamepad check");

		break;

	case 820:
		LOG("Button Inventory keyboard check");

		break;

	case 821:
		LOG("Button Party gamepad check");

		break;

	case 822:
		LOG("Button Quests keyboard check");

		break;

	case 823:
		LOG("Button Quests gamepad check");

		break;

	case 824:
		LOG("Button Map keyboard check");

		break;

	case 825:
		LOG("Button Map gamepad check");

		break;

	case 826:
		LOG("Button Settings keyboard check");

		break;

	case 827:
		LOG("Button Settings gamepad check");

		break;


		// Graphics settings
	case 828:
		LOG("Button Windows size");

		break;

	case 829:
		LOG("Checkbox Fullscreen check");

		break;

	case 830:
		LOG("Checkbox Vsync check");

		break;

	case 831:
		LOG("Button Max fps");

		break;


		// Audio settings
	case 832:
		LOG("Slider bar General volume");

		break;

	case 833:
		LOG("Slider bar Music volume");

		break;

	case 834:
		LOG("Slider bar Fx volume");

		break;
	}

	return true;
}
