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
	case ButtonType::LARGE:
		break;
	case ButtonType::SMALL:
		break;
	case ButtonType::START:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Start.png");
		break;
	case ButtonType::CLOSE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Close.png");
		break;
	case ButtonType::MENU:
		break;
	case ButtonType::SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::IN_SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonInSettings.png");
		break;
	case ButtonType::DIALOGUE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Dialogue.png");
		break;
	case ButtonType::CHECK:
		break;
	case ButtonType::CONTROL_SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
	case ButtonType::INVENTORY:
		break;
	case ButtonType::INV_NEXT_PAGE:
		break;
	case ButtonType::INV_PAGES:
		break;
	case ButtonType::SWAP_SKILL:
		break;
	case ButtonType::COMBAT_TARGET :
		break;
	case ButtonType::SKILL_1:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::SKILL_2:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::SKILL_3:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::SKILL_4:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	case ButtonType::CHANGE_POSITION:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/ButtonSettings.png");
		break;
	default:
		break;
	}

	fxHoverPath = "Assets/Audio/Fx/on_button.wav";
	fxHover = app->audio->LoadFx(fxHoverPath);
	hoverTest = false;
	isSelected = false;
}

GuiButton::~GuiButton()
{
	//delete buttonTex;
	app->tex->UnLoad(buttonTex);

	// TO DO: arreglo feo temporal? com se borra un boto, pregunta seria
	state = GuiControlState::NONE;
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

				if (state != GuiControlState::SELECTED)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
					{
						state = GuiControlState::PRESSED;
					}

					// If mouse button pressed -> Generate event!
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_UP)
					{
							NotifyObserverOfClick();
					}
				}


				state = GuiControlState::FOCUSED;
				NotifyObserverOfHover();

				if (hoverTest == false)
				{
					app->audio->PlayFx(fxHover);
					hoverTest = true;
				}
				if (previousState != state)
				{
					//LOG("Change state from %d to %d", previousState, state);
				}

			}
			else
			{
				(isSelected) ? state = GuiControlState::SELECTED : state = GuiControlState::NORMAL;
				hoverTest = false;
			}
		}
		else
		{
			if (state == GuiControlState::FOCUSED)
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
				NotifyObserverOfClick();
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

	if (buttonType == ButtonType::COMBAT_TARGET) { rect = { 0, 0, 48 * 1, 92 }; } //48 anchura de solo 1 corchete, 92 es la altura que tiene DE MOMENTO.
	else if(buttonType==ButtonType::START) { rect = { 5, 5, 20, 26 }; }
	else if (buttonType == ButtonType::DIALOGUE) { rect.x = 5; }

	// TO DO, EN DIALOGO LOS BOTONES DEBERIAN TENER MENOS OFFSET, ES DECIR, SE DEBERIAN PRINTAR CON JUSTIFICADO A LA IZQUIERDA, NO EN MEDIO
	// ADEMAS DE HACER LO DEL TRIM TEXT PARA QUE SI LA OPCION ES MAS LARGA SE PRINTE EN LINEAS DISTINTAS.
	// POR OTRO LADO INCLUSO EN CASO DE TENER UNA OPCION CON MAS DE UNA LINEA, QUIZAS INTERESARIA QUE EL FONTSIZE SEA MENOR PARA QUE SE ADAPTE 
	// A LA TEXTURA/BOUNDS DEL BOTON EN SI

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
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 0, 150, 50, 200, true, false);
			break;

		default:
			break;
		}
	}
	else
	{
		switch (state)
		{
		case GuiControlState::NONE:
			break;
		case GuiControlState::DISABLED:
			switch (buttonType)
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
				break;
			case ButtonType::CLOSE:
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				break;
			case ButtonType::IN_SETTINGS:
				break;
			case ButtonType::DIALOGUE:
				break;
			case ButtonType::CHECK:
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
			case ButtonType::COMBAT_TARGET:
				break;
			case ButtonType::SKILL_1:
				break;
			case ButtonType::SKILL_2:
				break;
			case ButtonType::SKILL_3:
				break;
			case ButtonType::SKILL_4:
				break;
			case ButtonType::CHANGE_POSITION:
				break;
			default:
				break;
			}
			break;
		case GuiControlState::NORMAL:
			switch (buttonType)
			{
			case ButtonType::NONE:
				break;
			case ButtonType::EXTRA_LARGE:
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::LARGE:
				break;
			case ButtonType::SMALL:
				break;
			case ButtonType::START:
				rect.y = 77;
				render->DrawTexture(buttonTex, bounds.x + bounds.w / 7, bounds.y + bounds.h / 2.7, &rect);
				break;
			case ButtonType::CLOSE:
				rect.y = 3;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::IN_SETTINGS:
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::DIALOGUE:
				rect.y = 5;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::CHECK:
				break;
			case ButtonType::CONTROL_SETTINGS:
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			case ButtonType::COMBAT_TARGET:
				break;
			case ButtonType::SKILL_1:
				break;
			case ButtonType::SKILL_2:
				break;
			case ButtonType::SKILL_3:
				break;
			case ButtonType::SKILL_4:
				break;
			case ButtonType::CHANGE_POSITION:
				break;
			default:
				break;
			}
			break;
		case GuiControlState::FOCUSED:
			switch (buttonType)
			{
			case ButtonType::NONE:
				break;
			case ButtonType::EXTRA_LARGE:
				rect.y = 53;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::LARGE:
				break;
			case ButtonType::SMALL:
				break;
			case ButtonType::START:
				rect.y = 5;
				render->DrawTexture(buttonTex, bounds.x + bounds.w / 7, bounds.y + bounds.h / 2.7, &rect);
				break;
			case ButtonType::CLOSE:
				rect.y = 66;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				rect.y = 53;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::IN_SETTINGS:
				rect.y = 53;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::DIALOGUE:
				rect.y = 65;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::CHECK:
				break;
			case ButtonType::CONTROL_SETTINGS:
				rect.y = 53;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			case ButtonType::COMBAT_TARGET:
				break;
			case ButtonType::SKILL_1:
				break;
			case ButtonType::SKILL_2:
				break;
			case ButtonType::SKILL_3:
				break;
			case ButtonType::SKILL_4:
				break;
			case ButtonType::CHANGE_POSITION:
				break;
			default:
				break;
			}
			break;
		case GuiControlState::PRESSED:
			switch (buttonType)
			{
			case ButtonType::NONE:
				break;
			case ButtonType::EXTRA_LARGE:
				rect.y = 105;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::LARGE:
				break;
			case ButtonType::SMALL:
				break;
			case ButtonType::START:
				rect.y = 41;
				render->DrawTexture(buttonTex, bounds.x + bounds.w / 7, bounds.y + bounds.h / 2.7, &rect);
				break;
			case ButtonType::CLOSE:
				rect.y = 127;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				rect.y = 105;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::IN_SETTINGS:
				rect.y = 105;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::DIALOGUE:
				rect.y = 125;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::CHECK:
				break;
			case ButtonType::CONTROL_SETTINGS:
				rect.y = 105;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			case ButtonType::COMBAT_TARGET:
				break;
			case ButtonType::SKILL_1:
				break;
			case ButtonType::SKILL_2:
				break;
			case ButtonType::SKILL_3:
				break;
			case ButtonType::SKILL_4:
				break;
			case ButtonType::CHANGE_POSITION:
				break;
			default:
				break;
			}
			break;
		case GuiControlState::SELECTED:
			break;
		default:
			break;
		}
	}


	if (text != "")
	{
		int offsetX = text.Length() * fontSize / 2;

		int x = (bounds.w - offsetX) / 2;
		int y = (bounds.h - fontSize) / 2;

		if (buttonType == ButtonType::START) {
			

			switch (state)
			{

			case GuiControlState::DISABLED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 50,50,50 });
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
			
		else
		{
			app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font);
		}

	}
	// TO TEST

	// Text
	//vector<SString> texts;
	//int max_chars_line = fontSize;
	//app->render->SplitText(text, &texts, fontSize, max_chars_line);

	//if (text != "")
	//{
		/*size_t lines = texts.size();
		for (size_t i = 0; i < lines; i++)
		{
			app->render->TextDraw(texts.at(i).GetString(), bounds.x + x, bounds.y + y + (fontSize + 3) * i, fontSize, font);
		}*/
		//app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font);
	//}

	return false;
}

