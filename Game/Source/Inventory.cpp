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

	for (int i=0; i <= 3; i++)
	{
		SDL_Rect buttonBounds;
		buttonBounds = { (362 + 37 * i), 404, 25, 25 };
		selectCharacter[i] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1000 + i, this, buttonBounds, ButtonType::SMALL);
	}

	return true;
}


bool Inventory::PreUpdate()
{
	return true;
}

bool Inventory::Update(float dt)
{
	bool ret = true;

	app->render->DrawTexture(inventoryIMG, 0 - app->render->camera.x, 0 - app->render->camera.y);

	y = 148;
	int x = 0;
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (cap == x)
		{
			y += 42;
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

	//Print Character
	if (app->itemManager->player->arrParty.at(app->itemManager->invPos) != nullptr)
	{
		app->render->DrawTexture(app->itemManager->player->arrParty.at(app->itemManager->invPos)->texture, 370 - app->render->camera.x, 180 - app->render->camera.y);
	}

	//LOAD STATS
	for (size_t i = 0; i < app->itemManager->player->arrParty.size(); i++)
	{
		if (app->itemManager->player->arrParty.at(i) != nullptr && app->itemManager->invPos == i)
		{
			app->itemManager->maxhp = app->itemManager->player->arrParty.at(i)->maxHp;
			app->itemManager->armor = app->itemManager->player->arrParty.at(i)->armor;
			app->itemManager->attack = app->itemManager->player->arrParty.at(i)->attack;
			app->itemManager->critDamage = app->itemManager->player->arrParty.at(i)->critDamage;
			app->itemManager->critProbability = app->itemManager->player->arrParty.at(i)->critRate;
			app->itemManager->precision = app->itemManager->player->arrParty.at(i)->precision;
			app->itemManager->esquiva = app->itemManager->player->arrParty.at(i)->dodge;
			app->itemManager->speed = app->itemManager->player->arrParty.at(i)->speed;
			app->itemManager->resistencia = app->itemManager->player->arrParty.at(i)->res;
		}
	}

	//print stats
	string h = to_string(app->itemManager->maxhp);
	app->render->TextDraw(h.c_str(), 330, 460, 15, Font::TEXT, { 0, 0, 0 });
	string at = to_string(app->itemManager->attack);
	app->render->TextDraw(at.c_str(), 330, 485, 15, Font::TEXT, { 0, 0, 0 });
	string cP = to_string(app->itemManager->critProbability);
	app->render->TextDraw(cP.c_str(), 330, 510, 15, Font::TEXT, { 0, 0, 0 });
	string cD = to_string(app->itemManager->critDamage);
	app->render->TextDraw(cD.c_str(), 330, 535, 15, Font::TEXT, { 0, 0, 0 });
	string p = to_string(app->itemManager->precision);
	app->render->TextDraw(p.c_str(), 330, 560, 15, Font::TEXT, { 0, 0, 0 });
	string ar = to_string(app->itemManager->armor);
	app->render->TextDraw(ar.c_str(), 520, 485, 15, Font::TEXT, { 0, 0, 0 });
	string e = to_string(app->itemManager->esquiva);
	app->render->TextDraw(e.c_str(), 520, 510, 15, Font::TEXT, { 0, 0, 0 });
	string r = to_string(app->itemManager->resistencia);
	app->render->TextDraw(r.c_str(), 520, 535, 15, Font::TEXT, { 0, 0, 0 });
	string s = to_string(app->itemManager->speed);
	app->render->TextDraw(s.c_str(), 520, 560, 15, Font::TEXT, { 0, 0, 0 });

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

	for (int i = 0; i <= 3; i++)
	{
		delete(selectCharacter[i]);
		selectCharacter[i] = nullptr;
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
									if (app->itemManager->nodeList[i]->whom == app->itemManager->nodeList[j]->whom)
									{
										app->itemManager->nodeList[i]->space = 2;
										for (size_t p = 0; p < app->itemManager->nodeList.size(); p++)
										{
											if (app->itemManager->nodeList[i]->whom == app->itemManager->nodeList[p]->whom && app->itemManager->nodeList[j]->whom == app->itemManager->nodeList[p]->whom)
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
									}
								}
								else
								{
									//Si es armadura
									if (app->itemManager->nodeList[i]->whom == app->itemManager->nodeList[j]->whom)
									{
										app->itemManager->MinusQuantity(app->itemManager->nodeList[j]->name.GetString());
									}
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
	switch (control->id)
	{
		case 1000:
			app->itemManager->invPos = 0;
			break;
		case 1001:
			app->itemManager->invPos = 1;
			break;
		case 1002:
			app->itemManager->invPos = 2;
			break;
		case 1003:
			app->itemManager->invPos = 3;
			break;
	}

	return true;
}