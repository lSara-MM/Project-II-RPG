#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Quest.h"
#include "List.h"

class QuestManager : public Module
{
public:

	QuestManager();

	// Destructor
	virtual ~QuestManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Quest* CreateQuest(QuestType type);

	void DestroyQuest(Quest* quest);

	void AddQuest(Quest* quest);

	//Get A list with the type of characters you want to have
	List<Quest*> GetQuestByType(QuestType type);

	bool initQuest();

public:

	pugi::xml_node questNode;

	Quest* quest1;
	Quest* quest2;
	Quest* quest3;

	List<Quest*> quests;

};

#endif 

