#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

#include "External/Optick/include/optick.h"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;

class EntityManager;
class Combat;
class DialogueSystem;
class GuiManager;

class Map;
class PathFinding;

class Physics;
class LogoScene;
class IntroScene;
class Scene;
class HouseOfTerrors;
class PracticeTent;
class Circus;
class LoseScene;
class LeaderboardScene;
class FadeToBlack;

class App
{
public:
	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGameRequest();
	void SaveGameRequest() ;
	bool LoadFromFile();
	bool SaveToFile() ;

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	void DisableAtStart();
	friend class Scene;
	friend class Physics;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	
	EntityManager* entityManager;
	Combat* combat;
	DialogueSystem* dialogueSystem;
	GuiManager* guiManager;
	
	Map* map;
	PathFinding* pathfinding;
	
	Physics* physics;
	LogoScene* lScene;
	IntroScene* iScene;
	Scene* scene;
	HouseOfTerrors* hTerrors;
	PracticeTent* practiceTent;
	Circus* circus;
	LoseScene* loseScene;
	FadeToBlack* fade;

	// Time control
	uint frames;
	float dt;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	

	Timer timer;
	PerfTimer ptimer;


	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;
	int  secondsSinceStartupTempo = 0;

	uint32 maxFrameDuration = 0;


	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

    bool saveGameRequested;
	bool loadGameRequested;
};

extern App* app;

#endif	// __APP_H__