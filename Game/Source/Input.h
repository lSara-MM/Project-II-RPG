#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Render.h"
#include "SDL/include/SDL.h"
#include "Point.h"

#include "External/SDL/include/SDL_gamecontroller.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

#define DEAD_ZONE 32000
#define MAX_BUTTONS 15

#define MAX_PLAYER_CHARS 10

using namespace std;

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum ButtonState
{
	BUTTON_IDLE = 0,
	BUTTON_DOWN,
	BUTTON_REPEAT,
	BUTTON_UP
};

struct GameController
{
	float j1_x, j1_y,j2_x, j2_y, RT, LT, A, B, X, Y, START, DPAD_UP, DPAD_DOWN, DPAD_RIGHT, DPAD_LEFT,
		GUIDE, BUTTON_LEFTSTICK, BUTTON_RIGHTSTICK, BUTTON_LEFTSHOULDER, BUTTON_RIGHTSHOULDER, BUTTON_MAX,BACK;

};

struct PlayerInput
{
public:
	PlayerInput(string input_, int max_chars_, bool entered_)
	{
		input = input_;
		max_chars = max_chars_;
		input_entered = entered_;
	}

public:
	string input;
	int max_chars;
	bool input_entered = false;
};


class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	ButtonState GetGamepadButton(int id) const
	{
		return gamepadState[id];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	bool HandleInput(SDL_Event event, PlayerInput* playerInput);
	void GetInput(PlayerInput* i);
	void RenderTempText(SString temp, const char* subs, iPoint pos, int fontsize = 10, Font font = Font::TEXT, SDL_Color color = { 0,0,0 });

	void HandleGamepadMouse(int mouseX, int mouseY, float mouseSpeed, float dt);

public:

	PlayerInput* playerName;

	// Keep a copy of the current version of the string
	string temp;
	bool getInput_B = false;

	bool godMode_B = false;

	// TO DELETE
	int currentHP_Bard = 0;
	int currentHP_Protagonist = 0;

	SDL_GameController* sdl_controller;
	GameController controller;
	
	bool gamepadGUI_B = false;

private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];

	ButtonState* gamepadState;
	float buttons[MAX_BUTTONS];

	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;

	// Handle input
	PlayerInput* playerInput_S;
};

#endif // __INPUT_H__