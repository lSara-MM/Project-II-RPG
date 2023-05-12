#include "ItemManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"


ItemManager::ItemManager() : Module()
{
	name.Create("items");
}

ItemManager::~ItemManager()
{
}

bool ItemManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Items");
	bool ret = true;

	return ret;
}

bool ItemManager::Start()
{
	LoadItems();
	return true;
}

bool ItemManager::Update(float dt)
{
	//Inventory
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (app->inventory->active)
		{
			app->inventory->Disable();
		}
		else {
			app->inventory->Enable();
		}
	}

	//Add items HardCode
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		pugi::xml_node pugiNode = items.first_child();
		AddQuantity(pugiNode, "potion");
	}
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		pugi::xml_node pugiNode = items.first_child();
		AddQuantity(pugiNode, "granade");
	}
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		pugi::xml_node pugiNode = items.first_child();
		AddQuantity(pugiNode, "legging");
	}
	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		pugi::xml_node pugiNode = items.first_child();
		AddQuantity(pugiNode, "ring");
	}
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		pugi::xml_node pugiNode = items.first_child();
		AddQuantity(pugiNode, "chest");
	}
	if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		pugi::xml_node pugiNode = items.first_child();
		AddQuantity(pugiNode, "pendant");
	}

	return true;
}

bool ItemManager::CleanUp()
{
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->quantity > 0)
		{
			nodeList[i]->CleanUp();
		}
	}

	nodeList.clear();

	return true;
}

int ItemManager::LoadItems()
{
	const char* file = "items.xml";
	pugi::xml_parse_result result = items.load_file(file);

	if (result == NULL)
	{
		LOG("Could not load items xml file %s. pugi error: %s", file, result.description());
		return -1;
	}
	else
	{
		pugi::xml_node pugiNode = items.first_child();

		while (pugiNode != NULL)
		{
			LoadNodes(pugiNode, tree);
			pugiNode = pugiNode.next_sibling("items");
		}
	}

	return 1;
}

void ItemManager::AddQuantity(pugi::xml_node& xml_trees, const char* name)
{
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->name == name)
		{
			if (nodeList[i]->max > nodeList[i]->quantity)
			{
				nodeList[i]->quantity++;
			}
			SaveItemState();
		}
	}
}

void ItemManager::MinusQuantity(const char* name)
{
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->name == name && nodeList[i]->quantity > 0)
		{
			if (nodeList[i]->type != 2)
			{
				nodeList[i]->quantity--;
				SaveItemState();
			}
			else
			{
				nodeList[i]->equiped = !nodeList[i]->equiped;
			}
			UseItem(nodeList[i]);	
		}
	}
}

void ItemManager::UseItem(ItemNode* item)
{
	for (size_t i = 0; i < app->combat->vecAllies.size(); i++)
	{
		if (item->type==2 && item->equiped==true)
		{
			app->combat->vecAllies[i]->currentHp += item->hp;
			app->combat->vecAllies[i]->maxHp += item->maxhp;
			app->combat->vecAllies[i]->armor += item->armor;
			app->combat->vecAllies[i]->attack += item->attack;
			app->combat->vecAllies[i]->critDamage += item->critDamage;
			app->combat->vecAllies[i]->critRate += item->critProbability;
			app->combat->vecAllies[i]->precision += item->precision;
			app->combat->vecAllies[i]->dodge += item->esquiva;
			app->combat->vecAllies[i]->speed += item->speed;
			app->combat->vecAllies[i]->res += item->resistencia;
		}
		else if (item->type == 2 && item->equiped == false)
		{
			app->combat->vecAllies[i]->currentHp -= item->hp;
			app->combat->vecAllies[i]->maxHp -= item->maxhp;
			app->combat->vecAllies[i]->armor -= item->armor;
			app->combat->vecAllies[i]->attack -= item->attack;
			app->combat->vecAllies[i]->critDamage -= item->critDamage;
			app->combat->vecAllies[i]->critRate -= item->critProbability;
			app->combat->vecAllies[i]->precision -= item->precision;
			app->combat->vecAllies[i]->dodge -= item->esquiva;
			app->combat->vecAllies[i]->speed -= item->speed;
			app->combat->vecAllies[i]->res -= item->resistencia;
		}

		if (item->type == 1)
		{
			if (item->kind == 1)
			{
				app->combat->vecAllies[i]->currentHp += item->hp;
			}
		}
	}

	if (item->quantity <= 0)
	{
		item->CleanUp();
	}
}

