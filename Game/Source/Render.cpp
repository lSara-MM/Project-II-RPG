#include "App.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = config.child("camera").attribute("x").as_int();
		camera.y = config.child("camera").attribute("y").as_int();
	}

	//initialise the SDL_ttf library
	TTF_Init();
	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{    
	// Free the font
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, SDL_RendererFlip flip) const
{
	bool ret = true;

	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		//LOG("Cannot blit to screen. SDL_RenderCopy error: %s,  %d, %d", SDL_GetError(), x, y);
		ret = false;
	}
	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}
	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}
	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(camera.x + x + radius * cos(i * factor));
		points[i].y = (int)(camera.y + y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}
	return ret;
}


bool Render::LoadState(pugi::xml_node& data, Module* module)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

bool Render::SaveState(pugi::xml_node& data)
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

bool Render::TextDraw(const char* text, int x, int y, int size, Font font, SDL_Color color)
{
	bool ret = true;

	//load a font into memory

	if (font == Font::UI)	{ ttf_font = TTF_OpenFont("Fonts/Milonga-Regular.ttf", size); }
	else if (font == Font::TEXT) { ttf_font = TTF_OpenFont("Fonts/FingerPaint-Regular.ttf", size); }
	else if (font == Font::TITLE) { ttf_font = TTF_OpenFont("Fonts/Circus.ttf", size); }
	
	if (!ttf_font)
	{
		LOG("Cannot open font. TTF_OpenFont error: %s", TTF_GetError());
		ret = false;
	}

	SDL_Color ttf_color;
	ttf_color.r = color.r;
	ttf_color.g = color.g;
	ttf_color.b = color.b;
	ttf_color.a = color.a;

	SDL_Rect ttf_rect;
	ttf_surface = TTF_RenderText_Solid(ttf_font, text, ttf_color);
	ttf_texture = SDL_CreateTextureFromSurface(renderer, ttf_surface);

	if (ttf_surface == nullptr)
	{
		LOG("Cannot open font. SDL_Surface* error: %s", SDL_GetError());
		ret = false;

		ttf_texture = nullptr;
		TTF_CloseFont(ttf_font);
	}
	else
	{
		ttf_rect.x = x * app->win->GetScale();
		ttf_rect.y = y * app->win->GetScale();
		ttf_rect.w = ttf_surface->w * app->win->GetScale();
		ttf_rect.h = ttf_surface->h * app->win->GetScale();

		SDL_FreeSurface(ttf_surface);
		if (SDL_RenderCopy(renderer, ttf_texture, NULL, &ttf_rect) != 0)
		{
			LOG("Cannot render text to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
		SDL_DestroyTexture(ttf_texture);
		ttf_texture = nullptr;
		TTF_CloseFont(ttf_font);
	}
	return ret;
}

void Render::SplitText(SString text_, vector<SString>* pTexts, int fontSize_, int max_chars_line_)
{
	string line = text_.GetString();

	for (int i = 0; i < pTexts->size(); i++)
	{
		pTexts->at(i).Clear();
	}
	pTexts->clear();
	pTexts->shrink_to_fit();

	if (text_.Length() > max_chars_line_)
	{
		int a = 0, b, startIndex = 0;
		int n = line.length() / max_chars_line_;
		for (int j = 0; j <= line.length() / max_chars_line_; j++)	// <= -> in case of decimal, get the round up number 
		{
			if (a != line.length())
			{
				a = max_chars_line_ + startIndex;
				if (a > line.size())
				{
					a = line.size() - startIndex;
				}
				if (a > 0)
				{
					int i = 0;
					do
					{
						b = line.find_first_of(arr.at(i++), a);
					} while (b == -1 && a < line.length() && i < arr.size());
					// If we reached the end of the word or the end of the input.
					string temp;
					temp.append(line, startIndex, ++b - startIndex);	// string text to append, int index start, int size of text to append
					pTexts->push_back(temp.c_str());
					startIndex = b;
				}
			}
		}
	}
	else
	{
		pTexts->push_back(line.c_str());
	}
}

bool Render::RenderTrimmedText(int x, int y, int offset, SString text, vector<SString>* pTexts, int fontSize_, int max_chars_line_, 
	SDL_Color color, Font font, float fontOffset, float dt_wait)
{
	bool ret = false;

	SplitText(text, pTexts, fontSize_, max_chars_line_);

	int lines = pTexts->size();
	if (fontOffset > 0) { fontSize_ = fontSize_ - lines * fontOffset; }

	if (dt_wait == 0)
	{
		for (int i = 0; i < lines; i++)
		{
			TextDraw(pTexts->at(i).GetString(), x, y + (fontSize_ + offset) * i, fontSize_, font, color);
		}

		for (int i = 0; i < pTexts->size(); i++)
		{
			pTexts->at(i).Clear();
		}

		pTexts->clear();
		pTexts->shrink_to_fit();

		return true;
	}
	else
	{
		int chars = 0;
		for (int i = 0; i < pTexts->size(); i++) { chars += pTexts->at(i).Length(); }

		string aux_str(text.GetString(), 0, charInNum);
		SString aux(aux_str.c_str());

		//LOG("waitToRender %f", waitToRender.ReadMSec());
		if (waitToRender.ReadMSec() > dt_wait && aux.Length() < chars)
		{
			charInNum++;
			SplitText(aux, &temp, fontSize_, max_chars_line_);

			waitToRender.Start();
		}
	
		if (aux.Length() < chars) { ret = false; }
		else { ret = true; }

		for (int j = 0; j < temp.size(); j++)
		{
			TextDraw(temp.at(j).GetString(), x, y + (fontSize_ + offset) * j, fontSize_, font, color);
		}
	}
	return ret;
}

void Render::ResetDtText()
{
	app->render->waitToRender.Start();
	app->render->charInNum = 1;

	for (int i = 0; i < temp.size(); i++)
	{
		temp.at(i).Clear();
	}

	temp.clear();
	temp.shrink_to_fit();
}

bool Render::VSyncOn()
{
	(vSync_B) ? flags = SDL_RENDERER_ACCELERATED : flags |= SDL_RENDERER_PRESENTVSYNC;
	return vSync_B;
}