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

	maxHp = parameters.attribute("maxHp").as_int();
	currentHp = parameters.attribute("currentHp").as_int();

	attack = parameters.attribute("attack").as_int();
	armor = parameters.attribute("armor").as_int();

	critRate = parameters.attribute("critRate").as_int();
	critDamage = parameters.attribute("critDamage").as_int();
	
	accuracy = parameters.attribute("accuracy").as_int();
	res = parameters.attribute("res").as_int();

	dodge = parameters.attribute("dodge").as_int();
	speed = parameters.attribute("speed").as_int();


	charaType = (CharacterType)parameters.attribute("type").as_int();
	charaClass = (CharacterClass)parameters.attribute("class").as_int();
	

	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	texturePath = parameters.attribute("texturePath").as_string();

	// texture section (for buttons and turns bar)
	SDL_Rect rect;
	rect.x = parameters.attribute("sectionX").as_int();
	rect.y = parameters.attribute("sectionY").as_int();
	rect.w = 74; rect.h = 74;
	texSection = rect;

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

	buffPath = "Assets/Audio/Fx/buff.wav";
	bufffx = app->audio->LoadFx(buffPath);

	debuffPath = "Assets/Audio/Fx/debuff.wav";
	debufffx = app->audio->LoadFx(debuffPath);

	id = parameters.attribute("id").as_int();

	textureOnturn = app->tex->Load("Assets/GUI/humoTurnos.png");

	return true;
}

bool Character::Start()
{
	if (strcmp(name.GetString(), "") == 0)
	{
		name = app->input->playerName->input.c_str();
	}

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
		texSection = rect;

		button->state = GuiControlState::NONE;
	}
	
	onTurn = false;

	return true;
}