void ItemManager::LoadNodes(pugi::xml_node& xml_trees, ItemNode* item)
{

	for (pugi::xml_node pugiNode = xml_trees.child("item"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("item"))
	{
		ItemNode* node = new ItemNode;

		node->quantity = pugiNode.attribute("quantity").as_int();
		node->type = pugiNode.attribute("type").as_int();
		node->name = pugiNode.attribute("name").as_string();
		node->kind = pugiNode.attribute("kind").as_int();
		node->hp = pugiNode.attribute("hp").as_int();
		if (node->type == 2)
		{
			node->attack = pugiNode.attribute("attack").as_int();
			node->critProbability = pugiNode.attribute("critProbability").as_int();
			node->critDamage = pugiNode.attribute("critDamage").as_int();
			node->precision = pugiNode.attribute("precision").as_int();
			node->armor = pugiNode.attribute("armor").as_int();
			node->esquiva = pugiNode.attribute("esquiva").as_int();
			node->resistencia = pugiNode.attribute("resistencia").as_int();
			node->speed = pugiNode.attribute("speed").as_int();
			node->equiped = pugiNode.attribute("equiped").as_bool();
		}
		node->path = pugiNode.attribute("texturepath").as_string();
		node->max = pugiNode.attribute("max").as_int();

		nodeList.push_back(node);
	}
	LoadItemState(xml_trees);

}

void ItemManager::LoadQuantity(int x, int y)
{
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->quantity > 0)
		{
			LoadButtons(x, y, i);

			nodeList[i]->ID = i;

			if (!nodeList[i]->equiped)
			{
				itemsTexture = app->tex->Load(nodeList[i]->path.GetString());
				app->render->DrawTexture(itemsTexture, (1000 + 52 * i), y);

				string c = to_string(nodeList[i]->quantity);
				app->render->TextDraw(c.c_str(), (840 + 52 * i), y + 26, 20, Font::TEXT, { 0, 0, 0 });
			}
			else if (nodeList[i]->equiped)
			{
				switch (nodeList[i]->kind)
				{
				case 1:
					itemsTexture = app->tex->Load(nodeList[i]->path.GetString());
					app->render->DrawTexture(itemsTexture, (1000 + 52 * i), y);
					break;
				case 2:
					itemsTexture = app->tex->Load(nodeList[i]->path.GetString());
					app->render->DrawTexture(itemsTexture, (1000 + 52 * i), y);
					break;
				case 3:
					itemsTexture = app->tex->Load(nodeList[i]->path.GetString());
					app->render->DrawTexture(itemsTexture, (1000 + 52 * i), y);
					break;
				case 4:
					itemsTexture = app->tex->Load(nodeList[i]->path.GetString());
					app->render->DrawTexture(itemsTexture, (1000 + 52 * i), y);
					break;
				case 5:
					itemsTexture = app->tex->Load(nodeList[i]->path.GetString());
					app->render->DrawTexture(itemsTexture, (1000 + 52 * i), y);
					break;
				}
			}

			app->tex->UnLoad(itemsTexture);
			itemsTexture = NULL;
		}
	}
}

void ItemManager::LoadButtons(int x, int y, int ID)
{
	SDL_Rect buttonBounds;
	if (nodeList[ID]->button == NULL)
	{
		int x_ = (800 + 52 * ID);
		buttonBounds = { x_, y, 52, 52 };
		nodeList[ID]->button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, ID, app->inventory, buttonBounds, ButtonType::SMALL);
	}
}

bool ItemManager::SaveItemState()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_state");

	pugi::xml_node items = node.append_child("items");
	pugi::xml_node item;

	// save items
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		item = items.append_child("item");
		item.append_attribute("name") = nodeList[i]->name.GetString();
		item.append_attribute("quantity") = nodeList[i]->quantity;
		item.append_attribute("equiped") = nodeList[i]->equiped;
	}

	ret = saveDoc->save_file("save_items.xml");

	return ret;
}

bool ItemManager::LoadItemState(pugi::xml_node& xml_trees)
{
	bool ret = true;

	const char* file = "save_items.xml";
	items.load_file(file);
	
	// load items
	for (pugi::xml_node pugiNode = items.first_child().first_child().first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("item"))
	{
		for (size_t i = 0; i < nodeList.size(); i++)
		{
			if (strcmp(pugiNode.attribute("name").as_string(), nodeList[i]->name.GetString()) == 0)
			{
				nodeList[i]->quantity = pugiNode.attribute("quantity").as_int();
				nodeList[i]->equiped = pugiNode.attribute("equipped").as_bool();
			}
		}
	}

	return ret;
}
