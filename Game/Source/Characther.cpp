#include "Characther.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Window.h"

#include "Scene.h"
#include "Combat.h"

#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Map.h"

#include "Log.h"
#include "Point.h"

Character::Character() : Entity(EntityType::UNKNOWN)
{
	name.Create("Character");

	active = true;
}

Character::~Character() {

}

bool Character::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Character::Start() {

	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangle(position.x + width / 2, position.y + height / 2, width, height, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	
	pbody->listener = this; 

	pbody->ctype = ColliderType::PLAYER;
	alive = true;
	
	return true;
}

bool Character::Update(float dt)
{
	currentAnimation = &idleAnim;
	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect, 1.0f, NULL, NULL, NULL, flipType);
	return true;
}

bool Character::CleanUp()
{
	app->tex->UnLoad(texture);
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	
	return true;
}

bool Character::Render()
{
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

void Character::ModifyHP(int num)
{
	if ((this->currentHp + num) > this->maxHp) 
	{
		this->currentHp = this->maxHp;

	}
	else if ((this->currentHp + num) < 0)
	{
		this->currentHp = 0;

		app->combat->EliminateCombatant(this);
		//Muere? No muere? Hay que hablar si hay death door
	}
	else
	{
		this->currentHp += num;
	}
}

int Character::CalculateDamage(int initialDmg) 
{
	int realDmg;
	//Idea 1, crecimiento lento
	//unsigned int defCalculus = (this->defense / 10) + 1; //Minimo valor de 1 ya que se usa para un logaritmo
	//float defLogRes = log2(defCalculus); //Numero usado para calcular el multiplicador, se usa log para crecimiento mas lento
	//float defMultiplier = defLogRes / (defLogRes + 1); //Multiplicador de la armor, cada vez crece mas despacio
	//int reductionPercent = this->defense * defMultiplier; //Porcentaje de reduccion, tiene en cuenta la armadura y su multiplicador
	//realDmg = initialDmg * (100 - reductionPercent)/100; //

	//Idea2, defensa mas relevante vs ataques mas debiles
	float armorRelevance = (this->armor / initialDmg) + 1;
	realDmg = initialDmg - ( (this->armor / 2) * armorRelevance);

	return realDmg;
}


