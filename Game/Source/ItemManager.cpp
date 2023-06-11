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
	loadParty_B = false;
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
	coinPath = config.attribute("coinpath").as_string();
	smallcoinPath = config.attribute("smallcoinpath").as_string();
	partyPath = config.attribute("partypath").as_string();
	selectedPath = config.attribute("selectedpath").as_string();
	charLockedPath = config.attribute("charLockedpath").as_string();
	protaPath = config.attribute("protapath").as_string();
	compaPath = config.attribute("compapath").as_string();
	twinsPath = config.attribute("twinspath").as_string();
	firePath = config.attribute("firepath").as_string();
	forgePath = config.attribute("forgepath").as_string();


	itemPath = config.attribute("itempath").as_string();

	fxpotypath = "Assets/Audio/Fx/heal.wav";
	potyfx = app->audio->LoadFx(fxpotypath);

	fxequippath = "Assets/Audio/Fx/equip.wav";
	equipfx = app->audio->LoadFx(fxequippath);

	fxunequippath = "Assets/Audio/Fx/unequip.wav";
	unequipfx = app->audio->LoadFx(fxunequippath);

	return ret;
}

bool ItemManager::Start()
{
	// Load playable characters and party
	PartyToNull();
	LoadAllPC();

	if (!loadParty_B)
	{		
		SetParty();
	}
	else
	{
		LoadParty();
	}

	LoadItems();
	
	itemsTexture = app->tex->Load(itemPath.GetString());
	coinTexture = app->tex->Load(coinPath);
	SmallcoinTexture = app->tex->Load(smallcoinPath);

	return true;
}

bool ItemManager::Update(float dt)
{
	return true;
}

