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
	SDL_Rect buttonBounds;
	buttonBounds = { 1150, 50, 60, 60 };
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1501, this, buttonBounds, ButtonType::CLOSE, "", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	buttonBounds = { 240, 540, 200, 80 };
	forgeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1502, this, buttonBounds, ButtonType::SMALL, "", 12, Font::UI, { 0, 0, 0, 0 }, 2, Easings::BOUNCE_OUT, AnimationAxis::DOWN_Y);

	forgeTexture = app->tex->Load(app->itemManager->forgePath);
	forgeInventoryTexture = app->tex->Load(app->itemManager->textureInventoryPath);

	//animation inventory
	forgeAnimation.Set();
	forgeAnimation.AddTween(100, 80, BOUNCE_OUT);
	forgeTransition_B = false;
	posYforgeAnimation = 0;

	return true;
}


bool Forge::PreUpdate()
{
	return true;
}

bool Forge::Update(float dt)
{
	if (forgeTransition_B)
	{
		forgeAnimation.Backward();
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)//poner invisibles los botones creados
		{
			if (app->itemManager->nodeList[i]->button != nullptr)
			{
				app->itemManager->nodeList[i]->button->state = GuiControlState::NONE;
			}
		}
		if (posYforgeAnimation == -750)
		{
			this->Disable();
		}
	}
	else
	{
		forgeAnimation.Foward();
	}
	forgeAnimation.Step(1, false);

	float point = forgeAnimation.GetPoint();
	int offset = -750;
	posYforgeAnimation = offset + point * (50 - offset);
	app->render->DrawTexture(forgeTexture, 80 - app->render->camera.x, posYforgeAnimation - app->render->camera.y);
	app->render->DrawTexture(forgeInventoryTexture, 700 - app->render->camera.x, offset + point * (80 - offset) - app->render->camera.y);
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
			if (posYforgeAnimation == 50)
			{
				app->itemManager->LoadForgeItems(x, y, app->itemManager->nodeList[i]);
			}
			x++;
		}
	}
	for (int i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (x >= 5)
		{
			y += 69;
			x = 0;
		}
		if (posYforgeAnimation == 50)
		{
			app->itemManager->LoadForgeItems(x, y, app->itemManager->armorItems[i]);
		}
		x++;
	}

	for (int i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->craft)
		{
			for (int j = 0; j < app->itemManager->nodeList.size(); j++)
			{
				if (app->itemManager->nodeList[j]->craft && i != j)
				{
					if (IsForge(app->itemManager->nodeList[j]->ID, app->itemManager->nodeList[i]->ID, false))
					{
						app->itemManager->LoadCraftItems(app->itemManager->nodeList[j]->ID, app->itemManager->nodeList[i]->ID, false);
					}
				}
			}
			for (int j = 0; j < app->itemManager->armorItems.size(); j++)
			{
				if (app->itemManager->armorItems[j]->craft)
				{
					if (IsForge(app->itemManager->nodeList[i]->ID, app->itemManager->armorItems[j]->ID, true))
					{
						IsArmor = true;
						app->itemManager->LoadCraftItems(app->itemManager->nodeList[i]->ID, app->itemManager->armorItems[j]->ID, true);
					}
				}
			}
		}
		else
		{
			if (toDelete)
			{
				for (int j = 0; j < app->itemManager->nodeList.size(); j++)
				{
					if (app->itemManager->nodeList[j]->canCraft)
					{
						app->itemManager->nodeList[j]->canCraft = false;
						app->itemManager->nodeList[j]->CleanUp();
					}
					if (app->itemManager->nodeList[j]->forge)
					{
						app->itemManager->nodeList[j]->canCraft = false;
						app->itemManager->nodeList[j]->forge = false;
						app->itemManager->nodeList[j]->CleanUp();
					}
				}
				toDelete = false;
			}
		}
	}

	//raton
	app->input->RenderMouse();

	return true;
}

// Called before quitting
bool Forge::CleanUp()
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		app->itemManager->nodeList[i]->craft = false;
		app->itemManager->nodeList[i]->CleanUp();
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		app->itemManager->armorItems[i]->craft = false;
		app->itemManager->armorItems[i]->CleanUp();
	}

	app->guiManager->DestroyGuiControl(exitButton);
	app->guiManager->DestroyGuiControl(forgeButton);

	app->tex->UnLoad(forgeInventoryTexture);
	app->tex->UnLoad(forgeTexture);

	forgePos = 0;

	return true;
}

bool Forge::IsForge(int ID0, int ID1, bool armor)
{
	bool ret=false;

	const char* file = "forge.xml";
	crafts.load_file(file);

	if (armor)
	{
		switch (ID0)
		{
		case 101:
			ret = true;
			break;
		case 102:
			ret = true;
			break;
		case 103:
			ret = true;
			break;
		case 104:
			ret = true;
			break;
		case 105:
			ret = true;
			break;
		case 106:
			ret = true;
			break;
		case 107:
			ret = true;
			break;
		case 108:
			ret = true;
			break;
		}
	}
	else
	{
		//Load items
		for (pugi::xml_node pugiNode = crafts.first_child().first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("craft"))
		{
			for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
			{
				if (ID0 == pugiNode.attribute("item1").as_int() && ID1 == pugiNode.attribute("item2").as_int() || ID0 == pugiNode.attribute("item2").as_int() && ID1 == pugiNode.attribute("item1").as_int())
				{
					ret = true;
				}
			}
		}
	}
	//pugiNode.attribute("name").as_string()
	return ret;
}

