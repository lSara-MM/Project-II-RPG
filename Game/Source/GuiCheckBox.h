#ifndef __GUICheckBox_H__
#define __GUICheckBox_H__

#include "GuiControl.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Tweening.h"

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, int speed = 1, Easings eType = Easings::BACK_OUT, AnimationAxis axisType = AnimationAxis::DOWN_Y);
	virtual ~GuiCheckBox();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;

	SDL_Texture* checkBoxTex;

	bool isForward_B;
	int boundsY_AUX, boundsX_AUX;

private:

	Tween animationButton;
	int step;
	AnimationAxis axisType;
	float point;
	int offset;
};

#endif // __GUICheckBox_H__