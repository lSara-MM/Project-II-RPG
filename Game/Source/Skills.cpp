#include "Skills.h"

#include "App.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Audio.h"
#include "Module.h"
#include "Render.h"
#include "Window.h"
#include "Character.h"
#include "Combat.h"

Skill::Skill()
{
	name = " ";
};

Skill::~Skill()
{
}

Skill::Skill(SString name, SString description, int posToUseStart_I, int posToUseEnd_I) 
{
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