bool Character::Update(float dt)
{
	if (isCombatant)
	{
		if (currentHp <= 0)
		{
			if (charaType == CharacterType::ALLY)
			{
				app->combat->RemoveCharacter(&app->combat->vecAllies, this);
			}
			else
			{
				app->combat->RemoveCharacter(&app->combat->vecEnemies, this);
			}
			return true;
		}

		// Render character
		app->render->DrawTexture(texture, position.x, position.y);

		// Health
		string HP_C = std::to_string(currentHp);
		const char* ch_hp = HP_C.c_str();
		app->render->TextDraw(ch_hp, position.x + 60, position.y - 57, 15, Font::UI, { 255, 255, 255 });

		// Health bar
		app->render->DrawRectangle({ position.x - 3, position.y - 37, 106, 16 }, 0, 0, 0);
		app->render->DrawRectangle({ position.x, position.y - 34, (currentHp * 100 / maxHp), 10 }, 181, 33, 33);

		// Character name
		app->render->TextDraw(name.GetString(), position.x + 3, position.y - 20, 11, Font::UI, { 255, 255, 255 });

		//Si es su turno pues hace cosas
		if (onTurn)
		{
			// TO DO: change to texture 
			//app->render->DrawRectangle({ position.x + 10, position.y + 195, 106, 10 }, 255, 0, 0);
			//app->render->DrawRectangle({ position.x + 10, position.y + 195, 106, 10 }, 255, 0, 0);

			SDL_Rect rect = { 0, 0, 126, 178 };
			/*app->render->DrawRectangle({ position.x + 10, position.y + 195, 106, 10 }, 255, 0, 0);
			app->render->DrawRectangle({ position.x + 10, position.y + 195, 106, 10 }, 255, 0, 0);*/
			app->render->DrawTexture(textureOnturn, position.x, position.y, &rect);
			
			//Si stunned pues no hace nada 
			if (GetStat(EffectType::STUN)==1)
			{
				if (!delayOn)
				{
					turnDelay.Start();
					delayOn = true;
				}
				if ((turnDelay.ReadMSec() > 1200 && delayOn) || app->input->godMode_B)
				{
					app->combat->NextTurn();
				}
			}
			else
			{			
				switch (charaType)
				{
				case CharacterType::ALLY:

					app->combat->HandleSkillsButtons(this);

					//Check hay una habilidad seleccionada
					if (app->combat->lastPressedAbility_I != -1)
					{
						//Check que hay un target 
						if (app->combat->targeted_Character != nullptr)
						{
							UseSkill(listSkills.At(app->combat->lastPressedAbility_I)->data, app->combat->targeted_Character);
							app->combat->targeted_Character = nullptr;
							listSkillsHistory.Add(app->combat->lastPressedAbility_I);

							//AQUI IRIAN POSIBLES PASIVAS EN UN SWITCH

							onTurn = false;
							app->combat->NextTurn();
						}
					}

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
					if ((turnDelay.ReadMSec() > 1200 && delayOn) || app->input->godMode_B)
					{
						switch (charaClass) //La idea es que cada classe tenga un comportamiento generico
						{
							int probSkill;
							case CharacterClass::MELEE_DPS:
												
								if (!listSkills.At(0)->data->PosCanBeUsed(positionCombat_I) && !listSkills.At(3)->data->PosCanBeUsed(positionCombat_I)) //No puede hacer ataques principales
								{
									//usar skill 2(1) (avance)
									UseSkill(listSkills.At(1)->data);

									listSkillsHistory.Add(2);
									break;
								}
								else
								{
									if (listSkillsHistory.end->data == 2)//Si se uso el turno pasado no se usa
									{
										probSkill = 0;
									}
									else //Si no se uso pues si puede que use el buff
									{
										probSkill = 30;
									}
									if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 3(2) (buff ofensivo)
										UseSkill(listSkills.At(2)->data);

										listSkillsHistory.Add(3);
										break;
									}
									else
									{
										if (listSkillsHistory.end->data == 3)//Si last skill buffo hace cleave
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
										//usar skill 4(3) (atk area)
										UseSkill(listSkills.At(3)->data);

										listSkillsHistory.Add(4);
										break;
									}
									else
									{
										//usar skill 1(0) (atk basico)
										UseSkill(listSkills.At(0)->data);

										listSkillsHistory.Add(1);
										break;
									}
								}
							break;
							case CharacterClass::RANGED_DPS:

								//Si esta en mala posicion aka no poder usar 0 y 3 pues usar reposicion 
								if (!listSkills.At(3)->data->PosCanBeUsed(positionCombat_I) && !listSkills.At(0)->data->PosCanBeUsed(positionCombat_I))
								{
									if (listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 2(1) (buff+reposition)
										UseSkill(listSkills.At(1)->data);

										listSkillsHistory.Add(2);
										break;
									}
								}
								else
								{
									if (listSkillsHistory.end->data != 1) //Solo si no se a movido, usado antes o usado el ataque tocho
									{
										probSkill = 0;
									}
									else
									{
										probSkill = 80;
									}
									//Usar gimick/cosa potente
									if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I)) 
									{
										//usar skill 3(2) (gimmick / debuffo)
										UseSkill(listSkills.At(2)->data);

										listSkillsHistory.Add(3);
									}
									else
									{
										if (listSkillsHistory.end->data == 3) //Usar ataque tocho que tiene targeting
										{
											probSkill = 75;
										}
										else
										{
											probSkill = 15;
										}
										//Usar ataque targeteado
										if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
										{
											//usar skill 4(3) (ataque tocho targeted)
											UseSkill(listSkills.At(3)->data);

											listSkillsHistory.Add(4);
										}
										else //Ataque basico, no hace falta if, si no podia usarla hubiera usado el movimiento
										{
											//usar skill 1(0) (ataque basico)
											UseSkill(listSkills.At(0)->data);

											listSkillsHistory.Add(1);
										}
									}

								}
								break;
							case CharacterClass::ASSASSIN:

								//Si se uso la preparacion usar si se puede la skill de asesinato, sino pues basic attack
								if (listSkillsHistory.end->data==2)
								{
									if (listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 4(3) (asesinato)
										UseSkill(listSkills.At(3)->data);

										listSkillsHistory.Add(4);
										break;
									}
									else
									{
										if (listSkills.At(0)->data->PosCanBeUsed(positionCombat_I))
										{
											//usar skill 1(0) (basic attack)
											UseSkill(listSkills.At(0)->data);

											listSkillsHistory.Add(1);
											break;
										}
									}
								}
								else //No kill mode
								{
									//Usar preparation
									if (listSkillsHistory.end->data == 2 || listSkillsHistory.end->prev->data == 2)
									{
										probSkill = 0;
									}
									else
									{
										if (listSkillsHistory.end->data == 3) //Si uso debuffo pues prepara asesinato
										{
											probSkill = 120;
										}
										else
										{
											probSkill = 50;
										}
									}
									if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I)) //Preparar asesinato
									{
										//usar skill 2(1) (preparacion asesinato)
										UseSkill(listSkills.At(1)->data);

										listSkillsHistory.Add(2);
										break;
									}
									else //Usar ataque basico o debuffo
									{
										//Probabilidad usar debuff
										if (listSkillsHistory.end->data==1) //Si uso el ataque basico
										{
											probSkill = 80;
										}
										else
										{
											probSkill = 40;
										}
										if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
										{
											//usar skill 3(2) (debuff)
											UseSkill(listSkills.At(2)->data);

											listSkillsHistory.Add(3);
											break;
										}
										else
										{
											if (CalculateRandomProbability(probSkill) && listSkills.At(0)->data->PosCanBeUsed(positionCombat_I)) //Preparar asesinato
											{
												//usar skill 1(0) (ataque basico)
												UseSkill(listSkills.At(0)->data);

												listSkillsHistory.Add(1);
												break;
											}
										}
									}
								}
								break;
							case CharacterClass::AOE_DPS:

								//Si uso el ataque tocho pues necesita descansar un turno
								if (listSkillsHistory.end->data==4)
								{
									listSkillsHistory.Add(0);
								}
								else
								{
									//Posicion jodida
									if (listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 3(2) (ataque debil por no posicionado)
										UseSkill(listSkills.At(2)->data);

										listSkillsHistory.Add(3);
										break;
									}
									else // Posicion habilitada para funcionar
									{
										probSkill = 65;
										for (int i = listSkillsHistory.Count() - 1; i > 0; i--)
										{
											if (listSkillsHistory.At(i)->data == 2)
											{
												//Por cada vez que se uso seguidamente la skill 2(1) menos probable usarla de nuevo
												probSkill -= 35;
											}
											else
											{
												break;
											}
										}
										if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I)) //Preparar asesinato
										{
											//usar skill 2(1) (cargar)
											UseSkill(listSkills.At(1)->data);

											listSkillsHistory.Add(2);
											break;
										}
										else
										{
											if (listSkillsHistory.end->data == 2)
											{
												probSkill = 85;
											}
											else
											{
												probSkill = 30;
											}
											if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I)) //Usar ataque en area tocho
											{
												//usar skill 2(1) (cargar)
												UseSkill(listSkills.At(1)->data);

												listSkillsHistory.Add(2);
												break;
											}
											else if( listSkills.At(0)->data->PosCanBeUsed(positionCombat_I)) //Usar ataque basico
											{
												//usar skill 1(0) (basic attack)
												UseSkill(listSkills.At(0)->data);

												listSkillsHistory.Add(1);
												break;
											}
										}
									}
								}
								break;
							case CharacterClass::HEALER:
								//Mirar si hay alguien esta  muy herido
								probSkill = 0;
								for (int i = 0; i < app->combat->vecEnemies.size(); i++)
								{
									if (app->combat->vecEnemies.at(i)->currentHp == app->combat->vecEnemies.at(i)->maxHp / 4) //Muy mal de vida
									{
										probSkill = 100; //Si un aliado esta muy herido es "garantizado" que intente usar la cura unitaria
									}
									else if (app->combat->vecEnemies.at(i)->currentHp == app->combat->vecEnemies.at(i)->maxHp / 2 && probSkill==0) //Algo mal de vida
									{
										probSkill = 40;
									}
								}
								//Usar cura unitaria (en el aliado mas herido), no la puede usar dos veces seguidas
								if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I) && listSkillsHistory.end->data != 2)
								{
									//Usar cura unitaria 2(1)
									UseSkill(listSkills.At(1)->data);

									listSkillsHistory.Add(2);
								}
								else
								{
									//Probabilidad usar heal area
									if (listSkillsHistory.end->data == 2) //Uso cura unitaria turno anterior
									{
										// 0, 12, 25
										probSkill /= 4;
									}
									else
									{
										//10, 30 0 60
										probSkill /= 2; 
										probSkill += 10;
									}
									if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
									{
										//Usar cura en area 4(3)
										UseSkill(listSkills.At(3)->data);

										listSkillsHistory.Add(4);
									}
									else
									{
										if (listSkillsHistory.end->data==3)
										{
											probSkill = 0;
										}
										else if(listSkillsHistory.end->prev->data == 3)
										{
											probSkill = 20;
										}
										else
										{
											probSkill = 45;
										}
										if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
										{
											//Usar gimmick propia 3(2)
											UseSkill(listSkills.At(2)->data);

											listSkillsHistory.Add(3);
										}
										else if ( listSkills.At(0)->data->PosCanBeUsed(positionCombat_I))
										{
											//Usar basic attack 1(0)
											UseSkill(listSkills.At(0)->data);

											listSkillsHistory.Add(1);
										}
									}

								}

								break;
							case CharacterClass::TANK:
							{
								if (currentHp >= maxHp / 2) //Alto de vida
								{
									if (listSkillsHistory.end->data == 3)//Si se uso el turno pasado no se usa
									{
										probSkill = 10;
									}
									else //Si no se uso pues casi siempre la usa
									{
										probSkill = 60;
									}
									if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 3(2) (tanqueo high HP)
										UseSkill(listSkills.At(2)->data);

										listSkillsHistory.Add(3);
										break;
									}
									else
									{
										probSkill = 75;
									}
								}
								else //Bajo de vida
								{
									if (listSkillsHistory.end->data == 2)//Si se uso el turno pasado no se repite casi
									{
										probSkill = 0;
									}
									else //Si no se uso pues casi siempre la usa
									{
										probSkill = 70;
									}
									if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 2(1) (tanqueo low HP)
										UseSkill(listSkills.At(1)->data);

										listSkillsHistory.Add(2);
										break;
									}
									{
										probSkill = 25;
									}
								}
								if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))//Ataques
								{
									//usar skill 4(3) (daño + debuff)
									UseSkill(listSkills.At(3)->data);

									listSkillsHistory.Add(4);
									break;
								}
								else
								{
									//usar skill 1(0) (daño solo) (es la skill mas debil)
									UseSkill(listSkills.At(0)->data);

									listSkillsHistory.Add(1);
									break;
								}
							}
							break;
							case CharacterClass::BUFFER:

								if (positionCombat_I > 0) //Posicion comoda
								{
									//Si ataque last turn poco probable usar attack
									if (listSkillsHistory.end->data == 2 || listSkillsHistory.end->data == 1)
									{
										probSkill = 10;
									}
									else //Si no se uso pues mas probable que la use
									{
										probSkill = 55;
									}
									//Usar un attack
									if (CalculateRandomProbability(probSkill) && listSkills.At(0)->data->PosCanBeUsed(positionCombat_I)) //Usar ataque
									{
										//usar skill 1(0) (atk)
										UseSkill(listSkills.At(0)->data);

										listSkillsHistory.Add(1);
										break;
									}
									else
									{
										//Depende de lo dañada que este la party pues buff defensivo o ofensivo
										int maxHPTeam=0;
										int actualHPTeam=0;
										for (int i = 0; i < app->combat->vecEnemies.size(); i++)
										{
											maxHPTeam += app->combat->vecEnemies.at(i)->maxHp;
											actualHPTeam += app->combat->vecEnemies.at(i)->currentHp;
										}
										probSkill = (actualHPTeam / maxHPTeam) * 100;
										if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I)) //Usar debuff area
										{
											//usar skill 3(2) (BUFF ATK)
											UseSkill(listSkills.At(2)->data);

											listSkillsHistory.Add(3);
											break;
										}
										else if (listSkills.At(3)->data->PosCanBeUsed(positionCombat_I)) //Debuff unitario
										{
											//usar skill 4(3) (BUFF DEFENSE)
											UseSkill(listSkills.At(3)->data);

											listSkillsHistory.Add(4);
											break;
										}
									}
								}
								else //Posicion jodido
								{
									if (listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 2(1) (debil de huida)
										UseSkill(listSkills.At(1)->data);

										listSkillsHistory.Add(2);
										break;
									}
								}
								break;
							case CharacterClass::DEBUFFER:

								if (positionCombat_I > 0) //Posicion comoda
								{	
									//Si ataque last turn poco probable usar attack
									if (listSkillsHistory.end->data == 2 || listSkillsHistory.end->data == 1)
									{
										probSkill = 10;
									}
									else //Si no se uso pues mas probable que la use
									{
										probSkill = 65;
									}
									//Usar un attack
									if (CalculateRandomProbability(probSkill) && listSkills.At(0)->data->PosCanBeUsed(positionCombat_I)) //Usar ataque
									{
										//usar skill 1(0) (atk)
										UseSkill(listSkills.At(0)->data);

										listSkillsHistory.Add(1);
										break;
									}
									else
									{
										if (listSkillsHistory.end->data == 3)//Usar debuffo area
										{
											probSkill = 25;
										}
										else //Usar debuffo area
										{
											probSkill = 75;
										}
										if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I)) //Usar debuff area
										{
											//usar skill 3(2) (area debuff)
											UseSkill(listSkills.At(2)->data);

											listSkillsHistory.Add(3);
											break;
										}
										else if (listSkills.At(3)->data->PosCanBeUsed(positionCombat_I)) //Debuff unitario
										{
											//usar skill 4(3) (unitary debuff)
											UseSkill(listSkills.At(3)->data);

											listSkillsHistory.Add(4);
											break;
										}
									}
								}
								else //Posicion jodido
								{
									if ( listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar skill 1 (debil de huida)
										UseSkill(listSkills.At(1)->data);

										listSkillsHistory.Add(2);
										break;
									}
								}
								break;
							case CharacterClass::BOSS:

								//Si es el primer turno usar el opening
								if (listSkillsHistory.end->data==0)
								{
									if (listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
									{
										//usar Opener
										UseSkill(listSkills.At(1)->data);

										listSkillsHistory.Add(2);
										break;
									}
									else //Si no pudiese usar la skill por cualquier motivo
									{
										//usar Skill Ataque basico
										UseSkill(listSkills.At(0)->data);

										listSkillsHistory.Add(1);
										break;
									}
								}
								else //No turno 1
								{
									if (currentHp >= (maxHp / 2)) //Mas de la mitad de la vida (no rage)
									{
										if (listSkillsHistory.end->data == 4)//Si uso la habilidad 4(3) no la puede usar de nuevo
										{
											probSkill = 0;
										}
										else
										{
											if (listSkillsHistory.end->data == 2)//Si uso la habilidad 2(1) opener pues usar el tocho
											{
												probSkill = 100;
											}
											else
											{
												probSkill = 65;
											}
										}

										//Usar habilidad 3 (golpe potente)
										if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
										{
											//usar skill 3 (attack poderoso)
											UseSkill(listSkills.At(3)->data);

											listSkillsHistory.Add(4);
											break;
										}
										else
										{
											if (listSkillsHistory.end->prev->data == 2) //Usado opener hace poco
											{
												if (listSkillsHistory.end->data == 2) { probSkill = 0; }//Usado turno pasado
												else { probSkill = 10; }
											}
											else
											{
												probSkill = 60;
											}

											//Usar skill 2 (1) opener
											if (CalculateRandomProbability(probSkill) && listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
											{
												//usar Opener
												UseSkill(listSkills.At(1)->data);

												listSkillsHistory.Add(2);
												break;
											}
											else //Usar default
											{
												//usar Skill Ataque basico
												UseSkill(listSkills.At(0)->data);

												listSkillsHistory.Add(1);
												break;
											}
										}
									}
									else// Vida baja (rage mode)
									{
										if (listSkillsHistory.end->prev->data == 2) //Usado rage skill hace poco
										{
											if (listSkillsHistory.end->data == 2) { probSkill = 0; }//Usado turno pasado
											else { probSkill = 15; }
										}
										else
										{
											probSkill = 85;
										}

										//Usar habilidad 3(2) (RageSkill)
										if (CalculateRandomProbability(probSkill) && listSkills.At(2)->data->PosCanBeUsed(positionCombat_I))
										{
											//usar skill 3 (attack poderoso)
											UseSkill(listSkills.At(2)->data);

											listSkillsHistory.Add(3);
											break;
										}
										else
										{
											if (listSkillsHistory.end->data == 4)//Si no se uso la habilidad 4(3) la usa
											{
												probSkill = 20;
											}
											else
											{
												probSkill = 100;
											}

											//Usar habilidad 3 (golpe potente)
											if (CalculateRandomProbability(probSkill) && listSkills.At(3)->data->PosCanBeUsed(positionCombat_I))
											{
												//usar skill 3 (attack poderoso)
												UseSkill(listSkills.At(3)->data);

												listSkillsHistory.Add(4);
												break;
											}
											else
											{
												if (listSkillsHistory.end->data == 2) //Usado opener antes
												{
													probSkill = 0;
												}
												else
												{
													probSkill = 40;
												}

												//Usar skill 2 (1) opener
												if (listSkills.At(1)->data->PosCanBeUsed(positionCombat_I))
												{
													//usar Opener
													UseSkill(listSkills.At(1)->data);

													listSkillsHistory.Add(2);
													break;
												}
												else //Usar default
												{
													//usar Skill Ataque basico
													UseSkill(listSkills.At(0)->data);

													listSkillsHistory.Add(1);
													break;
												}
											}
										}
									}
								}
								break;
							case CharacterClass::NO_CLASS:
								break;
							default:
								break;
						}
				
						app->combat->NextTurn();
						delayOn = false;
					}

					break;
				case CharacterType::NONE:
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y, &texSection);
	}
	return true;
}

