#include "DialogueTree.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

using namespace std;

void DialogueNode::SplitText(SString text_, int fontSize_, int max_chars_line_)
{
	string line = text_.GetString();

	if (text_.Length() > max_chars_line_)
	{
		int a, b, startIndex = 0;
		for (int j = 0; j <= line.length() / max_chars_line_; j++)	// <= -> in case of decimal, get the round up number 
		{
			a = max_chars_line_ + startIndex;
			b = line.find_first_of(" ", a);	// find first " " (space) from last trimmed to the end. 

			// If we reached the end of the word or the end of the input.
			string temp;
			temp.append(line, startIndex, b - startIndex);	// string text to append, int index start, int size of text to append
			texts.push_back(temp.c_str());
			startIndex = b;
		}
	}
	else
	{
		texts.push_back(line.c_str());
	}

	trimmed = true;
}

void DialogueNode::CleanUp()
{
	for (int j = 0; j < choicesList.size(); j++) { delete choicesList[j]; }
	choicesList.clear();
}


DialogueTree::DialogueTree(bool a)
{
	active = a;
}

bool DialogueTree::UpdateTree(float dt, Module* mod, iPoint pos)
{
	fontSize = 24;
	max_chars_line = fontSize * 3;


	if (!app->input->playerName.empty())
	{
		activeNode->text.Substitute("%x", app->input->playerName.c_str());
	}

	if (!activeNode->trimmed)
	{
		activeNode->SplitText(activeNode->text, fontSize, max_chars_line);
	}

	size_t lines = activeNode->texts.size();
	for (size_t i = 0; i < lines; i++)
	{
		app->render->TextDraw(activeNode->texts[i].GetString(), pos.x + 100, pos.y + 20 + 50 * i, fontSize, Font::TEXT, { 255, 255, 255 });
	}

	EventReturn(mod, pos);

	if (!updateOptions)
	{
		updateOptions = UpdateNodes(mod, pos, fontSize);
	}

	return true;
}

bool DialogueTree::UpdateNodes(Module* mod, iPoint pos, int fontSize)
{
	GuiButton* button;

	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		const char* ch_option = activeNode->choicesList[i]->text.GetString();	// SString to const char*	
		int w = activeNode->choicesList[i]->text.Length() * fontSize * 0.5 + 10;
		int h = fontSize + fontSize / 4;
		SDL_Rect bounds = { app->win->GetWidth() - w, pos.y - (h + fontSize) * (i + 1), w, h};

		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id, mod, bounds, ButtonType::LARGE, ch_option, fontSize);
		button->state = GuiControlState::NORMAL;
		listDialogueButtons.Add(button);
	}

	return true;
}

bool DialogueTree::EventReturn(Module* mod, iPoint pos)
{
	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		switch (activeNode->choicesList[i]->eventReturn)
		{
		case DIALOGUE_INPUT:

			if (!app->input->getInput_B)
			{
				// Get player's input
				app->input->getInput_B = true;
				updateOptions = true;

				if (app->input->nameEntered_B)
				{
					activeNode->choicesList[i]->text.Substitute("%x", app->input->playerName.c_str());	// Change %x for player's name - (const char* current word, const char* new word)
					updateOptions = UpdateNodes(mod, pos, fontSize);
				}
			}

			app->render->TextDraw("Player's name:", app->win->GetWidth() / 3, 100, fontSize, Font::TEXT, { 255, 255, 255 });
			if (!app->input->playerName.empty())
			{
				const char* ch_name = app->input->playerName.c_str();	// SString to const char*	
				app->render->TextDraw(ch_name, app->win->GetWidth() / 3 + fontSize * 7, 100, fontSize, Font::TEXT, { 255, 255, 255 });
			}

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
}