#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Physics.h"

#include "Animation.h"
#include "NPC.h"
#include "Character.h"
#include "SString.h"

struct SDL_Texture;

using namespace std;

class Player : public Entity
{
public:

	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void EndContact(PhysBody* physA, PhysBody* physB);

	void Controller(float dt);

	void LoadAllPC();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation currentAnim;

	Animation downAnim;
	Animation idleDownAnim;
	Animation upAnim;
	Animation idleUpAnim;
	Animation leftAnim;
	Animation idleLeftAnim;
	Animation rigthAnim;
	Animation idleRigthAnim;

	Animation hurtAnim;
	Animation deathAnim;
	Animation attackAnim;

	bool keyLockUp = false;
	bool keyLockDown = false;
	bool keyLockLeft = false;
	bool keyLockRigth = false;
	bool lockMovement = false;

	float dtP;
	float grav;
	b2Vec2 vel = { 0, 0 };

	SDL_RendererFlip flipType;
	PhysBody* pbody;

	iPoint position;

	bool PadLock;

	Npc* npcTalkingTo;
	bool npcInteract;
	vector<Character*> vecPC;

private:

	SDL_Texture* texture;
	const char* texturePath;

	const char* grass_path;
	uint walk_grass;

	// entrar sala
	const char* enterPath;
	uint enterZone;

	// interact fx
	const char* interactionPath;
	uint interactionfx;
	bool interactionTest;

	// interact fx
	const char* confirmPath;
	uint confirmInteractfx;
	
	int width, height;
	SDL_Texture* textureE;
};

#endif // __PLAYER_H__