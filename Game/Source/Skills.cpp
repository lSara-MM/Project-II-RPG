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

Skill::Skill(SString name, SString description, int posToUseStart_I, int posToUseEnd_I, int posToTargetStart_I, int posToTargetEnd_I, TargetingMethod method, int movementCaster, int movementTarget_,bool friendlyFire, bool areaSkill, bool autoTarget, float multiplierDmg_, int bonusAccuracy_, int bonusCritRate_, int bonusCritDamage_, int firstStatus, bool firstPositiveEffect,int firstDuration, int firstIntensity, int secondStatus, bool secondPositiveEffect, int secondDuration, int secondIntensity)
{
	//Info
	this->name = name;
	this->description = description;

	//Stats
	this->multiplierDmg = multiplierDmg_;
	this->bonusCritDamage = bonusCritDamage_;
	this->bonusCritRate = bonusCritRate_;
	this->bonusAccuracy = bonusAccuracy_;
	this->movementTarget = movementTarget_;

	//Status Effects 1
	this->firstStatus = firstStatus;
	this->firstPositiveEffect = firstPositiveEffect;
	this->firstDuration = firstDuration;
	this->firstIntensity = firstIntensity;

	//Status Effects 2
	this->secondStatus = secondStatus;
	this->secondPositiveEffect = secondPositiveEffect;
	this->secondDuration = secondDuration;
	this->secondIntensity = secondIntensity;

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
	this->methodTarget = method;
}

bool Skill::PosCanBeUsed(int positionCharacter)
{
	//True si dentro del rango, false si no
	return (positionCharacter >= posToUseStart_I && positionCharacter <= posToUseEnd_I);
}

//Retorna ultima posicion usable
int Skill::RangeCanTarget(vector<Character*> vec)
{
	//Mirar size del vector
	int max = vec.size()-1;
	//si mas peque�o el vector que 
	if (max < posToTargetEnd_I) { return max; }
	else if (max > posToTargetStart_I){return posToTargetEnd_I;}
	else { return -1; } //No se puede usar
	
}

int Skill::RandomTarget(int posInicial, int posFinal, vector<Character*> arr, TargetingMethod method)
{
	int pos= posInicial;
	int width = posFinal - posInicial + 1;
	int StatTracker = 99999;

	//Mirar si hay algun taunt
	for (int i = posInicial; i <= posFinal; i++)
	{
		if (arr.at(i)->GetStat(EffectType::TAUNT)!=0)
		{
			pos = i;
			return pos; //Si hay taunt pues targetea al taunt si esta en la posibilidad
		}
	}

	switch (method)
	{
	case TargetingMethod::RANDOM: //Si hay mecanica de taunt pues aqui ponerla
		do 
		{
			pos = rand() % width + posInicial;
		} while (pos >= arr.size());

		break;

	case TargetingMethod::LOWER_HP:
		for (int i = posInicial; i <= posFinal; i++)
		{
			if (arr.at(i)->currentHp<=StatTracker)
			{
				pos = i;
				StatTracker = arr.at(i)->currentHp;
			}
		}

		break;
	case TargetingMethod::LOWER_ARMOR:
		for (int i = posInicial; i <= posFinal; i++)
		{
			if (arr.at(i)->GetStat(EffectType::ARMOR) <= StatTracker)
			{
				pos = i;
				StatTracker = arr.at(i)->GetStat(EffectType::ARMOR);
			}
		}
		break;
	case TargetingMethod::LOWER_DODGE:
		for (int i = posInicial; i <= posFinal; i++)
		{
			if (arr.at(i)->GetStat(EffectType::DODGE) <= StatTracker)
			{
				pos = i;
				StatTracker = arr.at(i)->GetStat(EffectType::DODGE);
			}
		}
		break;
	
	case TargetingMethod::LOWER_RESISTANCE:
		for (int i = posInicial; i <= posFinal; i++)
		{
			if (arr.at(i)->GetStat(EffectType::RES) <= StatTracker)
			{
				pos = i;
				StatTracker = arr.at(i)->GetStat(EffectType::RES);
			}
		}
		break;

	default:
		break;
	}

	return pos;
}

