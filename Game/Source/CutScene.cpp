#include "CutScene.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include "IntroScene.h"
#include "Scene.h"

#include "FadeToBlack.h"
#include "Combat.h"


CutScene::CutScene() : Module()
{
	name.Create("CutScene");
}

// Destructor
CutScene::~CutScene()
{}

// Called before render is available
bool CutScene::Awake(pugi::xml_node& config)
{
	LOG("Loading CutScene");
	bool ret = true;

	pugi::xml_node pugiNode = config.first_child();

	for (int i = 0; pugiNode != NULL; i++)
	{
		NextImg.Add(pugiNode.attribute("texturepath").as_string());

		pugiNode = pugiNode.next_sibling("Img");
	}

	pugiNode = config.first_child();
	
	for (int i = 0; pugiNode != NULL; i++)
	{
		NextText.Add(pugiNode.attribute("textpath").as_string());

		pugiNode = pugiNode.next_sibling("Text");
	}

	pathcinematicpass = "Assets/Audio/Fx/entrar_sala.wav";
	cinematicpassfx = app->audio->LoadFx(pathcinematicpass);

	pathfirma = "Assets/Audio/Fx/firmafx.wav";
	firmafx = app->audio->LoadFx(pathfirma);

	return ret;
}

// Called before the first frame
bool CutScene::Start()
{
	passImg = 0;
	printText = false;
	StopCutScene = false;
	textHasEnded = false;

	RestartTimer();
	app->render->ResetDtText();

	for(int i = 0; i <= NextImg.Count() - 1; i++)
	{
		ImgToPrint.Add(app->tex->Load(NextImg.At(i)->data));
	}

	app->fade->FadingToBlackImages(ImgToPrint.At(passImg)->data, ImgToPrint.At(passImg)->data, 40);

	currentTexture = ImgToPrint.At(passImg)->data;

	StopCutScene = true;
	TextTimerToPrint = true;

	return true;
}

// Called each loop iteration
bool CutScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool CutScene::Update(float dt)
{
	//Este bloque de if's hace que se pasen las imagenes y el texto
	if (!StopCutScene)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetGamepadButton(SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
		{
  			printText = false;

			app->audio->PlayFx(cinematicpassfx);

			if (passImg < ImgToPrint.Count() - 1)
			{
				app->fade->FadingToBlackImages(currentTexture, ImgToPrint.At(passImg + 1)->data, 90);
			}
			else
			{
				//Esto era para intentar que la ultima imagen desaparezca con un fadetoBlack, pero no funciona por algun motivo
				app->fade->FadingToBlackImages(ImgToPrint.At(passImg)->data, nullptr, 90);
			}

			passImg += 1;
			StopCutScene = true;
			textHasEnded = false;
			TextTimerToPrint = false;

			app->render->ResetDtText();
			RestartTimer();
		}
	}

	//Esto renderiza la imagen que ahora est� en pantalla
	if (passImg <= ImgToPrint.Count() - 1)
		app->render->DrawTexture(currentTexture, 0, 0);


	if (TextTimerToPrint)
	{
		//Printa el texto
		if (printText)
		{
			if (passImg <= NextText.Count() - 1)
			{
				SDL_Rect rect = { 0, app->win->GetHeight() - 120, app->win->GetWidth(), app->win->GetHeight() - 110 };
				app->render->DrawRectangle(rect, 0, 0, 0, 100);
				text = NextText.At(passImg)->next->data.c_str();
				textHasEnded = app->render->RenderTrimmedText(20, app->win->GetHeight() - 100, 2, text, &texts, 20, 100,
					{ 255, 246, 240 }, Font::TEXT, 0, 30.0f);
			}
		}
	}

	//Esto renderiza y permite que puedas volver a dar al espacio apareciendo el texto Press Space cada segundo
	if (DeltaTime >= 1)
	{
		if (passImg <= ImgToPrint.Count() - 1)
		{
			printText = true;

			if (passImg <= ImgToPrint.Count() - 2 && textHasEnded)
			{
				StopCutScene = false;
				app->render->TextDraw("PRESS SPACE", app->win->GetWidth() - 200, app->win->GetHeight() - 50, 10, Font::UI, { 255, 246, 240 });
			}
		}
	}

	if (TextTimerToPrint)
	{
		if (passImg >= ImgToPrint.Count() - 1)
		{
			StopCutScene = true;

			// enable text input
			if (!app->input->getInput_B && !app->input->playerName->input_entered)
			{
				app->input->ActiveGetInput(app->input->playerName);
			}

			// render input
			if (app->input->getInput_B)
			{
				// TO DO adjust position when bg done
				iPoint pos = { 540, 575 };
				app->input->RenderTempText("%%", app->input->temp.c_str(), pos, 40, Font::TEXT, { 255, 246, 240 });
			}

			// if name entered, fade to black
			if (app->input->playerName->input_entered)
			{
				app->audio->PlayFx(firmafx);
				app->fade->FadingToBlack(this, (Module*)app->scene, 90);
			}
		}
	}

	//Esto resetea el timer a 0 provocando que el texto aparezca y desaparezca
	if (DeltaTime >= 2)
	{
		RestartTimer();
	}

	//Variables que necesita el timer
	mTicks = SDL_GetTicks() - mStartTicks;
	DeltaTime = mTicks * 0.001f;

	return true;
}

// Called each loop iteration
bool CutScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		passImg = ImgToPrint.Count() - 1;
		currentTexture = ImgToPrint.At(passImg)->data;
		text = NextText.At(passImg)->next->data.c_str();
	}

	return ret;
}

// Called before quitting
bool CutScene::CleanUp()
{
	LOG("Freeing LogoScene");

	for (int i = 0; i <= ImgToPrint.Count() - 1; i++) 
	{
		app->tex->UnLoad(ImgToPrint.At(i)->data);
	}


	ImgToPrint.Clear();
	app->input->temp = "";
	app->render->ResetDtText();
	return true;
}

void CutScene::RestartTimer()
{
	mStartTicks = SDL_GetTicks();
	mTicks = 0;
	DeltaTime = 0.0f;
}
