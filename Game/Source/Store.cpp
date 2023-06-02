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
	buttonBounds = { 290, 545, 180, 69 };
	buyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1500, this, buttonBounds, ButtonType::BUYITEM, "0", 30);

	buttonBounds = { 1150, 50, 60, 60 };
	closeStore = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::CLOSE);

	buttonBounds = { 240, 535, 40, 20 };
	Add = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1502, this, buttonBounds, ButtonType::ADD);

	buttonBounds = { 240, 600, 40, 20 };
	Minus = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1503, this, buttonBounds, ButtonType::MINUS);

	//animation inventory
	inventoryAnimation.Set();
	inventoryAnimation.AddTween(100, 120, BOUNCE_OUT);
	inventoryTransition_B = false;

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
	inventoryAnimation.Step(2, false);

	float point = inventoryAnimation.GetPoint();
	int offset = -750;

	app->render->DrawTexture(potion, 80 - app->render->camera.x, offset + point * (50 - offset) - app->render->camera.y);
	app->render->DrawTexture(inventory, 700 - app->render->camera.x, offset + point * (80 - offset) - app->render->camera.y);

	bool ret = true;

	return ret;
}

bool Store::PostUpdate()
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

	for (int i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if ((app->itemManager->nodeList[i]->toSell && app->itemManager->nodeList[i]->type == 3 && app->itemManager->nodeList[i]->quantity > 0) || (app->itemManager->nodeList[i]->toSell && app->itemManager->nodeList[i]->type == 1))
		{
			app->itemManager->ItemToSell(app->itemManager->nodeList[i]);
		}
	}

	//arreglo cuestionable pero bueno, it works 
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
	{
		app->render->DrawTexture(app->input->cursorPressedTex, app->scene->mouseX_scene - app->render->camera.x, app->scene->mouseY_scene - app->render->camera.y);
	}
	else
	{
		app->render->DrawTexture(app->input->cursorIdleTex, app->scene->mouseX_scene - app->render->camera.x, app->scene->mouseY_scene - app->render->camera.y);
	}

	string c = to_string(CurrentPrice);
	buyButton->text = c.c_str();

	string h = to_string(SellQuantity);
	app->render->TextDraw(h.c_str(), 250, 550, 40, Font::TEXT, { 255, 255, 255 });

	return true;
}

// Called before quitting
bool Store::CleanUp()
{
	app->itemManager->SaveItemState();

	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr)
		{
			app->itemManager->nodeList[i]->toSell = false;
			app->itemManager->nodeList[i]->CleanUp();
		}
	}

	app->guiManager->DestroyGuiControl(buyButton);
	app->guiManager->DestroyGuiControl(closeStore);
	app->guiManager->DestroyGuiControl(Add);
	app->guiManager->DestroyGuiControl(Minus);

	app->tex->UnLoad(inventory);
	app->tex->UnLoad(potion);

	return true;
}

bool Store::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control->id != 1500 || control->id != 1501)
	{
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
		{
			if (app->itemManager->nodeList[i]->ID == control->id)
			{
				if (app->itemManager->nodeList[i]->toSell == false)
				{
					for (size_t j = 0; j < app->itemManager->nodeList.size(); j++)
					{
						if (j != i)
						{
							if (app->itemManager->nodeList[j]->toSell)
							{
								app->itemManager->nodeList[j]->toSell = false;
								SellQuantity = 0;
							}
						}
					}
					app->itemManager->nodeList[i]->toSell = true;
					SellQuantity++;

					if (app->itemManager->nodeList[i]->type == 1)
					{
						buyButton->buttonType = ButtonType::BUYITEM;
					}
					else
					{
						buyButton->buttonType = ButtonType::SELLITEM;
					}
				}
				else
				{
					app->itemManager->nodeList[i]->toSell = false;
					SellQuantity = 0;
				}

				CurrentPrice = app->itemManager->nodeList[i]->price * SellQuantity;
			}
		}
	}

	switch (control->id)
	{
	case 1501:
		this->Disable();
		app->scene->player->lockMovement = false;
		break;

	case 1500:
		if (buyButton->buttonType == ButtonType::BUYITEM)
		{
			if (app->itemManager->coins >= CurrentPrice)
			{
				for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
				{
					if (app->itemManager->nodeList[i]->toSell && app->itemManager->nodeList[i]->quantity + SellQuantity <= app->itemManager->nodeList[i]->max)
					{
						app->itemManager->AddQuantity(i, SellQuantity);
						app->itemManager->coins -= CurrentPrice;
						SellQuantity = 0;
						app->itemManager->nodeList[i]->toSell = false;
					}
				}
			}
		}
		else
		{
			for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
			{
				if (app->itemManager->nodeList[i]->toSell)
				{
					for (int j = 0; j < CurrentPrice; j++)
					{
						app->itemManager->MinusQuantity(app->itemManager->nodeList[i]);
					}
					app->itemManager->coins += CurrentPrice;
					SellQuantity = 0;
					app->itemManager->nodeList[i]->toSell = false;
				}
			}
		}
		break;

	case 1502:
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
		{
			if (app->itemManager->nodeList[i]->toSell)
			{
				if (app->itemManager->nodeList[i]->type == 1)
				{
					if (app->itemManager->nodeList[i]->max > SellQuantity)
					{
						SellQuantity++;
						CurrentPrice = app->itemManager->nodeList[i]->price * SellQuantity;
					}
				}
				else
				{
					if (app->itemManager->nodeList[i]->quantity < SellQuantity)
					{
						SellQuantity++;
						CurrentPrice = app->itemManager->nodeList[i]->price * SellQuantity;
					}
				}
			}
		}
		break;
	case 1503:
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
		{
			if (app->itemManager->nodeList[i]->toSell)
			{
				if (app->itemManager->nodeList[i]->type == 1)
				{
						SellQuantity--;
						if (SellQuantity <= 0)
						{
							app->itemManager->nodeList[i]->toSell = false;
						}
						CurrentPrice = app->itemManager->nodeList[i]->price * SellQuantity;
				}
				else
				{
					SellQuantity++;
					if (SellQuantity <= 0)
					{
						app->itemManager->nodeList[i]->toSell = false;
					}
					CurrentPrice = app->itemManager->nodeList[i]->price * SellQuantity;
				}
			}
		}
		break;
	}

	return true;
}