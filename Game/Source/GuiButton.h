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
	bool CleanUp();

	bool Draw(Render* render);
	void DrawSkill(int charaId, int skillNumber, int state);//skill number = numero skill-1, state->normal=0, focused=1, pressed = 2

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

	const char* fxclickPath;
	uint fxclick;

	bool hoverTest;
	bool isSelected;
	int step;//velocidad animacion

private:
	Tween animationButton;
	AnimationAxis axisType;

	float point;
	int offset;

	vector<SString> texts;
};

#endif // __GUIBUTTON_H__