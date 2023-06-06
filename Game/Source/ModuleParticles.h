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
	bool Start() override;

	// Called at the beginning of the application loop
	// Removes all particles pending to delete
	bool PreUpdate() override;

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	bool Update();

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	bool PostUpdate() override;

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp() override;

	// Called when a particle collider hits another collider
	void OnCollision(Collider* c1, Collider* c2);

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	Particle* AddParticle(const Particle& particle, int x, int y, uint delay = 0);

public:
	//Template particle for an explosion
	Particle explosion;
	Particle explosionG;

	//Template particle for a laser
	Particle laser;

	//Template particles for a normal bullet
	Particle bulletN;
	Particle bulletNE;
	Particle bulletE;
	Particle bulletSE;
	Particle bulletS;
	Particle bulletSW;
	Particle bulletW;
	Particle bulletNW;

	Particle TbulletN;
	Particle TbulletNE;
	Particle TbulletE;
	Particle TbulletSE;
	Particle TbulletS;
	Particle TbulletSW;
	Particle TbulletW;
	Particle TbulletNW;

	Particle enemyBullet;
	Particle enemyBulletL;
	Particle enemyBulletR;

	Particle deathAnim;
	Particle rescuedAnim;
	Particle deathredAnim;
	Particle deathgreenAnim;

	Particle BulletBoss;
	Particle BulletBossR;
	Particle BulletBossL;

	Particle granadeN;
	Particle granadeNE;
	Particle granadeE;
	Particle granadeSE;
	Particle granadeS;
	Particle granadeSW;
	Particle granadeW;
	Particle granadeNW;

	Particle playerdies;


private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* texture = nullptr;

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

};

#endif // !__MODULEPARTICLES_H__