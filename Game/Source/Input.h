#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Render.h"
#include "SDL/include/SDL.h"
#include "Point.h"
#include "Timer.h"
#include "External/SDL/include/SDL_gamecontroller.h"

#define NUM_MOUSE_BUTTONS 5
#define DEAD_ZONE 32700
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
	Timer timer;
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
	
	// Active getting input
	// Needs the Struct of text input (ex. GetInput(playerName) will change playerName text)
	void ActiveGetInput(PlayerInput* i);

	// Render what player is writing, temporal
	void RenderTempText(SString temp, const char* subs, iPoint pos, int fontsize = 10, Font font = Font::TEXT, SDL_Color color = { 0,0,0 });

	void HandleGamepadMouse(int mouseX, int mouseY, float mouseSpeed, float dt);

	void RenderMouse();

public:

	PlayerInput* playerName;

	// Keep a copy of the current version of the string
	string temp;
	bool getInput_B;

	bool godMode_B = false;

	//Este bool hace que te borre una vez hacia atras, cuando haya superado el numero m�ximo de caracteres.
	bool backSpaceMax = false;

	SDL_GameController* sdl_controller;
	GameController controller;
	
	float mouseSpeed_F;

	int posX, posY;
	string sceneNameSaved;
	bool coso;
	SDL_Texture* cursorIdleTex;
	SDL_Texture* cursorPressedTex;
	bool stepSound_B = false;//controlar se escuche pasos personaje
	int mouseX;
	int mouseY;

private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];

	ButtonState* gamepadState;
	float buttons[MAX_BUTTONS];

	int	mouseMotionX;
	int mouseMotionY;

	// Handle input
	PlayerInput* playerInput_S;
};

#endif // __INPUT_H__