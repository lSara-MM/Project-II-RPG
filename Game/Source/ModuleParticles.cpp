#include "ModuleParticles.h"

#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Log.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles() : Module()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture0 = app->tex->Load("Assets/Textures/particle0.png");
	texture1 = app->tex->Load("Assets/Textures/particle1.png");
	texture2 = app->tex->Load("Assets/Textures/particle2.png");
	texture3 = app->tex->Load("Assets/Textures/particle3.png");
	//// Explosion particle
	//explosion.anim.PushBack({246, 87, 27, 28});
	//explosion.anim.PushBack({276, 86, 31, 33});
	//explosion.anim.PushBack({310, 86, 28, 32});
	//explosion.anim.PushBack({342, 87, 29, 29});
	//explosion.anim.loop = false;
	//explosion.anim.speed = 0.3f;
	//
	//// Granade explosion
	//explosionG.anim.PushBack({ 0, 404, 49, 55 });
	//explosionG.anim.PushBack({ 49, 404, 49, 55 });
	//explosionG.anim.PushBack({ 98, 404, 49, 55 });
	//explosionG.anim.PushBack({ 147, 404, 49, 55 });
	//explosionG.anim.PushBack({ 196, 404, 49, 55 });
	//explosionG.anim.PushBack({ 245, 404, 49, 55 });
	//explosionG.anim.loop = false;
	//explosionG.anim.speed = 0.3f;

	return true;
}

bool ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

bool ModuleParticles::CleanUp()
{
	/*LOG("Unloading particles");*/

	// Delete all remaining active particles on application exit 
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr)
		{
			particles[i]->pendingToDelete = true;
			break;
		}
	}
}

bool ModuleParticles::Update(float dt)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if(particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if(particle->Update(dt) == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return true;
}

bool ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < 500; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive && Modulo == 0)
		{
			app->render->DrawTexture(texture0, particle->position.x, particle->position.y);
		}

		if (particle != nullptr && particle->isAlive && Modulo == 1)
		{
			app->render->DrawTexture(texture1, particle->position.x, particle->position.y);
		}

		if (particle != nullptr && particle->isAlive && Modulo == 2)
		{
			app->render->DrawTexture(texture2, particle->position.x, particle->position.y);
		}
		if (particle != nullptr && particle->isAlive && Modulo == 3)
		{
			app->render->DrawTexture(texture3, particle->position.x, particle->position.y);
		}
	}

	return true;
}

Particle* ModuleParticles::AddParticle(int x, int y, int delay, int m)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < 500; ++i)
	{
		particles[i] = nullptr;
	}

	Modulo = m;

	if (Modulo == 0)
	{
		//Create circle around position designated
		for (uint i = 0; i < 50; ++i)
		{
			//Finding an empty slot for a new particle
			if (particles[i] == nullptr)
			{
				int radius = 40;
				newParticle = new Particle;
				newParticle->lifetime = delay;			// We start the frameCount as the negative delay
				float angle = (2 * 3.14159 * i) / 50;
				float particleX = x + radius * cos(angle);
				float particleY = y + radius * sin(angle);
				newParticle->speed.x = 0;
				newParticle->speed.y = 0;
				newParticle->position.x = particleX;						// so when frameCount reaches 0 the particle will be activated
				newParticle->position.y = particleY;
				newParticle->isAlive = true;

				particles[i] = newParticle;
			}
		}
	}

	if (Modulo == 1)
	{
		//Create particles like an explosion
		for (uint i = 0; i < 100; ++i)
		{
			//Finding an empty slot for a new particle
			if (particles[i] == nullptr)
			{

				newParticle = new Particle;
				newParticle->lifetime = delay;			// We start the frameCount as the negative delay
				float particleX = x + static_cast<float>(rand() % 10);
				float particleY = y + static_cast<float>(rand() % 10);

				newParticle->position.x = particleX;
				newParticle->position.y = particleY;

				// Configurar velocidad aleatoria para simular una explosión
				newParticle->speed.x = static_cast<float>(rand() % 10 - 5);  // Rango: -5 a 5
				newParticle->speed.y = static_cast<float>(rand() % 10 - 5);  // Rango: -5 a 5
				newParticle->isAlive = true;

				particles[i] = newParticle;
			}
		}
	}

	if (Modulo == 2 || Modulo == 3)
	{
		//Create particles like a curacion
		for (uint i = 0; i < 100; ++i)
		{
			//Finding an empty slot for a new particle
			if (particles[i] == nullptr)
			{

				newParticle = new Particle;
				newParticle->lifetime = delay;			// We start the frameCount as the negative delay
				float particleX = x + static_cast<float>(rand() % 10);
				float particleY = y;

				newParticle->position.x = particleX;
				newParticle->position.y = particleY;

				// Configurar velocidad aleatoria para simular una explosión
				if (Modulo == 2) {
					newParticle->speed.x = static_cast<float>(rand() % 6 - 3);  // Rango: -3 a 3 en movimiento horizontal
					newParticle->speed.y = static_cast<float>(rand() % 10 - 15);  // Rango: -15 a -5 hacia arriba
					newParticle->isAlive = true;
				}

				if (Modulo == 3) {
					newParticle->speed.x = static_cast<float>(rand() % 6 - 3);  // Rango: -3 a 3 en movimiento horizontal
					newParticle->speed.y = static_cast<float>(rand() % 10 + 5);  // Rango: 5 a 14 hacia abajo
					newParticle->isAlive = true;
				}

				particles[i] = newParticle;
			}
		}
	}

	return newParticle;
}