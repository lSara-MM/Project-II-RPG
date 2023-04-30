#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "GuiButton.h"
#include "Animation.h"
#include <array>

struct SDL_Texture;

enum class CharacterType
{
	ALLY = 0,
	ENEMY,
	NONE
};

enum class CharacterClass
{
	MELEE_DPS,
	RANGED_DPS,
	AOE_DPS,
	HEALER,
	TANK,
	BUFFER,
	DEBUFFER,
	ASSASSIN,
	DOT,

	NO_CLASS
};

class Character : public Entity
{
public:

	Character();
	Character(EntityType type_);

	virtual ~Character();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool CalculateRandomProbability(int bonus_I, int against_I = 0); //Retorna true si logras el chance, false si no

	void ModifyHP(int cantidad); //Positivo para curar negativo para dañar

	int CalculateDamage(int damage); //ERIC:Va haber que poner muchos mas atributos

	bool ResistStatusEffect(/*efecto,precision*/); //Hacer cuando se hagan status effects, aun no.
	
	

	//Gets, dan las stats sumadas (base+eqipo+buffos)
	/*int GetMaxHP() { return maxHp; }
	int GetHealth() { return currentHp; }
	int GetAttack() { return attack; }
	int GetArmor() { return armor; }
	int GetSpeed() { return speed; }*/

public:
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;

	float dtP;

	// Gestion de Turnos
	bool onTurn;
	bool alive;
	Timer turnDelay;
	bool delayOn=false;

	// Stats
	SString name;

	// STATS
	int maxHp; // La base a level 0/1 es 1000
	int currentHp;
	int attack; // 200 mas menos es la base
	int critRate; // Base 10, es un porcentaje
	int critDamage; // Base 120, , luego se ha de dividir entre 100 a lo 120= x1,2
	int precision; // La base es 100%, es un porcentaje que se enfrenta vs la resistencia y precision
	int armor; // MAXIMO ABSOLUTO 200, mas menos full items en tanque 120
	int dodge; // Probabilidad de no recibir nada de daño, base es 0%
	int res; //Probabilidad de no recibir un efecto de estado, base 10%
	int speed; // Indicador quien va primero en la iniciativa

	// Posicion combate (va del 0 al 3)
	int positionCombat_I;
	
	CharacterType charaType; // Ally o Enemy
	CharacterClass charaClass;

	////Skills Descriptions
	//SString skills_C[4]; //Hay que describirlas aqui para poder llamarlo desde el combat
	//vector<Skills*> skillsList;

	GuiButton* button;

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __CHARACTER_H__