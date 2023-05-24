#ifndef __LOOTMANAGER_H__
#define __LOOTMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "Point.h"
#include "SString.h"
#include "Render.h"
#include "Scene.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "ItemManager.h"
#include "Player.h"
#include "Character.h"
#include "Combat.h"

#include <vector>
using namespace std;


enum class ChestTypes
{
	RARE,
	COMON,
	EMPTY,
	COMBAT,
};

enum class ChestDungeon
{
	TERRORS,
	BEASTS,
};

class Chest
{
	public:
		Chest(int x_, int y_, int type_, int dungeon_, SString texturePath_)
		{
			x_ = x; y_ = y;

			switch (type_)
			{
			case 0:
				type = ChestTypes::RARE;
				break;
			case 1:
				type = ChestTypes::COMON;
				break;
			case 2:
				type = ChestTypes::EMPTY;
				break;
			case 3:
				type = ChestTypes::COMBAT;
				break;
			}

			switch (dungeon_)
			{
			case 0:
				dungeon = ChestDungeon::TERRORS;
				break;
			case 1:
				dungeon = ChestDungeon::BEASTS;
				break;
			}

			texture = app->tex->Load(texturePath_.GetString());

		};
		~Chest() {};
		bool Start()
		{
			hitbox = app->physics->CreateRectangle(x - app->render->camera.x, y - app->render->camera.x, 64, 64, bodyType::STATIC);
			sensor = app->physics->CreateRectangleSensor((x-10) - app->render->camera.x, (y-10) - app->render->camera.x, 74, 74, bodyType::STATIC);

			switch (type)
			{
			case ChestTypes::RARE:

				switch (dungeon)
				{
				case ChestDungeon::TERRORS:

					break;
				case ChestDungeon::BEASTS:

					break;
				}

				break;

			case ChestTypes::COMON:

				switch (dungeon)
				{
				case ChestDungeon::TERRORS:

					break;
				case ChestDungeon::BEASTS:

					break;
				}

				break;

			case ChestTypes::EMPTY:

				break;

			case ChestTypes::COMBAT:

				break;
			}

			return true;
		}
		bool Update(float dt)
		{
			return true;
		}
		bool CleanUp()
		{
			if (hitbox != nullptr)
			{
				hitbox->body->GetWorld()->DestroyBody(hitbox->body);
			}
			if (sensor != nullptr)
			{
				sensor->body->GetWorld()->DestroyBody(sensor->body);
			}

			app->tex->UnLoad(texture);
			return true;
		}

	private:

		int x, y;

		ChestTypes type;
		ChestDungeon dungeon;

		SDL_Texture* texture;

		PhysBody* hitbox;
		PhysBody* sensor;

		Animation* currentAnimation = nullptr;

		// A set of animations
		Animation openAnim;
		Animation iddleAnim;

};

class LootManager : public Module
{
	public:
		LootManager();
		~LootManager();

		// Called before render is available
		bool Awake(pugi::xml_node& config);
		bool Start();
		bool Update(float dt);
		bool CleanUp();

		bool LoadLootState(pugi::xml_node& xml_trees);
		bool SaveLootState();

	private:

};

#endif // __LOOTMANAGER_H__
