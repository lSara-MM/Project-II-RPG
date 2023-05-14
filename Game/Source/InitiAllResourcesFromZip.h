#ifndef __InitAllResourcesFromZip_H__
#define __InitAllResourcesFromZip_H__

#include "Module.h"
#include "App.h"

#include "Player.h"


struct SDL_Texture;

class InitAllResourcesFromZip : public Module
{
public:

	InitAllResourcesFromZip();

	// Destructor
	virtual ~InitAllResourcesFromZip();

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

	//load the texture
	void LoadTexFile(const pugi::xml_document& dataFile);

	//load the fx
	void LoadAudioFile(const pugi::xml_document& dataFile);

	//load and play the music
	void LoadGUI(const pugi::xml_document& dataFile);

public:

	Player* player;
	SDL_Texture* img;

private:

};

#endif // __LOGOSCENE_H__
