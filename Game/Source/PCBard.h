#ifndef __PCBARD_H__
#define __PCBARD_H__

#include "Entity.h"
#include "Characther.h"
#include "PartyMember.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "Animation.h"

struct SDL_Texture;

class Bard : public PartyMember
{
public:

	Bard();

	virtual ~Bard();

	bool Awake();

	bool Start();

	bool Update(float dt);

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

	SString skills_C[4] = { "Habilidad 1 Barda", "Habilidad 2 Barda","Habilidad 3 Barda","Habilidad 4 Barda" };
	

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
	int auxtres;
};

#endif // __PCBARD_H__