#include "FadeToBlack.h"

#include "App.h"
#include "Window.h"
#include "Render.h"
#include "CutScene.h"
#include "PuzzleManager.h"

#include "SDL/include/SDL_render.h"
#include "Log.h"

FadeToBlack::FadeToBlack() : Module()
{
	name.Create("fadeToBlack");
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Awake(pugi::xml_node& config)
{
	LOG("Loading FadeToBlack");
	
	bool ret = true;
	screenRect = { 0, 0, app->win->GetWidth() * app->win->GetScale(), app->win->GetHeight()* app->win->GetScale() };
	
	return ret;
}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	modulesOnOff = false;
	ImagesOnOff = false;

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			if(modulesOnOff)
			{
				moduleToDisable->Disable();
				moduleToEnable->Enable();

				modulesOnOff = false;

				currentStep = Fade_Step::FROM_BLACK;
			}

			if(ImagesOnOff)
			{
				//Si se quiere a�adir otra script que necesite pasar entre dos imagenes debe ponerse aqu�
				if(app->cutScene->active)
					app->cutScene->currentTexture = ImageToEnter;

				if (app->puzzleManager->active)
				{
					app->puzzleManager->currentDark = ImageToEnter;
				}

				ImagesOnOff = false;

				currentStep = Fade_Step::FROM_BLACK;
			}
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::FadingToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if(currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

	
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		modulesOnOff = true;

		ret = true;
	}

	return ret;
}

bool FadeToBlack::FadingToBlackImages(SDL_Texture* ImageToPass, SDL_Texture* ImageToEnter, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;


		this->ImageToPass = ImageToPass;
		this->ImageToEnter = ImageToEnter;

		ImagesOnOff = true;

		ret = true;
	}

	return ret;
}