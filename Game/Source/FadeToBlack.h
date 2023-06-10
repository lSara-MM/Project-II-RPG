#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "Tweening.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;

class FadeToBlack : public Module
{
public:
	//Constructor
	FadeToBlack();

	//Destructor
	~FadeToBlack();

	bool Awake(pugi::xml_node& config);
	// Called when the module is activated
	// Enables the blending mode for transparency
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the fade logic
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Performs the render call of a black rectangle with transparency
	bool PostUpdate() override;

	bool CleanUp() override;

	// Called from another module
	// Starts the fade process which has two steps, fade_out and fade_in
	// After the first step, the modules should be switched
	bool FadingToBlack(Module* toDisable, Module* toEnable, float frames = 60);

	bool FadingToBlackImages(SDL_Texture* ImageToPass, SDL_Texture* ImageToEnter, float frames = 60);

private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;

	bool modulesOnOff;
	bool ImagesOnOff;

	const char* texturepathIzq;
	const char* texturepathDer;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;

	// The modules that should be switched after the first step
	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;	
	
	SDL_Texture* ImageToPass = nullptr;
	SDL_Texture* ImageToEnter = nullptr;
	SDL_Texture* TransitionBG_Izq = nullptr;
	SDL_Texture* TransitionBG_Der = nullptr;

	pugi::xml_node configNode;

private:
	bool transition_B;
	Tween backgroundAnimation;
	int posYani_I;
	bool returnTitle_B;
};

#endif //__FADETOBLACKH__