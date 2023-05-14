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

// TO TEST
#include "SceneWin_Lose.h"

Character::Character() : Entity(EntityType::COMBAT_CHARA)
{
	//name.Create("CombatCharacter");
	active = true;
	isCombatant = true;
}

Character::~Character()
{
	CleanUp();
}

bool Character::Awake()
{
	name = parameters.attribute("name").as_string();
	
	if (strcmp(name.GetString(), "") == 0)
	{
		name = app->input->playerName->input.c_str();
	}

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

	int skillIDs[4];
	skillIDs[0] = parameters.attribute("skill1ID").as_int();
	skillIDs[1] = parameters.attribute("skill2ID").as_int();
	skillIDs[2] = parameters.attribute("skill3ID").as_int();
	skillIDs[3] = parameters.attribute("skill4ID").as_int();
	
	LoadSkill(skillIDs);
	isCombatant = true;

	//fx
	healPath = "Assets/Audio/Fx/heal.wav";
	healfx = app->audio->LoadFx(healPath);

	hitPath = "Assets/Audio/Fx/hit.wav";
	hitfx = app->audio->LoadFx(hitPath);

	return true;
}

bool Character::Start()
{
	texture = app->tex->Load(texturePath);

	SDL_Rect buttonBounds;

	if (isCombatant && !app->sceneWin_Lose->active) // TO TEST: win/lose active if necessary
	{
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

		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, app->combat, buttonBounds, ButtonType::COMBAT_TARGET);
		app->combat->listButtons.Add(button);

		// Texture section
		sectionRect = nullptr;
		button->state = GuiControlState::SELECTED;
		button->isSelected = true;
	}
	else
	{
		// TO TEST
		buttonBounds = { 700 + 126 * positionCombat_I, 200, 125, 180 };
		position = { 700 + 126 * positionCombat_I, 200 };

		button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, app->combat, buttonBounds, ButtonType::COMBAT_TARGET);

		// Texture section
		SDL_Rect rect = { 0, 0, 0, 0 };
		sectionRect = &rect;

		button->state = GuiControlState::NONE;
	}
	
	onTurn = false;

	return true;
}

