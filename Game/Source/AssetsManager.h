#ifndef __ASSETSMANAGER_H__
#define __ASSETSMANAGER_H__

#include "Module.h"

#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "../Game/Source/External/PhysFS/libx86/physfs.lib" )

class AssetsManager : public Module
{
public:

	// Constructor
	AssetsManager();

	// Destructor
	~AssetsManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Return the number of bytes of a PhysFS file that you send it
	uint LoadData(const char* path, char** buffer) const;

	// Allows you to enter a path and extract a image or audio form it, or any kind of archive except form XML
	SDL_RWops* Load(const char* path) const;

};

#endif // __ASSETSMANAGER_H__
