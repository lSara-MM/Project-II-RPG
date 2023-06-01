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

	if (app->combat->active)
	{
		for (int i = 0; i <= 3; i++)
		{
			SDL_Rect buttonBounds;
			buttonBounds = { 0, 0, 0, 0 };
			selectCharacter[i] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1000 + i, this, buttonBounds, ButtonType::SMALL);
		}
	}
	else
	{
		for (int i = 0; i <= 3; i++)
		{
			SDL_Rect buttonBounds;
			buttonBounds = { (362 + 34 * i), 407, 25, 25 };
			selectCharacter[i] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1000 + i, this, buttonBounds, ButtonType::SMALL);
		}
	}

	//animation inventory
	inventoryAnimation.Set();
	inventoryAnimation.AddTween(100, 120, BACK_OUT);
	inventoryTransition_B = false;

	return true;
}


bool Inventory::PreUpdate()
{
	return true;
}

bool Inventory::Update(float dt)
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
	int offset = -1300;

	if (app->combat->active)
	{
		y = 460;
	}
	else
	{
		app->render->DrawTexture(inventoryIMG, offset + point * (0 - offset) - app->render->camera.x, 0 - app->render->camera.y);

		y = 148;
	}

	bool ret = true;

	return ret;
}

bool Inventory::PostUpdate()
{
	int x = 0;

	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (cap == x)
		{
			y += 42;
			x = 0;
		}
		app->itemManager->LoadQuantity(x, y, app->itemManager->armorItems[i]);
		if (app->itemManager->armorItems[i]->equiped == false)
		{
			x++;
		}
	}
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->type != 2)
		{
			if (cap == x)
			{
				y += 42;
				x = 0;
			}

			app->itemManager->LoadQuantity(x, y, app->itemManager->nodeList[i]);

			if (app->itemManager->nodeList[i]->equiped == false)
			{
				if (app->itemManager->nodeList[i]->quantity > 0)
				{
					x++;
				}
			}
		}
	}

	if (app->combat->active)
	{
	}
	else
	{
		//Print Character
		if (app->itemManager->arrParty.at(app->itemManager->invPos) != nullptr)
		{
			SDL_Texture* playerTexture = app->tex->Load(app->itemManager->arrParty.at(app->itemManager->invPos)->texturePath);
			app->render->DrawTexture(playerTexture, offset + point * (370 - offset) - app->render->camera.x, 180 - app->render->camera.y);
			app->tex->UnLoad(playerTexture);
			playerTexture = NULL;
		}

		string cn = to_string(app->itemManager->coins);
		app->render->TextDraw(cn.c_str(), 980, 110, 20, Font::TEXT, { 0, 0, 0 });

		//LOAD STATS
		if (app->itemManager->arrParty.at(app->itemManager->invPos) != nullptr)
		{
			app->itemManager->maxhp = app->itemManager->arrParty.at(app->itemManager->invPos)->maxHp;
			app->itemManager->armor = app->itemManager->arrParty.at(app->itemManager->invPos)->armor;
			app->itemManager->attack = app->itemManager->arrParty.at(app->itemManager->invPos)->attack;
			app->itemManager->critDamage = app->itemManager->arrParty.at(app->itemManager->invPos)->critDamage;
			app->itemManager->critProbability = app->itemManager->arrParty.at(app->itemManager->invPos)->critRate;
			app->itemManager->accuracy = app->itemManager->arrParty.at(app->itemManager->invPos)->accuracy;
			app->itemManager->esquiva = app->itemManager->arrParty.at(app->itemManager->invPos)->dodge;
			app->itemManager->speed = app->itemManager->arrParty.at(app->itemManager->invPos)->speed;
			app->itemManager->resistencia = app->itemManager->arrParty.at(app->itemManager->invPos)->res;

			//Print Name
			int offsetX = app->itemManager->arrParty.at(app->itemManager->invPos)->name.Length() * 30 / 2;

			int x = (860 - offsetX) / 2;
			app->render->TextDraw((app->itemManager->arrParty.at(app->itemManager->invPos)->name.GetString()), offset + point * (x - offset), 100, 30, Font::TEXT, { 0, 0, 0 });

			//print stats
			string h = to_string(app->itemManager->maxhp);
			app->render->TextDraw(h.c_str(), offset + point * (330 - offset), 465, 15, Font::TEXT, { 0, 0, 0 });
			string at = to_string(app->itemManager->attack);
			app->render->TextDraw(at.c_str(), offset + point * (330 - offset), 490, 15, Font::TEXT, { 0, 0, 0 });
			string cP = to_string(app->itemManager->critProbability);
			app->render->TextDraw(cP.c_str(), offset + point * (330 - offset), 515, 15, Font::TEXT, { 0, 0, 0 });
			string cD = to_string(app->itemManager->critDamage);
			app->render->TextDraw(cD.c_str(), offset + point * (330 - offset), 540, 15, Font::TEXT, { 0, 0, 0 });
			string p = to_string(app->itemManager->accuracy);
			app->render->TextDraw(p.c_str(), offset + point * (330 - offset), 565, 15, Font::TEXT, { 0, 0, 0 });
			string ar = to_string(app->itemManager->armor);
			app->render->TextDraw(ar.c_str(), offset + point * (520 - offset), 490, 15, Font::TEXT, { 0, 0, 0 });
			string e = to_string(app->itemManager->esquiva);
			app->render->TextDraw(e.c_str(), offset + point * (520 - offset), 515, 15, Font::TEXT, { 0, 0, 0 });
			string r = to_string(app->itemManager->resistencia);
			app->render->TextDraw(r.c_str(), offset + point * (520 - offset), 540, 15, Font::TEXT, { 0, 0, 0 });
			string s = to_string(app->itemManager->speed);
			app->render->TextDraw(s.c_str(), offset + point * (520 - offset), 565, 15, Font::TEXT, { 0, 0, 0 });
		}
	}

	return true;
}

