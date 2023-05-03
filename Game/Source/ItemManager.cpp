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

	return true;
}

bool ItemManager::CleanUp()
{
	nodeList.clear();
	itemCount.clear();
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
			LoadItemState(pugiNode);
			LoadNodes(pugiNode, tree);
			pugiNode = pugiNode.next_sibling("items");
		}
	}

	return 1;
}

void ItemManager::AddQuantity(pugi::xml_node& xml_trees, const char* name)
{
	size_t i = 0;
	for (pugi::xml_node pugiNode = xml_trees.child("item"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("item"))
	{
			if (nodeList[i]->name == name)
			{
				nodeList[i]->quantity++;	
				LoadQuantity(xml_trees);
				LoadItemState(xml_trees);
			}
			i++;
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
		textItem_path = pugiNode.attribute("texturepath").as_string();

		nodeList.push_back(node);

	}
	LoadQuantity(xml_trees);

}

void ItemManager::LoadQuantity(pugi::xml_node& xml_trees)
{

	for (pugi::xml_node pugiNode = xml_trees.child("item"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("item"))
	{
		ItemNode* node = new ItemNode;

		for (size_t i = 0; i < nodeList.size(); i++)
		{
			for (int n = 0; n <= nodeList[i]->quantity; n++)
			{
				node->type = pugiNode.attribute("type").as_int();
				node->name = pugiNode.attribute("name").as_string();
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
				textItem_path = pugiNode.attribute("texturepath").as_string();

				itemCount.push_back(node);
			}
		}
	}
	SaveItemState();
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
		item.append_attribute("quantity") = nodeList[i]->quantity;
		item.append_attribute("name") = nodeList[i]->name.GetString();
	}

	ret = saveDoc->save_file("save_items.xml");

	return ret;
}

bool ItemManager::LoadItemState(pugi::xml_node& xml_trees)
{
	bool ret = true;

	const char* file = "save_items.xml";
	pugi::xml_parse_result result = items.load_file(file);
	
	// load items
	for (pugi::xml_node pugiNode = items.first_child(); pugiNode != NULL; pugiNode = pugiNode.next_sibling("item"))
	{
		for (size_t i = 0; i < nodeList.size(); i++)
		{
			if (strcmp(pugiNode.append_attribute("name").as_string(), nodeList[i]->name.GetString()))
			{
				  nodeList[i]->quantity = pugiNode.append_attribute("quantity").as_int();
			}
		}
	}
	LoadQuantity(xml_trees);

	return ret;
}
