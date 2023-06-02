#include "DialogueSystem.h"
#include "QuestManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

DialogueSystem::DialogueSystem() : Module()
{
	name.Create("dialogue");
	textSpeed = TextSpeed::MEDIUM;
}

DialogueSystem::~DialogueSystem()
{
}

bool DialogueSystem::Awake(pugi::xml_node& config)
{
	LOG("Loading Dialogue System");
	bool ret = true;
	textBox_path = config.attribute("textBoxBg").as_string();

	return ret;
}

bool DialogueSystem::Start()
{
	textBox_tex = app->tex->Load(textBox_path);
	hasEnded = false;
	return true;
}

bool DialogueSystem::Update(float dt)
{
	if (activeTree != nullptr)
	{
		//Text box
		iPoint pos = { 0, (app->win->GetHeight() - 245) };
		app->render->DrawTexture(textBox_tex, pos.x - app->render->camera.x, pos.y - app->render->camera.y);

		if (!activeTree->UpdateTree(dt, app->dialogueSystem, pos))
		{
			app->guiManager->Draw();
		}
		else
		{
			hasEnded = true;
			Disable();
		}
	}

	return true;
}

bool DialogueSystem::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	activeTree->activeNode->trimmed = false;
	playerInput = activeTree->activeNode->choicesList[control->id];

	if (playerInput->eventReturn == DIALOGUE_SAVE)
	{
		activeTree->activeNode->playerAnswer = control->id;
		SaveDialogueState();
	}

	// Check if last node
	if (playerInput->nextNode != -1)
	{
		activeTree->activeNode = activeTree->nodeList[playerInput->nextNode];
		activeTree->updateOptions = false;
	} 
	else
	{
		hasEnded = true;
		CleanUp();
	}
	
	//app->guiManager->CleanUp();

	return true;
}

bool DialogueSystem::CleanUp()
{
	if (activeTree != nullptr)
	{
		activeTree->CleanUp();
		activeTree->activeNode = nullptr;
		activeTree->nodeList.clear();
		delete activeTree;
		activeTree = nullptr;
	}

	app->tex->UnLoad(textBox_tex);
	//app->guiManager->CleanUp();

	return true;
}

void DialogueSystem::PerformDialogue(vector<int> dialoguesID)
{
	int id = rand() % dialoguesID.size();
	app->dialogueSystem->LoadDialogue(dialoguesID.at(id));
}


int DialogueSystem::LoadDialogue(int dialogueID)
{
	const char* file = "dialogues.xml";
	pugi::xml_parse_result result = dialogues.load_file(file);

	DialogueTree* tree = new DialogueTree(false);

	if (result == NULL)
	{
		LOG("Could not load dialog xml file %s. pugi error: %s", file, result.description());
		return -1;
	}
	else
	{
		pugi::xml_node pugiNode = dialogues.first_child().first_child();

		for (int i = 0; i <= dialogueID && pugiNode != NULL; i++)
		{
			 if (pugiNode.attribute("ID").as_int() == dialogueID)
			 {
				 tree->treeID = pugiNode.attribute("ID").as_int();
			
				 tree->activeNode = LoadNodes(pugiNode, tree);	
				 activeTree = tree;	 
				 break;
			 }
			 else
			 {
				 pugiNode = pugiNode.next_sibling("dialogueTree");
			 }
		}
	}

	return dialogueID;
}

DialogueNode* DialogueSystem::LoadNodes(pugi::xml_node& xml_trees, DialogueTree* tree)
{
	DialogueNode* first_node = new DialogueNode;

	for (pugi::xml_node pugiNode = xml_trees.child("node"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("node"))
	{
		DialogueNode* node = new DialogueNode;

		node->nodeID = pugiNode.attribute("id").as_int();
		node->name = pugiNode.attribute("name").as_string();
		node->text = pugiNode.attribute("text").as_string();

		LoadChoices(pugiNode, node);

		tree->nodeList.push_back(node);

		if (node->nodeID == 0) { first_node = node; }	// return the first node to set as the active one
	}

	return first_node;
}

void DialogueSystem::LoadChoices(pugi::xml_node& xml_node, DialogueNode* node)
{
	for (pugi::xml_node choice = xml_node.child("choice"); choice != NULL; choice = choice.next_sibling("choice"))
	{
		DialogueChoice* option = new DialogueChoice;
		option->nextNode = choice.attribute("nextNode").as_int();
		option->text = choice.attribute("option").as_string();
	
		option->eventReturn = choice.attribute("eventReturn").as_int();

		node->choicesList.push_back(option);
	}
}

SString DialogueSystem::ChangeTextSpeed()
{
	switch (textSpeed)
	{
	case TextSpeed::SLOW:
		textSpeed = TextSpeed::MEDIUM;
		return "Medium";
		break;
	case TextSpeed::MEDIUM:
		textSpeed = TextSpeed::FAST;
		return "Fast";
		break;
	case TextSpeed::FAST:
		textSpeed = TextSpeed::SLOW;
		return "Slow";
		break;
	default:
		break;
	}

	return "Medium";
}

TextSpeed DialogueSystem::GetTextSpeed()
{
	return textSpeed;
}

SString DialogueSystem::GetTextSpeedSString()
{
	switch (textSpeed)
	{
	case TextSpeed::SLOW:
		return "Slow";
		break;
	case TextSpeed::MEDIUM:
		return "Medium";
		break;
	case TextSpeed::FAST:
		return "Fast";
		break;
	default:
		break;
	}

	return "Medium";
}

bool DialogueSystem::LoadDialogueState()
{
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_dialogue.xml");
	pugi::xml_node node = gameStateFile.child("save_choices");
	
	bool ret = true;

	if (result == NULL)
	{
		LOG("Could not load xml file save_dialogue.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		string temp = node.child("player").attribute("player_name").as_string();
		app->input->playerName->input = temp.c_str();
		app->input->playerName->input_entered = true;

		for (size_t i = 0; i < activeTree->nodeList.size() && !node.child("node").empty(); i++)
		{
			for (int j = 0; j < activeTree->nodeList[i]->choicesList.size(); j++)
			{
				if (activeTree->nodeList[i]->choicesList[j]->eventReturn == 3)
				{
					activeTree->nodeList[i]->nodeID = node.child("node").attribute("id").as_int();
					activeTree->nodeList[i]->playerAnswer = node.child("node").attribute("answer").as_int();
				}
			}
		}
	}

	return ret;
}


bool DialogueSystem::SaveDialogueState()
{
	bool ret = true;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_choices");


	pugi::xml_node player = node.append_child("player");

	// save player's name
	player.append_attribute("player_name") = app->input->playerName->input.c_str();

	// save important choices
	if (activeTree != nullptr)
	{
		for (size_t i = 0; i < activeTree->nodeList.size(); i++)
		{
			for (int j = 0; j < activeTree->nodeList[i]->choicesList.size(); j++)
			{
				if (activeTree->nodeList[i]->playerAnswer > -1 && activeTree->nodeList[i]->choicesList[j]->eventReturn == 3)
				{
					player = node.append_child("node");
					player.append_attribute("id") = activeTree->nodeList[i]->nodeID;
					player.append_attribute("answer") = activeTree->nodeList[i]->playerAnswer;
					player.append_attribute("text") = activeTree->nodeList[i]->choicesList[activeTree->nodeList[i]->playerAnswer]->text.GetString();
					break;
				}
			}
		}
	}

	ret = saveDoc->save_file("save_dialogue.xml");

	return ret;
}