// Called before quitting
bool Inventory::CleanUp()
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->quantity > 0)
		{
			app->itemManager->nodeList[i]->CleanUp();
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->button != nullptr && app->itemManager->armorItems[i]->quantity > 0)
		{
			app->itemManager->armorItems[i]->CleanUp();
		}
	}

	for (int i = 0; i <= 3; i++)
	{
		app->guiManager->DestroyGuiControl(selectCharacter[i]);
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
			if (app->itemManager->nodeList[i]->type == 3 && app->itemManager->nodeList[i]->type == 1) break;

			app->itemManager->MinusQuantity(app->itemManager->nodeList[i]);
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->ID == control->id)
		{
			app->itemManager->armorItems[i]->whom = app->itemManager->invPos;

			for (size_t j = 0; j < app->itemManager->armorItems.size(); j++)
			{
				if (app->itemManager->armorItems[j]->type == app->itemManager->armorItems[i]->type)
				{
					if (app->itemManager->armorItems[j]->kind == app->itemManager->armorItems[i]->kind)
					{
						if (app->itemManager->armorItems[j]->equiped)
						{
							if (j != i)
							{
								if (app->itemManager->armorItems[j]->kind == 5 || app->itemManager->armorItems[j]->kind == 6)
								{
									if (app->itemManager->armorItems[i]->whom == app->itemManager->armorItems[j]->whom)
									{
										app->itemManager->armorItems[i]->space = 2;
										for (size_t p = 0; p < app->itemManager->armorItems.size(); p++)
										{
											if (app->itemManager->armorItems[i]->whom == app->itemManager->armorItems[p]->whom && app->itemManager->armorItems[j]->whom == app->itemManager->armorItems[p]->whom)
											{
												if (app->itemManager->armorItems[p]->space > 0)
												{
													if (app->itemManager->armorItems[j]->kind == app->itemManager->armorItems[p]->kind)
													{
														if (p != j && p != i)
														{
															if (app->itemManager->armorItems[j]->space == 1)
															{
																app->itemManager->MinusQuantity(app->itemManager->armorItems[j]);
																app->itemManager->armorItems[i]->space = 1;
																app->itemManager->MinusQuantity(app->itemManager->armorItems[i]);
																break;
															}
															else
															{
																app->itemManager->MinusQuantity(app->itemManager->armorItems[p]);
																app->itemManager->armorItems[i]->space = 1;
																app->itemManager->MinusQuantity(app->itemManager->armorItems[i]);
																break;
															}
														}
													}
												}
											}
										}
									}
								}
								else
								{
									//Si es armadura
									if (app->itemManager->armorItems[i]->whom == app->itemManager->armorItems[j]->whom)
									{
										app->itemManager->MinusQuantity(app->itemManager->armorItems[j]);
									}
								}
							}
						}
					}
				}
			}
			if ((app->itemManager->armorItems[i]->kind == 5 || app->itemManager->armorItems[i]->kind == 6) && app->itemManager->armorItems[i]->space == 0)
			{
				app->itemManager->armorItems[i]->space = 1;
			}
			app->itemManager->MinusQuantity(app->itemManager->armorItems[i]);
		}
	}
	switch (control->id)
	{
		case 1000:
			if (app->itemManager->arrParty.at(0) != nullptr)
			{
				app->itemManager->invPos = 0;
			}
			break;
		case 1001:
			if (app->itemManager->arrParty.at(1) != nullptr)
			{
				app->itemManager->invPos = 1;
			}
			break;
		case 1002:
			if (app->itemManager->arrParty.at(2) != nullptr)
			{
				app->itemManager->invPos = 2;
			}
			break;
		case 1003:
			if (app->itemManager->arrParty.at(3) != nullptr)
			{
				app->itemManager->invPos = 3;
			}
			break;
	}

	return true;
}