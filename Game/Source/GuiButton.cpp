#include "GuiButton.h"
#include "GuiManager.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, ButtonType bType, const char* text, int fontSize, Font font) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->fontSize = fontSize;
	this->font = font;

	buttonType = bType;

	buttonTex = app->tex->Load("Assets/GUI/UI_buttons.png");
	//Dos Opciones, o cargar de un PNG distinto esas texturas si es el botton de combate o modificar el UI_buttons
}

GuiButton::~GuiButton()
{
	delete buttonTex;
}

bool GuiButton::Update(float dt)
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

bool GuiButton::Draw(Render* render)
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	int offsetX = 0; int offsetY = 0;

	if (buttonType == ButtonType::EXTRA_LARGE) { rect = { 0, 0, 93, 118 }; offsetX = 50;	offsetY = 10; }
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
	if (buttonType == ButtonType::SWAP_SKILL) { rect = { 90, 0, 82, 80 }; }

	if (buttonType == ButtonType::COMBAT_TARGET) { rect = { 0,0,48* 1,92  }; } //48 anchura de solo 1 corchete, 92 es la altura que tiene DE MOMENTO.

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
			render->DrawRectangle({ bounds.x, bounds.y, bounds.w * 2, bounds.h * 2 }, 0, 255, 0, 200, true, false);
			if (buttonType == ButtonType::EXTRA_LARGE) { rect = { 343, 0, 210, 80 }; }
			if (buttonType == ButtonType::LARGE) { rect = { 180, 0, 120, 40 }; }
			if (buttonType == ButtonType::SMALL) { rect = { 0, 0, 56, 41 }; }

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
			}
			
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


	int size = fontSize;
	int x = rect.w - text.Length() * size / 2 - offsetX;
	int y = rect.h - size / 2 + offsetY;

	if(text != "") app->render->TextDraw(text.GetString(), bounds.x + x / 4, bounds.y + y / 4, size, font);

	return false;
}