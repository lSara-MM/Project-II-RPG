#include "GuiButton.h"
#include "GuiManager.h"
#include "IntroScene.h"
#include "Combat.h"
GuiButton::GuiButton(uint32 id, SDL_Rect bounds, ButtonType bType, const char* text, int fontSize, Font font, int speed, Easings eType, AnimationAxis axisType) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->fontSize = fontSize;
	this->font = font;
	color = { 0, 0, 0 };
	
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
		buttonTex = app->tex->Load("Assets/GUI/UIArt/loseButton.png");
		break;
	case ButtonType::LARGE:
		break;
	case ButtonType::SMALL:
		break;
	case ButtonType::PARTY:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/charSelected.png");
		break;
	case ButtonType::START:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/Start.png");
		break;
	case ButtonType::CLOSE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsSettings.png");
		break;
	case ButtonType::MENU:
		break;
	case ButtonType::SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsSettings.png");
		break;
	case ButtonType::IN_SETTINGS:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsSettings.png");
		break;
	case ButtonType::DIALOGUE:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonDialogue.png");
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
		buttonTex = app->tex->Load("Assets/GUI/Target_To_Select.png");
		break;
	case ButtonType::SKILL_1:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsCombat.png");
		break;
	case ButtonType::SKILL_2:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsCombat.png");
		break;
	case ButtonType::SKILL_3:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsCombat.png");
		break;
	case ButtonType::SKILL_4:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsCombat.png");
		break;
	case ButtonType::CHANGE_POSITION:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsCombat.png");
		break;
	case ButtonType::SKIPPY:
		buttonTex = app->tex->Load("Assets/GUI/UIArt/buttonsCombat.png");
		break;
	case ButtonType::ITEM:
		buttonTex = app->tex->Load("Assets/GUI/cuadroPociones.png");
		break;
	case ButtonType::BUYITEM:
		buttonTex = app->tex->Load("Assets/GUI/Botton_Buy_Sell_MouseOn.png");
		break;
	case ButtonType::SELLITEM:
		buttonTex = app->tex->Load("Assets/GUI/Botton_Buy_Sell_MouseOn.png");
		break;
	case ButtonType::ADD:
		buttonTex = app->tex->Load("Assets/GUI/Botton_Buy_Sell_MouseOn.png");
		break;
	case ButtonType::MINUS:
		buttonTex = app->tex->Load("Assets/GUI/Botton_Buy_Sell_MouseOn.png");
		break;
	default:
		break;
	}

	fxHoverPath = "Assets/Audio/Fx/on_button.wav";
	fxHover = app->audio->LoadFx(fxHoverPath);

	fxclickPath = "Assets/Audio/Fx/Button_Menu.wav";
	fxclick = app->audio->LoadFx(fxclickPath);
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
		/*if (!app->input->gamepadGUI_B)
		{*/
			app->input->GetMousePosition(mouseX, mouseY);
			//LOG("Mouse x: %d Mouse y: %d", mouseX, mouseY);
			//LOG("bounds.x: %d bounds.h: %d", bounds.x, bounds.y);
			
			GuiControlState previousState = state;

			if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
				mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

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

				if (!isSelected)
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_REPEAT)
					{
						//app->audio->PlayFx(fxclick);TODO:aclaracion->aqui suena todo el rato mientras mantienes pulsado, tecnicamente no se ha hecho el pressed como tal, eso es cuando sueltas
						state = GuiControlState::PRESSED;
					}

					// If mouse button pressed -> Generate event!
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == ButtonState::BUTTON_UP)
					{
						app->audio->PlayFx(fxclick);
						NotifyObserverOfClick();
					}
				}
			}
			else if (hoverTest)
			{
				(isSelected) ? state = GuiControlState::SELECTED : state = GuiControlState::NORMAL;
				hoverTest = false;

				NotifyObserverOutHover();
			}
		/*}
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
		}*/
		
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

bool GuiButton::CleanUp()
{
	//delete buttonTex;
	app->tex->UnLoad(buttonTex);

	// TO DO: arreglo feo temporal? com se borra un boto, pregunta seria
	state = GuiControlState::NONE;

	return true;
}