bool Character::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y, sectionRect);
	
	if (isCombatant)
	{
		//if (app->input->godMode_B)
		{
			string position_C = std::to_string(positionCombat_I);
			const char* ch_pos = position_C.c_str();
			app->render->TextDraw(ch_pos, position.x + 60, position.y - 20, 15);
			app->render->TextDraw(name.GetString(), position.x + 5, position.y + 180, 10);
			string HP_C = std::to_string(currentHp);
			const char* ch_hp = HP_C.c_str();
			app->render->TextDraw(ch_hp, position.x + 60, position.y - 40, 15);
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

				if (app->combat->lastPressedAbility_I != -1)
				{
					if (app->combat->targeted_Character != nullptr)
					{
						UseSkill(listSkills.At(app->combat->lastPressedAbility_I)->data, app->combat->targeted_Character);
						app->combat->targeted_Character = nullptr;
						onTurn = false;
						app->combat->NextTurn();
					}
				}
				//app->combat->HandleSkillsButtons(listSkills);

				break;
			case CharacterType::ENEMY:

				if (listSkillsHistory.start == nullptr)
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

					{
						int probSkill;
						if (!listSkills.At(0)->data->PosCanBeUsed(positionCombat_I) && !listSkills.At(3)->data->PosCanBeUsed(positionCombat_I)) //Alto de vida
						{
							//usar skill 2 (avance)
							UseSkill(listSkills.At(2)->data);

							listSkillsHistory.Add(2);
							break;
						}
						else
						{
							if (listSkillsHistory.end->data == 1)//Si se uso el turno pasado no se usa
							{
								probSkill = 0;
							}
							else //Si no se uso pues casi siempre la usa
							{
								probSkill = 25;
							}
							if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
							{
								//usar skill 1 (buff ofensivo)
								UseSkill(listSkills.At(1)->data);

								listSkillsHistory.Add(1);
								break;
							}
							else
							{
								if (listSkillsHistory.end->data == 1)//Si se uso el turno pasado no se usa
								{
									probSkill = 75;
								}
								else
								{
									probSkill = 25;
								}
							}
							if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
							{
								//usar skill 3 (atk area)
								UseSkill(listSkills.At(3)->data);

								listSkillsHistory.Add(3);
								break;
							}
							else
							{
								//usar skill 0 (atk basico)
								UseSkill(listSkills.At(0)->data);

								listSkillsHistory.Add(0);
								break;
							}
						}
					}

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

						{
						int probSkill;
						if (currentHp >= maxHp / 2) //Alto de vida
						{
							if (listSkillsHistory.end->data == 2)//Si se uso el turno pasado no se usa
							{
								probSkill = 10;
							}
							else //Si no se uso pues casi siempre la usa
							{
								probSkill = 60;
							}
							if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
							{
								//usar skill 2 (tanqueo high HP)
								UseSkill(listSkills.At(2)->data);

								listSkillsHistory.Add(2);
								break;
							}
							else
							{
								probSkill = 75;
							}

						}
						else //Bajo de vida
						{
							if (listSkillsHistory.end->data == 1)//Si se uso el turno pasado no se repite casi
							{
								probSkill = 15;
							}
							else //Si no se uso pues casi siempre la usa
							{
								probSkill = 80;

							}
							if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
							{
								//usar skill 1
								UseSkill(listSkills.At(1)->data);

								listSkillsHistory.Add(1);
								break;
							}
							{
								probSkill = 25;
							}
						}
						if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))//Ataques
						{
							//usar skill 3 (daño + debuff)
							UseSkill(listSkills.At(3)->data);

							listSkillsHistory.Add(3);
							break;
						}
						else
						{
							//usar skill 0 (daño solo) (es la skill mas debil)
							UseSkill(listSkills.At(0)->data);

							listSkillsHistory.Add(0);
							break;
						}
					}


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
					app->combat->NextTurn();
				}

				break;
			case CharacterType::NONE:
				break;
			default:
				break;
			}
		}
	}

	return true;
}