bool ItemManager::PostUpdate()
{
	int x, y = 0;

	for (size_t i = 0; i < app->itemManager->nodeList.size(); i++)
	{
		if (app->itemManager->nodeList[i]->printStats && app->itemManager->nodeList[i]->toSell == false)
		{
			if (app->combat->active)
			{
				x = (720 + 70 * app->itemManager->nodeList[i]->x) + 70;
				y = app->itemManager->nodeList[i]->y - 32;
			}
			else if (app->combat->active == false)
			{
				if (app->store->active || app->forge->active)
				{
					if (app->itemManager->nodeList[i]->type == 1)
					{
						x = 200 + (64 * app->itemManager->nodeList[i]->x) + 70;
						y = 200 + app->itemManager->nodeList[i]->y - 32;
					}
					else if (app->itemManager->nodeList[i]->toSell)
					{

					}
					else
					{
						x = 800 + (64 * app->itemManager->nodeList[i]->x) + 70;
						y = 200 + app->itemManager->nodeList[i]->y - 32;
					}
				}
				else
				{
					x = (680 + 70 * app->itemManager->nodeList[i]->x) + 70;
					y = app->itemManager->nodeList[i]->y - 32;
				}
			}

			if (app->itemManager->nodeList[i]->canCraft)
			{
				x = (170 + 80 * app->itemManager->nodeList[i]->x) + 70;
				y = 300 - 32;
			}

			int w = app->itemManager->nodeList[i]->name.Length() * 3 + 80;
			app->render->DrawRectangle({x - app->render->camera.x, y - app->render->camera.y, w, 150}, 0, 0, 0, 180);

			//print stats
			app->render->TextDraw(app->itemManager->nodeList[i]->name.GetString(), x + 5, y + 5, 12, Font::TEXT, { 255, 246, 240 });
			string h = to_string(app->itemManager->nodeList[i]->hp);
			app->render->TextDraw("HP: ", x + 5, y + 25, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(h.c_str(), x + 60, y + 25, 10, Font::TEXT, { 255, 246, 240 });
			string mh = to_string(app->itemManager->nodeList[i]->maxhp);
			app->render->TextDraw("MAXHP: ", x + 5, y + 37, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(mh.c_str(), x + 60, y + 37, 10, Font::TEXT, { 255, 246, 240 });
			string at = to_string(app->itemManager->nodeList[i]->attack);
			app->render->TextDraw("ATTK: ", x + 5, y + 49, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(at.c_str(), x + 60, y + 49, 10, Font::TEXT, { 255, 246, 240 });
			string cP = to_string(app->itemManager->nodeList[i]->critRate);
			app->render->TextDraw("CRIT_RATE: ", x + 5, y + 61, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(cP.c_str(), x + 70, y + 61, 10, Font::TEXT, { 255, 246, 240 });
			string cD = to_string(app->itemManager->nodeList[i]->critDamage);
			app->render->TextDraw("CRIT_DMG: ", x + 5, y + 73, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(cD.c_str(), x + 70, y + 73, 10, Font::TEXT, { 255, 246, 240 });
			string p = to_string(app->itemManager->nodeList[i]->accuracy);
			app->render->TextDraw("ACCURACY: ", x + 5, y + 85, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(p.c_str(), x + 70, y + 85, 10, Font::TEXT, { 255, 246, 240 });
			string ar = to_string(app->itemManager->nodeList[i]->armor);
			app->render->TextDraw("ARMOR: ", x + 5, y + 97, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(ar.c_str(), x + 60, y + 97, 10, Font::TEXT, { 255, 246, 240 });
			string e = to_string(app->itemManager->nodeList[i]->dodge);
			app->render->TextDraw("DODGE: ", x + 5, y + 109, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(e.c_str(), x + 60, y + 109, 10, Font::TEXT, { 255, 246, 240 });
			string r = to_string(app->itemManager->nodeList[i]->res);
			app->render->TextDraw("RES: ", x + 5, y + 121, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(r.c_str(), x + 60, y + 121, 10, Font::TEXT, { 255, 246, 240 });
			string s = to_string(app->itemManager->nodeList[i]->speed);
			app->render->TextDraw("SPEED: ", x + 5, y + 132, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(s.c_str(), x + 60, y + 132, 10, Font::TEXT, { 255, 246, 240 });
		}
	}

	for (size_t i = 0; i < app->itemManager->armorItems.size(); i++)
	{
		if (app->itemManager->armorItems[i]->printStats && app->itemManager->armorItems[i]->equiped == false)
		{
			if (app->combat->active == false)
			{
				if (app->forge->active)
				{
					x = 800 + (64 * app->itemManager->armorItems[i]->x) + 70;
					y = 200 + app->itemManager->armorItems[i]->y - 32;
				}
				else
				{
					x = (680 + 70 * app->itemManager->armorItems[i]->x) + 70;
					y = app->itemManager->armorItems[i]->y - 32;
				}
			}

			int w = app->itemManager->armorItems[i]->name.GetCapacity() * 3 + 80;
			app->render->DrawRectangle({ x - app->render->camera.x, y - app->render->camera.y,  w, 150 }, 0, 0, 0, 180);

			//print stats
			app->render->TextDraw(app->itemManager->armorItems[i]->name.GetString(), x + 5, y + 5, 12, Font::TEXT, { 255, 246, 240 });
			string h = to_string(app->itemManager->armorItems[i]->maxhp);
			app->render->TextDraw("MAXHP: ", x + 5, y + 25, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(h.c_str(), x + 60, y + 25, 10, Font::TEXT, { 255, 246, 240 });
			string at = to_string(app->itemManager->armorItems[i]->attack);
			app->render->TextDraw("ATTK: ", x + 5, y + 37, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(at.c_str(), x + 60, y + 37, 10, Font::TEXT, { 255, 246, 240 });
			string cP = to_string(app->itemManager->armorItems[i]->critRate);
			app->render->TextDraw("CRIT_RATE: ", x + 5, y + 49, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(cP.c_str(), x + 70, y + 49, 10, Font::TEXT, { 255, 246, 240 });
			string cD = to_string(app->itemManager->armorItems[i]->critDamage);
			app->render->TextDraw("CRIT_DMG: ", x + 5, y + 61, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(cD.c_str(), x + 70, y + 61, 10, Font::TEXT, { 255, 246, 240 });
			string p = to_string(app->itemManager->armorItems[i]->accuracy);
			app->render->TextDraw("ACCURACY: ", x + 5, y + 73, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(p.c_str(), x + 70, y + 73, 10, Font::TEXT, { 255, 246, 240 });
			string ar = to_string(app->itemManager->armorItems[i]->armor);
			app->render->TextDraw("ARMOR: ", x + 5, y + 85, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(ar.c_str(), x + 60, y + 85, 10, Font::TEXT, { 255, 246, 240 });
			string e = to_string(app->itemManager->armorItems[i]->dodge);
			app->render->TextDraw("DODGE: ", x + 5, y + 97, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(e.c_str(), x + 60, y + 97, 10, Font::TEXT, { 255, 246, 240 });
			string r = to_string(app->itemManager->armorItems[i]->res);
			app->render->TextDraw("RES: ", x + 5, y + 109, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(r.c_str(), x + 60, y + 109, 10, Font::TEXT, { 255, 246, 240 });
			string s = to_string(app->itemManager->armorItems[i]->speed);
			app->render->TextDraw("SPEED: ", x + 5, y + 121, 10, Font::TEXT, { 255, 246, 240 });
			app->render->TextDraw(s.c_str(), x + 60, y + 121, 10, Font::TEXT, { 255, 246, 240 });
		}
	}

	return true;
}

bool ItemManager::CleanUp()
{
	if (!app->combat->active) { app->SaveToFile(); }

	SaveItemState();
	app->combat->SaveCombat();

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

	// party
	PartyToNull();

	for (int i = 0; i < vecPC.size(); i++)
	{
		if (vecPC.at(i) != nullptr)
		{
			app->entityManager->DestroyEntity(vecPC.at(i));
			vecPC.at(i) = nullptr;
		}
	}
	vecPC.clear();
	vecPC.shrink_to_fit();

	app->tex->UnLoad(itemsTexture);
	itemsTexture = NULL;	
	
	app->tex->UnLoad(coinTexture);
	coinTexture = NULL;	
	
	app->tex->UnLoad(SmallcoinTexture);
	SmallcoinTexture = NULL;

	page = 0;

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
	LoadArmorItmes();
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
		if (app->forge->active)
		{
			if (item->quantity > 0)
			{
				if (item->type == 2)
				{
					ArmorForge(item);
				}
				else if (item->type == 3)
				{
					item->quantity--;
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
}

void ItemManager::UseItem(ItemNode* item)
{
	StatusEffect* statusEffect;

	if (app->combat->active)
	{
		switch (item->ID)
		{
		case 85:
			app->combat->listInitiative[app->combat->charaInTurn]->ModifyHP(app->combat->listInitiative[app->combat->charaInTurn]->maxHp / item->hp);
			app->audio->PlayFx(potyfx);
			break;
		case 86:
			app->combat->listInitiative[app->combat->charaInTurn]->ModifyHP(app->combat->listInitiative[app->combat->charaInTurn]->maxHp / item->hp);
			app->audio->PlayFx(potyfx);
			break;
		case 87:
			statusEffect = new StatusEffect(item->armor, 3, true, EffectType::ARMOR);
			app->combat->listInitiative[app->combat->charaInTurn]->listStatusEffects.Add(statusEffect);
			app->audio->PlayFx(potyfx);
			break;
		case 88:
			statusEffect = new StatusEffect(item->attack, 3, true, EffectType::ATTACK);
			app->combat->listInitiative[app->combat->charaInTurn]->listStatusEffects.Add(statusEffect);
			app->audio->PlayFx(potyfx);
			break;
		case 89:
			statusEffect = new StatusEffect(item->critRate, 2, true, EffectType::CRIT_RATE);
			app->combat->listInitiative[app->combat->charaInTurn]->listStatusEffects.Add(statusEffect);
			app->audio->PlayFx(potyfx);
			break;
		case 90:
			statusEffect = new StatusEffect(item->critDamage, 2, true, EffectType::CRIT_DMG);
			app->combat->listInitiative[app->combat->charaInTurn]->listStatusEffects.Add(statusEffect);
			app->audio->PlayFx(potyfx);
			break;
		case 91:
			statusEffect = new StatusEffect(item->accuracy, 2, true, EffectType::ACCURACY);
			app->combat->listInitiative[app->combat->charaInTurn]->listStatusEffects.Add(statusEffect);
			app->audio->PlayFx(potyfx);
			break;
		case 92:
			statusEffect = new StatusEffect(item->dodge, 3, true, EffectType::DODGE);
			app->combat->listInitiative[app->combat->charaInTurn]->listStatusEffects.Add(statusEffect);
			app->audio->PlayFx(potyfx);
			break;
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

					app->audio->PlayFx(equipfx);
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
					app->audio->PlayFx(unequipfx);
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

	app->inventory->ReOrderInventory();

	if (item->quantity <= 0)
	{
		item->CleanUp();
	}
}

void ItemManager::UseItemPostBattle()
{
	for (int j = 0; j < armorItems.size(); j++)
	{
		for (int i = 0; i < arrParty.size(); i++)
		{
			if (arrParty.at(i) != nullptr && armorItems[j]->whom == i)
			{
				if (armorItems[j]->equiped == true)
				{
					arrParty.at(i)->maxHp -= armorItems[j]->maxhp;
					arrParty.at(i)->armor -= armorItems[j]->armor;
					arrParty.at(i)->attack -= armorItems[j]->attack;
					arrParty.at(i)->critDamage -= armorItems[j]->critDamage;
					arrParty.at(i)->critRate -= armorItems[j]->critRate;
					arrParty.at(i)->accuracy -= armorItems[j]->accuracy;
					arrParty.at(i)->dodge -= armorItems[j]->dodge;
					arrParty.at(i)->speed -= armorItems[j]->speed;
					arrParty.at(i)->res -= armorItems[j]->res;
				}
			}
		}
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
		node->maxhp = pugiNode.attribute("maxHp").as_int();
		node->attack = pugiNode.attribute("attack").as_int();
		node->critRate = pugiNode.attribute("critRate").as_int();
		node->critDamage = pugiNode.attribute("critDamage").as_int();
		node->accuracy = pugiNode.attribute("accuracy").as_int();
		node->armor = pugiNode.attribute("armor").as_int();
		node->dodge = pugiNode.attribute("dodge").as_int();
		node->res = pugiNode.attribute("res").as_int();
		node->speed = pugiNode.attribute("speed").as_int();
		node->equiped = pugiNode.attribute("equiped").as_bool();
		node->price = pugiNode.attribute("price").as_int();
	
		node->max = pugiNode.attribute("max").as_int();

		nodeList.push_back(node);
	}

	LoadItemState();
	LoadArmorItmes();
	LoadArmorState();
}

void ItemManager::ArmorForge(ItemNode* item)
{
	int ID = 0;
	for (int i = 0; i < armorItems.size(); i++)
	{
		if (strcmp(armorItems[i]->name.GetString(), item->name.GetString()) == 0)
		{
			ID = i;
		}
	}
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->craft)
		{
			armorItems[ID]->hp += nodeList[i]->hp;
			armorItems[ID]->maxhp += nodeList[i]->maxhp;
			armorItems[ID]->attack += nodeList[i]->attack;
			armorItems[ID]->critRate += nodeList[i]->critRate;
			armorItems[ID]->critDamage += nodeList[i]->critDamage;
			armorItems[ID]->accuracy += nodeList[i]->accuracy;
			armorItems[ID]->armor += nodeList[i]->armor;
			armorItems[ID]->dodge += nodeList[i]->dodge;
			armorItems[ID]->res += nodeList[i]->res;
			armorItems[ID]->speed += nodeList[i]->speed;
		}
	}
}

void ItemManager::LoadArmorItmes()
{
	for (int i = 0; i < armorItems.size(); i++)
	{
		armorItems[i]->CleanUp();
	}
	armorItems.clear();

	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->type == 2)
		{
			for(int j = 0; j < nodeList[i]->quantity; j++)
			{ 
				ItemNode* node = new ItemNode;
				node->ID = nodeList[i]->ID;
				node->quantity = nodeList[i]->quantity;
				node->position.x = nodeList[i]->position.x;
				node->position.y = nodeList[i]->position.y;
				node->type = nodeList[i]->type;
				node->name = nodeList[i]->name;
				node->kind = nodeList[i]->kind;

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

				item->x = x;
				item->y = y;

				app->render->DrawTexture(itemsTexture, (720 + 70 * x) - app->render->camera.x, y - app->render->camera.y, &seccion);

				string c = to_string(item->quantity);
				app->render->TextDraw(c.c_str(), (720 + 80 * x), y + 30, 20, Font::TEXT, { 0, 0, 0 });
			}
		}
		else
		{
			if (item->equiped && item->whom != invPos)
			{
				item->CleanUp();
			}

			LoadButtons(x, y, item);

			item->x = x;
			item->y = y;

			if (item->equiped == false)
			{
				app->render->DrawTexture(itemsTexture, (680 + 70 * x) - app->render->camera.x, y - app->render->camera.y, &seccion);

				if (item->type != 2)
				{
					string c = to_string(item->quantity);
					app->render->TextDraw(c.c_str(), (720 + 70 * x), y + 30, 20, Font::TEXT, { 0, 0, 0 });
				}
			}
			else if (item->equiped && item->whom == invPos)
			{
				switch (item->kind)
				{
				case 1:
					app->render->DrawTexture(itemsTexture, (185 + 70) - app->render->camera.x, 295 - app->render->camera.y, &seccion);
					break;
				case 2:
					app->render->DrawTexture(itemsTexture, (185 + 70) - app->render->camera.x, 220 - app->render->camera.y, &seccion);
					break;
				case 3:
					app->render->DrawTexture(itemsTexture, (185 + 70) - app->render->camera.x, 370 - app->render->camera.y, &seccion);
					break;
				case 4:
					app->render->DrawTexture(itemsTexture, (185 + 70) - app->render->camera.x, 145 - app->render->camera.y, &seccion);
					break;
				case 5:
					if (item->space == 1)
					{
						app->render->DrawTexture(itemsTexture, (465 + 70) - app->render->camera.x, 145 - app->render->camera.y, &seccion);
					}
					else
					{
						app->render->DrawTexture(itemsTexture, (465 + 70) - app->render->camera.x, 220 - app->render->camera.y, &seccion);
					}
					break;
				case 6:
					if (item->space == 1)
					{
						app->render->DrawTexture(itemsTexture, (465 + 70) - app->render->camera.x, 295 - app->render->camera.y, &seccion);
					}
					else
					{
						app->render->DrawTexture(itemsTexture, (465 + 70) - app->render->camera.x, 370 - app->render->camera.y, &seccion);
					}
				}
			}
		}
	}
}
void ItemManager::LoadSellItems(int x, int y, ItemNode* item)
{
	if (item->toSell == false)
	{
		SDL_Rect seccion = { 64 * item->position.x, 64 * item->position.y, 64, 64 };

		LoadStoreButtons(x, y, item);

		item->x = x;
		item->y = y;

		app->render->DrawTexture(itemsTexture, (800 + (69 * x)) - app->render->camera.x, 200 + y - app->render->camera.y, &seccion);
		string c = to_string(item->price);
		app->render->DrawTexture(app->itemManager->SmallcoinTexture, 800 + (69 * x) + 45 - app->render->camera.x, y + 250 - app->render->camera.y);
		app->render->TextDraw(c.c_str(), 800 + (69 * x), y + 240, 20, Font::TEXT, { 0, 0, 0 });
	}

}
void ItemManager::LoadStoreItems(int x, int y, ItemNode* item)
{
	if (item->toSell == false)
	{
		SDL_Rect seccion = { 64 * item->position.x, 64 * item->position.y, 64, 64 };

		LoadStoreButtons(x, y, item);

		item->x = x;
		item->y = y;

		app->render->DrawTexture(itemsTexture, (200 + (69 * x)) - app->render->camera.x, 200 + y - app->render->camera.y, &seccion);
		string c = to_string(item->price);
		app->render->DrawTexture(app->itemManager->SmallcoinTexture, 200 + (69 * x) + 45 - app->render->camera.x, y + 250 - app->render->camera.y);
		app->render->TextDraw(c.c_str(), (200 + (69 * x)), y + 240, 20, Font::TEXT, { 0, 0, 0 });
	}
}

void ItemManager::ItemToSell(ItemNode* item)
{
	SDL_Rect seccion = { 64 * item->position.x, 64 * item->position.y, 64, 64 };

	LoadStoreButtons(0, 350, item);

	app->render->DrawTexture(itemsTexture, 160 - app->render->camera.x, 200 + 350 - app->render->camera.y, &seccion);

	//Print Name
	int offsetX = item->name.GetCapacity() * 30 / 2;

	int x = (720 - offsetX) / 2;
	app->render->TextDraw(item->name.GetString(), x, 500, 30, Font::TEXT, { 255, 246, 240 });
}

void ItemManager::LoadForgeItems(int x, int y, ItemNode* item)
{
	if (item->craft == false)
	{
		SDL_Rect seccion = { 64 * item->position.x, 64 * item->position.y, 64, 64 };

		LoadForgeButtons(x, y, item);

		item->x = x;
		item->y = y;

		app->render->DrawTexture(itemsTexture, (800 + (69 * x)) - app->render->camera.x, 200 + y - app->render->camera.y, &seccion);
	}
	else
	{
		SDL_Rect seccion = { 64 * item->position.x, 64 * item->position.y, 64, 64 };

		LoadForgeButtons(x, y, item);
		
		if (item->forgePos == 1)
		{
			app->render->DrawTexture(itemsTexture, (380) - app->render->camera.x, 220 - app->render->camera.y, &seccion);
		}
		else
		{
			app->render->DrawTexture(itemsTexture, (230) - app->render->camera.x, 220 - app->render->camera.y, &seccion);
		}
	}

}

void ItemManager::LoadCraftItems(int ID0, int ID1, bool armor)
{
	int x = 0;
	
	if (armor)
	{
		int ID = 0;
		for (size_t i = 0; i < armorItems.size(); i++)
		{
			if (armorItems[i]->ID == ID1)
			{
				ID = i;
			}
		}
		for (size_t i = 0; i < nodeList.size(); i++)
		{
			if (strcmp(nodeList[i]->name.GetString(), armorItems[ID]->name.GetString()) == 0)
			{
				SDL_Rect seccion = { 64 * nodeList[i]->position.x, 64 * nodeList[i]->position.y, 64, 64 };

				LoadCraftButtons(x, nodeList[i]);

				nodeList[i]->x = x;

				nodeList[i]->canCraft = true;
				nodeList[i]->craft = true;

				if (nodeList[i]->forge)
				{
					app->render->DrawTexture(itemsTexture, 180 - app->render->camera.x, 400 - app->render->camera.y, &seccion);
					//Print Item Name
					int offsetX = nodeList[i]->name.Length() * 30 / 2;

					int x = (740 - offsetX) / 2;
					app->render->TextDraw(nodeList[i]->name.GetString(), x, 410, 30, Font::TEXT, { 255, 246, 240 });
				}
				else
				{
					app->render->DrawTexture(itemsTexture, (135 + (85 * x)) - app->render->camera.x, 300 - app->render->camera.y, &seccion);
				}
			}
		}
	}
	else
	{
		for (pugi::xml_node pugiNode = app->forge->crafts.first_child().first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("craft"))
		{
			for (size_t i = 0; i < nodeList.size(); i++)
			{
				if (nodeList[i]->quantity > 0)
				{
					if (ID0 == pugiNode.attribute("item1").as_int() && ID1 == pugiNode.attribute("item2").as_int() || ID0 == pugiNode.attribute("item2").as_int() && ID1 == pugiNode.attribute("item1").as_int())
					{
						for (pugi::xml_attribute attribute = pugiNode.first_attribute(); attribute != NULL; attribute = attribute.next_attribute())
						{
							if (strcmp(attribute.name(), "id") == 0)
							{
								SDL_Rect seccion = { 64 * nodeList[attribute.as_int()]->position.x, 64 * nodeList[attribute.as_int()]->position.y, 64, 64 };

								LoadCraftButtons(x, nodeList[attribute.as_int()]);

								nodeList[attribute.as_int()]->x = x;

								nodeList[attribute.as_int()]->canCraft = true;
								nodeList[attribute.as_int()]->craft = true;

								if (nodeList[attribute.as_int()]->forge)
								{
									app->render->DrawTexture(itemsTexture, 180 - app->render->camera.x, 400 - app->render->camera.y, &seccion);
									//Print Item Name
									int offsetX = nodeList[attribute.as_int()]->name.Length() * 30 / 2;

									int x = (740 - offsetX) / 2;
									app->render->TextDraw(nodeList[attribute.as_int()]->name.GetString(), x, 410, 30, Font::TEXT, { 255, 246, 240 });
								}
								else
								{
									app->render->DrawTexture(itemsTexture, (135 + (85 * x)) - app->render->camera.x, 300 - app->render->camera.y, &seccion);
								}
								x++;
							}
						}
						break;
					}
				}
			}
		}
	}
}
void ItemManager::LoadCraftButtons(int x, ItemNode* item)
{
	SDL_Rect buttonBounds;

	if (item->forge == false)
	{
		buttonBounds = { (135 + (85 * x)), 300 , 64, 64 };
	}
	else
	{
		buttonBounds = { 180 , 400 , 64, 64 };
	}

	if (item->button != nullptr)
	{
	}
	else
	{
		item->Start();
	}

	if (item->button != nullptr)
	{
		item->button->bounds = buttonBounds;
		item->button->id = item->ID;
		item->button->observer = app->forge;
	}
}

void ItemManager::LoadForgeButtons(int x, int y, ItemNode* item)
{
	SDL_Rect buttonBounds;

	if (item->craft == false)
	{
		buttonBounds = { (800 + (69 * x)), 200 + y, 64, 64 };
	}
	else
	{
		if (item->forgePos == 1)
		{
			buttonBounds = { (380), 220, 64, 64 };
		}
		else
		{
			buttonBounds = { (230), 220, 64, 64 };

		}
	}

	if (item->button != nullptr)
	{
	}
	else
	{
		item->Start();
	}

	if (item->button != nullptr)
	{
		item->button->bounds = buttonBounds;
		item->button->id = item->ID;
		item->button->observer = app->forge;
	}
}

void ItemManager::LoadStoreButtons(int x, int y, ItemNode* item)
{
	SDL_Rect buttonBounds;
	if (item->type == 1)
	{
		buttonBounds = { (200 + (69 * x)), 200 + y, 64, 64 };
	}
	else
	{
		buttonBounds = { (800 + (69 * x)), 200 + y, 64, 64 };
	}

	if (item->toSell)
	{
		buttonBounds = { 160, 200 + y, 64, 64 };
	}

	if (item->button != nullptr)
	{
	}
	else
	{
		item->Start();
	}

	if (item->button != nullptr)
	{
		item->button->bounds = buttonBounds;
		item->button->id = item->ID;
		item->button->observer = app->store;
	}
}

void ItemManager::LoadButtons(int x, int y, ItemNode* item)
{
	SDL_Rect buttonBounds;
	buttonBounds = { (680 + 71 * x), y, 64, 64 };

	if (app->combat->active)
	{
		buttonBounds = { (720 + 70 * x), y, 64, 64 };

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
				buttonBounds = { (185 + 70), 295, 64, 64 };
				break;
			case 2:
				buttonBounds = { (185 + 70), 220, 64, 64 };
				break;
			case 3:
				buttonBounds = { (185 + 70), 370, 64, 64 };
				break;
			case 4:
				buttonBounds = { (185 + 70), 145, 64, 64 };
				break;
			case 5:
				if (item->space == 1)
				{
					buttonBounds = { (465 + 70), 145, 64, 64 };
				}
				else
				{
					buttonBounds = { (465 + 70), 220, 64, 64 };
				}
				break;
			case 6:
				if (item->space == 1)
				{
					buttonBounds = { (465 + 70), 295, 64, 64 };
				}
				else
				{
					buttonBounds = { (465 + 70), 370, 64, 64 };
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
	pugi::xml_node coins_ = items.append_child("coins");

	coins_.append_attribute("coin") = coins;

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
		armor.append_attribute("id") = armorItems[i]->ID;
		armor.append_attribute("quantity") = armorItems[i]->quantity;
		armor.append_attribute("equiped") = armorItems[i]->equiped;
		armor.append_attribute("space") = armorItems[i]->space;
		armor.append_attribute("whom") = armorItems[i]->whom;

		armor.append_attribute("hp") = armorItems[i]->hp;
		armor.append_attribute("maxHp") = armorItems[i]->maxhp;
		armor.append_attribute("attack") = armorItems[i]->attack;
		armor.append_attribute("critRate") = armorItems[i]->critRate;
		armor.append_attribute("critDamage") = armorItems[i]->critDamage;
		armor.append_attribute("accuracy") = armorItems[i]->accuracy;
		armor.append_attribute("armor") = armorItems[i]->armor;
		armor.append_attribute("dodge") = armorItems[i]->dodge;
		armor.append_attribute("res") = armorItems[i]->res;
		armor.append_attribute("speed") = armorItems[i]->speed;
	}

	ret = saveDoc->save_file("save_items.xml");

	return ret;
}

bool ItemManager::LoadItemState()
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
			}
		}
	}

	pugi::xml_node pugiNode = items.first_child().first_child().child("coins");

	coins = pugiNode.attribute("coin").as_int();

	return ret;
}

bool ItemManager::LoadArmorState()
{
	//Load items
	for (pugi::xml_node pugiNode = items.first_child().first_child().child("armor"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("armor"))
	{
		for (size_t i = 0; i < armorItems.size(); i++)
		{
			if (pugiNode.attribute("id").as_int() == armorItems[i]->ID)
			{
				armorItems[i]->equiped = pugiNode.attribute("equiped").as_bool();
				armorItems[i]->space = pugiNode.attribute("space").as_int();
				armorItems[i]->whom = pugiNode.attribute("whom").as_int();
				armorItems[i]->hp = pugiNode.attribute("hp").as_int();
				armorItems[i]->maxhp = pugiNode.attribute("maxHp").as_int();
				armorItems[i]->attack = pugiNode.attribute("attack").as_int();
				armorItems[i]->critRate = pugiNode.attribute("critRate").as_int();
				armorItems[i]->critDamage = pugiNode.attribute("critDamage").as_int();
				armorItems[i]->accuracy = pugiNode.attribute("accuracy").as_int();
				armorItems[i]->armor = pugiNode.attribute("armor").as_int();
				armorItems[i]->dodge = pugiNode.attribute("dodge").as_int();
				armorItems[i]->res = pugiNode.attribute("res").as_int();
				armorItems[i]->speed = pugiNode.attribute("speed").as_int();

				if (armorItems[i]->equiped)
				{
					UseItem(armorItems[i]);
				}
			}
		}
	}

	return true;
}


// Party
void ItemManager::AddCharaToParty(int id)
{
	for (int i = 0; i < vecPC.size(); i++)
	{
		if (vecPC.at(i)->id == id)
		{
			for (int j = 0; i < arrParty.size(); j++)
			{
				if (arrParty.at(j) == nullptr)
				{
					arrParty.at(j) = vecPC.at(i);
					arrParty.at(j)->positionCombat_I = j;
					break;
				}
			}
		}
	}
}

void ItemManager::ChangeParty(int prevId, int newId)
{
	arrParty.at(prevId)->positionCombat_I = newId;
	arrParty.at(newId)->positionCombat_I = prevId;

	Character* temp = arrParty.at(prevId);
	arrParty.at(prevId) = arrParty.at(newId);
	arrParty.at(newId) = temp;

	temp = nullptr;
}

void ItemManager::LoadAllPC()
{
	for (pugi::xml_node itemNode = app->entityManager->entityNode.child("CombatCharacter"); itemNode; itemNode = itemNode.next_sibling("CombatCharacter"))
	{
		Character* chara = (Character*)app->entityManager->CreateEntity(EntityType::COMBAT_CHARA);
		chara->parameters = itemNode;

		chara->Awake();
		chara->isCombatant = false;
		chara->positionCombat_I = -1;

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

		int i = 0;
		for (pugi::xml_attribute attr = data.child("party").attribute("id"); attr; attr = attr.next_attribute())
		{
			int id = attr.as_int();
			app->itemManager->AddCharaToParty(id);
			partySize = ++i;
		}
	}

	app->combat->LoadCombat();

	return true;
}

void ItemManager::SetParty()
{
	for (int i = 0; i < vecPC.size(); i++)
	{
		// TO DO: change commented per uncommented
		if (i == arrParty.size() - 1) break;
		//if (i == arrParty.size()) break;
		arrParty.at(i) = vecPC.at(i);
		arrParty.at(i)->positionCombat_I = i;
		partySize = i;
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
	}
}
