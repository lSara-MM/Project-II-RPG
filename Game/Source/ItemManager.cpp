#include "ItemManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "IntroScene.h"

#include "EntityManager.h"


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
	textureInventoryPath = config.attribute("inventorystorepath").as_string();
	texturePotionsPath = config.attribute("potionstorepath").as_string();

	itemPath = config.attribute("itempath").as_string();

	return ret;
}

bool ItemManager::Start()
{
	// Load playable characters and party
	PartyToNull();
	LoadAllPC();

	if (!app->input->coso && !app->iScene->previousGame_B)
	{		
		SetParty();
	}
	else
	{
		LoadParty();
	}

	LoadItems();
	
	itemsTexture = app->tex->Load(itemPath.GetString());

	return true;
}

bool ItemManager::Update(float dt)
{
	return true;
}

bool ItemManager::CleanUp()
{
	SaveItemState();

	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->quantity > 0 && nodeList[i]->button != nullptr)
		{
			nodeList[i]->CleanUp();
		}
	}
	for (size_t i = 0; i < armorItems.size(); i++)
	{
		if (armorItems[i]->quantity > 0 && armorItems[i]->button != nullptr)
		{
			armorItems[i]->CleanUp();
		}
	}

	nodeList.clear();
	armorItems.clear();

	//ClearParty();
	//for (int i = 0; i < vecPC.size(); i++)
	//{
	//	//delete vecPC.at(i);
	//	vecPC.at(i) = nullptr;
	//}


	vecPC.clear();
	vecPC.shrink_to_fit();

	app->tex->UnLoad(itemsTexture);
	itemsTexture = NULL;

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

void ItemManager::AddQuantity(int id, int quantity)
{
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->ID == id)
		{
			for (int q = 0; q < quantity; q++)
			{
				if (nodeList[i]->max > nodeList[i]->quantity)
				{
					nodeList[i]->quantity++;
				}
			}
		}
	}
}

void ItemManager::BuyItem(int ID, int quantity)
{
	AddQuantity(ID, quantity);

	for (int i = 0; i < nodeList.size(); i++)
	{
		if(nodeList[i]->ID == ID)
		{ 
			coins -= nodeList[i]->price* quantity;
		}
	}
}

void ItemManager::SellItem(int ID, int quantity)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->ID == ID)
		{
			for (int q = 0; q < quantity; q++)
			{
				MinusQuantity(nodeList[i]);
			}
			coins += nodeList[i]->price * quantity;
		}
	}
}

void ItemManager::MinusQuantity(ItemNode* item)
{
	if (app->combat->active)
	{
		if (app->combat->listInitiative[app->combat->charaInTurn]->charaType == CharacterType::ALLY)
		{

			if (item->quantity > 0)
			{
				if (item->type == 1)
				{
					item->quantity--;
				}
				UseItem(item);
			}
			
		}
	}
	else
	{
		if (item->quantity > 0)
		{
			if (item->type == 2)
			{
				item->equiped = !item->equiped;
			}
			else if (item->type == 4 || (item->type == 3 && app->store->active))
			{
				item->quantity--;
			}
			UseItem(item);
		}
	}
}

