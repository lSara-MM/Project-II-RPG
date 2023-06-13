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
	buyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1500, this, buttonBounds, ButtonType::BUYITEM, "0", 30, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 1150, 50, 60, 60 };
	closeStore = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::CLOSE, "", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 240, 535, 40, 20 };
	Add = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1502, this, buttonBounds, ButtonType::ADD,"", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 240, 600, 40, 20 };
	Minus = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1503, this, buttonBounds, ButtonType::MINUS,"", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);//añadido animacion quede igual menu

	//animation inventory
	storeAnimation.Set();
	storeAnimation.AddTween(100, 80, BOUNCE_OUT);
	storeTransition_B = false;
	posYstoreAnimation = 0;

	fxbuypath = "Assets/Audio/Fx/buy.wav";
	buyfx = app->audio->LoadFx(fxbuypath);

	return true;
}


bool Store::PreUpdate()
{
	return true;
}

bool Store::Update(float dt)
{
	if (storeTransition_B)
	{
		storeAnimation.Backward();
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)//poner invisibles los botones creados
		{
			if (app->itemManager->nodeList[i]->button != nullptr)
			{
				app->itemManager->nodeList[i]->button->state = GuiControlState::NONE;
			}
		}
		if (posYstoreAnimation == -750)
		{
			this->Disable();
		}
	}
	else
	{
		storeAnimation.Foward();
	}
	storeAnimation.Step(1, false);

	float point = storeAnimation.GetPoint();
	int offset = -750;
	posYstoreAnimation = offset + point * (50 - offset);
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
			if (posYstoreAnimation == 50)
			{
				app->itemManager->LoadSellItems(x, y, app->itemManager->nodeList[i]);
			}
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
			if (posYstoreAnimation == 50)//se printa cuando animacion acaba, queda raro otherwise
			{
				app->itemManager->LoadStoreItems(x, y, app->itemManager->nodeList[i]);
			}
			x++;
		}
	}

	for (int i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if ((app->itemManager->nodeList[i]->toSell && app->itemManager->nodeList[i]->type == 3 && app->itemManager->nodeList[i]->quantity > 0) || (app->itemManager->nodeList[i]->toSell && app->itemManager->nodeList[i]->type == 1))
		{
			if (posYstoreAnimation == 50)
			{
				app->itemManager->ItemToSell(app->itemManager->nodeList[i]);
			}
		}
	}

	string c = to_string(CurrentPrice);
	buyButton->text = c.c_str();
	if (app->itemManager->coins < CurrentPrice && buyButton->buttonType == ButtonType::BUYITEM) { buyButton->color = { 230, 33, 33 }; }
	else if (buyButton->buttonType == ButtonType::SELLITEM) { buyButton->color = { 100, 250, 90 }; }
	else { buyButton->color = { 0, 0, 0 }; }

	if (storeTransition_B)
	{
		storeAnimation.Backward();	
	}
	else
	{
		storeAnimation.Foward();
	}
	storeAnimation.Step(1, false);

	float point = storeAnimation.GetPoint();
	int offset = -750;

	string h = to_string(SellQuantity);
	app->render->TextDraw(h.c_str(), 250, offset + point * (550 - offset), 40, Font::TEXT, { 255, 246, 240 });

	app->render->DrawTexture(app->itemManager->coinTexture, 1150 - app->render->camera.x, offset + point * (165 - offset) - app->render->camera.y);

	string cn = to_string(app->itemManager->coins);
	app->render->TextDraw(cn.c_str(), 1090, offset + point * (165 - offset), 25, Font::TEXT, { 255, 246, 240 });


	if ( app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_B) == ButtonState::BUTTON_DOWN)//con mando es bastante molesto tener que ir con el mouse al boton de cerrar
	{
		storeTransition_B = true;
		buyButton->isForward_B = false;
		closeStore->isForward_B = false;
		Add->isForward_B = false;
		Minus->isForward_B = false;
		app->scene->player->lockMovement = false;
	}

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

	CurrentPrice = 0;
	SellQuantity = 0;

	app->tex->UnLoad(inventory);
	app->tex->UnLoad(potion);

	return true;
}

bool Store::OnGuiMouseHoverEvent(GuiControl* control)
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->button->id == control->id)
		{
			app->itemManager->nodeList[i]->printStats = true;
		}
	}

	return true;
}

bool Store::OnGuiMouseOutHoverEvent(GuiControl* control)
{

	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->button->id == control->id)
		{
			app->itemManager->nodeList[i]->printStats = false;
		}
	}

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
		//this->Disable();
		storeTransition_B = true;
		buyButton->isForward_B = false;
		closeStore->isForward_B = false;
		Add->isForward_B = false;
		Minus->isForward_B = false;
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

		app->audio->PlayFx(buyfx);

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
					if (app->itemManager->nodeList[i]->quantity > SellQuantity)
					{
						SellQuantity++;
						CurrentPrice = app->itemManager->nodeList[i]->price * SellQuantity;
					}
				}
			}
		}
		app->audio->PlayFx(buyfx);
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
		app->audio->PlayFx(buyfx);
		break;
	}

	return true;
}