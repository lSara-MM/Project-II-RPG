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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	//dialoguesID.push_back(parameters.attribute("dialogue").as_int());
	
	//string temp = parameters.attribute("dialogueID").as_string();
	//for (int i = 0; i < temp.size(); i++)
	//{
	//	string s(1, temp.at(i));
	//	dialoguesID.push_back(stoi(s));
	//}

	//// no funciona, como se hace un for de atributos :/
	//for (pugi::xml_attribute attr : parameters.attributes())
	//{
	//	if (parameters.name() == "dialogueID")
	//	{
	//		dialoguesID.push_back(attr.as_int());
	//	}
	//}

	return true;
}

bool Npc::Start() {

	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;
	
	//npcMerch = app->physics->CreateRectangle(positionMerch.x + width / 2, positionMerch.y + height / 2, width, height, bodyType::STATIC);
	//npcMerch->body->SetFixedRotation(true);
	//npcMerch->listener = this;

	//npcMerchSensor = app->physics->CreateRectangleSensor(positionMerch.x + width / 2, positionMerch.y + height / 2, width*2, height*2, bodyType::STATIC);
	//npcMerchSensor->body->SetFixedRotation(true);
	//npcMerchSensor->ctype = ColliderType::NPC;
	//npcMerchSensor->listener = this;

	return true;
}

bool Npc::Update(float dt)
{
	/*npcMerch->body->SetGravityScale(0);
	npcMerchSensor->body->SetGravityScale(0);*/

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
	/*npcMerch->body->GetWorld()->DestroyBody(npcMerch->body);
	npcMerchSensor->body->GetWorld()->DestroyBody(npcMerchSensor->body);*/
	
	return true;
}


void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	}
}