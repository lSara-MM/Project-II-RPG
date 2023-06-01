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
	inventory = app->tex->Load(app->itemManager->textureInventoryPath);
	potion = app->tex->Load(app->itemManager->texturePotionsPath);

	SDL_Rect buttonBounds;
	buttonBounds = { 275, 545, 180, 80 };
	buyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1500, this, buttonBounds, ButtonType::SMALL);

	buttonBounds = { 1150, 50, 60, 60 };
	closeStore = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::CLOSE);

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

	app->render->DrawTexture(potion, 80 - app->render->camera.x, 50 - app->render->camera.y);
	app->render->DrawTexture(inventory, 700 - app->render->camera.x, 80 - app->render->camera.y);

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
			app->itemManager->LoadSellItems(x, y, app->itemManager->nodeList[i]);
			x++;
		}
	}

	x = 0;
	y = 0;
	for (int i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->type == 1)
		{
			if (x >= 3)
			{
				y += 69;
				x = 0;
			}
			app->itemManager->LoadStoreItems(x, y, app->itemManager->nodeList[i]);
			x++;
		}
	}

	bool ret = true;

	return ret;
}

// Called before quitting
bool Store::CleanUp()
{
	app->itemManager->SaveItemState();

	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr)
		{
			app->itemManager->nodeList[i]->CleanUp();
		}
	}

	app->guiManager->DestroyGuiControl(buyButton);
	app->guiManager->DestroyGuiControl(closeStore);

	app->tex->UnLoad(inventory);
	app->tex->UnLoad(potion);

	return true;
}

bool Store::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 1501:
		this->Disable();
		app->scene->player->lockMovement = false;
		break;
	}
	return true;
}