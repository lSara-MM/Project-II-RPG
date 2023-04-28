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
	textBox_tex = app->tex->Load(textBox_path);
	hasEnded = false;
	return true;
}

bool ItemManager::Update(float dt)
{
	if (activeTree != nullptr)
	{
		//Text box
		iPoint pos = { 0, (app->win->GetHeight() - 245) };
		app->render->DrawTexture(textBox_tex, pos.x - app->render->camera.x, pos.y - app->render->camera.y);

		activeTree->UpdateTree(dt, app->dialogueSystem, pos);
		app->guiManager->Draw();
	}

	return true;
}

bool ItemManager::CleanUp()
{
	nodeList.clear()
	app->tex->UnLoad(textBox_tex);

	return true;
}

int ItemManager::LoadItems()
{
	const char* file = "items.xml";
	pugi::xml_parse_result result = items.load_file(file);

	ItemNode* tree = new ItemNode;

	if (result == NULL)
	{
		LOG("Could not load items xml file %s. pugi error: %s", file, result.description());
		return -1;
	}
	else
	{
		pugi::xml_node pugiNode = items.first_child().first_child();

		for (pugiNode != NULL)
		{
			tree->activeNode = LoadNodes(pugiNode, tree);
			pugiNode = pugiNode.next_sibling("item");
		}
	}

	return 1;
}

ItemNode* ItemManager::LoadNodes(pugi::xml_node& xml_trees, ItemNode* item)
{
	ItemNode* first_node = new ItemNode;

	for (pugi::xml_node pugiNode = xml_trees.child("node"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("node"))
	{
		ItemNode* node = new ItemNode;

		node->quantity = pugiNode.attribute("type").as_int();
		node->type = pugiNode.attribute("type").as_int();
		node->name = pugiNode.attribute("name").as_string();
		node->type = pugiNode.attribute("type").as_int();
		if (node->type == 1) { node->kind = pugiNode.attribute("kind").as_int(); }
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
		}
		textBox_path = pugiNode.attribute("texturepath").as_string();

		item->nodeList.push_back(node);

	}
	LoadQuantity(xml_trees, item);

	return first_node;
}

ItemNode* ItemManager::LoadQuantity(pugi::xml_node& xml_trees, ItemNode* item)
{
	ItemNode* first_node = new ItemNode;

	for (pugi::xml_node pugiNode = xml_trees.child("node"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("node"))
	{
		ItemNode* node = new ItemNode;

		node->quantity = pugiNode.attribute("type").as_int();

		for (int n=0; n<=node->quantity; n++)
		{
			node->type = pugiNode.attribute("type").as_int();
			node->name = pugiNode.attribute("name").as_string();
			node->type = pugiNode.attribute("type").as_int();
			if (node->type == 1) { node->kind = pugiNode.attribute("kind").as_int(); }
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
			}
			textBox_path = pugiNode.attribute("texturepath").as_string();

			item->itemCount.push_back(node);
		}
	}

	return first_node;
}

bool ItemManager::SaveItemState()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_state");

	// save items
	if (activeTree != nullptr)
	{
		for (size_t i = 0; i < activeTree->nodeList.size(); i++)
		{
			for (int j = 0; j < activeTree->nodeList[i]->choicesList.size(); j++)
			{
					player = node.append_child("item");
					player.append_attribute("quantity") = activeTree->nodeList[i]->quantity;
					break;
			}
		}
	}

	ret = saveDoc->save_file("save_items.xml");

	return ret;
}