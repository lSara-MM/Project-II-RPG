#include "ItemManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"


ItemManager::ItemManager() : Module()
{
	name.Create("itemmanager");
}

ItemManager::~ItemManager()
{
}

bool ItemManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Items");
	bool ret = true;

	texturePath = config.attribute("inventorypath").as_string();

	return ret;
}

bool ItemManager::Start()
{
	player->LoadAllPC();
	player->SetParty();

	LoadItems();

	return true;
}

bool ItemManager::Update(float dt)
{
	//Inventory
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
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
		AddQuantity(pugiNode, "granade");
		AddQuantity(pugiNode, "legging");
		AddQuantity(pugiNode, "ring");
		AddQuantity(pugiNode, "ring_2");
		AddQuantity(pugiNode, "chest");
		AddQuantity(pugiNode, "chest_2");
		AddQuantity(pugiNode, "pendant");
		AddQuantity(pugiNode, "shoe");
		AddQuantity(pugiNode, "hat");
	}
	return true;
}

bool ItemManager::CleanUp()
{
	SaveItemState();

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
	for (size_t i = 0; i < player->arrParty.size(); i++)
	{
		if (player->arrParty.at(i) != nullptr && invPos==i)
		{

			if (item->type == 2 && item->equiped == true)
			{
				player->arrParty.at(i)->currentHp += item->hp;
				player->arrParty.at(i)->maxHp += item->maxhp;
				player->arrParty.at(i)->armor += item->armor;
				player->arrParty.at(i)->attack += item->attack;
				player->arrParty.at(i)->critDamage += item->critDamage;
				player->arrParty.at(i)->critRate += item->critProbability;
				player->arrParty.at(i)->precision += item->precision;
				player->arrParty.at(i)->dodge += item->esquiva;
				player->arrParty.at(i)->speed += item->speed;
				player->arrParty.at(i)->res += item->resistencia;
			}
			else if (item->type == 2 && item->equiped == false)
			{
				player->arrParty.at(i)->currentHp -= item->hp;
				player->arrParty.at(i)->maxHp -= item->maxhp;
				player->arrParty.at(i)->armor -= item->armor;
				player->arrParty.at(i)->attack -= item->attack;
				player->arrParty.at(i)->critDamage -= item->critDamage;
				player->arrParty.at(i)->critRate -= item->critProbability;
				player->arrParty.at(i)->precision -= item->precision;
				player->arrParty.at(i)->dodge -= item->esquiva;
				player->arrParty.at(i)->speed -= item->speed;
				player->arrParty.at(i)->res -= item->resistencia;

				if (item->space > 0)
				{
					item->space = 0;
				}
			}

			if (item->type == 1)
			{
				if (item->kind == 1)
				{
					player->arrParty.at(i)->currentHp += item->hp;
				}
			}

			item->whom = i;
		}
	}

	//Temporal
	if (item->type == 2 && item->equiped == false)
	{
		if (item->space > 0)
		{
			item->space = 0;
		}
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
		if (node->kind == 5 || node->kind == 6)
		{
			node->space = pugiNode.attribute("space").as_int();
		}
		node->hp = pugiNode.attribute("hp").as_int();
		if (node->type == 2)
		{
			node->maxhp = pugiNode.attribute("maxhp").as_int();
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

void ItemManager::LoadQuantity(int x, int y, int i)
{
	if (nodeList[i]->quantity > 0)
	{
		LoadButtons(x, y, i);

		nodeList[i]->ID = i;

		itemsTexture = app->tex->Load(nodeList[i]->path.GetString());

		if (!nodeList[i]->equiped)
		{
			app->render->DrawTexture(itemsTexture, (671 + 42 * x) - app->render->camera.x, y - app->render->camera.y);

			nodeList[i]->whom = invPos;

			string c = to_string(nodeList[i]->quantity);
			app->render->TextDraw(c.c_str(), (671 + 42 * x), y + 16, 20, Font::TEXT, { 0, 0, 0 });
		}
		else if (nodeList[i]->equiped && nodeList[i]->whom == invPos)
		{
			switch (nodeList[i]->kind)
			{
			case 1:
				app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 295 - app->render->camera.y);
				break;
			case 2:
				app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 245 - app->render->camera.y);
				break;
			case 3:
				app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 345 - app->render->camera.y);
				break;
			case 4:
				app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 195 - app->render->camera.y);
				break;
			case 5:
				if (nodeList[i]->space == 1)
				{
					app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 185 - app->render->camera.y);
				}
				else
				{
					app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 235 - app->render->camera.y);
				}
				break;
			case 6:
				if (nodeList[i]->space == 1)
				{
					app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 300 - app->render->camera.y);
				}
				else
				{
					app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 348 - app->render->camera.y);
				}
			}
		}
		else if (nodeList[i]->equiped && nodeList[i]->whom != invPos)
		{
			nodeList[i]->CleanUp();
		}


		//LOAD STATS
		for (size_t i = 0; i < player->arrParty.size(); i++)
		{
			if (player->arrParty.at(i) != nullptr && invPos == i)
			{
				maxhp = player->arrParty.at(i)->maxHp;
				armor = player->arrParty.at(i)->armor;
				attack = player->arrParty.at(i)->attack;
				critDamage = player->arrParty.at(i)->critDamage;
				critProbability = player->arrParty.at(i)->critRate;
				precision = player->arrParty.at(i)->precision;
				esquiva = player->arrParty.at(i)->dodge;
				speed = player->arrParty.at(i)->speed;
				resistencia = player->arrParty.at(i)->res;
			}
		}

		//print stats
		string h = to_string(maxhp);
		app->render->TextDraw(h.c_str(), 330, 460, 15, Font::TEXT, { 0, 0, 0 });
		string at = to_string(attack);
		app->render->TextDraw(at.c_str(), 330, 485, 15, Font::TEXT, { 0, 0, 0 });
		string cP = to_string(critProbability);
		app->render->TextDraw(cP.c_str(), 330, 510, 15, Font::TEXT, { 0, 0, 0 });
		string cD = to_string(critDamage);
		app->render->TextDraw(cD.c_str(), 330, 535, 15, Font::TEXT, { 0, 0, 0 });
		string p = to_string(precision);
		app->render->TextDraw(p.c_str(), 330, 560, 15, Font::TEXT, { 0, 0, 0 });
		string ar = to_string(armor);
		app->render->TextDraw(ar.c_str(), 520, 485, 15, Font::TEXT, { 0, 0, 0 });
		string e = to_string(esquiva);
		app->render->TextDraw(e.c_str(), 520, 510, 15, Font::TEXT, { 0, 0, 0 });
		string r = to_string(resistencia);
		app->render->TextDraw(r.c_str(), 520, 535, 15, Font::TEXT, { 0, 0, 0 });
		string s = to_string(speed);
		app->render->TextDraw(s.c_str(), 520, 560, 15, Font::TEXT, { 0, 0, 0 });

		app->tex->UnLoad(itemsTexture);
		itemsTexture = NULL;
	}
}

