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

	texturePath = parameters.attribute("texturePath").as_string();
	return true;
}

bool Character::Start()
{
	texture=app->tex->Load(texturePath);

	SDL_Rect buttonBounds;
	ButtonType buttonType;

	switch (type)
	{
	case EntityType::COMBAT_CHARA:
		
		if (charaType == CharacterType::ALLY)
		{
			buttonBounds = { 400 - 126 * positionCombat_I, 200, 125, 180 };
		}

		if (charaType == CharacterType::ENEMY)
		{
			buttonBounds = { 700 + 126 * positionCombat_I, 200, 125, 180 };
		}

		buttonType = ButtonType::COMBAT_TARGET;
		break;

		// later on, inventory idea purposes
	case EntityType::MENU_CHARA:
		break;
	default:
		break;
	}
	
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, app->combat, buttonBounds, ButtonType::COMBAT_TARGET);

	return true;
}

bool Character::Update(float dt)
{
	//Asignar posicion en funcion de combat position y type
	if (charaType == CharacterType::ALLY)
	{
		position = { 400 - 126 * positionCombat_I, 200};
	}

	if (charaType == CharacterType::ENEMY)
	{
		position = { 700 + 126 * positionCombat_I, 200};
	}

	app->render->DrawTexture(texture, position.x, position.y);
	
	
	if(app->input->godMode_B)
	{
		onTurn = true;
		string position_C = std::to_string(positionCombat_I);
		const char* ch_pos = position_C.c_str();
		app->render->TextDraw(ch_pos, position.x+ 60, position.y - 20, 15);
		app->render->TextDraw(name.GetString(), position.x+5 , position.y +180, 10);
	}
	

	//Si es su turno pues hace cosas
	if(onTurn)
	{
			//testing
			string time ;
			const char* tempus;
			

		switch (charaType)
		{
		case CharacterType::ALLY:

			break;
		case CharacterType::ENEMY:

			if(!delayOn)
			{
				turnDelay.Start();
				delayOn = true;
			}
			if (turnDelay.ReadMSec()>2000 && delayOn)
			{
			
				//TURNO CON SUS COSAS
			
				

			}
			if (turnDelay.ReadMSec() > 4000)
			{
				delayOn = false;
				//NEXT TURN
			}


			break;
		case CharacterType::NONE:
			break;
		default:
			break;
		}
	}


	return true;
}

bool Character::CleanUp()
{
	//delete button;
	button = nullptr;

	return true;
}

