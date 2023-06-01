#ifndef __SKILLS_H__
#define __SKILLS_H__

#include "App.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "Character.h"

#include "Log.h"
#include "SString.h"
#include <vector>

#include "SDL/include/SDL.h"

class Character;
enum class CharacterType;

enum class SkillType //
{
	ATTACK = 1, //IMPAR OFENSIVO
	HEAL, //PAR DEFENSIVO
	DEBUFF,
	BUFF,
	
};

enum class TargetingMethod //Como se ataca
{
	RANDOM=0,
	LOWER_HP,
	LOWER_ARMOR,
	LOWER_DODGE
};
// prueba
class Skill
{
public:
	Skill();
	~Skill();

	Skill(SString name, SString description, 
		int posToUseStart_I, int posToUseEnd_I, int posToTargetStart_I, int posToTargetEnd_I, TargetingMethod method,
		int movementCaster,	int movementTarget,bool targetFriend, bool areaSkill, bool autoTarget,
		float multiplierDmg,int bonusAccuracy,	int bonusCritRate,int bonusCritDamage, 
		int status,bool positiveEffect,int duration,int intensity);

	bool PosCanBeUsed(int positionCharacter);

	int RangeCanTarget(vector<Character*> vec);

	int RandomTarget(int posInicial, int posFinal, vector<Character*> arr, TargetingMethod method = TargetingMethod::RANDOM);

	 
public:
	SString name;
	SString description;


	SkillType type;

	//Gestion movimiento
	int movementCaster; //0 si no te mueve, -x si te mueve hacia delante +x si te mueve hacia atras
	int movementTarget;
	Character* charaToMove;

	//Gestion potencia
	float multiplierDmg;
	int bonusAccuracy;
	int bonusCritRate;
	int bonusCritDamage;
	//Variable de que tipo de stat usan para el escaldo?

	//Status Effects
	int status;
	bool positiveEffect;
	int duration;
	int intensity;

	//Posicion para hacer la skill (min 0 max 3)
	int posToUseStart_I; //Posicion mas baja donde puedes usar el ataque
	int posToUseEnd_I; //Posicion mas alta donde puedes hacer el ataque, si es solo una pues poner el mismo numero que la inicial

	//Posicion que puedes targetear
	CharacterType typeTargeted;	// Pa que se necesita eso si tienes el personaje que hace el target?
	int posToTargetStart_I; //Posicion mas baja donde puedes usar el ataque
	int posToTargetEnd_I; //Posicion mas alta donde puedes targetear el ataque, si es solo una pues poner el mismo numero que la inicial
	bool targetFriend; //Targetea a su equipo o los enemigos
	bool areaSkill; //Si es true al hacerse la skill targetea todo da igual que posicion hayas pulsado
	bool autoTarget;//Targetearse a si mismo
	TargetingMethod methodTarget; //Para enemigos solo, como targetean, si random, por vida, etc


private:

	
};

#endif // __SKILLS_H__