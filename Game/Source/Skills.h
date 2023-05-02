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


	bool PosCanBeUsed(int positionCharacter)
	{
		//True si dentro del rango, false si no
		return (positionCharacter >= posToTargetStart_I && positionCharacter <= posToTargetEnd_I);
	}

	// Calculate how many characters affect and type
	void GetRange(int target, vector<Character*>* arr);

public:
	SString name;
	SString description;


	SkillType type;

		//Gestion movimiento
		int movement; //0 si no te mueve, -x si te mueve hacia delante +x si te mueve hacia atras

		//Gestion potencia
		float multiplierDmg;
		int bonusPrecision;
		int bonusCritRate;
		int bonusCritDamage;
		//Variable de que tipo de stat usan para el escaldo?

		enum skillType //Quiza no haga falta
		{
			ATTACK=0,
			HEAL, 
			BUFF,
			DEBUFF,
		};

		//FUNCIONES
		bool CanBeUsed(int positionCharacter)
		{
			//True si dentro del rango, false si no
			return (positionCharacter >= posToTargetStart_I && positionCharacter <= posToTargetEnd_I);
		}

	private:

	




	//Posicion para hacer la skill (min 0 max 3)
	int posToUseStart_I; //Posicion mas baja donde puedes usar el ataque
	int posToUseEnd_I; //Posicion mas alta donde puedes hacer el ataque, si es solo una pues poner el mismo numero que la inicial

	// NOSE COMO PRETENDES USAR ESTO PERO VOY A INTENTAR COSAS A MI ROLLO AVER QUE TAL XD
	//Posicion que puedes targetear (quiza hay que meter si es area o no)
	CharacterType typeTargeted;
	int posToTargetStart_I; //Posicion mas baja donde puedes usar el ataque
	int posToTargetEnd_I; //Posicion mas alta donde puedes targetear el ataque, si es solo una pues poner el mismo numero que la inicial
	bool areaSkill; //Si es true al hacerse la skill targetea todo da igual que posicion hayas pulsado
	
	// Sara
	int range;
	 
	//Gestion movimiento
	int movement; //0 si no te mueve, -x si te mueve hacia delante +x si te mueve hacia atras
	Character* charaToMove;

private:

};

#endif // __SKILLS_H__