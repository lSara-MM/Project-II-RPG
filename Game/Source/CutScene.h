#ifndef __CutScene_H__
#define __CutScene_H__

#include "Module.h"
#include "Player.h"
#include "Tweening.h"


struct SDL_Texture;

class CutScene : public Module
{
public:

	CutScene();

	// Destructor
	virtual ~CutScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void RestartTimer();

public:

	int passImg;

	bool OnceFade;

	uint32 mTicks;
	uint32 mStartTicks;
	uint32 DeltaTime;

private:
	SDL_Texture* ImgToPrint;

	SDL_Texture* FirstImg;
	const char* pathFirstImg;	
	
	SDL_Texture* SecondtImg;
	const char* pathSecondImg;	
	
	SDL_Texture* ThirdImg;
	const char* pathThirdImg;

	SDL_Texture* FourthImg;
	const char* pathFourthImg;	
	
	SDL_Texture* FifthImg;
	const char* pathFifthImg;	
	
	SDL_Texture* SixImg;
	const char* pathSixImg;
};

#endif
