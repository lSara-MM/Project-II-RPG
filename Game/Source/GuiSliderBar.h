#ifndef __GUI_SLIDERBAR_H__
#define __GUI_SLIDERBAR_H__

#include "GuiControl.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Tweening.h"

class GuiSliderBar : public GuiControl
{
public:

	GuiSliderBar(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds, int speed = 1, Easings eType = Easings::BACK_OUT, AnimationAxis axisType=AnimationAxis::DOWN_Y);
	virtual ~GuiSliderBar();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	SDL_Rect sliderBounds;

	int volume, volume100;

	SDL_Texture* SliderBarTex;

	bool isForward_B;

	int boundsY_AUX, boundsX_AUX, sliderBoundsY_AUX, sliderBoundsX_AUX;

private:
	Tween animationButton;
	int step;
	AnimationAxis axisType;
	float point;
	int offset;
};

#endif // __GUI_SLIDERBAR_H__