#ifndef __ECHOUSEHEALER_H__
#define __ECHOUSEHEALER_H_

#include "Entity.h"
#include "Characther.h"
#include "CombatEnemy.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "Animation.h"

struct SDL_Texture;

class HouseHealer : public CombatEnemy
{
public:

	HouseHealer();

	virtual ~HouseHealer();

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

#endif // __PCHOUSEHEALER_H__
