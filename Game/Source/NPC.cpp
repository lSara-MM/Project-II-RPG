#include "Npc.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Window.h"
#include "Textures.h"

#include "Scene.h"

#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"

Npc::Npc() : Entity(EntityType::NPC)
{
	name.Create("Npc");

	
	active = true;
}

Npc::~Npc() {

}

bool Npc::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Npc::Start() {

	texture = app->tex->Load(texturePath);
	currentAnimation = &currentAnim;
	
	/*pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; */

	//pbody->ctype = ColliderType::Npc;

	return true;
}

bool Npc::Update(float dt)
{
	//pbody->body->SetGravityScale(0);

	if (app->scene->pause_B)
	{
		dtP = 0;
	}
	else if (!app->scene->pause_B)
	{
		dtP = dt / 1000;
	}

	//Controller(dtP);

	//vel = b2Vec2(vel.x * dtP, vel.y * dtP);
	////Set the velocity of the pbody of the Npc
	//pbody->body->SetLinearVelocity(vel);

	////Update Npc position in pixels
	//position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width / 2;
	//position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height / 2;

	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	return true;
}

bool Npc::CleanUp()
{
	app->tex->UnLoad(texture);
	//pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}


void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	}
}