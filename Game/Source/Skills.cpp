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

Skill::Skill(SString name, SString description, int posToUseStart_I, int posToUseEnd_I, int posToTargetStart_I, int posToTargetEnd_I, int movementCaster, int movementTarget,bool friendlyFire, bool areaSkill, bool autoTarget, float multiplierDmg, int bonusPrecision, int bonusCritRate, int bonusCritDamage)
{
	//Info
	this->name = name;
	this->description = description;

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
	this->targetFriend = friendlyFire;
	this->areaSkill = areaSkill;
	this->autoTarget = autoTarget;
}

bool Skill::PosCanBeUsed(int positionCharacter)
{
	//True si dentro del rango, false si no
	return (positionCharacter >= posToTargetStart_I && positionCharacter <= posToTargetEnd_I);
}

//Retorna ultima posicion usable
int Skill::RangeCanTarget(vector<Character*> vec)
{
	//Mirar size del vector
	int max = vec.size();
	//si mas pequeño el vector que 
	if (max < posToTargetEnd_I) { return max; }
	else if (max > posToTargetStart_I){return posToTargetEnd_I;}
	else { return -1; } //No se puede usar
	
}

int Skill::RandomTarget(int posInicial, int posFinal, TargetingMethod method)
{
	int pos= posInicial;
	int width = posFinal - posInicial + 1;
	switch (method)
	{
	case TargetingMethod::RANDOM:
		
		pos = rand() % width + posInicial;
		break;

	case TargetingMethod::LOWER_HP:
		break;

	default:
		break;
	}

	return pos;
}