void ItemManager::LoadButtons(int x, int y, int ID)
{

	SDL_Rect buttonBounds;
	buttonBounds = { (671 + 42 * x), y, 40, 40 };

	if (nodeList[ID]->button != nullptr || (nodeList[ID]->equiped && nodeList[ID]->whom != invPos))
	{	}
	else
	{
		nodeList[ID]->button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, ID, app->inventory, buttonBounds, ButtonType::SMALL);
	}

	if (nodeList[ID]->equiped)
	{
		switch (nodeList[ID]->kind)
		{
		case 1:
			buttonBounds = { (218 + 52), 295, 32, 32 };
			break;
		case 2:
			buttonBounds = { (218 + 52), 245, 32, 32 };
			break;
		case 3:
			buttonBounds = { (218 + 52), 345, 32, 32 };
			break;
		case 4:
			buttonBounds = { (218 + 52), 195, 32, 32 };
			break;
		case 5:
			if (nodeList[ID]->space == 1)
			{
				buttonBounds = { (510 + 52), 185, 32, 32 };
			}
			else
			{
				buttonBounds = { (510 + 52), 235, 32, 32 };
			}
			break;
		case 6:
			if (nodeList[ID]->space == 1)
			{
				buttonBounds = { (510 + 52), 300, 32, 32 };
			}
			else
			{
				buttonBounds = { (510 + 52), 348, 32, 32 };
			}
			break;
		}
	}

	if (nodeList[ID]->button != NULL)
	{
		nodeList[ID]->button->bounds = buttonBounds;
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
		item.append_attribute("space") = nodeList[i]->space;
		item.append_attribute("whom") = nodeList[i]->whom;
	}

	ret = saveDoc->save_file("save_items.xml");

	return ret;
}

bool ItemManager::LoadItemState(pugi::xml_node& xml_trees)
{
	bool ret = true;

	const char* file = "save_items.xml";
	items.load_file(file);
	
	//Load items
	for (pugi::xml_node pugiNode = items.first_child().first_child().first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("item"))
	{
		for (size_t i = 0; i < nodeList.size(); i++)
		{
			if (strcmp(pugiNode.attribute("name").as_string(), nodeList[i]->name.GetString()) == 0)
			{
				nodeList[i]->quantity = pugiNode.attribute("quantity").as_int();
				nodeList[i]->equiped = pugiNode.attribute("equiped").as_bool();
				nodeList[i]->space = pugiNode.attribute("space").as_bool();
				nodeList[i]->whom = pugiNode.attribute("whom").as_bool();
				if (nodeList[i]->equiped)
				{
					UseItem(nodeList[i]);
				}
			}
		}
	}

	return ret;
}
