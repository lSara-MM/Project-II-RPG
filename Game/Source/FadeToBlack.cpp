#include "FadeToBlack.h"

#include "App.h"
#include "Window.h"
#include "Render.h"
#include "CutScene.h"
#include "PuzzleManager.h"

#include "Combat.h"
#include "LogoScene.h"

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
	
	texturepathIzq = config.attribute("texturepathIZQ").as_string();
	texturepathDer = config.attribute("texturepathDER").as_string();

	bool ret = true;
	screenRect = { 0, 0, app->win->GetWidth() * app->win->GetScale(), app->win->GetHeight()* app->win->GetScale() };

	backgroundAnimation.Set();
	backgroundAnimation.AddTween(100, 30, CUBIC_OUT);
	posYani_I = 0;
	
	return ret;
}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	modulesOnOff = false;
	ImagesOnOff = false;

	transition_B = false;
	TransitionBG_Izq = app->tex->Load(texturepathIzq);
	TransitionBG_Der = app->tex->Load(texturepathDer);

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	//app->audio->PlayMusic(winMusicPath);

	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			if(modulesOnOff)
			{
				transition_B = true;
				moduleToDisable->Disable();
				moduleToEnable->Enable();

				modulesOnOff = false;

				currentStep = Fade_Step::FROM_BLACK;
			}

			if(ImagesOnOff)
			{
				//Si se quiere añadir otra script que necesite pasar entre dos imagenes debe ponerse aquí
				if (app->cutScene->active)
				{
					app->cutScene->currentTexture = ImageToEnter;
					app->cutScene->TextTimerToPrint = true;
				}

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

	if (moduleToDisable != app->combat && moduleToDisable != app->lScene)
	{
		if (transition_B)
		{
			backgroundAnimation.Backward();
		}
		else
		{
			backgroundAnimation.Foward();
		}

		backgroundAnimation.Step(1, false);
		float point = backgroundAnimation.GetPoint();
		int offset = -1300;

		posYani_I = offset + point * (0 - offset);

		offset = -1300;
		app->render->DrawTexture(TransitionBG_Izq, offset + point * (0 - offset) - app->render->camera.x, 0 - app->render->camera.y);

		offset = 1300;
		app->render->DrawTexture(TransitionBG_Der, offset + point * (640 - offset) - app->render->camera.x, 0 - app->render->camera.y);
	}

	return true;
}

bool  FadeToBlack::CleanUp()
{
	app->tex->UnLoad(TransitionBG_Izq);
	app->tex->UnLoad(TransitionBG_Der);

	return true;
}

bool FadeToBlack::FadingToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	transition_B = false;

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