#include "Skills.h"

#include "App.h"
#include "GuiManager.h"

#include "Audio.h"
#include "Module.h"
#include "Render.h"
#include "Window.h"
#include "Combat.h"

Skill::Skill()
{
	name = " ";
};

Skill::~Skill()
{
}

Skill::Skill(SString name, SString description, int posToUseStart_I, int posToUseEnd_I, int posToTargetStart_I, int posToTargetEnd_I, int movementCaster, int movementTarget, bool areaSkill, bool autoTarget, float multiplierDmg, int bonusPrecision, int bonusCritRate, int bonusCritDamage)
{
	//Info
	this->name = name;
	this->description = description;
	this->type = type;

	//Stats
	this->multiplierDmg = multiplierDmg;
	this->bonusCritDamage = bonusCritDamage;
	this->bonusCritRate = bonusCritRate;
	this->bonusPrecision = bonusPrecision;
	this->movementTarget = movementTarget;

	//Movement & position
	this->movementCaster = movementCaster;
	this->posToUseStart_I = posToUseStart_I;
	this->posToUseEnd_I = posToUseEnd_I;

	//Targeting
	this->posToTargetStart_I = posToTargetStart_I;
	this->posToTargetEnd_I = posToTargetEnd_I;
	this->areaSkill = areaSkill;
	this->autoTarget = autoTarget;
}



bool Skill::PosCanBeUsed(int positionCharacter)
{
	//True si dentro del rango, false si no
	return (positionCharacter >= posToTargetStart_I && positionCharacter <= posToTargetEnd_I);
}

int Skill::RangeCanTarget(vector<Character> vec)
{
	Character a;
	a.charaType == CharacterType::ALLY;
	//Mirar size del vector
	//si mas pequeño el vector que 
	return 0;
}