bool GuiButton::Draw(Render* render)
{
	if (buttonType != ButtonType::ITEM)
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
		case AnimationAxis::FLEE:
			point = animationButton.GetPoint();
			offset = 75;
			bounds.y = offset + point * (boundsY_AUX - offset);
			break;
		default:
			break;
		}
	}

	SDL_Rect rect = { 0, 0, bounds.w, bounds.h };

	if (buttonType == ButtonType::COMBAT_TARGET) { rect = { 0, 0, 120, 12 }; } 
	else if (buttonType == ButtonType::START) { rect = { 5, 5, 20, 26 }; }
	else if (buttonType == ButtonType::SETTINGS) { rect = { 1, 1, 154, 66 }; }
	else if (buttonType == ButtonType::IN_SETTINGS) { rect = { 230, 0 , 230, 94}; }
	else if (buttonType == ButtonType::CLOSE) { rect = { 0, 200, 62, 63 }; }
	else if (buttonType == ButtonType::SKIPPY) { rect = { 451, 1, 132, 44 }; }
	else if (buttonType == ButtonType::DIALOGUE) { rect = { 0, 0, 371, 65 }; }
	else if (buttonType == ButtonType::EXTRA_LARGE) { rect.x = 5, rect.y = 5; }
	else if (buttonType == ButtonType::CHANGE_POSITION) { rect = { 401, 1, 47, 47 }; }

	int offsetX = text.Length() * fontSize / 2;

	int x = (bounds.w - offsetX) / 2;
	int y = (bounds.h - fontSize) / 2;

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
		{
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 0, 150, 50, 200, true, false);
		} break;

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
				if (app->combat->listInitiative.Count() > app->combat->charaInTurn)
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 0, 3);
				}
				break;
			case ButtonType::SKILL_2:
				if (app->combat->listInitiative.Count() > app->combat->charaInTurn)
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 1, 3);
				}
				break;
			case ButtonType::SKILL_3:
				if (app->combat->listInitiative.Count() > app->combat->charaInTurn)
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 2, 3);
				}
				break;
			case ButtonType::SKILL_4:
				if (app->combat->listInitiative.Count() > app->combat->charaInTurn)
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 3, 3);
				}
				break;
			case ButtonType::CHANGE_POSITION:
				rect.y = 101;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SKIPPY:
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
				render->DrawTexture(buttonTex, bounds.x-app->render->camera.x, bounds.y-app->render->camera.y, &rect);
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
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::IN_SETTINGS:
				rect.y = 191;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x-42, bounds.y - app->render->camera.y-22, &rect);
				break;
			case ButtonType::DIALOGUE:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::CHECK:
				break;
			case ButtonType::CONTROL_SETTINGS:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::ITEM:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y);
				break;
			case ButtonType::BUYITEM:
				rect = { 0, 0, 175, 69 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SELLITEM:
				rect = { 0, 0, 175, 69 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::ADD:
				rect = { 0, 69, 44, 25 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::MINUS:
				rect = { 132, 69, 44, 25 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			case ButtonType::COMBAT_TARGET:
				render->DrawTexture(buttonTex, bounds.x, bounds.y + bounds.h + 7, &rect);
				break;
			case ButtonType::SKILL_1:
				if (app->combat->charaInTurn<app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 0, 0);
				}
				break;
			case ButtonType::SKILL_2:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 1, 0);
				}
				break;
			case ButtonType::SKILL_3:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 2, 0);
				}
				break;
			case ButtonType::SKILL_4:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 3, 0);
				}
				break;
			case ButtonType::CHANGE_POSITION:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SKIPPY:
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
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
				rect.y = 65;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
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
				rect.x = 62;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				rect.y = 67;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::IN_SETTINGS:
				rect.y = 0;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x - 42, bounds.y - app->render->camera.y - 22, &rect);
				break;
			case ButtonType::DIALOGUE:
				rect.y = 67;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::CHECK:
				break;
			case ButtonType::CONTROL_SETTINGS:
				rect.y = 53;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::ITEM:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y);
				break;
			case ButtonType::BUYITEM:
				rect = { 175, 0, 174, 69 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SELLITEM:
				rect = { 175, 0, 174, 69 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::ADD:
				rect = { 44, 69, 44, 25 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::MINUS:
				rect = { 176, 69, 44, 25 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			case ButtonType::COMBAT_TARGET:
				rect.y = 12;
				render->DrawTexture(buttonTex, bounds.x, bounds.y + bounds.h + 7, &rect);
				break;
			case ButtonType::SKILL_1:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 0, 1);
				}
				break;
			case ButtonType::SKILL_2:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 1, 1);
				}
				break;
			case ButtonType::SKILL_3:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 2, 1);
				}
				break;
			case ButtonType::SKILL_4:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 3, 1);
				}
				break;
			case ButtonType::CHANGE_POSITION:
				rect.y = 51;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SKIPPY:
				rect.y = 45;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
				break;
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
				rect.y = 125;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
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
				rect.x = 124;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::MENU:
				break;
			case ButtonType::SETTINGS:
				rect.y = 132;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::IN_SETTINGS:
				rect.y = 96;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x - 42, bounds.y - app->render->camera.y - 22, &rect);
				break;
			case ButtonType::DIALOGUE:
				rect.y = 134;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::CHECK:
				break;
			case ButtonType::CONTROL_SETTINGS:
				rect.y += (2*bounds.h);
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::INVENTORY:
				break;
			case ButtonType::ITEM:
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y);
				break;
			case ButtonType::BUYITEM:
				rect = { 350, 0, 175, 69 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SELLITEM:
				rect = { 350, 0, 175, 69 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::ADD:
				rect = { 88, 69, 44, 25 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::MINUS:
				rect = { 220, 69, 44, 25 };
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::INV_NEXT_PAGE:
				break;
			case ButtonType::INV_PAGES:
				break;
			case ButtonType::SWAP_SKILL:
				break;
			case ButtonType::COMBAT_TARGET:
				rect.y = 24;
				render->DrawTexture(buttonTex, bounds.x, bounds.y + bounds.h + 7, &rect);
				break;
			case ButtonType::SKILL_1:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 0, 2);
				}
				break;
			case ButtonType::SKILL_2:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 1, 2);
				}
				break;
			case ButtonType::SKILL_3:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 2, 2);
				}
				break;
			case ButtonType::SKILL_4:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 3, 2);
				}
				break;
			case ButtonType::CHANGE_POSITION:
				rect.y = 101;
				render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);
				break;
			case ButtonType::SKIPPY:
				rect.y = 89;
				render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
			default:
				break;
			}
			break;
		case GuiControlState::SELECTED:
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
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 0, 3);
				}
				break;
			case ButtonType::SKILL_2:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 1, 3);
				}
				break;
			case ButtonType::SKILL_3:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 2, 3);
				}
				break;
			case ButtonType::SKILL_4:
				if (app->combat->charaInTurn < app->combat->listInitiative.Count())
				{
					DrawSkill(app->combat->listInitiative.At(app->combat->charaInTurn)->data->id, 3, 3);
				}
				break;
			case ButtonType::CHANGE_POSITION:
				break;
			case ButtonType::SKIPPY:
				break;
			case ButtonType::PARTY:
				rect = { 0, 0, 164, 248};
				render->DrawTexture(buttonTex, bounds.x-app->render->camera.x, bounds.y-app->render->camera.y, &rect);
				break;
			default:
				break;
			}
		}
	}

	if (text != "")
	{
		if (buttonType == ButtonType::START) {
			switch (state)
			{
			case GuiControlState::DISABLED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 50, 50, 50 });
			} break;

			case GuiControlState::NORMAL:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 163, 163, 163 });

			} break;

			case GuiControlState::FOCUSED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 249, 224, 58 });

			} break;

			case GuiControlState::PRESSED:
			{
				app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, { 180, 34, 42 });

			} break;

			case GuiControlState::SELECTED:
				break;

			default:
				break;
			}
		}			
		else if (buttonType == ButtonType::DIALOGUE)
		{
			offsetX = fontSize * 2.75f;

			int offsetY = fontSize;
			int max_chars_line = 31;

			app->render->RenderTrimmedText(bounds.x + offsetX, bounds.y + offsetY, 2, text, &texts, fontSize, max_chars_line, color, Font::UI, 2.5f);
		}
		else if (buttonType == ButtonType::SKIPPY && state == GuiControlState::DISABLED)
		{
			//no printar nada si disabled
		}
		else
		{
			if (buttonType == ButtonType::SETTINGS_TEXT)//no hace falta centre el texto en este caso
			{
				x = 0; y = 0;
			}
			app->render->TextDraw(text.GetString(), bounds.x + x, bounds.y + y, fontSize, font, color);
			
		}
	}
	return false;
}

void GuiButton::DrawSkill(int charaId, int skillNumber, int state)
{
	SDL_Rect rect = { 201, 201, 47, 47 };

	switch (charaId)
	{
	case 0:
		rect.x += 50*state;
		rect.y += 50*skillNumber;
		break;
	case 1:
		rect.x += 50*state;
		rect.y = 1+50*skillNumber;
		break;
	case 2:
		rect.x = 1 + 50 * state;
		rect.y +=  50 * skillNumber;
		break;
	case 3:
		rect.x = 1 + 50 * state;
		rect.y = 1 + 50 * skillNumber;
		break;
	default:
		break;
	}
	app->render->DrawTexture(buttonTex, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y, &rect);

}