bool Character::CleanUp()
{
	if(texture != nullptr)
		app->tex->UnLoad(texture);

	app->guiManager->DestroyGuiControl(button);
	
	return true;
}


bool Character::ModifyHP(int hp)
{
	currentHp += hp;

	if (currentHp >= maxHp)
	{
		currentHp = maxHp;
	}
	if (currentHp <= 0)
	{
		currentHp = 0;
	}

	if (currentHp <= 0)
	{
		LOG("%s died", name.GetString());
		//if (charaType == CharacterType::ALLY) { app->combat->RemoveCharacter(&app->combat->vecAllies, this); }
		//else if (charaType == CharacterType::ENEMY) { app->combat->RemoveCharacter(&app->combat->vecEnemies, this); }
		return false;
	}

	return true;
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
	StatusEffect* statusEffect1 = new StatusEffect(skill->firstIntensity, skill->firstDuration, skill->firstPositiveEffect, (EffectType)skill->firstStatus);
	StatusEffect* statusEffect2 = new StatusEffect(skill->secondIntensity, skill->secondDuration, skill->secondPositiveEffect, (EffectType)skill->secondStatus);
	if (skill->multiplierDmg >= 0) //Curacion o buffo, no hace falta calcular esquiva ni nada 
	{
		app->audio->PlayFx(healfx);
		
		//Primer efecto de estado
		if (skill->firstPositiveEffect) //Efecto de estado positivo
		{	
			//Blees -1 pues no lo hace
			if (defender->GetStat(EffectType::BLESS)!=-1 || skill->firstPositiveEffect!=-1) {defender->listStatusEffects.Add(statusEffect1);}
			app->audio->PlayFx(bufffx);
			//Movimiento
			app->combat->MoveCharacter(defender, skill->movementTarget);
		}
		else
		{
			if(CalculateRandomProbability(caster->GetStatModifier(EffectType::ACCURACY)*(skill->bonusAccuracy + caster->accuracy), defender->GetStat(EffectType::RES)))
			{
				if (defender->GetStat(EffectType::BLESS) != 1 || skill->firstPositiveEffect != -1){defender->listStatusEffects.Add(statusEffect1);}
				//Movimiento
				app->combat->MoveCharacter(defender, skill->movementTarget);
			}
			app->audio->PlayFx(debufffx);
		}
		//Segundo efecto de estado
		if (skill->secondPositiveEffect) //Efecto de estado positivo
		{
			if (defender->GetStat(EffectType::BLESS) != -1 || skill->secondPositiveEffect != -1) { defender->listStatusEffects.Add(statusEffect2); }
			app->audio->PlayFx(bufffx);
		}
		else
		{
			if (CalculateRandomProbability(caster->GetStatModifier(EffectType::ACCURACY) * (skill->bonusAccuracy + caster->accuracy), defender->GetStat(EffectType::RES)))
			{
				if (defender->GetStat(EffectType::BLESS) != 1 || skill->secondPositiveEffect != -1) { defender->listStatusEffects.Add(statusEffect2); }
			}
		}
		return(caster->maxHp / 5 * skill->multiplierDmg);
	}
	else //Es un ataque 
	{
		if (!CalculateRandomProbability(caster->GetStatModifier(EffectType::ACCURACY) * (skill->bonusAccuracy + caster->accuracy), defender->GetStat(EffectType::DODGE)))
		{
			//Enemigo esquiva
			return 0;
		}
		else
		{
			int damage;
			damage = skill->multiplierDmg * caster->GetStat(EffectType::ATTACK);
			if (CalculateRandomProbability(caster->GetStatModifier(EffectType::CRIT_RATE) * (skill->bonusCritRate + caster->critRate))) //Si true hay critico
			{
				//El daño critico es mas potente
				damage *= ( 100 + 2*(caster->GetStatModifier(EffectType::CRIT_DMG) * (skill->bonusCritDamage + caster->critDamage)) ) / 100;
			}

			//Primer efecto estado 
			if (skill->firstPositiveEffect) //Efecto de estado positivo
			{
				if (defender->GetStat(EffectType::BLESS) != -1 || skill->firstPositiveEffect != -1) { defender->listStatusEffects.Add(statusEffect1); }
				//Movimiento
				app->audio->PlayFx(bufffx);
				app->combat->MoveCharacter(defender, skill->movementTarget);
			}
			else
			{
				if (CalculateRandomProbability(caster->GetStatModifier(EffectType::ACCURACY) * (skill->bonusAccuracy + caster->accuracy), defender->GetStat(EffectType::RES)))
				{
					//Movimiento
					app->combat->MoveCharacter(defender, skill->movementTarget || skill->firstPositiveEffect != -1);
					if (defender->GetStat(EffectType::BLESS) != 1) { defender->listStatusEffects.Add(statusEffect1); }
				}
				app->audio->PlayFx(debufffx);
			}

			//Segundo efecto estado 
			if (skill->firstPositiveEffect) //Efecto de estado positivo
			{
				if (defender->GetStat(EffectType::BLESS) != -1 || skill->secondPositiveEffect != -1) { defender->listStatusEffects.Add(statusEffect2); }
			}
			else
			{
				if (CalculateRandomProbability(caster->GetStatModifier(EffectType::ACCURACY) * (skill->bonusAccuracy + caster->accuracy), defender->GetStat(EffectType::RES)))
				{
					if (defender->GetStat(EffectType::BLESS) != -1 || skill->secondPositiveEffect != -1) { defender->listStatusEffects.Add(statusEffect2); }
				}
			}

			// Calcular reduccion de la defensa
			float armorRelevance = (10 * defender->GetStat(EffectType::ARMOR) / abs(damage + 1)) + 1;
			damage += ((defender->GetStat(EffectType::ARMOR)) * armorRelevance); //Esta con mas ya que damage es negativo
			if (damage > 0) { damage = -1; }

			app->audio->PlayFx(hitfx);
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
				
				//Stats
				float mult = aux.attribute("multiplierDmg").as_float();
				int dmgCrit = aux.attribute("bonusCritDamage").as_int();
				int probCrit = aux.attribute("bonusCritRate").as_int();
				int accuracy = aux.attribute("bonusAccuracy").as_int();
				

				//Status Effect 1
				int firstStatusID = aux.attribute("firstStatusID").as_int();
				bool firstPositiveEffect_B = aux.attribute("firstStatusPositive").as_bool();
				int firstDurationEffect_I = aux.attribute("firstStatusDuration").as_int();
				int firstIntensityEffect_I = aux.attribute("firstStatusIntesity").as_int();

				//Status Effect 1
				int secondtStatusID = aux.attribute("secondStatusID").as_int();
				bool secondPositiveEffect_B = aux.attribute("secondStatusPositive").as_bool();
				int secondDurationEffect_I = aux.attribute("secondStatusDuration").as_int();
				int secondIntensityEffect_I = aux.attribute("secondStatusIntesity").as_int();

				//Caster
				int movUsuario = aux.attribute("movementCaster").as_int();
				int posInicialUso = aux.attribute("posToUseStart").as_int();
				int posFinallUso = aux.attribute("posToUseEnd").as_int();

				//Targeet
				int movTarget = aux.attribute("movementTarget").as_int();
				int posInicialTarget = aux.attribute("posToTargetStart").as_int();
				int posFinalTarget = aux.attribute("posToTargetEnd").as_int();
				bool friendlyFire = aux.attribute("friendlyFire").as_bool();
				bool area = aux.attribute("areaSkill").as_bool();
				bool autoTarget = aux.attribute("autoTarget").as_bool();
				TargetingMethod method = (TargetingMethod)aux.attribute("targetMethod").as_int();

				listSkills.Add(new Skill(nombre, descripcion,
					posInicialUso, posFinallUso, posInicialTarget, posFinalTarget, method,
					movUsuario, movTarget, friendlyFire, area, autoTarget, 
					mult, accuracy, probCrit, dmgCrit, firstStatusID,firstPositiveEffect_B,firstDurationEffect_I,firstIntensityEffect_I,
					secondtStatusID, secondPositiveEffect_B, secondDurationEffect_I, secondIntensityEffect_I));
			}
		}
	}
}