void ItemManager::UseItem(ItemNode* item)
{
	if (app->combat->active)
	{

		if (item->kind != 2)
		{
			app->combat->listInitiative[app->combat->charaInTurn]->ModifyHP(item->hp);
		}
	}
	else
	{
		for (size_t i = 0; i < arrParty.size(); i++)
		{
			if (arrParty.at(i) != nullptr && item->whom == i)
			{
				if (item->type == 2 && item->equiped == true)
				{
					arrParty.at(i)->ModifyHP(item->hp);
					arrParty.at(i)->maxHp += item->maxhp;
					arrParty.at(i)->armor += item->armor;
					arrParty.at(i)->attack += item->attack;
					arrParty.at(i)->critDamage += item->critDamage;
					arrParty.at(i)->critRate += item->critRate;
					arrParty.at(i)->accuracy += item->accuracy;
					arrParty.at(i)->dodge += item->dodge;
					arrParty.at(i)->speed += item->speed;
					arrParty.at(i)->res += item->res;
				}
				else if (item->type == 2 && item->equiped == false)
				{
					arrParty.at(i)->maxHp -= item->maxhp;
					arrParty.at(i)->armor -= item->armor;
					arrParty.at(i)->attack -= item->attack;
					arrParty.at(i)->critDamage -= item->critDamage;
					arrParty.at(i)->critRate -= item->critRate;
					arrParty.at(i)->accuracy -= item->accuracy;
					arrParty.at(i)->dodge -= item->dodge;
					arrParty.at(i)->speed -= item->speed;
					arrParty.at(i)->res -= item->res;

					if (item->space > 0)
					{
						item->space = 0;
					}
				}

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

		node->ID = pugiNode.attribute("id").as_int();
		node->quantity = pugiNode.attribute("quantity").as_int();
		node->position.x = pugiNode.attribute("x").as_int();
		node->position.y = pugiNode.attribute("y").as_int();
		node->type = pugiNode.attribute("type").as_int();
		node->name = pugiNode.attribute("name").as_string();
		node->kind = pugiNode.attribute("kind").as_int();

		if (node->kind == 5 || node->kind == 6)
		{
			node->space = pugiNode.attribute("space").as_int();
		}

		node->hp = pugiNode.attribute("hp").as_int();
		node->maxhp = pugiNode.attribute("maxhp").as_int();
		node->attack = pugiNode.attribute("attack").as_int();
		node->critRate = pugiNode.attribute("critRate").as_int();
		node->critDamage = pugiNode.attribute("critDamage").as_int();
		node->accuracy = pugiNode.attribute("accuracy").as_int();
		node->armor = pugiNode.attribute("armor").as_int();
		node->dodge = pugiNode.attribute("dodge").as_int();
		node->res = pugiNode.attribute("res").as_int();
		node->speed = pugiNode.attribute("speed").as_int();
		node->equiped = pugiNode.attribute("equiped").as_bool();
	
		node->max = pugiNode.attribute("max").as_int();

		nodeList.push_back(node);
	}

	LoadArmorItmes();
	LoadItemState(xml_trees);
}

void ItemManager::LoadArmorItmes()
{
	armorItems.clear();

	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->type == 2)
		{
			for(int j=0; j<nodeList[i]->quantity; j++)
			{ 
				ItemNode* node = new ItemNode;
				node->ID = nodeList[i]->ID;
				node->quantity = nodeList[i]->quantity;
				node->position.x = nodeList[i]->position.x;
				node->position.y = nodeList[i]->position.y;
				node->type = nodeList[i]->type;
				node->name = nodeList[i]->name;
				node->kind = nodeList[i]->kind;

				if (node->kind == 5 || node->kind == 6)
				{
					node->space = nodeList[i]->space;
				}

				node->hp = nodeList[i]->hp;
				node->maxhp = nodeList[i]->maxhp;
				node->attack = nodeList[i]->attack;
				node->critRate = nodeList[i]->critRate;
				node->critDamage = nodeList[i]->critDamage;
				node->accuracy = nodeList[i]->accuracy;
				node->armor = nodeList[i]->armor;
				node->dodge = nodeList[i]->dodge;
				node->res = nodeList[i]->res;
				node->speed = nodeList[i]->speed;
				node->equiped = nodeList[i]->equiped;

				node->max = nodeList[i]->max;

				armorItems.push_back(node);
			}
		}
	}
	for (int i = 0; i < armorItems.size(); i++)
	{
		armorItems[i]->ID = 200 + i;
	}
}

