#ifndef __ECHOUSEDPS_H__
#define __ECHOUSEDPS_H_

#include "Entity.h"
#include "Characther.h"
#include "CombatEnemy.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "Animation.h"

struct SDL_Texture;

class HouseDPS : public CombatEnemy
{
public:

	HouseDPS();

	virtual ~HouseDPS();

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

	float dtP;

	SDL_RendererFlip flipType;

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __PCHOUSEDPS_H__
