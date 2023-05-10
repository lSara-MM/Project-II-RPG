#include "DialogueTree.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"


void DialogueNode::CleanUp()
{
	for (int j = 0; j < choicesList.size(); j++) { delete choicesList[j]; }
	choicesList.clear();
	
	texts.clear();
}


DialogueTree::DialogueTree(bool a)
{
	active = a;
}

bool DialogueTree::UpdateTree(float dt, Module* mod, iPoint pos)
{
	max_chars_line = FONT_SIZE * 5;

	// descomentar quan els noms dels npc estiguin al dialogues xml
	app->render->TextDraw(activeNode->name.GetString(), pos.x + 50, pos.y - FONT_SIZE * 2, FONT_SIZE, Font::TEXT, { 255, 255, 255 });
	
	if (!app->input->playerName.empty())
	{
		activeNode->text.Substitute("%x", app->input->playerName.c_str());
	}

	if (!activeNode->trimmed)
	{
		app->render->SplitText(activeNode->text, &activeNode->texts, FONT_SIZE, max_chars_line);
		activeNode->trimmed = true;
	}

	size_t lines = activeNode->texts.size();
	for (size_t i = 0; i < lines; i++)
	{
		app->render->TextDraw(activeNode->texts[i].GetString(), pos.x + 100, pos.y + 50 + 50 * i, FONT_SIZE, Font::TEXT, { 255, 255, 255 });
	}

	EventReturn(mod, pos);

	if (!updateOptions)
	{
		updateOptions = UpdateNodes(mod, pos);
	}

	return true;
}

bool DialogueTree::UpdateNodes(Module* mod, iPoint pos)
{
	GuiButton* button;

	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		const char* ch_option = activeNode->choicesList[i]->text.GetString();	// SString to const char*	
		int w = FONT_SIZE * 15;
		int h = FONT_SIZE * 3;
		SDL_Rect bounds = { app->win->GetWidth() - w, pos.y - (h + FONT_SIZE) * (i + 1), w, h};
		//SDL_Rect bounds = { 900, pos.y - (h + FONT_SIZE) * (i + 1), w, h };

		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id, mod, bounds, ButtonType::LARGE, ch_option, FONT_SIZE);
		button->state = GuiControlState::NORMAL;
		listDialogueButtons.Add(button);
	}

	return true;
}

bool DialogueTree::EventReturn(Module* mod, iPoint pos)
{
	SString temp;
	
	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		switch (activeNode->choicesList[i]->eventReturn)
		{
		case DIALOGUE_INPUT:

			//if (!app->input->getInput_B)
			//{
			//	// Get player's input
			//	app->input->getInput_B = true;
			//	updateOptions = true;

			//	if (app->input->nameEntered_B)
			//	{
			//		activeNode->choicesList[i]->text.Substitute("%x", app->input->playerName.c_str());	// Change %x for player's name - (const char* current word, const char* new word)
			//		updateOptions = UpdateNodes(mod, pos, fontSize);
			//	}
			//}

			//temp = "Player's name:  %%";
			//temp.Substitute("%", app->input->playerName.c_str());
			//app->render->TextDraw(temp.GetString(), app->win->GetWidth() / 3, 100, fontSize, Font::TEXT, { 255, 255, 255 });
			break;

		case DIALOGUE_SAVE:
			// see on DialogueSystem::OnGuiMouseClickEvent();
			break;
		case DIALOGUE_IF:


			break;
		default:
			return false;
			break;
		}
	}

	return true;
}

void DialogueTree::CleanUp()
{
	for (int j = 0; j < nodeList.size(); j++)
	{
		nodeList[j]->CleanUp();
		delete nodeList[j];
	}
	
	nodeList.clear();

	listDialogueButtons.Clear();
}