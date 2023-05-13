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

	switch (bType)
	{
	case ButtonType::NONE:
		break;
	case ButtonType::EXTRA_LARGE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Generic.png");
		break;
	case ButtonType::START:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Start.png");
		break;
	case ButtonType::CLOSE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Close.png");
		break;
	case ButtonType::DIALOGUE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Dialogue.png");
		break;
	case ButtonType::SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::IN_SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonInSettings.png");
		break;
	case ButtonType::CONTROL_SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::INVENTORY:
		break;
	case ButtonType::COMBAT_TARGET:
		break;
	default:
		break;
	}

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

				if (hoverTest == false)
				{
					app->audio->PlayFx(fxHover);
					hoverTest = true;
				}
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
		if (buttonType == ButtonType::SETTINGS)
		{
			// Draw the right button depending on state
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 0, 0, 136, 50 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 0, 53, 136, 50 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 0, 105, 136, 50 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::SELECTED: 
				break;

			default:
				break;
			}
		}
		if (buttonType == ButtonType::IN_SETTINGS)
		{
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 0, 0, 155, 52 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 0, 53, 155, 52 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 0, 105, 155, 52 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::SELECTED: 
				break;

			default:
				break;
			}
		}
		if (buttonType == ButtonType::CONTROL_SETTINGS)
		{
			// Draw the right button depending on state
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 0, 0, 34, 34 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 0, 53, 34, 34 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 0, 105, 34, 34 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::SELECTED: 
				break;

			default:
				break;
			}
		}
		if (buttonType == ButtonType::CLOSE)
		{
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 1, 3, 57, 57 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 1, 66, 57, 57 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 1, 127, 57, 57 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::SELECTED:
				break;

			default:
				break;
			}
		}

		if (buttonType == ButtonType::EXTRA_LARGE)
		{
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 0, 0, 200, 50 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 0, 53, 200, 50 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 0, 105, 200, 50 };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::SELECTED:
				break;

			default:
				break;
			}
		}

		if (buttonType == ButtonType::DIALOGUE)
		{
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 5, 5, bounds.x, bounds.y };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 5, 65, bounds.x, bounds.y };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 5, 125, bounds.x, bounds.y };
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

			} break;

			case GuiControlState::SELECTED:
				break;

			default:
				break;
			}
		}
		if (buttonType == ButtonType::START)
		{
			switch (state)
			{

			case GuiControlState::DISABLED:
			{

			} break;

			case GuiControlState::NORMAL:
			{
				rect = { 5, 77, 20, 26 };
				render->DrawTexture(buttonTex, bounds.x+bounds.w/7, bounds.y+bounds.h/2.7, &rect);

			} break;

			case GuiControlState::FOCUSED:
			{
				rect = { 5, 5, 20, 26 };
				render->DrawTexture(buttonTex, bounds.x+ bounds.w / 7, bounds.y+ bounds.h / 2.7, &rect);

			} break;

			case GuiControlState::PRESSED:
			{
				rect = { 5, 41, 20, 26 };
				render->DrawTexture(buttonTex, bounds.x+ bounds.w / 7, bounds.y+ bounds.h / 2.7, &rect);

			} break;

			case GuiControlState::SELECTED:
				break;

			default:
				break;
			}
		}
	}



	if (buttonType == ButtonType::START )
	{
		if (text != "")
		{
			int offsetX = text.Length() * fontSize / 2;

			int x = (bounds.w - offsetX) / 2;
			int y = (bounds.h - fontSize) / 2;

			switch (state)
			{

			case GuiControlState::DISABLED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 200,200,200 });
			} break;

			case GuiControlState::NORMAL:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 163,163,163 });

			} break;

			case GuiControlState::FOCUSED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 249,224,58 });

			} break;

			case GuiControlState::PRESSED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 180,34,42 });

			} break;

			case GuiControlState::SELECTED:
				break;

			default:
				break;
			}
		}
		
	}

	else
	{
		int offsetX = text.Length() * fontSize / 2;

		int x = (rect.w - offsetX) / 2;
		int y = (rect.h - fontSize) / 2;
		if (text != "") app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font);
	}


	return false;
}

