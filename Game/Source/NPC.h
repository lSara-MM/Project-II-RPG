#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Physics.h"

#include "Animation.h"
#include "SString.h"
#include <vector>

struct SDL_Texture;

class Npc : public Entity
{
public:

	Npc();

	virtual ~Npc();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool PerformDialogue();

public:

	SString name;

	iPoint position;
	int width, height;

	// The pointer to the current Npc animation
	// It will be switched depending on the Npc's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;
	
	vector<int> dialoguesID;

	float dtP;
	
	SDL_RendererFlip flipType;
	PhysBody* pbody;
	PhysBody* pSensor;

private:

	SDL_Texture* texture;
	const char* texturePath;
};

#endif // __NPC_H__