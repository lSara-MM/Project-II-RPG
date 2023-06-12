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
	//name.Create("Npc");

	idleAnim.PushBack({ 96, 0, 96, 96 });
	idleAnim.PushBack({ 192, 0, 96, 96 });
	idleAnim.PushBack({ 96, 0, 96, 96 });
	idleAnim.PushBack({ 288, 0, 96, 96 });

	idleAnim.speed = 0.07f;
	idleAnim.loop = true;

	active = true;
}

Npc::~Npc() {
	CleanUp();
}

bool Npc::Awake() {

	name = parameters.attribute("name").as_string();

	if (strcmp(name.GetString(), "PINKY") == 0) 
	{ 
		isAnimated = true;
		pugi::xml_attribute from = parameters.first_attribute().next_attribute();
		pugi::xml_attribute to = from.next_attribute();

		for (int i = from.as_int(); i <= to.as_int(); i++)
		{
			dialoguesID.push_back(i);
		}
	}
	else
	{
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
	float offset = 1.5f;//con el offset dividido entre 2 no queda bien el collider
	pbody = app->physics->CreateRectangle(position.x + width / offset, position.y + height / offset, width, height, bodyType::STATIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	flipType = SDL_FLIP_NONE;

	pSensor = app->physics->CreateRectangleSensor(position.x + width / offset, position.y + height / offset, width * 2, height * 2, bodyType::STATIC, app->scene->npcSetID++);
	pSensor->body->SetFixedRotation(true);
	pSensor->ctype = ColliderType::NPC;
	pSensor->listener = this;

	pbody->body->SetGravityScale(0);
	pSensor->body->SetGravityScale(0);

	return true;
}

bool Npc::Update(float dt)
{
	if (app->menus->pause_B)//ERROR
	{
		dtP = 0;
	}
	else if (!app->menus->pause_B)
	{
		dtP = dt / 1000;
	}

	SDL_Rect rect = { 0, 0, 96, 96 };

	if (isAnimated)
	{
		currentAnimation->Update();
		rect = currentAnimation->GetCurrentFrame();
	}
	
	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);

	return true;
}

bool Npc::CleanUp()
{
	if(texture != nullptr)
		app->tex->UnLoad(texture);

	if(pbody != nullptr)
		pbody->body->GetWorld()->DestroyBody(pbody->body);

	if(pSensor != nullptr)
		pSensor->body->GetWorld()->DestroyBody(pSensor->body);
	
	dialoguesID.clear();
	dialoguesID.shrink_to_fit();
	return true;
}


void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	}
}

void Npc::PinkyIsAngry()
{
	isAnimated = false;
	dialoguesID.erase(dialoguesID.begin());
}