//Funcion para la maquina 
bool Character::UseSkill(Skill* skill)
{
	if(skill->autoTarget)
	{
		ModifyHP(ApplySkill(this, this, skill)); //Lanzarsela a si mismo
		app->audio->PlayFx(healfx);
		return true;
	}

	if (skill->targetFriend) //Targetea a gente de su propio grupo
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
				if (skill->targetFriend)
				{
					for (size_t i = skill->posToTargetStart_I; i < endRange; i++)
					{
						//Atacar a todos
						if (!app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill))) { break; }
					}
				}
				else
				{
					for (size_t i = skill->posToTargetStart_I; i < endRange; i++)
					{
					//Atacar a todos
					if (!app->combat->vecAllies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(i), skill))) { break; }
					}
				}		
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange, app->combat->vecAllies,skill->methodTarget);
				if (!app->combat->vecAllies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(objective), skill))) { break; }
			}
			app->audio->PlayFx(healfx);
			break;
		case CharacterType::ENEMY:
			endRange = skill->RangeCanTarget(app->combat->vecEnemies);
			if (endRange == -1)
			{
				return false;
			}

			if (skill->areaSkill)
			{
				for (size_t i = skill->posToTargetStart_I; i < endRange; i++)
				{
					//Atacar a todos
					if (!app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill))) { break; }
				}
				break;
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange, app->combat->vecEnemies, skill->methodTarget);
				app->combat->vecEnemies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(objective), skill));
			}
			app->audio->PlayFx(healfx);
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
		case CharacterType::ALLY: //Esto es un tanto inutil, quiza lo acabe borrando

			endRange = skill->RangeCanTarget(app->combat->vecEnemies);
			if (endRange == -1)
			{
				return false;
			}

			if (skill->areaSkill)
			{
				for (size_t i = skill->posToTargetStart_I; i < endRange; i++)
				{
					//Atacar a todos
					if (!app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill))) { break; }
				}
				app->audio->PlayFx(hitfx);
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange, app->combat->vecAllies, skill->methodTarget);
				app->combat->vecEnemies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(objective), skill));
				app->audio->PlayFx(hitfx);
			}
			app->audio->PlayFx(hitfx);
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
					if (!app->combat->vecAllies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(i), skill))) {break; }
				}
				app->audio->PlayFx(hitfx);
			}
			else
			{
				int objective = skill->RandomTarget(skill->posToTargetStart_I, endRange, app->combat->vecAllies, skill->methodTarget);
				app->audio->PlayFx(hitfx);
				if (!app->combat->vecAllies.at(objective)->ModifyHP(ApplySkill(this, app->combat->vecAllies.at(objective), skill))) { break; }
				app->audio->PlayFx(hitfx);
			}
			break;
			app->audio->PlayFx(hitfx);
			break;
		default:
			break;
		}
	}

	//Movimiento del lanzador, el movimiento del objetivo se hace en el apply skill
	if (skill->movementCaster != 0) { app->combat->MoveCharacter(this, skill->movementCaster); }
	
	return true;
}

