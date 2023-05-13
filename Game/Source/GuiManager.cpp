#include "GuiManager.h"

#include "App.h"
#include "Audio.h"
#include "Textures.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

#include "Tweening.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, Module* observer, SDL_Rect bounds, ButtonType bType, const char* text, int fontSize, Font font, SDL_Rect sliderBounds, int speed, Easings eType, AnimationAxis axisType)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = (GuiControl*) new GuiButton(id, bounds, bType,  text, fontSize, font, speed, eType, axisType);
		break;
	case GuiControlType::TOGGLE:
		break;
	case GuiControlType::CHECKBOX:
		guiControl = (GuiControl*) new GuiCheckBox(id, bounds,speed,eType, axisType);
		break;
	case GuiControlType::SLIDER:
		break;
	case GuiControlType::SLIDERBAR:
		guiControl = (GuiControl*) new GuiSliderBar(id, bounds, sliderBounds, speed, eType, axisType);
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
	default:
		break;
	}

	//Set the observer
	guiControl->SetObserver(observer);

	guiControl->fxPath = "Assets/Audio/Fx/Button_Menu.wav";
	guiControl->fxControl = app->audio->LoadFx(guiControl->fxPath);

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList.start;

		while (control != nullptr)
		{
			if (control->data->state != GuiControlState::NONE) { control->data->Update(dt); }
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if (control->data->state != GuiControlState::NONE) { control->data->Draw(app->render); }
		control = control->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	/*while (control != nullptr)
	{
		RELEASE(control);
	}*/

	guiControlsList.Clear();

	return true;
}



