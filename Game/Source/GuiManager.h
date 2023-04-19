#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 bool Update(float dt);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, Module* observer, SDL_Rect bounds, ButtonType bType = ButtonType::NONE, const char* text = "", int fontSize = 12, Font font = Font::UI, SDL_Rect sliderBounds = {0,0,0,0});
	//void DestroyGuiControl(GuiControl* gui);
	//void AddGuiControl(GuiControl* gui);

public:

	List<GuiControl*> guiControlsList;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;

	bool GUI_debug = false;
};

#endif // __GUIMANAGER_H__
