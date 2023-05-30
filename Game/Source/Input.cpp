#include "App.h"
#include "Input.h"
#include "Window.h"

#include "DialogueSystem.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"


#include "GuiManager.h"

#include "GuiButton.h"


#define MAX_KEYS 300

Input::Input() : Module()
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	gamepadState = new ButtonState[MAX_BUTTONS];
	memset(gamepadState, BUTTON_IDLE, sizeof(ButtonState) * MAX_BUTTONS);
	
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_Init(SDL_INIT_GAMECONTROLLER);
	sdl_controller = SDL_GameControllerOpen(0);

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	//Enable Unicode
	//SDL_EnableUNICODE(SDL_ENABLE);

	playerName = new PlayerInput("", MAX_PLAYER_CHARS, false);
	
	temp = "";
	getInput_B = false;

	backSpaceMax = false;
	coso = false;

	mouseSpeed_F = 0.4f;

	SDL_ShowCursor(true);	// To Do: to change to false? 

	SDL_StopTextInput();

	cursorIdleTex = app->tex->Load("Assets/Textures/cursor_select.png");
	cursorPressedTex = app->tex->Load("Assets/Textures/cursor_select_tap.png");
	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if(mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_KEYDOWN:
				if (getInput_B) { HandleInput(event, playerInput_S); }
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	SDL_GameControllerUpdate();

	controller.A = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_A);
	controller.B = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_B);
	controller.X = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_X);
	controller.Y = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_Y);
	controller.DPAD_DOWN = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	controller.DPAD_UP = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
	controller.DPAD_LEFT = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	controller.DPAD_RIGHT = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	controller.START = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_START);

	//BOTONES EXTRA PORSI
	controller.BACK = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_BACK);
	controller.GUIDE = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_GUIDE);
	controller.BUTTON_LEFTSTICK = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_LEFTSTICK);
	controller.BUTTON_RIGHTSTICK = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
	controller.BUTTON_LEFTSHOULDER = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	controller.BUTTON_RIGHTSHOULDER = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

	//SDL no tiene el equivalente de SDL_GetKeyboardState para mando, lo he hecho de esta manera para mapear todos los botones que tiene
	buttons[0] = controller.A;
	buttons[1] = controller.B;
	buttons[2] = controller.X;
	buttons[3] = controller.Y;
	buttons[4] = controller.BACK;
	buttons[5] = controller.GUIDE;
	buttons[6] = controller.START;
	buttons[7] = controller.BUTTON_LEFTSTICK;
	buttons[8] = controller.BUTTON_RIGHTSTICK;
	buttons[9] = controller.BUTTON_LEFTSHOULDER;
	buttons[10] = controller.BUTTON_RIGHTSHOULDER;
	buttons[11] = controller.DPAD_UP;
	buttons[12] = controller.DPAD_DOWN;
	buttons[13] = controller.DPAD_LEFT;
	buttons[14] = controller.DPAD_RIGHT;

	for (int i = 0; i < MAX_BUTTONS; ++i)
	{
		if (buttons[i] == 1.0)
		{
			if (gamepadState[i] == BUTTON_IDLE)
				gamepadState[i] = BUTTON_DOWN;
			else
				gamepadState[i] = BUTTON_REPEAT;
		}
		else
		{
			if (gamepadState[i] == BUTTON_REPEAT || gamepadState[i] == BUTTON_DOWN)
				gamepadState[i] = BUTTON_UP;
			else
				gamepadState[i] = BUTTON_IDLE;
		}
	}

	controller.j1_x = fabsf(SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTX)) > DEAD_ZONE ? SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTX) : 0.0f;
	controller.j1_y = fabsf(SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTY)) > DEAD_ZONE ? SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTY) : 0.0f;
	controller.j2_x = fabsf(SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTX)) > DEAD_ZONE ? SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTX) : 0.0f;
	controller.j2_y = fabsf(SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTY)) > DEAD_ZONE ? SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTY) : 0.0f;
	controller.RT = fabsf(SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) > DEAD_ZONE ? SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) : 0.0f;
	controller.LT = fabsf(SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) > DEAD_ZONE ? SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) : 0.0f;

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	RELEASE(playerName);
	app->tex->UnLoad(cursorIdleTex);
	app->tex->UnLoad(cursorPressedTex);
	//Disable Unicode
	//SDL_EnableUNICODE(SDL_DISABLE);
	return true;
}


bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}


bool Input::HandleInput(SDL_Event event, PlayerInput* playerInput)
{
	// If the string less than maximum size
	if (temp.length() < playerInput->max_chars)
	{
		//Append the character
		temp += (char)event.key.keysym.sym;
	}

	// If backspace was pressed and the string isn't blank
	if ((event.key.keysym.sym == SDLK_BACKSPACE) && !temp.empty())
	{
		if (backSpaceMax) 
		{
			temp.erase(temp.length() - 1);
			backSpaceMax = false;
		}

		// Remove a character from the end
		if(temp.length() > 0)
		{
			temp.erase(temp.length() - 1);
			backSpaceMax = true;
		}
	}

	if ((event.key.keysym.sym == SDLK_RETURN) && !temp.empty())
	{
		if (temp.length() < playerInput->max_chars) { temp.erase(temp.length() - 1); }

		// TODO Call Save name
		app->dialogueSystem->SaveDialogueState();	
		getInput_B = false;

		playerInput->input = temp;
		playerInput->input_entered = true;
		return true;
	}

	// Ignore shift
	if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT)
	{
		// Append the character
		temp.erase(temp.length() - 1);
	}
}

void Input::ActiveGetInput(PlayerInput* i)
{
	getInput_B = true;
	i->input_entered = false;
	playerInput_S = i;
}

void Input::RenderTempText(SString temp, const char* subs, iPoint pos, int fontsize, Font font, SDL_Color color)
{
	// Substitute("character to substitute", new characters)
	temp.Substitute("%", subs);
	app->render->TextDraw(temp.GetString(), pos.x, pos.y, fontsize, font, color);
}

void Input::HandleGamepadMouse(int mouseX, int mouseY, float mouseSpeed, float dt)
{
	int speed_X = 0; int speed_Y = 0;
	
	// TO DO: uncomment the if

	//if (!godMode_B)
	//{
		if (app->input->controller.j1_x > 0)
		{
			speed_X = mouseSpeed * dt;

			if (app->input->controller.j1_y > 0)
			{
				speed_Y = mouseSpeed * dt;
			}
			if (app->input->controller.j1_y < 0)
			{
				speed_Y = -mouseSpeed * dt;
			}
		}

		else if (app->input->controller.j1_x < 0)
		{
			speed_X = -mouseSpeed * dt;

			if (app->input->controller.j1_y > 0)
			{
				speed_Y = mouseSpeed * dt;
			}
			if (app->input->controller.j1_y < 0)
			{
				speed_Y = -mouseSpeed * dt;
			}
		}

		else if (app->input->controller.j1_y > 0)
		{
			speed_Y = mouseSpeed * dt;

			if (app->input->controller.j1_x > 0)
			{
				speed_X = mouseSpeed * dt;
			}
			if (app->input->controller.j1_x < 0)
			{
				speed_X = -mouseSpeed * dt;
			}
		}

		else if (app->input->controller.j1_y < 0)
		{
			speed_Y = -mouseSpeed * dt;
			if (app->input->controller.j1_x > 0)
			{
				speed_X = mouseSpeed * dt;
			}
			if (app->input->controller.j1_x < 0)
			{
				speed_X = -mouseSpeed * dt;
			}
		}

		//SDL_WarpMouseInWindow(app->win->window, mouseX + speed_X, mouseY + speed_Y);
	//}
}

