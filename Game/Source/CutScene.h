#ifndef __CutScene_H__
#define __CutScene_H__

#include "Module.h"
#include "Player.h"
#include "Tweening.h"
#include <vector>

using namespace std;


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

	SDL_Texture* currentTexture;

private:
	int passImg;

	bool printText;
	bool StopCutScene;
	bool textHasEnded;

	uint32 mTicks;
	uint32 mStartTicks;
	uint32 DeltaTime;

	SString text;
	vector<SString> texts;
	string textToSString;

	List<SDL_Texture*> ImgToPrint;

	List<const char*> NextImg;

	List<string> NextText;
};

#endif
