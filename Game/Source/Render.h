#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Timer.h"
#include "Point.h"
#include "SString.h"
#include <vector>
#include <array>

#include "SDL/include/SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"

using namespace std;

enum Font
{
	UI,
	TEXT,
	TITLE
};


class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

	// Text, x, y, size, {color}
	bool TextDraw(const char* text, int x, int y, int size, Font font = Font::UI, SDL_Color color = { 0, 0, 0 });
	// Text to split, vector to store trimmed text, fontsize, max characters in line
	void SplitText(SString text, vector<SString>* pTexts, int fontSize_, int max_chars_line_);
	// Call draw text of a trimmed text
	void RenderTrimmedText(int x, int y, int offset, SString text, vector<SString>* pTexts, int fontSize_, int max_chars_line_, 
		float fontOffset = 0, Font font = Font::UI, float dt_wait = 0, SDL_Color color = { 0, 0, 0 });
	void ResetDtText();

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Load / Save methods
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	bool VSyncOn();

public:
	bool vSync_B;

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

	// Rendering text
	SDL_Surface* ttf_surface = nullptr;
	SDL_Texture* ttf_texture = nullptr;
	TTF_Font* ttf_font = nullptr;

private:
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	array <char, 6> arr = {' ', '.', ',', '?', '!', '\n'};

	// Text render char by char
	int charInNum = 1;
	Timer waitToRender;
	vector <SString> temp;
};

#endif // __RENDER_H__