bool Character::CleanUp()
{
	if(texture != nullptr)
		app->tex->UnLoad(texture);

	/*delete button;
	button = nullptr;*/

	RELEASE(button);
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
int Character::ApplySkill(Character* caster, Character* defender, Skill* skill)
{
	if (skill->multiplierDmg >= 0) //Curacion o buffo, no hace falta calcular esquiva ni nada 
	{
		return(caster->maxHp/5 * skill->multiplierDmg);
		if (skill->positiveEffect) //Efecto de estado positivo
		{
			defender->listStatusEffects.Add(&StatusEffect::StatusEffect(skill->intensity, skill->duration, skill->positiveEffect, (EffectType)skill->status));
		}
		else
		{
			if(CalculateRandomProbability(skill->bonusPrecision + caster->GetStat(EffectType::PRECISION), defender->GetStat(EffectType::RES)))
			{
				defender->listStatusEffects.Add(&StatusEffect::StatusEffect(69, 3, true, EffectType::NONE));
			}
		}
	}
	else //Es un ataque 
	{
		if (!CalculateRandomProbability(skill->bonusPrecision + caster->GetStat(EffectType::PRECISION), defender->GetStat(EffectType::DODGE)))
		{
			//Enemigo esquiva
			return 0;
		}
		else
		{
			int damage;
			damage = skill->multiplierDmg * caster->GetStat(EffectType::ATTACK);
			if (CalculateRandomProbability(skill->bonusCritRate + caster->GetStat(EffectType::CRIT_RATE))) //Si true hay critico
			{
				damage *= (skill->bonusCritDamage + caster->GetStat(EffectType::CRIT_DMG));
			}

			if (skill->positiveEffect) //Efecto de estado positivo
			{
				defender->listStatusEffects.Add(&StatusEffect::StatusEffect(69, 3, true, EffectType::NONE));
			}
			else
			{
				if (CalculateRandomProbability(skill->bonusPrecision + caster->GetStat(EffectType::PRECISION), defender->GetStat(EffectType::RES)))
				{
					defender->listStatusEffects.Add(&StatusEffect::StatusEffect(69, 3, true, EffectType::NONE));
				}
			}

			// Calcular reduccion de la defensa
			float armorRelevance = (defender->GetStat(EffectType::ARMOR) / abs(damage+1)) + 1;
			damage += ((defender->GetStat(EffectType::ARMOR) / 2) * armorRelevance); //Esta con mas ya que damage es negativo

			return damage;
		}
	}
}

void Character::LoadSkill(int arr[4])
{
	//Cargar skills
	pugi::xml_parse_result parseResult = skillsFile.load_file("skills.xml");
	skillNode = skillsFile.child("skills");
	for (int i = 0; i < 4; i++)
	{
		for (pugi::xml_node aux = skillNode.child("skill"); aux; aux = aux.next_sibling("skill"))
		{
			if (aux.attribute("id").as_int() == arr[i])
			{
				SString nombre = aux.attribute("name").as_string();
				SString descripcion = aux.attribute("description").as_string();
				SkillType tipo = (SkillType)aux.attribute("name").as_int();
				
				float mult = aux.attribute("multiplierDmg").as_float();
				int dmgCrit = aux.attribute("bonusCritDamage").as_int();
				int probCrit = aux.attribute("bonusCritRate").as_int();
				int precision = aux.attribute("bonusPrecision").as_int();
				int movTarget = aux.attribute("movementTarget").as_int();

				int statusID = aux.attribute("statusID").as_int();
				bool positiveEffect = aux.attribute("positive").as_bool();
				int duration = aux.attribute("duration").as_int();
				int intensity = aux.attribute("intensity").as_int();

				int movUsuario = aux.attribute("movementCaster").as_int();
				int posInicialUso = aux.attribute("posToUseStart").as_int();
				int posFinallUso = aux.attribute("posToUseEnd").as_int();
				
				int posInicialTarget = aux.attribute("posToTargetStart").as_int();
				int posFinalTarget = aux.attribute("posToTargetEnd").as_int();
				bool friendlyFire = aux.attribute("friendlyFire").as_bool();
				bool area = aux.attribute("areaSkill").as_bool();
				bool autoTarget = aux.attribute("autoTarget").as_bool();

				listSkills.Add(new Skill(nombre, descripcion,
					posInicialUso, posFinallUso, posInicialTarget, posFinalTarget,
					movUsuario, movTarget, friendlyFire, area, autoTarget, 
					mult, precision, probCrit, dmgCrit, statusID,positiveEffect,duration,intensity));
			}
		}
	}
}

//Funcion para la maquina 
bool Character::UseSkill(Skill* skill)
{
	if(skill->autoTarget)
	{
		this->ModifyHP(ApplySkill(this, this, skill)); //Lanzarsela a si mismo
		if(skill->movementCaster!=0)
		{app->combat->MoveCharacter(&app->combat->vecEnemies, this, skill->movementCaster); }
		return true;
	}

	if(skill->targetFriend) //Targetea a gente de su propio grupo
	{
		//ERIC: QUIZA HAYA QUE CAMBIAR A QUE NO HAYA FALLO POSIBLE, IDK
		switch (charaType)
		{
			int endRange;
		case CharacterType::ALLY:

			endRange = skill->RangeCanTarget(app->combat->vecAllies);
			if (endRange == -1)
			{
				return false;
			}

			if (skill->areaSkill)
			{
				for (size_t i = skill->posToTargetStart_I; i <= endRange; i++)
				{
					//Atacar a todos
					app->combat->vecAllies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(i), skill));
				}
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange);
				app->combat->vecAllies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(objective), skill));
			}
			break;
		case CharacterType::ENEMY:
			endRange = skill->RangeCanTarget(app->combat->vecEnemies);
			if (endRange == -1)
			{
				return false;
			}

			if (skill->areaSkill)
			{
				for (size_t i = skill->posToTargetStart_I; i <= endRange; i++)
				{
					//Atacar a todos
					app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill));
				}
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange);
				app->combat->vecEnemies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(objective), skill));
			}
			break;
			
		case CharacterType::NONE:
			break;
		default:
			break;
		}
	}
	else //Targetea party contraria
	{
		switch (charaType)
		{
		int endRange;
		case CharacterType::ALLY:

			endRange = skill->RangeCanTarget(app->combat->vecEnemies);
			if (endRange == -1)
			{
				return false;
			}

			if (skill->areaSkill)
			{
				for (size_t i = skill->posToTargetStart_I; i <= endRange; i++)
				{
					//Atacar a todos
					app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill));
				}
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange);
				app->combat->vecEnemies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(objective), skill));
			}
			break;

			break;
		case CharacterType::ENEMY:

			endRange = skill->RangeCanTarget(app->combat->vecAllies);
			if (endRange == -1)
			{
				return false;
			}

			if (skill->areaSkill)
			{
				for (int i = skill->posToTargetStart_I; i <= endRange; i++)
				{
					//Atacar a todos
					app->combat->vecAllies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(i), skill));
					if (CalculateRandomProbability(skill->bonusPrecision + this->precision, app->combat->vecAllies.at(i)->res))
					{

					}
				}
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange);
				app->combat->vecAllies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(objective), skill));
				if (CalculateRandomProbability(skill->bonusPrecision + this->precision, app->combat->vecAllies.at(objective)->res)) 
				{
					if (skill->movementTarget != 0)
					app->combat->MoveCharacter(&app->combat->vecAllies, app->combat->vecAllies.at(objective), skill->movementTarget);
				}
			}
			break;

			break;
		default:
			break;
		}
	}
	
	
	return true;
}

