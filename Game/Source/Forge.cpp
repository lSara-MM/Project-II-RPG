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
	inventory = app->tex->Load(app->itemManager->textureInventoryPath);
	potion = app->tex->Load(app->itemManager->texturePotionsPath);

	SDL_Rect buttonBounds;
	buttonBounds = { 290, 545, 180, 69 };
	buyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1500, this, buttonBounds, ButtonType::BUYITEM, "0", 30, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 1150, 50, 60, 60 };
	closeStore = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::CLOSE, "", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 240, 535, 40, 20 };
	Add = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1502, this, buttonBounds, ButtonType::ADD,"", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 240, 600, 40, 20 };
	Minus = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1503, this, buttonBounds, ButtonType::MINUS,"", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);//añadido animacion quede igual menu

	//animation inventory
	inventoryAnimation.Set();
	inventoryAnimation.AddTween(100, 80, BOUNCE_OUT);
	inventoryTransition_B = false;

	return true;
}


bool Forge::PreUpdate()
{
	return true;
}

bool Forge::Update(float dt)
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
	if (app->itemManager->coins < CurrentPrice && buyButton->buttonType == ButtonType::BUYITEM) { buyButton->color = { 230, 33, 33 }; }
	else if (buyButton->buttonType == ButtonType::SELLITEM) { buyButton->color = { 100, 250, 90 }; }
	else { buyButton->color = { 0, 0, 0 }; }

	string h = to_string(SellQuantity);
	app->render->TextDraw(h.c_str(), 250, 550, 40, Font::TEXT, { 255, 255, 255 });

	app->render->DrawTexture(app->itemManager->coinTexture, 1150 - app->render->camera.x, 180 - app->render->camera.y);

	string cn = to_string(app->itemManager->coins);
	app->render->TextDraw(cn.c_str(), 1125, 180, 25, Font::TEXT, { 255, 255, 255 });


	if ( app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == ButtonState::BUTTON_DOWN)//con mando es bastante molesto tener que ir con el mouse al boton de cerrar
	{
		this->Disable();
		app->scene->player->lockMovement = false;
	}

	return true;
}

// Called before quitting
bool Forge::CleanUp()
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

	CurrentPrice = 0;
	SellQuantity = 0;

	app->tex->UnLoad(inventory);
	app->tex->UnLoad(potion);

	return true;
}

bool Forge::OnGuiMouseClickEvent(GuiControl* control)
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
					SellQuantity--;
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