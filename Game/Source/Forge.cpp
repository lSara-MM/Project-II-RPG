#include "Forge.h"
#include "Input.h"
#include "app.h"
#include "Log.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "ItemManager.h"

Forge::Forge(unsigned cap) : Module()
{
	name.Create("forge");
	this->cap = cap;
	y = 0;
}

// Destructor
Forge::~Forge()
{

}

bool Forge::Start()
{
	SDL_Rect buttonBounds;
	buttonBounds = { 1150, 50, 60, 60 };
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::CLOSE, "", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	return true;
}


bool Forge::PreUpdate()
{
	return true;
}

bool Forge::Update(float dt)
{

	bool ret = true;

	return ret;
}

bool Forge::PostUpdate()
{
	//raton
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
	{
		app->render->DrawTexture(app->input->cursorPressedTex, app->input->mouseX - app->render->camera.x, app->input->mouseY - app->render->camera.y);
	}
	else
	{
		app->render->DrawTexture(app->input->cursorIdleTex, app->input->mouseX - app->render->camera.x, app->input->mouseY - app->render->camera.y);
	}

	return true;
}

// Called before quitting
bool Forge::CleanUp()
{
	app->guiManager->DestroyGuiControl(exitButton);
	return true;
}

bool Forge::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{

	case 1501:
		this->Disable();
		break;
	}
	return true;
}