//Para aliados
bool Character::UseSkill(Skill* skill, Character* target)
{
	int endRange;
	if (skill->targetFriend) { endRange = skill->RangeCanTarget(app->combat->vecAllies); }
	else { endRange = skill->RangeCanTarget(app->combat->vecEnemies); }

	if (skill->areaSkill)
	{
		for (size_t i = skill->posToTargetStart_I; i <= endRange; i++) //Creo que las skills de area hay que lanzarlas de atras a delante
		{
			//Atacar a todos
			switch (target->charaType)
			{
			case CharacterType::ALLY:
				app->combat->vecAllies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(i), skill));
				app->audio->PlayFx(healfx);
				break;
			case CharacterType::ENEMY:
				app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill));
				app->audio->PlayFx(hitfx);
				break;
			case CharacterType::NONE:
				break;
			default:
				break;
			}
			
		}
	}
	else
	{
		target->ModifyHP(ApplySkill(this, target, skill));
		if (CalculateRandomProbability(skill->bonusPrecision + this->precision, target->res))
		{
			if (skill->movementTarget != 0)
			app->combat->MoveCharacter(&app->combat->vecEnemies, target, skill->movementTarget);
		}
	}

	if (skill->movementCaster != 0)
	app->combat->MoveCharacter(&app->combat->vecEnemies, target, skill->movementTarget);

	return true;
}

// Status Effects
int Character::GetStat(EffectType statType)
{
	float output = 0;
	int base = 0;

	switch (statType)
	{
	case EffectType::CURRENT_HP:
		base = maxHp;

		for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
		{
			if (i->data->type == statType)
			{
				output = output + i->data->intensity;
			}
			i->data->turnsLeft--;
		}

		return (base * output / 100);

		break;
	case EffectType::ATTACK:
		base = attack;
		break;
	case EffectType::CRIT_RATE:
		base = critRate;
		break;
	case EffectType::CRIT_DMG:
		base = critDamage;
		break;
	case EffectType::PRECISION:
		base = precision;
		break;
	case EffectType::ARMOR:
		base = armor;
		break;
	case EffectType::DODGE:
		base = dodge;
		break;
	case EffectType::RES:
		base = res;
		break;
	default:
		break;
	}

	for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
	{
		if (i->data->type == statType)
		{
			output = output + i->data->intensity;
		}
		if (i->data->type != EffectType::CURRENT_HP)
		{
			i->data->turnsLeft--;
		}		
	}

	return base * ((100 + output) / 100);
}
