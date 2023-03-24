#ifndef __CHARACTHER_H__
#define __CHARACTHER_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "Animation.h"

struct SDL_Texture;

class Characther : public Entity
{
public:

	Characther();

	virtual ~Characther();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void ModifyHP(int cantidad); //Positivo para curar negativo para dañar

	int CalculateDamage(int damage);

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

	//Gestion de Turnos
	bool onTurn;
	bool alive;

	//Stats
	int maxHp;
	int currentHp;
	int attack;
	int armor; //MAXIMO ABSOLUTO 200, mas menos full items en tanque 120
	int speed;

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __CHARACTHER_H__