#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"

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
};

#endif // __GUIBUTTON_H__