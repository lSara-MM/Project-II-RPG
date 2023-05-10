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
	//name.Create("CombatCharacter");
	active = true;
}

Character::~Character()
{
	delete button;
	button = nullptr;
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
	texture = app->tex->Load(texturePath);

	SDL_Rect buttonBounds;
	ButtonType buttonType;

	switch (type)
	{
	case EntityType::COMBAT_CHARA:
		
		if (charaType == CharacterType::ALLY)
		{
			buttonBounds = { 400 - 126 * positionCombat_I, 200, 125, 180 };
			position = { 400 - 126 * positionCombat_I, 200 };
		}

		if (charaType == CharacterType::ENEMY)
		{
			buttonBounds = { 700 + 126 * positionCombat_I, 200, 125, 180 };
			position = { 700 + 126 * positionCombat_I, 200 };
		}

		buttonType = ButtonType::COMBAT_TARGET;
		break;

		// later on, inventory idea purposes
		// the idea is to set a square smaller button with a section of the texture of the head
		// the button should have a normal button texture and the character sprite would go ABOVE it and cropped to fit
		// the SDL_Rect that defines the section from the spritesheet / the spritesheet used should depend on the character ID or something? idk
	case EntityType::MENU_CHARA:
		buttonBounds = { 500 + 130 * 1, 200, 125, 180 };
		position = { 500 + 130 * 1, 200 };

		buttonType = ButtonType::INVENTORY;
		break;
	default:
		break;
	}
	
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, app->combat, buttonBounds, buttonType);

	onTurn = false; //Empiezan sin tocarle a nadie

	return true;
}

bool Character::Update(float dt)
{
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
	if (onTurn)
	{
		//testing
		string time;
		const char* tempus;


		switch (charaType)
		{
		case CharacterType::ALLY:

			break;
		case CharacterType::ENEMY:

			if (listSkillsHistory.start==nullptr)
			{
				listSkillsHistory.Add(0); //Si esta vacia ponerle algo para que no pete
			}
			if (!delayOn)
			{
				turnDelay.Start();
				delayOn = true;
			}
			if (turnDelay.ReadMSec() > 2000 && delayOn)
			{
				switch (charaClass) //La idea es que cada classe tenga un comportamiento generico
				{
				case CharacterClass::MELEE_DPS:
					break;
				case CharacterClass::RANGED_DPS:
					break;
				case CharacterClass::ASSASSIN:
					break;
				case CharacterClass::AOE_DPS:
					break;
				case CharacterClass::HEALER:
					break;
				case CharacterClass::TANK:
					
					//int probSkill;
					//if (currentHp>=maxHp/2) //Alto de vida
					//{
					//	if (listSkillsHistory.end->data == 4)//Si se uso el turno pasado no se usa
					//	{
					//		probSkill = 10;						
					//	}
					//	else //Si no se uso pues casi siempre la usa
					//	{
					//		probSkill = 60;
					//	}
					//	if (CalculateRandomProbability(probSkill))
					//	{
					//		//usar skill 4 (tanqueo high HP)
					//		listSkillsHistory.Add(4);
					//		armor += 5; //TEMPORAL
					//		break;
					//	}
					//	else
					//	{
					//		probSkill = 75;
					//	} 

					//}
					//else //Bajo de vida
					//{
					//	if (listSkillsHistory.end->data==2)//Si se uso el turno pasado no se repite casi
					//	{
					//		probSkill = 15;
					//	}
					//	else //Si no se uso pues casi siempre la usa
					//	{
					//		probSkill = 80;

					//	}
					//	if (CalculateRandomProbability(probSkill))
					//	{
					//		//usar skill 2
					//		listSkillsHistory.Add(2);
					//		this->ModifyHP(400); //TEMPORAL
					//		break;
					//	}
					//	{
					//		probSkill = 25;
					//	}
					//}
					//if (CalculateRandomProbability(probSkill))//Ataques
					//{
					//	//usar skill 4 (daño + debuff)
					//	listSkillsHistory.Add(3);
					//	app->combat->vecAllies.at(0)->ModifyHP(-130);//TEMPORAL
					//	app->combat->vecAllies.at(0)->attack-=10;
					//	break;
					//}
					//else
					//{
					//	//usar skill 1 (daño solo)
					//	listSkillsHistory.Add(1);
					//	app->combat->vecAllies.at(0)->ModifyHP(-150);//TEMPORAL
					//	break;
					//}

			
			
					break;
				case CharacterClass::BUFFER:
					break;
				case CharacterClass::DEBUFFER:
					break;
				case CharacterClass::DOT:
					break;
				case CharacterClass::NO_CLASS:
					break;
				default:
					break;
				}
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
	app->tex->UnLoad(texture);

	return true;
}


void Character::ModifyHP(int hp)
{
	currentHp += hp;

	if (currentHp >= maxHp)
	{
		currentHp = maxHp;
	}

	if (currentHp <= 0)
	{
		if (charaType == CharacterType::ALLY) { app->combat->RemoveCharacter(&app->combat->vecAllies, this); }
		else if (charaType == CharacterType::ENEMY) { app->combat->RemoveCharacter(&app->combat->vecEnemies, this); }
	}
}

bool Character::CalculateRandomProbability(int bonus_I, int against_I)
{
	//Generamos numero aleatorio
	int randomNum_I = rand() % 100;

	int finalRand_I = randomNum_I + bonus_I - against_I; //Aplicamos el bonus de stat y restamos el del enemigo

	if (finalRand_I >= 100)
	{
		//El numero final supera el 100, por lo tanto acierta
		return true;
	}
	else
	{
		//Numero menor que 100, falla 
		return false;
	}
}

//Provisional full
int Character::CalculateDamage(Character* caster, Character* defender)
{
	if (1 > 0) //Curacion, no hace falta calcular esquiva ni nada 
	{
		return(caster->attack /** skill->multiplierDmg*/);
	}
	else //Es un ataque 
	{
		if (!CalculateRandomProbability((/*skill->bonusPrecision + */caster->precision), defender->dodge))
		{
			//Enemigo esquiva
			return 0;
		}
		else
		{
			int damage;
			damage = /*skill->multiplierDmg **/ caster->attack;
			if (CalculateRandomProbability(/*skill->bonusCritRate +*/ caster->critRate)) //Si true hay critico
			{
				damage *= (/*skill->bonusCritDmg +*/ caster->critDamage);
			}

			// Calcular reduccion de la defensa
			float armorRelevance = (defender->armor / abs(damage)) + 1;
			damage = +((defender->armor / 2) * armorRelevance); //Esta con mas ya que damage es negativo

			return damage;
		}
	}
}
