#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"
#include "Particle.h"
#include "Physics.h"

#define MAX_ACTIVE_PARTICLES 500

struct SDL_Texture;
struct Collider;

class ModuleParticles : public Module
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	ModuleParticles();

	//Destructor
	~ModuleParticles();

	// Called when the module is activated
	// Loads the necessary textures for the particles
	bool Start();

	// Called at the beginning of the application loop
	// Removes all particles pending to delete
	bool PreUpdate();

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	bool Update(float dt);

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	bool PostUpdate();

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp();

	// Called when a particle collider hits another collider
	void OnCollision(Collider* c1, Collider* c2);

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	Particle* AddParticle(int x, int y, int delay, int m, int quantity, int speedX = 6, int speedY = 0);

public:
	//Template particle for an explosion
	Particle explosion;
	Particle explosionG;
	int particleType = 0;

private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* texture0 = nullptr;
	SDL_Texture* texture1 = nullptr;
	SDL_Texture* texture2 = nullptr;
	SDL_Texture* texture3 = nullptr;
	

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };
};

#endif // !__MODULEPARTICLES_H__