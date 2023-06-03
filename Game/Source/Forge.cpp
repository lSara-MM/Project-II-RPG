#include "Forge.h"
#include "Input.h"
#include "app.h"
#include "Log.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "ItemManager.h"

Forge::Forge(unsigned cap) : Module()
{
	name.Create("forge");
	this->cap = cap;
	y = 0;
}

// Destructor
Forge::~Forge()
{

}

bool Forge::Start()
{

	return true;
}


bool Forge::PreUpdate()
{
	return true;
}

bool Forge::Update(float dt)
{

	bool ret = true;

	return ret;
}

bool Forge::PostUpdate()
{

	return true;
}

// Called before quitting
bool Forge::CleanUp()
{

	return true;
}

bool Forge::OnGuiMouseClickEvent(GuiControl* control)
{

	return true;
}