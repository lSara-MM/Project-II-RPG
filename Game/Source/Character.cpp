#include "Character.h"

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

Character::Character() : Entity(EntityType::COMBAT_CHARA)
{
	name.Create("CombatCharacter");
	active = true;
}

Character::~Character()
{
}

bool Character::Awake()
{
	//COSA PARA PROBARLO EN EL MOMENTO
	int id = 1;
	//Idea identificacion cada Chara cual es
	while (parameters.attribute("id").as_int() != id && parameters != NULL)
	{
		parameters = parameters.next_sibling();
	}

	name = parameters.attribute("name").as_string();

	maxHp = parameters.attribute("maxHp").as_int();
	currentHp = parameters.attribute("currentHp").as_int();

	attack = parameters.attribute("attack").as_int();
	armor = parameters.attribute("armor").as_int();

	critRate = parameters.attribute("critRate").as_int();
	critDamage = parameters.attribute("critDamage").as_int();
	
	precision = parameters.attribute("precision").as_int();
	res = parameters.attribute("res").as_int();

	dodge = parameters.attribute("dodge").as_int();
	speed = parameters.attribute("speed").as_int();


	charaType = (CharacterType)parameters.attribute("type").as_int();
	charaClass = (CharacterClass)parameters.attribute("class").as_int();
	


	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	return false;
}

bool Character::Start()
{
	
	return false;
}

bool Character::Update(float dt)
{
	return false;
}

bool Character::CleanUp()
{

	return false;
}