//Para aliados
bool Character::UseSkill(Skill* skill, Character* target)
{
	int endRange;
	if (skill->targetFriend) { endRange = skill->RangeCanTarget(app->combat->vecAllies);}
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
				break;
			case CharacterType::ENEMY:
				app->combat->vecEnemies.at(i)->ModifyHP(ApplySkill(this, app->combat->vecEnemies.at(i), skill));
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
	}
	//Si la skill mueve moverte
	if (skill->movementCaster != 0)
	{
		//Movimiento
		app->combat->MoveCharacter(this, skill->movementCaster);
	}

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
			//i->data->turnsLeft--; Se gestionara aparte
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
	case EffectType::ACCURACY:
		base = accuracy;
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
	case EffectType::TAUNT:
		base = 0;
		for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
		{
			if (i->data->type == statType)
			{
				base = 1; //Semi bool, si tiene taunt pues 1, sino sera 0
			}
		}

		return base;
	case EffectType::STUN:
		base = 0;
		for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
		{
			if (i->data->type == statType)
			{
				base = 1; //Semi bool, si tiene taunt pues 1, sino sera 0
			}
		}

		return base;
	case EffectType::BLESS:
		for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
		{
			if (i->data->type == statType)
			{
				base = i->data->intensity; //Consegir el tipo de bless (1:Block Negative,0?,-1:Block Positive)
			}
		}

		return base;
	default:
		break;
	}

	for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
	{
		if (i->data->type == statType)
		{
			output = output + i->data->intensity;
		}
		//Esto no tiene mucho sentido
		/*if (i->data->type != EffectType::CURRENT_HP)
		{
			i->data->turnsLeft--;
		}*/		
	}

	return base * ((100 + output) / 100);
}

float Character::GetStatModifier(EffectType statType)
{
	float output = 0;

	for (ListItem<StatusEffect*>* i = listStatusEffects.start; i != nullptr; i = i->next)
	{
		if (i->data->type == statType)
		{
			output = output + i->data->intensity;
		}
		/*if (i->data->type != EffectType::CURRENT_HP)
		{
			i->data->turnsLeft--;
		}*/
	}

	return ((100 + output) / 100);
}

bool Character::ReduceCountStatusEffects()
{
	List<int> toDelete;

	for (int i = 0; i < listStatusEffects.Count(); i++)
	{
		listStatusEffects.At(i)->data->turnsLeft--;
		if (listStatusEffects.At(i)->data->turnsLeft<=0)
		{
			toDelete.Add(i); //Guarda la posicon en la lista
		}
	}

	//Borar efectos de estado, se hace al reves para que la lista no haga saltos raros
	for (int j = toDelete.Count()-1; j >= 0; j--)
	{
		listStatusEffects.Del(listStatusEffects.At(toDelete.At(j)->data));
	}
	
	toDelete.Clear();

	return true;
}
