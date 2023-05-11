#ifndef __QUEST_H__
#define __QUEST_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

class PhysBody;

enum class QuestType
{
	FIND,
	COLLECT,
	TRAVEL,
	INTERACT,
	UNKNOWN
};

class Quest
{
public:

	Quest(QuestType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Quest::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Quest::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void EndContact(PhysBody* physA, PhysBody* physB) {

	};

public:

	SString title;
	SString desc;
	QuestType type;
	bool active = false;
	int state = 0;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	bool renderable = true;
};

#endif // __ENTITY_H__
