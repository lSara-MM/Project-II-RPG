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

	forgeTexture = app->tex->Load(app->itemManager->forgePath);
	forgeInventoryTexture = app->tex->Load(app->itemManager->textureInventoryPath);

	//animation inventory
	forgeAnimation.Set();
	forgeAnimation.AddTween(100, 80, BOUNCE_OUT);
	forgeTransition_B = false;
	posYforgeAnimation = 0;

	return true;
}


bool Forge::PreUpdate()
{
	return true;
}

bool Forge::Update(float dt)
{
	if (forgeTransition_B)
	{
		forgeAnimation.Backward();
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)//poner invisibles los botones creados
		{
			if (app->itemManager->nodeList[i]->button != nullptr)
			{
				app->itemManager->nodeList[i]->button->state = GuiControlState::NONE;
			}
		}
		if (posYforgeAnimation == -750)
		{
			this->Disable();
		}
	}
	else
	{
		forgeAnimation.Foward();
	}
	forgeAnimation.Step(1, false);

	float point = forgeAnimation.GetPoint();
	int offset = -750;
	posYforgeAnimation = offset + point * (50 - offset);
	app->render->DrawTexture(forgeTexture, 80 - app->render->camera.x, posYforgeAnimation - app->render->camera.y);
	app->render->DrawTexture(forgeInventoryTexture, 700 - app->render->camera.x, offset + point * (80 - offset) - app->render->camera.y);
	bool ret = true;

	return ret;
}

bool Forge::PostUpdate()
{

	//Load all items
	int x = 0;
	int y = 0;
	for (int i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->type == 3 && app->itemManager->nodeList[i]->quantity > 0)
		{
			if (x >= 5)
			{
				y += 69;
				x = 0;
			}
			if (posYforgeAnimation == 50)
			{
				app->itemManager->LoadForgeItems(x, y, app->itemManager->nodeList[i]);
			}
			x++;
		}
	}
	for (int i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (x >= 5)
		{
			y += 69;
			x = 0;
		}
		if (posYforgeAnimation == 50)
		{
			app->itemManager->LoadForgeItems(x, y, app->itemManager->armorItems[i]);
		}
		x++;
	}

	for (int i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->craft)
		{
			for (int j = 0; j < app->itemManager->nodeList.size(); j++)
			{
				if (app->itemManager->nodeList[j]->craft && i != j)
				{
					if (IsForge(app->itemManager->nodeList[j]->ID, app->itemManager->nodeList[i]->ID, false))
					{

					}
				}
			}
			for (int j = 0; j < app->itemManager->armorItems.size(); j++)
			{
				if (app->itemManager->armorItems[j]->craft)
				{
					if (IsForge(app->itemManager->nodeList[j]->ID, app->itemManager->armorItems[i]->ID, true))
					{

					}
				}
			}
		}
	}

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
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr)
		{
			app->itemManager->nodeList[i]->craft = false;
			app->itemManager->nodeList[i]->CleanUp();
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->button != nullptr)
		{
			app->itemManager->armorItems[i]->craft = false;
			app->itemManager->armorItems[i]->CleanUp();
		}
	}

	app->guiManager->DestroyGuiControl(exitButton);
	app->guiManager->DestroyGuiControl(forgeButton);

	app->tex->UnLoad(forgeInventoryTexture);
	app->tex->UnLoad(forgeTexture);

	return true;
}

bool Forge::IsForge(int ID0, int ID1, bool armor)
{
	bool ret = true;

	return ret;
}

bool Forge::OnGuiMouseHoverEvent(GuiControl* control)
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->button->id == control->id)
		{
			app->itemManager->nodeList[i]->printStats = true;
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->button != nullptr && app->itemManager->armorItems[i]->button->id == control->id)
		{
			app->itemManager->armorItems[i]->printStats = true;
		}
	}

	return true;
}

bool Forge::OnGuiMouseOutHoverEvent(GuiControl* control)
{

	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->button->id == control->id)
		{
			app->itemManager->nodeList[i]->printStats = false;
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->button != nullptr && app->itemManager->armorItems[i]->button->id == control->id)
		{
			app->itemManager->armorItems[i]->printStats = false;
		}
	}

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