#ifndef __CHARACTHER_H__
#define __CHARACTHER_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "Animation.h"

struct SDL_Texture;

class Character : public Entity
{
public:

	Character();

	virtual ~Character();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool Render(); //Se llama a ser printado desde combat(?)

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

	//Posicion combate
	int positionCombat_I; //Del uno al cuatro
	enum CharatherType
	{
		ALLY,
		ENEMY,
		NONE 
	};
	CharatherType charaType_I; //Va del 1 al 4
	
	int GetHealth() { return currentHp; }
	int GetAttack() { return attack; }
	int GetArmor() { return armor; }
	int GetSpeed() { return speed; }
	int GetMaxHP() { return maxHp; }

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __CHARACTHER_H__