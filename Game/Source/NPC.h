#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Physics.h"

#include "Animation.h"

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

	void Controller(float dt);

public:
	SString NpcName;

	// The pointer to the current Npc animation
	// It will be switched depending on the Npc's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation currentAnim;
	Animation downAnim;
	Animation idleDownAnim;
	Animation upAnim;
	Animation idleUpAnim;
	Animation leftAnim;
	Animation idleLeftAnim;
	Animation rigthAnim;
	Animation idleRigthAnim;
	Animation hurtAnim;
	Animation deathAnim;
	Animation attackAnim;

	bool keyLockUp = false;
	bool keyLockDown = false;
	bool keyLockLeft = false;
	bool keyLockRigth = false;

	float dtP;
	float grav;
	b2Vec2 vel = { 0, 0 };

	SDL_RendererFlip flipType;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __NPC_H__