bool Forge::OnGuiMouseHoverEvent(GuiControl* control)
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->button->id == control->id)
		{
			if (app->itemManager->nodeList[i]->craft == false)
			{
				app->itemManager->nodeList[i]->printStats = true;
			}
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->button != nullptr && app->itemManager->armorItems[i]->button->id == control->id)
		{
			if (app->itemManager->armorItems[i]->craft == false)
			{
				app->itemManager->armorItems[i]->printStats = true;
			}
		}
	}

	return true;
}

bool Forge::OnGuiMouseOutHoverEvent(GuiControl* control)
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->button != nullptr && app->itemManager->nodeList[i]->button->id == control->id)
		{
			app->itemManager->nodeList[i]->printStats = false;
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->button != nullptr && app->itemManager->armorItems[i]->button->id == control->id)
		{
			app->itemManager->armorItems[i]->printStats = false;
		}
	}

	return true;
}

bool Forge::OnGuiMouseClickEvent(GuiControl* control)
{
	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->ID == control->id && app->itemManager->nodeList[i]->type != 2)
		{
			app->itemManager->nodeList[i]->craft = true;

			if (app->itemManager->nodeList[i]->craft && forgePos == 2 && app->itemManager->nodeList[i]->forgePos == 0)
			{
				app->itemManager->nodeList[i]->craft = false;
				break;
			}

			if (app->itemManager->nodeList[i]->craft && forgePos != 0 && app->itemManager->nodeList[i]->forgePos != 0)
			{
				if (forgePos == 1 && app->itemManager->nodeList[i]->forgePos == 1)
				{
					app->itemManager->nodeList[i]->forgePos = 0;
					forgePos = 0;
					app->itemManager->nodeList[i]->craft = false;
					break;
				}
				else if (forgePos == 2 && app->itemManager->nodeList[i]->forgePos == 2)
				{
					app->itemManager->nodeList[i]->forgePos = 0;
					forgePos = 1;
					app->itemManager->nodeList[i]->craft = false;
					toDelete = true;
					break;
				}
			}

			if (app->itemManager->nodeList[i]->craft && forgePos == 0)
			{
				app->itemManager->nodeList[i]->forgePos = 1;
				forgePos = 1;
				break;
			}
			else if (app->itemManager->nodeList[i]->craft && forgePos == 1)
			{
				app->itemManager->nodeList[i]->forgePos = 2;
				forgePos = 2;
				break;
			}
		}
	}
	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->ID == control->id)
		{
			app->itemManager->armorItems[i]->craft = true;

			if (app->itemManager->armorItems[i]->craft && forgePos == 2 && app->itemManager->armorItems[i]->forgePos == 0)
			{
				app->itemManager->armorItems[i]->craft = false;
				break;
			}

			if (app->itemManager->armorItems[i]->craft && forgePos != 0 && app->itemManager->armorItems[i]->forgePos != 0)
			{
				if (forgePos == 1 && app->itemManager->armorItems[i]->forgePos == 1)
				{
					app->itemManager->armorItems[i]->forgePos = 0;
					forgePos = 0;
					app->itemManager->armorItems[i]->craft = false;
					break;
				}
				else if (forgePos == 2 && app->itemManager->armorItems[i]->forgePos == 2)
				{
					app->itemManager->armorItems[i]->forgePos = 0;
					forgePos = 1;
					app->itemManager->armorItems[i]->craft = false;
					toDelete = true;
					break;
				}
			}

			if (app->itemManager->armorItems[i]->craft && forgePos == 0)
			{
				app->itemManager->armorItems[i]->forgePos = 1;
				forgePos = 1;
				break;
			}
			else if (app->itemManager->armorItems[i]->craft && forgePos == 1)
			{
				app->itemManager->armorItems[i]->forgePos = 2;
				forgePos = 2;
				break;
			}

		}
	}

	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->canCraft && app->itemManager->nodeList[i]->ID == control->id)
		{
			app->itemManager->nodeList[i]->forge = !app->itemManager->nodeList[i]->forge; 
			break;
		}
	}

	switch (control->id)
	{

	case 1501:
		this->Disable();
		break;
	case 1502:
		for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
		{
			if (app->itemManager->nodeList[i]->forge)
			{
				app->itemManager->nodeList[i]->forge = false;
				app->itemManager->nodeList[i]->canCraft = false;
				if (IsArmor == false)
				{
					app->itemManager->AddQuantity(i, 1);
				}
				else
				{
					IsArmor = false;
				}
				app->itemManager->nodeList[i]->CleanUp();
				for (size_t j = 0; j < app->itemManager->nodeList.size(); j++)
				{
					if (app->itemManager->nodeList[j]->craft)
					{
						app->itemManager->nodeList[j]->craft = false;
						app->itemManager->MinusQuantity(app->itemManager->nodeList[j]);
						app->itemManager->nodeList[j]->CleanUp();
					}

				}
				for (size_t j = 0; j < app->itemManager->armorItems.size(); j++)
				{
					if (app->itemManager->armorItems[j]->craft)
					{
						app->itemManager->armorItems[j]->craft = false;
						app->itemManager->MinusQuantity(app->itemManager->armorItems[j]);
						app->itemManager->nodeList[j]->CleanUp();
					}
				}
			}
		}
		forgePos = 0;
		break;
	}
	return true;
}