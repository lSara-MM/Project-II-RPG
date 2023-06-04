#ifndef __PARTICLES_H__
#define __PARTICLES__

#include "Module.h"

#include "DynArray.h"
#include "Point.h"
#include "List.h"


class Particles : public Module
{
public:

	Particles();

	// Destructor
	~Particles();

	// Called before quitting
	bool CleanUp();

private:

};



#endif // __PARTICLES_H__