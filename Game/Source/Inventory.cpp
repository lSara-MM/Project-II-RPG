#include "Inventory.h"
#include "Input.h"
#include "app.h"
#include "Log.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "ItemManager.h"


Inventory::Inventory(unsigned cap) : Module()
{
	name.Create("Inventory");
	this->cap = cap;
	y = 0;
}

// Destructor
Inventory::~Inventory()
{

}

bool Inventory::Start()
{
	inventoryIMG = app->tex->Load(app->itemManager->texturePath);

	return true;
}


bool Inventory::PreUpdate()
{
	return true;
}

bool Inventory::Update(float dt)
{
	bool ret = true;

	app->render->DrawTexture(inventoryIMG, 225 - app->render->camera.x, 60 - app->render->camera.y);

	y = 132;
	int x = 0;
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (cap == x)
		{
			y += 52;
			x = 0;
		}
		app->itemManager->LoadQuantity(x, y, i);
		if (app->itemManager->nodeList[i]->equiped == false)
		{
			if (app->itemManager->nodeList[i]->quantity > 0)
			{
				x++;
			}
		}
	}
	
	return ret;
}

// Called before quitting
bool Inventory::CleanUp()
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != NULL)
		{
			app->itemManager->nodeList[i]->CleanUp();
		}
	}

	app->tex->UnLoad(inventoryIMG);

	return true;
}

bool Inventory::OnGuiMouseClickEvent(GuiControl* control)
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->ID == control->id)
		{
			if (app->itemManager->nodeList[i]->type == 1 && app->itemManager->nodeList[i]->kind == 2) break;
			for (size_t j = 0; j < app->itemManager->nodeList.size(); j++)
			{
				if (app->itemManager->nodeList[j]->type == app->itemManager->nodeList[i]->type)
				{
					if (app->itemManager->nodeList[j]->kind == app->itemManager->nodeList[i]->kind)
					{
						if (app->itemManager->nodeList[j]->equiped)
						{
							if (j!=i)
							{
								if (app->itemManager->nodeList[j]->kind == 5 || app->itemManager->nodeList[j]->kind == 6)
								{
									app->itemManager->nodeList[i]->space = 2;
									for (size_t p = 0; p < app->itemManager->nodeList.size(); p++)
									{
										if (app->itemManager->nodeList[p]->space > 0)
										{
											if (app->itemManager->nodeList[j]->kind == app->itemManager->nodeList[p]->kind)
											{
												if (p != j && p != i)
												{
													if (app->itemManager->nodeList[j]->space == 1)
													{
														app->itemManager->MinusQuantity(app->itemManager->nodeList[j]->name.GetString());
														app->itemManager->nodeList[i]->space = 1;
														app->itemManager->MinusQuantity(app->itemManager->nodeList[i]->name.GetString());
														break;
													}
													else
													{
														app->itemManager->MinusQuantity(app->itemManager->nodeList[p]->name.GetString());
														app->itemManager->nodeList[i]->space = 1;
														app->itemManager->MinusQuantity(app->itemManager->nodeList[i]->name.GetString());
														break;
													}
												}
											}
										}
									}
								}
								else
								{
									app->itemManager->MinusQuantity(app->itemManager->nodeList[j]->name.GetString());
								}
							}
						}
					}
				}
			}
			if ((app->itemManager->nodeList[i]->kind == 5 || app->itemManager->nodeList[i]->kind == 6) && app->itemManager->nodeList[i]->space == 0)
			{
				app->itemManager->nodeList[i]->space = 1;
			}
			app->itemManager->MinusQuantity(app->itemManager->nodeList[i]->name.GetString());
		}
	}

	return true;
}