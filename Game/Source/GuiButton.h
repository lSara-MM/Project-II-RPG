#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Tweening.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, ButtonType bType, const char* text = "", int fontSize = 12, Font font = Font::UI);
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

	const char* fxHoverPath;
	uint fxHover;
	int hoverTest;

private:
	Tween animationButton;
};

#endif // __GUIBUTTON_H__