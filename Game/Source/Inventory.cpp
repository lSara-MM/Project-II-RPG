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

	return true;
}


bool Inventory::PreUpdate()
{
	return true;
}

bool Inventory::Update(float dt)
{
	bool ret = true;

	y = 200;
	int x = 0;
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		for (int n = 0; n < app->itemManager->nodeList[i]->quantity; n++)
		{
			if (cap == x)
			{
				y += 52;
				x = 0;
			}
		}
		app->itemManager->LoadQuantity(x, y);
		x++;
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
	return true;
}

bool Inventory::OnGuiMouseClickEvent(GuiControl* control)
{
	int j = 0;
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->ID == control->id)
		{
			app->itemManager->MinusQuantity(app->itemManager->nodeList[i]->name.GetString());
		}
	}

	return true;
}