void ItemManager::LoadQuantity(int x, int y, ItemNode* item)
{
	SDL_Rect seccion = { 64 * item->position.x, 64 * item->position.y, 64, 64 };

	if (item->quantity > 0)
	{
		if (app->combat->active)
		{

			if (item->type == 1)
			{
				LoadButtons(x, y, item);

				app->render->DrawTexture(itemsTexture, (720 + 42 * x) - app->render->camera.x, y - app->render->camera.y, &seccion);

				string c = to_string(item->quantity);
				app->render->TextDraw(c.c_str(), (720 + 42 * x), y + 16, 20, Font::TEXT, { 0, 0, 0 });
			}
		}
		else
		{
			if (item->equiped && item->whom != invPos)
			{
				item->CleanUp();
			}

			LoadButtons(x, y, item);

			if (item->equiped == false)
			{
				app->render->DrawTexture(itemsTexture, (680 + 42 * x) - app->render->camera.x, y - app->render->camera.y, &seccion);

				if (item->type != 2)
				{
					string c = to_string(item->quantity);
					app->render->TextDraw(c.c_str(), (680 + 42 * x), y + 16, 20, Font::TEXT, { 0, 0, 0 });
				}
			}
			else if (item->equiped && item->whom == invPos)
			{
				switch (item->kind)
				{
				case 1:
					app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 295 - app->render->camera.y, &seccion);
					break;
				case 2:
					app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 245 - app->render->camera.y, &seccion);
					break;
				case 3:
					app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 345 - app->render->camera.y, &seccion);
					break;
				case 4:
					app->render->DrawTexture(itemsTexture, (218 + 52) - app->render->camera.x, 195 - app->render->camera.y, &seccion);
					break;
				case 5:
					if (item->space == 1)
					{
						app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 185 - app->render->camera.y, &seccion);
					}
					else
					{
						app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 235 - app->render->camera.y, &seccion);
					}
					break;
				case 6:
					if (item->space == 1)
					{
						app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 300 - app->render->camera.y, &seccion);
					}
					else
					{
						app->render->DrawTexture(itemsTexture, (510 + 52) - app->render->camera.x, 348 - app->render->camera.y, &seccion);
					}
				}
			}
		}
	}
}

