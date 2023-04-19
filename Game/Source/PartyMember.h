#ifndef __PARTYMEMBER_H__
#define __PARTYMEMBER_H__

#include "Entity.h"
#include "Characther.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "Animation.h"

////List PCs
//#include "PCProtagonist.h" //Hace petar al player

struct SDL_Texture;

class PartyMember : public Character
{
public:

	PartyMember();

	PartyMember(Character*);

	virtual ~PartyMember();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool Render();

	bool CleanUp();

	

public:
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;
	Animation forwardAnim;
	Animation jumpAnim;
	Animation hurtAnim;
	Animation deathAnim;
	Animation attackAnim;

	float dtP;

	SDL_RendererFlip flipType;
	PhysBody* pbody;

	
	

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __PARTYMEMBER_H__