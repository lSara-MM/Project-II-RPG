#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Module.h"
#include "Render.h"
#include "Textures.h"

#include "Log.h"
#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class GuiControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class GuiControlState
{
	NONE,
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};


enum class ButtonType
{
	NONE,
	EXTRA_LARGE,
	LARGE,
	SMALL,

	START,
	CLOSE,
	MENU,
	SETTINGS,
	IN_SETTINGS,
	DIALOGUE,
	CHECK,
	CONTROL_SETTINGS,
	INVENTORY,
	INV_NEXT_PAGE,
	INV_PAGES,
	SWAP_SKILL,
	SETTINGS_TEXT,//para animacion texto en settings
	//For combat
	COMBAT_TARGET,
	SKILL_1,
	SKILL_2,
	SKILL_3,
	SKILL_4,
	CHANGE_POSITION,
	SKIPPY,
	PARTY,
	ITEM,
	BUYITEM,
	SELLITEM,
	ADD,
	MINUS,
};

enum class AnimationAxis
{
	DOWN_Y,
	UP_Y,
	LEFT_X,
	RIGHT_X,
	FLEE,
};


class GuiControl
{
public:

	GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text, int fontSize) :
		type(type),
		state(GuiControlState::NORMAL),
		bounds(bounds),
		text(text),
		fontSize(fontSize)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw(Render* render)
	{
		return true;
	}

	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	void SetObserver(Module* module)
	{
		observer = module;
	}

	void NotifyObserverOfClick()
	{
		observer->OnGuiMouseClickEvent(this);
	}

	void NotifyObserverOfHover()
	{
		observer->OnGuiMouseHoverEvent(this);
	}

	void NotifyObserverOutHover()
	{
		observer->OnGuiMouseOutHoverEvent(this);
	}

public:

	uint32 id;
	GuiControlType type;
	GuiControlState state;

	SString text;           // Control text (if required)
	SDL_Rect bounds;        // Position and size
	SDL_Color color;        // Tint color

	const char* fxPath;
	uint fxControl;


	SDL_Texture* texture;   // Texture atlas reference
	SDL_Rect section;       // Texture atlas base section

	//Font font;              // Text font
	int fontSize;

	Module* observer;        // Observer module (it should probably be an array/list)
};

#endif // __GUICONTROL_H__