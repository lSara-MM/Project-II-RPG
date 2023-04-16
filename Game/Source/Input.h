#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "SDL/include/SDL.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

#define NAME_MAX_CHARS 10

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

enum KeyType
{
	KEY_MOVE_UP,
	KEY_MOVE_LEFT,
	KEY_MOVE_RIGHT,
	KEY_MOVE_DOWN,
	KEY_INTERACT,
	KEY_INVENTORY,
	KEY_PARTY,
	KEY_QUESTS,
	KEY_MAP,
	KEY_SETTINGS
};

struct KeyBinding
{
	KeyType keyType;
	const char* key;		// character
	int key_num;	// scancode number

	KeyBinding(KeyType type, char num)
	{
		keyType = type;
		//key = num;
		key_num = (Uint16)num;
		//printf("%d", key_num);
		//printf(" scancode %d", SDL_SCANCODE_W);
	}

	KeyBinding(KeyType type, SDL_Scancode num)
	{
		keyType = type;
		key = SDL_GetScancodeName(num);
		key_num = num;
	}

	~KeyBinding() {};
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

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	void HandleInput(SDL_Event event);

public:
	string playerName;
	bool nameEntered_B = false;
	bool getInput_B = false;

	bool godMode_B = true;

private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;



	KeyBinding* move_up_k = new KeyBinding(KeyType::KEY_MOVE_UP, 'W');
	KeyBinding* move_left_k = new KeyBinding(KeyType::KEY_MOVE_UP, SDL_SCANCODE_A);
	KeyBinding* move_right_k;
	KeyBinding* move_down_k;
	KeyBinding* interact_k;

	KeyBinding* move_up_g;
	KeyBinding* move_left_g;
	KeyBinding* move_right_g;
	KeyBinding* move_down_g;
	KeyBinding* interact_g;


	KeyBinding* invetory_k;
	KeyBinding* party_k;
	KeyBinding* quests_k;
	KeyBinding* map_k;
	KeyBinding* settings_k;

	KeyBinding* invetory_g;
	KeyBinding* party_g;
	KeyBinding* quests_g;
	KeyBinding* map_g;
	KeyBinding* settings_g;
};

#endif // __INPUT_H__