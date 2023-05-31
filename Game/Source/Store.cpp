#include "Store.h"
#include "Input.h"
#include "app.h"
#include "Log.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "ItemManager.h"

Store::Store(unsigned cap) : Module()
{
	name.Create("store");
	this->cap = cap;
	y = 0;
}

// Destructor
Store::~Store()
{

}

bool Store::Start()
{
	inventoryIMG = app->tex->Load(app->itemManager->textureStorePath);

	SDL_Rect buttonBounds;
	buttonBounds = { 275, 545, 180, 80 };
	buyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1500, this, buttonBounds, ButtonType::SMALL);

	buttonBounds = { 1150, 50, 60, 60 };
	closeStore = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::SMALL);

	//animation inventory
	inventoryAnimation.Set();
	inventoryAnimation.AddTween(100, 100, BOUNCE_IN_OUT);
	inventoryTransition_B = true;

	return true;
}


bool Store::PreUpdate()
{
	return true;
}

bool Store::Update(float dt)
{
	if (inventoryTransition_B)
	{
		inventoryAnimation.Backward();
	}
	else
	{
		inventoryAnimation.Foward();
	}
	inventoryAnimation.Step(1, false);

	float point = inventoryAnimation.GetPoint();
	int offset = -1300;

	app->render->DrawTexture(inventoryIMG, 0 - app->render->camera.x, 0 - app->render->camera.y);

	bool ret = true;

	return ret;
}

// Called before quitting
bool Store::CleanUp()
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->quantity > 0)
		{
			app->itemManager->nodeList[i]->CleanUp();
		}
	}

	app->guiManager->DestroyGuiControl(buyButton);
	app->guiManager->DestroyGuiControl(closeStore);

	app->tex->UnLoad(inventoryIMG);

	return true;
}

bool Store::OnGuiMouseClickEvent(GuiControl* control)
{

	return true;
}