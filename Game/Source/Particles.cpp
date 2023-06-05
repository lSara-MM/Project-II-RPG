#include "App.h"
#include "Particles.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

Particles::Particles() : Module()
{
	name.Create("Particles");
}

// Destructor
Particles::~Particles()
{
	
}


// Called before quitting
bool Particles::CleanUp()
{
	LOG("Freeing Particles library");

	

	return true;
}

const int SCREEN_WIDTH = 800;   // Screen width
const int SCREEN_HEIGHT = 600;  // Screen height
const int PARTICLE_SIZE = 5;    // Particle size
const float MAX_VELOCITY = 0.25; // Maximum particle velocity
const int FADE_DELAY = 1000;    // Fade delay in milliseconds
const int SPREAD_FACTOR = 5;    // Particle spread factor

Particle::Particle(int x, int y) : x(x), y(y)
{
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.1415f; // Random angle between 0 and 2*pi
    float speed = static_cast<float>(rand()) / RAND_MAX * MAX_VELOCITY; // Random speed between 0 and MAX_VELOCITY
    float spread = static_cast<float>(rand()) / RAND_MAX * SPREAD_FACTOR; // Random spread factor between 0 and SPREAD_FACTOR

    // Calculate velocity components based on angle, speed, and spread factor
    velocity_x = cos(angle) * (speed + spread);
    velocity_y = sin(angle) * (speed + spread);
    velocity_x *= 0.90; // 1% reduction in x-velocity per update
    velocity_y *= 0.90; // 1% reduction in y-velocity per update

    age = 0;
    r = 255;
    g = 255;
    b = 255;
}

void Particle::update()
{
    x += velocity_x;
    y += velocity_y;
    ++age;
}

void Particle::render()
{
    // Render the particle using our draw function (e.g., drawCircle or drawQuad)
    // with the current position (x, y), size (PARTICLE_SIZE), and color (r, g, b).
    // The fading effect can be achieved by adjusting the alpha value based on the age and FADE_DELAY.
    int alpha = 255 - (age * 255) / FADE_DELAY;
    /*app->render->DrawCircle(x, y1, r1, 255, 255, 255);*/
    app->render->DrawCircle(x, y, PARTICLE_SIZE, r, g, b, alpha);
    // or
    // drawQuad(x, y, PARTICLE_SIZE, PARTICLE_SIZE, r, g, b, alpha);
}

bool Particle::isFaded()
{
    return age >= FADE_DELAY;
}

ParticleSystem::ParticleSystem(int numParticles)
{
    std::srand(std::time(nullptr)); // Initialize the random number generator

    for (int i = 0; i < numParticles; ++i)
    {
        Particle particle(0, 0);  // Start with particles at (0, 0)
        particles.push_back(particle);
    }
}

void ParticleSystem::createParticles(int x, int y)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        Particle particle(x, y);
        particles.push_back(particle);
    }
}

void ParticleSystem::updateParticles()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i].update();
        if (particles[i].isFaded())
        {
            particles.erase(particles.begin() + i);
            --i;
        }
    }
}

void ParticleSystem::renderParticles()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i].render();
    }
}

bool ParticleSystem::isEmpty()
{
    return particles.empty();
}


