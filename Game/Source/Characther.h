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

	bool CalculateRandomProbability(int bonus_I, int against_I = 0); //Retorna true si logras el chance, false si no

	void ModifyHP(int cantidad); //Positivo para curar negativo para dañar

	int CalculateDamage(int damage); //ERIC:Va haber que poner muchos mas atributos

	bool ResistStatusEffect(/*efecto,precision*/); //Hacer cuando se hagan status effects, aun no.
	

public:
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;

	float dtP;

	SDL_RendererFlip flipType;

	//Gestion de Turnos
	bool onTurn;
	bool alive;

	//Stats
	SString name;

	//STATS
	int maxHp; //La base a level 0/1 es 1000
	int currentHp;
	int attack; //200 mas menos es la base
	int critProbability; // Base 10, es un porcentaje
	int critDamage; // Base 120, , luego se ha de dividir entre 100 a lo 120= x1,2
	int precision; //La base es 100%, es un porcentaje que se enfrenta vs la resistencia y precision
	int armor; //MAXIMO ABSOLUTO 200, mas menos full items en tanque 120
	int esquiva; //Probabilidad de no recibir nada de daño, base es 0%
	int resistencia; //Probabilidad de no recibir un efecto de estado, base 10%
	int speed; //Indicador quien va primero en la iniciativa

	//Posicion combate
	int positionCombat_I; //Del uno al cuatro
	enum CharacterType
	{
		ALLY,
		ENEMY,
		NONE 
	};
	CharacterType charaType_I; //Ally o Enemy
	
	//Gets, dan las stats sumadas (base+eqipo+buffos)
	int GetMaxHP() { return maxHp; }
	int GetHealth() { return currentHp; }
	int GetAttack() { return attack; }
	int GetArmor() { return armor; }
	int GetSpeed() { return speed; }
	
	class Skill
	{
	public:
		Skill() { name = " "; };
		~Skill();

		Skill() {}
		SString name;
		SString description;
		
		//Quiza hay que hacer un struct para las posiciones en plan posinicial y pos final
		//Posicion para hacer la skill (min 1 max 4)
		int posToUseStart_I; //Posicion mas baja donde puedes usar el ataque
		int posToUseEnd_I; //Posicion mas alta donde puedes hacer el ataque, si es solo una pues poner el mismo numero que la inicial

		//Posicion que puedes targetear (quiza hay que meter si es area o no)

		//Gestion movimiento
		int movement; //0 si no te mueve, -x si te mueve hacia delante +x si te mueve hacia atras

	private:

	};

	//Skills Descriptions
	SString skills_C[4]; //Hay que describirlas aqui para poder llamarlo desde el combat

private:

	SDL_Texture* texture;
	const char* texturePath;

	iPoint  position;
	int width, height;
};

#endif // __CHARACTHER_H__