#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	//Get A list with the type of characters you want to have
	List<Entity*> GetEntitiesByType(EntityType type);

public:

	List<Entity*> entities;

	pugi::xml_node entityNode;

	//Pido perd�n no borren 
	int tpID = 21;

};

#endif // __ENTITYMANAGER_H__
