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

	//idleAnim.PushBack({ 64, 0, 64, 64 });
	//idleAnim.PushBack({ 64, 0, 64, 64 });

	//idleAnim.speed = 0.1f;

	active = true;
}

Npc::~Npc() {

}

// no entra en el pugi, porque T-T
bool Npc::Awake() {

	NpcName = parameters.attribute("name").as_string();

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

	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::STATIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pSensor = app->physics->CreateRectangleSensor(position.x + width / 2, position.y + height / 2, width * 2, height * 2, bodyType::STATIC, app->scene->npcSetID++);
	pSensor->body->SetFixedRotation(true);
	pSensor->ctype = ColliderType::NPC;
	pSensor->listener = this;

	return true;
}

bool Npc::Update(float dt)
{
	pbody->body->SetGravityScale(0);
	pSensor->body->SetGravityScale(0);

	if (app->scene->pause_B)
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