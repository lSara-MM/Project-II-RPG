#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Tweening.h"
#include <vector>

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, ButtonType bType, const char* text = "", int fontSize = 12, Font font = Font::UI, int speed = 1, Easings eType = Easings::DEF, AnimationAxis axisType= AnimationAxis::DOWN_Y);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;

	SDL_Texture* buttonTex;
	ButtonType buttonType;
	Font font;
	//control animation
	bool isForward_B;

	int boundsY_AUX;
	int boundsX_AUX;

	const char* fxHoverPath;
	uint fxHover;
	bool hoverTest;

private:
	Tween animationButton;
	int step;
	AnimationAxis axisType;

	float point;
	int offset;
};

#endif // __GUIBUTTON_H__