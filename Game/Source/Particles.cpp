#include "App.h"
#include "Particles.h"

#include "Defs.h"
#include "Log.h"

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


