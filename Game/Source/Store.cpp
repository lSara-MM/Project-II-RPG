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
	buttonBounds = { (300), 407, 40, 30 };
	(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1500, this, buttonBounds, ButtonType::SMALL);

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

	bool ret = true;

	int x = 0;

	if (app->combat->active)
	{
		y = 460;
	}
	else
	{
		app->render->DrawTexture(inventoryIMG, 0 - app->render->camera.x, 0 - app->render->camera.y);

		y = 148;
	}

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
			app->render->DrawTexture(playerTexture, 370 - app->render->camera.x, 180 - app->render->camera.y);
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
			app->render->TextDraw((app->itemManager->arrParty.at(app->itemManager->invPos)->name.GetString()), x, 100, 30, Font::TEXT, { 0, 0, 0 });

			//print stats
			string h = to_string(app->itemManager->maxhp);
			app->render->TextDraw(h.c_str(), 330, 465, 15, Font::TEXT, { 0, 0, 0 });
			string at = to_string(app->itemManager->attack);
			app->render->TextDraw(at.c_str(), 330, 490, 15, Font::TEXT, { 0, 0, 0 });
			string cP = to_string(app->itemManager->critProbability);
			app->render->TextDraw(cP.c_str(), 330, 515, 15, Font::TEXT, { 0, 0, 0 });
			string cD = to_string(app->itemManager->critDamage);
			app->render->TextDraw(cD.c_str(), 330, 540, 15, Font::TEXT, { 0, 0, 0 });
			string p = to_string(app->itemManager->accuracy);
			app->render->TextDraw(p.c_str(), 330, 565, 15, Font::TEXT, { 0, 0, 0 });
			string ar = to_string(app->itemManager->armor);
			app->render->TextDraw(ar.c_str(), 520, 490, 15, Font::TEXT, { 0, 0, 0 });
			string e = to_string(app->itemManager->esquiva);
			app->render->TextDraw(e.c_str(), 520, 515, 15, Font::TEXT, { 0, 0, 0 });
			string r = to_string(app->itemManager->resistencia);
			app->render->TextDraw(r.c_str(), 520, 540, 15, Font::TEXT, { 0, 0, 0 });
			string s = to_string(app->itemManager->speed);
			app->render->TextDraw(s.c_str(), 520, 565, 15, Font::TEXT, { 0, 0, 0 });
		}
	}

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

bool Store::OnGuiMouseClickEvent(GuiControl* control)
{

	return true;
}