void ItemManager::LoadButtons(int x, int y, ItemNode* item)
{
	SDL_Rect buttonBounds;
	buttonBounds = { (680 + 42 * x), y, 40, 40 };

	if (app->combat->active)
	{
		buttonBounds = { (720 + 42 * x), y, 40, 40 };

		if (item->button != nullptr)
		{}else
		{ 
			item->Start();
		}
	}
	else
	{
		if (item->button != nullptr || (item->equiped && item->whom != invPos))
		{
		}
		else
		{
			item->Start();
		}

		if (item->equiped)
		{
			switch (item->kind)
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
				if (item->space == 1)
				{
					buttonBounds = { (510 + 52), 185, 32, 32 };
				}
				else
				{
					buttonBounds = { (510 + 52), 235, 32, 32 };
				}
				break;
			case 6:
				if (item->space == 1)
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
	}

	if (item->button != nullptr)
	{
		item->button->bounds = buttonBounds;
		item->button->id = item->ID;
	}
}

bool ItemManager::SaveItemState()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_state");

	pugi::xml_node items = node.append_child("items");
	pugi::xml_node item;
	pugi::xml_node armor;
	pugi::xml_node coins = items.append_child("coins");

	coins.append_attribute("coin") = coins;

	// save items
	for (int i = 0; i < nodeList.size(); i++)
	{
		item = items.append_child("item");
		item.append_attribute("name") = nodeList[i]->name.GetString();
		item.append_attribute("quantity") = nodeList[i]->quantity;
		item.append_attribute("equiped") = nodeList[i]->equiped;
		item.append_attribute("space") = nodeList[i]->space;
		item.append_attribute("whom") = nodeList[i]->whom;
	}

	// save items
	for (int i = 0; i < armorItems.size(); i++)
	{
		armor = items.append_child("armor");
		armor.append_attribute("name") = armorItems[i]->name.GetString();
		armor.append_attribute("quantity") = armorItems[i]->quantity;
		armor.append_attribute("equiped") = armorItems[i]->equiped;
		armor.append_attribute("space") = armorItems[i]->space;
		armor.append_attribute("whom") = armorItems[i]->whom;
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

	//Load items
	for (pugi::xml_node pugiNode = items.first_child().first_child().first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("armor"))
	{
		for (size_t i = 0; i < armorItems.size(); i++)
		{
			if (strcmp(pugiNode.attribute("name").as_string(), armorItems[i]->name.GetString()) == 0)
			{
				armorItems[i]->quantity = pugiNode.attribute("quantity").as_int();
				armorItems[i]->equiped = pugiNode.attribute("equiped").as_bool();
				armorItems[i]->space = pugiNode.attribute("space").as_bool();
				armorItems[i]->whom = pugiNode.attribute("whom").as_bool();
				if (armorItems[i]->equiped)
				{
					UseItem(nodeList[i]);
				}
			}
		}
	}

	pugi::xml_node pugiNode = items.first_child().first_child().child("coins");

	coins = pugiNode.attribute("coin").as_int();

	return ret;
}


// Party
void ItemManager::AddCharaToParty(SString chara)
{
	for (int i = 0; i < vecPC.size(); i++)
	{
		if (strcmp(vecPC.at(i)->name.GetString(), chara.GetString()) == 0)
		{
			for (int i = 0; i < arrParty.size(); i++)
			{
				if (arrParty.at(i) == nullptr)
				{
					arrParty.at(i) = vecPC.at(i);
					arrParty.at(i)->positionCombat_I = i;
					break;
				}
			}
		}
	}
}

void ItemManager::AddCharaToParty(int id)
{
	for (int i = 0; i < vecPC.size(); i++)
	{
		if (vecPC.at(i)->id == id)
		{
			for (int i = 0; i < arrParty.size(); i++)
			{
				if (arrParty.at(i) == nullptr)
				{
					arrParty.at(i) = vecPC.at(i);
					arrParty.at(i)->positionCombat_I = i;
					break;
				}
			}
		}
	}
}

void ItemManager::LoadAllPC()
{
	for (pugi::xml_node itemNode = app->entityManager->entityNode.child("CombatCharacter"); itemNode; itemNode = itemNode.next_sibling("CombatCharacter"))
	{
		Character* chara = (Character*)app->entityManager->CreateEntity(EntityType::MENU_CHARA);
		chara->parameters = itemNode;

		chara->Awake();
		chara->isCombatant = false;

		chara->Start();

		chara->charaType = CharacterType::ALLY;
		vecPC.push_back(chara);
		chara = nullptr;
	}
}

bool ItemManager::LoadParty()
{
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		return false;
	}
	else
	{
		pugi::xml_node& data = gameStateFile.child("save_state").child(app->entityManager->name.GetString());
		
		for (pugi::xml_attribute attr = data.child("party").attribute("id"); attr; attr = attr.next_attribute())
		{
			int id = data.child("party").attribute("id").as_int();
			app->itemManager->AddCharaToParty(id);
		}
	}

	return true;
}

void ItemManager::SetParty()
{
	// TO DO when party available
	for (int i = 0; i < vecPC.size(); i++)
	{
		// TO DO: change commented per uncommented
		if (i == arrParty.size() - 1) break;
		//if (i == arrParty.size()) break;
		arrParty.at(i) = vecPC.at(i);
		arrParty.at(i)->positionCombat_I = i;
	}
}

void ItemManager::PartyToNull()
{
	for (int i = 0; i < arrParty.size(); i++)
	{
		if (arrParty.at(i) != nullptr)
		{
			arrParty.at(i) = nullptr;
		}
		else
		{
			break;
		}
	}
}