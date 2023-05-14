#include "InitiAllResourcesFromZip.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "AssetsManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

InitAllResourcesFromZip::InitAllResourcesFromZip() : Module()
{
	name.Create("InitAllResourcesFromZip");
}

// Destructor
InitAllResourcesFromZip::~InitAllResourcesFromZip()
{}

// Called before render is available
bool InitAllResourcesFromZip::Awake(pugi::xml_node& config)
{
	LOG("Loading InitiAllResourcesFromZip");

	bool ret = true;
	return ret;
}

// Called before the first frame
bool InitAllResourcesFromZip::Start()
{

	//char* buffer;
	//pugi::xml_document file;

	////TODO 5 (Solved): Load data from the "data.xml" with the function LoadData(), and send the buffer too
	//int bytes = app->assetsManager->LoadData("data.xml", &buffer);

	////Here we will make the space in memory that needs the program to open the files
	//pugi::xml_parse_result result = file.load_buffer(buffer, bytes);

	////Release the buffer
	//RELEASE_ARRAY(buffer);

	//LoadTexFile(file);
	//LoadAudioFile(file);
	//LoadGUI(file);

	return true;
}

// Called each loop iteration
bool InitAllResourcesFromZip::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool InitAllResourcesFromZip::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool InitAllResourcesFromZip::PostUpdate()
{

	return true;
}

// Called before quitting
bool InitAllResourcesFromZip::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void InitAllResourcesFromZip::LoadTexFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node tex = dataFile.child("data").child("texture");
	//player->texture = app->tex->Load(tex.attribute("file2").as_string());
}

void InitAllResourcesFromZip::LoadAudioFile(const pugi::xml_document& dataFile)
{
	pugi::xml_node audio = dataFile.child("data").child("fx");
	//app->audio->LoadFx(fx.attribute("file").as_string());
}

void InitAllResourcesFromZip::LoadGUI(const pugi::xml_document& dataFile)
{
	pugi::xml_node GUI = dataFile.child("data").child("mus");
	//app->audio->PlayMusic(mus.attribute("file").as_string());
}