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
	ATTACK = 0,
	HEAL,
	BUFF,
	DEBUFF,
};

// prueba
class Skill
{
public:
	Skill();
	~Skill();

	Skill(SString name, SString description, int posToUseStart_I, int posToUseEnd_I);

	bool PosCanBeUsed(int positionCharacter);

	int RangeCanTarget(vector<Character> vec);

public:
	SString name;
	SString description;


	SkillType type;

	//Gestion movimiento
	int movement; //0 si no te mueve, -x si te mueve hacia delante +x si te mueve hacia atras
	Character* charaToMove;

	//Gestion potencia
	float multiplierDmg;
	int bonusPrecision;
	int bonusCritRate;
	int bonusCritDamage;
	//Variable de que tipo de stat usan para el escaldo?

private:

	//Posicion para hacer la skill (min 0 max 3)
	int posToUseStart_I; //Posicion mas baja donde puedes usar el ataque
	int posToUseEnd_I; //Posicion mas alta donde puedes hacer el ataque, si es solo una pues poner el mismo numero que la inicial

	//Posicion que puedes targetear (quiza hay que meter si es area o no)
	CharacterType typeTargeted;	// Pa que se necesita eso si tienes el personaje que hace el target?
	int posToTargetStart_I; //Posicion mas baja donde puedes usar el ataque
	int posToTargetEnd_I; //Posicion mas alta donde puedes targetear el ataque, si es solo una pues poner el mismo numero que la inicial
	bool areaSkill; //Si es true al hacerse la skill targetea todo da igual que posicion hayas pulsado
};

#endif // __SKILLS_H__