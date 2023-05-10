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

//bool Skill::PosCanBeUsed(int pos)
//{
//}

// idea WIP
void Skill::GetRange(int target, vector<Character*>* arr)
{
	arr->at(target);
}
