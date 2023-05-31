#include "DialogueTree.h"
#include "QuestManager.h"
#include "PuzzleManager.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

using namespace std;

void DialogueNode::CleanUp()
{
	for (int j = 0; j < choicesList.size(); j++) { delete choicesList[j]; }
	choicesList.clear();
	
	texts.clear();
}


DialogueTree::DialogueTree(bool a)
{
	updateOptions = false;
	active = a;
}

bool DialogueTree::UpdateTree(float dt, Module* mod, iPoint pos)
{
	max_chars_line = FONT_SIZE * 5;

	app->render->TextDraw(activeNode->name.GetString(), pos.x + 50, pos.y - FONT_SIZE * 2, FONT_SIZE, Font::TEXT, { 255, 255, 255 });
	
	if (!app->input->playerName->input.empty())
	{
		activeNode->text.Substitute("%x", app->input->playerName->input.c_str());
	}

	if (!activeNode->trimmed)
	{
		app->render->ResetDtText();
		activeNode->trimmed = true;
	}

	float dt_wait = 25.0f;
	//RenderTrimmedText(int x, int y, int offset, SString text, vector<SString>* pTexts, int fontSize_, int max_chars_line_, float fontOffset, Font font, int dtNext, SDL_Color color)
	app->render->RenderTrimmedText(pos.x + 100, pos.y + 50, 10, activeNode->text, &activeNode->texts, FONT_SIZE, max_chars_line, 0, Font::TEXT, dt_wait, { 255, 255, 255 });

	/*size_t lines = activeNode->texts.size();
	for (size_t i = 0; i < lines; i++)
	{
		app->render->TextDraw(activeNode->texts.at(i).GetString(), pos.x + 100, pos.y + 50 + (FONT_SIZE + 10) * i, FONT_SIZE, Font::TEXT, { 255, 255, 255 });
	}*/

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
		const char* ch_option = activeNode->choicesList.at(i)->text.GetString();	// SString to const char*	
		int w = FONT_SIZE * 20;
		int h = FONT_SIZE * 2.5;
		SDL_Rect bounds = { app->win->GetWidth() - w, pos.y - (h + FONT_SIZE) * (i + 1), w, h};

		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i + GUI_id, mod, bounds, ButtonType::DIALOGUE,
			ch_option, FONT_SIZE, Font::TEXT, { 0, 0, 0, 0 }, 5, Easings::CUBIC_IN, AnimationAxis::LEFT_X);
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

		case DIALOGUE_QUEST:

			if (activeNode->name == "Igor and Ivan" && !app->questManager->quest1->active)
			{
				app->questManager->quest1->active = true;
				app->questManager->SaveState();
			}

			if (activeNode->name == "ORACLE")
			{
				app->questManager->quest2->complete = true;
				app->questManager->quest2->active = true;
				app->questManager->SaveState();
			}

			if (activeNode->name == "NIKITA")
			{
				if(activeNode->nodeID == 7 && !app->questManager->quest2->active)
				{
					app->questManager->quest2->active = true;
					app->questManager->SaveState();
				}

				if (activeNode->nodeID == 8 && !app->questManager->quest3->active)
				{
					app->questManager->quest3->active = true;
					app->questManager->SaveState();
				}
			}

			break;
		case DIALOGUE_STORE:
			app->store->Enable();
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