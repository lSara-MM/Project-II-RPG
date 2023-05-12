#include "Quest.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Window.h"
#include "Textures.h"

#include "Scene.h"
#include "DialogueSystem.h"

Quest1::Quest1() : Quest(QuestType::INTERACT)
{
	name.Create("quest1");

	active = false;
}

Quest1::~Quest1() {

}

bool Quest1::Awake() {

}

bool Quest1::Start() {

}

bool Quest1::Update(float dt) {

}

bool Quest1::CleanUp() {

}