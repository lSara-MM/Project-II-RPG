#ifndef __SKILLS_H__
#define __SKILLS_H__

#include "App.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Audio.h"
#include "Module.h"
#include "Render.h"
#include "Window.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

// prueba
class Skill
	{
	public:
		Skill() { name = " "; };
		~Skill();

		Skill(SString name,SString description, int posToUseStart_I,int posToUseEnd_I) {}
		SString name;
		SString description;
		
		
		//Posicion para hacer la skill (min 0 max 3)
		int posToUseStart_I; //Posicion mas baja donde puedes usar el ataque
		int posToUseEnd_I; //Posicion mas alta donde puedes hacer el ataque, si es solo una pues poner el mismo numero que la inicial

		//Posicion que puedes targetear (quiza hay que meter si es area o no)
		CharacterType typeTrgeted;
		int posToTargetStart_I; //Posicion mas baja donde puedes usar el ataque
		int posToTargetEnd_I; //Posicion mas alta donde puedes targetear el ataque, si es solo una pues poner el mismo numero que la inicial
		bool areaSkill; //Si es true al hacerse la skill targetea todo da igual que posicion hayas pulsado

		//Gestion movimiento
		int movement; //0 si no te mueve, -x si te mueve hacia delante +x si te mueve hacia atras

		enum skillType //
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

	};



#endif // __SKILLS_H__