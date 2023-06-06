#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

#include "GuiButton.h"
#include "Animation.h"
#include <array>
#include "Skills.h"

struct SDL_Texture;
class Skill;

// Status effect
enum class EffectType
{
	NONE = -1,
	CURRENT_HP = 0,
	ATTACK,
	CRIT_RATE,
	CRIT_DMG,
	ACCURACY,
	ARMOR,
	DODGE,
	RES,
	TAUNT,
	STUN,
	BLESS //Inmunidad a effecto positivo o negativo.(1 negativos, 0 todos, -1 negativos)
};

struct StatusEffect
{
public:

	StatusEffect(int intensity_, int turnsLeft_, bool isPositive_, EffectType type_)
	{
		intensity = intensity_;
		turnsLeft = turnsLeft_;
		isPositive = isPositive_;
		type = type_;
	}

	/*bool StatusUpdate(int* affectedStat)
	{
		if (turnsLeft > 0)
		{
			affectedStat += intensity;
			turnsLeft--;
			return true;
		}
		return false;
	}*/

public:
	
	int intensity;
	int turnsLeft;
	bool isPositive; // true positive - false negative
	EffectType type;
};


// Character
enum class CharacterType
{
	ALLY = 0,
	ENEMY,
	NONE
};

enum class CharacterClass
{
	MELEE_DPS=0,
	RANGED_DPS,
	ASSASSIN,
	AOE_DPS,
	TANK,
	HEALER,
	BUFFER,
	DEBUFFER,
	BOSS,

	NO_CLASS
};


class Character : public Entity
{
public:

	Character();

	virtual ~Character();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	//Retorna true si logras el chance, false si no
	bool CalculateRandomProbability(int bonus_I, int against_I = 0); 

	bool ModifyHP(int hp); //Positivo para curar negativo para dañar

	int ApplySkill(Character* caster, Character* defender, Skill* skill); //ERIC:Va haber que poner muchos mas atributos	
	void LoadSkill(int arr[4]);

	bool UseSkill(Skill* skill);
	bool UseSkill(Skill* skill, Character* target);

	//StatusEffecrs
	int GetStat(EffectType statType);
	float GetStatModifier(EffectType statType);
	bool ReduceCountStatusEffects();

public:
	bool isCombatant;
	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation idleAnim;

	float dtP;

	// Gestion de Turnos
	bool onTurn;
	Timer turnDelay;
	bool delayOn = false;

	// STATS
	int maxHp; // La base a level 0/1 es 1000
	int currentHp;
	int attack; // 200 mas menos es la base
	int critRate; // Base 10, es un porcentaje
	int critDamage; // Base 120, , luego se ha de dividir entre 100 a lo 120= x1,2
	int accuracy; // La base es 100%, es un porcentaje que se enfrenta vs la resistencia y accuracy
	int armor; // MAXIMO ABSOLUTO 200, mas menos full items en tanque 120
	int dodge; // Probabilidad de no recibir nada de daño, base es 0%
	int res; // Probabilidad de no recibir un efecto de estado, base 10%
	int speed; // Indicador quien va primero en la iniciativa

	// Posicion combate (va del 0 al 3)
	int positionCombat_I;
	
	CharacterType charaType; // Ally o Enemy
	CharacterClass charaClass;

	////Skills Descriptions
	List<Skill*> listSkills;
	List<StatusEffect*> listStatusEffects;
	List<int> listSkillsHistory; //Aqui guardamos un historial de que skills se ha usado.
	
	GuiButton* button;

	// Texture position
	iPoint  position;
	SDL_Texture* texture;
	SDL_Rect texSection;
	
	//para dibujar, estaba en xml y no se usaba, para aprovechar
	int id;

	const char* texturePath;
	
private:
	

	int width, height;

	pugi::xml_document skillsFile;
	pugi::xml_node skillNode;

	//fx
	const char* healPath;
	uint healfx;

	const char* hitPath;
	uint hitfx;

	const char* buffPath;
	uint bufffx;

	const char* debuffPath;
	uint debufffx;

	//textura supersaiyan
	SDL_Texture* textureOnturn;

	
};

#endif // __CHARACTER_H__