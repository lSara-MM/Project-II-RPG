#ifndef __DIALOGTREE_H__
#define __DIALOGTREE_H__

#include "GuiManager.h"
#include "Module.h"
#include "GuiButton.h"
#include "Store.h"

#include "List.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"
#include <vector>

#define FONT_SIZE 20

#define DIALOGUE_INPUT 2
#define DIALOGUE_SAVE 3
#define DIALOGUE_IF 4
#define DIALOGUE_QUEST 5
#define DIALOGUE_STORE 6
#define DIALOGUE_FORGE 7

using namespace std;

class DialogueChoice
{
public:
	DialogueChoice() {};
	~DialogueChoice() {};

public:
	int choiceID;
	SString text;
	int nextNode;
	int eventReturn;
};


class DialogueNode
{
public:
	DialogueNode() {};
	~DialogueNode() {};
	
	void CleanUp();

public:
	SString name;
	SString text;

	int nodeID;
	vector <DialogueChoice*> choicesList;
	int playerAnswer = -1;

	bool trimmed = false;
	vector<SString> texts;
};


class DialogueTree
{
public:
	DialogueTree() {};
	DialogueTree(bool a);
	~DialogueTree() {};

	bool UpdateTree(float dt, Module* mod, iPoint pos);
	bool UpdateNodes(Module* mod, iPoint pos);
	bool EventReturn(Module* mod, iPoint pos);
	void CleanUp();

public:
	int treeID;
	bool active = false;
	vector <DialogueNode*> nodeList;

	DialogueNode* activeNode;
	bool updateOptions;

private:
	List <GuiButton*> listDialogueButtons;
	int GUI_id = 0;

	int max_chars_line;

	float dtWait;
	bool notifyEnd;
};

#endif //__DIALOGTREE_H__