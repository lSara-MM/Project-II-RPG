//#ifndef __PARTICLES_H__
//#define __PARTICLES__
//
//#include "Module.h"
//
//#include "DynArray.h"
//#include "Point.h"
//#include "List.h"
//#include <iostream>
//#include <vector>
//
//
////class Particles : public Module
////{
////public:
////
////	Particles();
////
////	// Destructor
////	~Particles();
////
////	// Called before quitting
////	bool CleanUp();
////
////private:
////
////	Particle particles;
////    List<Particle> particlepool;
////
////};
////
////struct Particle {
////	float x, y;      // Position
////	float vx, vy;    // Velocity
////	float ax, ay;    // Acceleration
////	float lifespan;  // Remaining lifespan
////	// Other properties...
////};
//#ifndef PARTICLE_SYSTEM_H
//#define PARTICLE_SYSTEM_H
//
//#include <vector>
//class Particles : public Module
//{
//public:
//
//    Particles();
//
//    // Destructor
//    ~Particles();
//
//    // Called before quitting
//    bool CleanUp();
//
//private:
//
//
//};
//
//class Particle
//{
//public:
//    Particle(int x, int y);
//    void update();
//    void render();
//    bool isFaded();
//
//private:
//    float x;
//    float y;
//    float velocity_x;
//    float velocity_y;
//    int age;
//    int r;
//    int g;
//    int b;
//};
//
//class ParticleSystem
//{
//public:
//    ParticleSystem(int numParticles);
//    void createParticles(int x, int y);
//    void updateParticles();
//    void renderParticles();
//    bool isEmpty();
//
//private:
//    std::vector<Particle> particles;
//};
//
//#endif  // PARTICLE_SYSTEM_H
//
//
//
//#endif // __PARTICLES_H__