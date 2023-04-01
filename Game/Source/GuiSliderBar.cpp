#include "GuiSliderBar.h"
#include "GuiManager.h"

GuiSliderBar::GuiSliderBar(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds) : GuiControl(GuiControlType::SLIDERBAR, id)
{
	this->bounds = bounds;
	this->sliderBounds = sliderBounds;

	SliderBarTex = app->tex->Load("Assets/GUI/UI_bars.png");
}

GuiSliderBar::~GuiSliderBar()
{
	delete SliderBarTex;
}

bool GuiSliderBar::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);
		//LOG("Mouse x: %d Mouse y: %d", mouseX, mouseY);
		//LOG("bounds.x: %d bounds.h: %d", bounds.x, bounds.y);

		GuiControlState previousState = state;

		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (previousState != state)
			{
				//LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				sliderBounds.x = mouseX;	volume = bounds.x + mouseX - 500;

				volume100 = volume * SDL_MIX_MAXVOLUME / 60;
				//LOG("volume %d, %d", volume, volume100);
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiSliderBar::Draw(Render* render)
{
	SDL_Rect button_rect = { 351, 0, 41, 60 };
	SDL_Rect slider_static_rect = { 0, 14, 350, 13 };
	SDL_Rect slider_dynamic_rect = { 0, 0, sliderBounds.w / 2 - sliderBounds.x, 13 };

	if (app->guiManager->GUI_debug)
	{
		// Draw the right button depending on state
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			render->DrawRectangle({ bounds.x, bounds.y - bounds.h / 2, bounds.w, bounds.h }, 200, 200, 200, 255, false, false);
			render->DrawRectangle({ sliderBounds.x, sliderBounds.y - sliderBounds.h / 2, sliderBounds.w, sliderBounds.h }, 200, 200, 200, 255, true, false);

		} break;

		case GuiControlState::NORMAL:
		{
			render->DrawRectangle({ bounds.x, bounds.y - bounds.h / 2, bounds.w, bounds.h }, 0, 0, 255, 255, false, false);
			render->DrawRectangle({ sliderBounds.x, sliderBounds.y - sliderBounds.h / 2, sliderBounds.w, sliderBounds.h }, 0, 0, 255, 255, true, false);

		}	break;

		case GuiControlState::FOCUSED:
		{
			render->DrawRectangle({ bounds.x, bounds.y - bounds.h / 2, bounds.w, bounds.h }, 255, 0, 255, 255, false, false);
			render->DrawRectangle({ sliderBounds.x, sliderBounds.y - sliderBounds.h / 2, sliderBounds.w, sliderBounds.h }, 255, 0, 255, 255, true, false);

		} break;

		case GuiControlState::PRESSED:
		{
			render->DrawRectangle({ bounds.x, bounds.y - bounds.h / 2, bounds.w, bounds.h }, 0, 255, 0, 255, false, false);
			render->DrawRectangle({ sliderBounds.x,sliderBounds.y - sliderBounds.h / 2, sliderBounds.w, sliderBounds.h }, 0, 255, 0, 255, true, false);

		} break;

		case GuiControlState::SELECTED:
			break;

		default:
			break;
		}
	}
	else
	{
		render->DrawTexture(SliderBarTex, bounds.x - 3, bounds.y - 5, &slider_static_rect);
		render->DrawTexture(SliderBarTex, bounds.x - 3, bounds.y - 5, &slider_dynamic_rect);

		// Draw the right SliderBar depending on state
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			//render->DrawTexture(SliderBarTex, sliderBounds.x, sliderBounds.y, &rect);

		} break;

		case GuiControlState::NORMAL:
		{
			render->DrawTexture(SliderBarTex, sliderBounds.x, sliderBounds.y - sliderBounds.h / 2, &button_rect);

		} break;

		case GuiControlState::FOCUSED:
		{
			render->DrawTexture(SliderBarTex, sliderBounds.x, sliderBounds.y - sliderBounds.h / 2, &button_rect);

		} break;

		case GuiControlState::PRESSED:
		{
			render->DrawTexture(SliderBarTex, sliderBounds.x, sliderBounds.y - sliderBounds.h / 2, &button_rect);

		} break;

		case GuiControlState::SELECTED:
			break;

		default:
			break;
		}
	}

	return false;
}