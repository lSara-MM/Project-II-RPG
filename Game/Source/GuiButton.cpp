#include "GuiButton.h"
#include "GuiManager.h"
#include "IntroScene.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, ButtonType bType, const char* text, int fontSize, Font font, int speed, Easings eType, AnimationAxis axisType) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->fontSize = fontSize;
	this->font = font;
	
	this->step = speed;//velocidad actualiza animacion

	this->axisType = axisType;

	boundsY_AUX = this->bounds.y;//auxliar controlar movimiento animacion
	boundsX_AUX = this->bounds.x;

	isForward_B = true;
	animationButton.Set();
	animationButton.AddTween(100, 80, eType);

	buttonType = bType;

	buttonTex = app->tex->Load("Assets/GUI/UI_buttons.png");

	fxHoverPath = "Assets/Audio/Fx/on_button.wav";
	fxHover = app->audio->LoadFx(fxHoverPath);
	hoverTest = false;
}

GuiButton::~GuiButton()
{
	delete buttonTex;
}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		if (!app->input->gamepadGUI_B)
		{
			app->input->GetMousePosition(mouseX, mouseY);
			//LOG("Mouse x: %d Mouse y: %d", mouseX, mouseY);
			//LOG("bounds.x: %d bounds.h: %d", bounds.x, bounds.y);

			GuiControlState previousState = state;

			if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
				mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

				state = GuiControlState::FOCUSED;
				if (hoverTest == false)
				{
					app->audio->PlayFx(fxHover);
					hoverTest = true;
				}
				if (previousState != state)
				{
					//LOG("Change state from %d to %d", previousState, state);
				}

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
				{
					state = GuiControlState::PRESSED;
				}

				// If mouse button pressed -> Generate event!
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_UP)
				{
					NotifyObserver();
				}
			}

			else
			{
				state = GuiControlState::NORMAL;
				hoverTest = false;
			}
		}

		else
		{
			if (state==GuiControlState::FOCUSED)
			{
				if (hoverTest == false)
				{
					app->audio->PlayFx(fxHover);
					hoverTest == true;
				}
				if (app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
				{
					state = GuiControlState::PRESSED;
				}
				
			}

			if (state == GuiControlState::PRESSED && app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_UP)
			{
				NotifyObserver();
				state = GuiControlState::FOCUSED;
			}
		}
		
	}

	if (isForward_B)
	{
		animationButton.Foward();
	}

	else
	{
		animationButton.Backward();
	}

	animationButton.Step(step, false);


	return false;
}

bool GuiButton::Draw(Render* render)
{
	switch (axisType)
	{
	case AnimationAxis::DOWN_Y:
		point = animationButton.GetPoint();
		offset = -750;
		bounds.y = offset + point * (boundsY_AUX - offset);
		break;
	case AnimationAxis::UP_Y:
		point = animationButton.GetPoint();
		offset = 750;
		bounds.y = offset + point * (boundsY_AUX - offset);
		break;
	case AnimationAxis::LEFT_X:
		point = animationButton.GetPoint();
		offset = 1300;
		bounds.x = offset + point * (boundsX_AUX - offset);
		break;
	case AnimationAxis::RIGHT_X:
		point = animationButton.GetPoint();
		offset = -1300;
		bounds.x = offset + point * (boundsX_AUX - offset);
		break;
	default:
		break;
	}

	SDL_Rect rect = { 0, 0, bounds.w, bounds.h };

	/*if (buttonType == ButtonType::EXTRA_LARGE) { rect = { 0, 0, 93, 118 }; offsetX = 50;	offsetY = 10; }
	if (buttonType == ButtonType::LARGE) { rect = { 0, 0, 93, 118 }; offsetX = 20;	offsetY = 3; }
	if (buttonType == ButtonType::SMALL) { rect = { 0, 0, 93, 118 }; offsetX = -15;	offsetY = 6; }

	if (buttonType == ButtonType::START) { rect = { 0, 0, 93, 118 }; offsetX = 50;	offsetY = 10; }
	if (buttonType == ButtonType::CLOSE) { rect = { 188, 89, 100, 96 }; offsetX = 50;	offsetY = 10; }
	if (buttonType == ButtonType::MENU) { rect = { 94, 39, 93, 118 }; offsetX = 20;	offsetY = 3; }
	if (buttonType == ButtonType::SETTINGS) { rect = { 90, 0, 26, 28 }; offsetX = -15;	offsetY = 6; }
	if (buttonType == ButtonType::IN_SETTINGS) { rect = { 90, 0, 26, 28 }; offsetX = -15;	offsetY = 6; }
	if (buttonType == ButtonType::CONTROL_SETTINGS) { rect = { 90, 0, 89, 90 }; offsetX = 50;	offsetY = 10; }
	if (buttonType == ButtonType::INVENTORY) { rect = { 188, 0, 89, 88 }; offsetX = 50;	offsetY = 10; }
	if (buttonType == ButtonType::INV_NEXT_PAGE) { rect = { 188, 189, 53, 57 }; offsetX = 20;	offsetY = 3; }
	if (buttonType == ButtonType::INV_PAGES) { rect = { 156, 160, 19, 20 }; offsetX = -15;	offsetY = 6; }
	if (buttonType == ButtonType::SWAP_SKILL) { rect = { 90, 0, 82, 80 }; }*/

	if (buttonType == ButtonType::COMBAT_TARGET) { rect = { 0, 0, 48 * 1, 92 }; } //48 anchura de solo 1 corchete, 92 es la altura que tiene DE MOMENTO.

	

	if (app->guiManager->GUI_debug)
	{
		// Draw the right button depending on state
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 200, 200, 200, 200, true, false);

		} break;

		case GuiControlState::NORMAL:
		{
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h}, 0, 0, 255, 200, true, false);

		}	break;

		case GuiControlState::FOCUSED:
		{
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 255, 0, 255, 200, true, false);
		} break;

		case GuiControlState::PRESSED:
		{
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 0, 255, 0, 200, true, false);

		} break;

		case GuiControlState::SELECTED:
			break;

		default:
			break;
		}
	}
	else
	{
		// Draw the right button depending on state
		switch (state)
		{

		case GuiControlState::DISABLED:
		{
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

		} break;

		case GuiControlState::NORMAL:
		{
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

		} break;

		case GuiControlState::FOCUSED:
		{
			/*switch (buttonType)
			{
			case ButtonType::NONE:
				break;
			case ButtonType::EXTRA_LARGE:
				break;
			case ButtonType::LARGE:
				break;
			case ButtonType::SMALL:
				break;
			case ButtonType::START:
				rect = { 94, 39, 93, 50 };
				break;
			case ButtonType::CLOSE:
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				rect = { 94, 39, 93, 118 };
				break;
			case ButtonType::IN_SETTINGS:
				break;
			case ButtonType::CONTROL_SETTINGS:
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			default:
				break;
			}*/
			
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

		} break;

		case GuiControlState::PRESSED:
		{
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

		} break;

		case GuiControlState::SELECTED: //render->DrawRectangle(bounds, 0, 255, 0, 255);
			break;

		default:
			break;
		}
	}

	int offsetX = text.Length() * fontSize / 2;
	
	int x = (rect.w - offsetX) / 2;
	int y = (rect.h - fontSize) / 2;

	if(text != "") app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font);

	return false;
}

