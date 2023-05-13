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
#include "DialogueSystem.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"

#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Npc::Npc() : Entity(EntityType::NPC)
{
	name.Create("Npc");

	idleAnim.PushBack({ 0, 0, 96, 96 });
	idleAnim.PushBack({ 0, 0, 96, 96 });

	idleAnim.speed = 0.1f;

	active = true;
}

Npc::~Npc() {

}

bool Npc::Awake() {

	name = parameters.attribute("name").as_string();

	// Load dialogue IDs
	for (pugi::xml_attribute attr = parameters.first_attribute(); attr; attr = attr.next_attribute())
	{
		if (strcmp(attr.name(), "dialogueID") == 0)
		{
			dialoguesID.push_back(attr.as_int());
		}
		if (strcmp(attr.next_attribute().name(), "dialogueID") != 0)
		{
			break;
		}
	}

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Npc::Start() {
	//srand(time(NULL));

	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::STATIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	flipType = SDL_FLIP_NONE;

	pSensor = app->physics->CreateRectangleSensor(position.x + width / 2, position.y + height / 2, width * 2, height * 2, bodyType::STATIC, app->scene->npcSetID++);
	pSensor->body->SetFixedRotation(true);
	pSensor->ctype = ColliderType::NPC;
	pSensor->listener = this;

	pbody->body->SetGravityScale(0);
	pSensor->body->SetGravityScale(0);

	return true;
}

bool Npc::Update(float dt)
{
	if (app->scene->pause_B)//ERROR
	{
		dtP = 0;
	}
	else if (!app->scene->pause_B)
	{
		dtP = dt / 1000;
	}

	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	return true;
}

bool Npc::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	pSensor->body->GetWorld()->DestroyBody(pSensor->body);
	
	return true;
}


void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	}
}