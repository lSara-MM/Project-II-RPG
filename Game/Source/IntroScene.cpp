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

#include "EntityManager.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "PuzzleManager.h"
#include "DialogueSystem.h"
#include "Combat.h"

#include <iostream>
using namespace std;
#include <sstream>

IntroScene::IntroScene() : Module()
{
	name.Create("introScene");
}

// Destructor
IntroScene::~IntroScene()
{}

bool IntroScene::Awake(pugi::xml_node& config)
{
	LOG("Loading IntroScene");
	bool ret = true;

	// iterate all objects in the IntroScene
	// Check https://pugixml.org/docs/quickstart.html#access
	music_intro = config.attribute("audioIntroPath").as_string();
	//mouse_Speed = config.attribute("mouseSpeed").as_float();
	texturePath = config.attribute("background").as_string();

	//Save boton continue
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	IntroLoadNode = gameStateFile.child("save_state").child("introScene");

	LoadState(IntroLoadNode);

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	IntroSaveNode = saveStateNode.append_child("introScene");

	animationTitle.Set();
	animationTitle.AddTween(100, 100, BOUNCE_IN_OUT);

	animationBackground.Set();
	animationBackground.AddTween(100, 80, BACK_OUT);

	return ret;
}

// Called before the first frame
bool IntroScene::Start()
{
	app->audio->PlayMusic(music_intro, 1.0f);
	texture = app->tex->Load(texturePath);
	
	if (app->questManager->active) 
	{
		app->questManager->active = false;
	}

	// buttons
	for (int i = 0; buttons[i] != "\n"; i++)
	{
		listButtons.Add((GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + 1, this, { 25, 350 + 77 * i, 200, 70 }, ButtonType::START, buttons[i], 20,  Font::UI, { 0,0,0,0 }, 1, Easings::DEF, AnimationAxis::RIGHT_X));
	}

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->input->playerName->input_entered = false;
	transition_B = false;
	exit_B = false;

	app->itemManager->loadParty_B = false;

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
	if (transition_B)
	{
		animationTitle.Backward();
		animationBackground.Backward();
	}
	else
	{
		animationTitle.Foward();
		animationBackground.Foward();
	}

	animationTitle.Step(1, false);
	animationBackground.Step(1, false);

	float point = animationBackground.GetPoint();
	int offset = -1300;

	app->render->DrawTexture(texture, offset + point * (0 - offset)-app->render->camera.x, 0-app->render->camera.y);

	point = animationTitle.GetPoint();
	
	app->render->TextDraw("TWISTED", 100, offset + point * (50 - offset), 100, Font::TITLE, { 181, 33, 33 });
	app->render->TextDraw("TENT", 250, offset + point * (160 - offset), 100, Font::TITLE, { 181, 33, 33 });

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		transition_B = true;
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}

		app->input->coso = false;
		app->questManager->resetPuzzlesAndQuests();
		app->fade->FadingToBlack(this, (Module*)app->scene, 5);
	}
		
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->guiManager->GUI_debug = !app->guiManager->GUI_debug;

	if (previousGame_B && !app->menus->settings_B)
	{
		listButtons.start->next->data->state = GuiControlState::NORMAL;
		//LOG("Continue");
	}
	else if(!previousGame_B)
	{
		listButtons.start->next->data->state = GuiControlState::DISABLED;
	}

	return true;
}

// Called each loop iteration
bool IntroScene::PostUpdate()
{
	bool ret = true;

	if (exit_B) return false;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		LOG("general %d, %d music, %d fx", app->menus->pSettings->pAudio->general->volume100, app->menus->pSettings->pAudio->music->volume100,app->menus->pSettings->pAudio->fx->volume100);

	if (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN) {
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::NORMAL;
		}
		app->menus->pSettings->settings_B = false;
	}

	return ret;
}

// Called before quitting
bool IntroScene::CleanUp()
{
	LOG("Freeing IntroScene");
	app->input->temp = "";

	listButtons.Clear();
	app->tex->UnLoad(texture);

	return true;
}

bool IntroScene::LoadState(pugi::xml_node& data, Module* module)
{
	previousGame_B = data.child("previousGame").attribute("state_B").as_bool();

	return true;
}

bool IntroScene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node previousGame = data.append_child("previousGame");
	previousGame.append_attribute("state_B") = true;

	return true;
}

bool IntroScene::OnGuiMouseHoverEvent(GuiControl* control)
{
	

	return true;
}

bool IntroScene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	app->audio->PlayFx(control->fxControl);

	switch (control->id)
	{
	case 1:
		LOG("Button start click");
		transition_B = true;
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			//Resetear Puzzles y Quests
			app->questManager->resetPuzzlesAndQuests();
			i->data->isForward_B = false;
		}

		app->input->coso = false;
		app->entityManager->tpID = 21;

		app->fade->FadingToBlack(this, (Module*)app->cutScene, 45);
		break;
	case 2:
		LOG("Button continue click");
		transition_B = true;
		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->isForward_B = false;
		}

		app->itemManager->loadParty_B = true;
		app->input->coso = true;
		app->dialogueSystem->LoadDialogueState();
		app->LoadFromFile(this);
		continueGame_B = true;
		break;
	case 3:
		LOG("Button settings click");
		app->menus->settings_B = true;

		for (ListItem<GuiButton*>* i = listButtons.start; i != nullptr; i = i->next)
		{
			i->data->state = GuiControlState::DISABLED;
		}
		break;
	case 4:
		LOG("Button Exit game click");
		exit_B = true;
		break;
	}

	return true;
}