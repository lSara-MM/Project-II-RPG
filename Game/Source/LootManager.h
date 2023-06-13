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
#include "PuzzleManager.h"
#include "FadeToBlack.h"
#include "HouseOfTerrors.h"
#include "HouseOfTerrors.h"
#include "BeastTent.h"
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

class ItemLoot
{
public:
	ItemLoot() {};
	~ItemLoot() {};
public:
	int ID;
	int quantity;
	SString name;
};

class LootTable
{
public:
	LootTable(ChestTypes type_, ChestDungeon dungeon_)
	{
		type = type_;
		dungeon = dungeon_;
	};
	~LootTable() {};

	bool Awake(pugi::xml_node& config);
	bool Start()
	{
		switch (type)
		{
		case ChestTypes::COMON:
			switch (dungeon)
			{
			case ChestDungeon::TERRORS:
				random = rand() % 5;
				switch (random)
				{
				case 0:
					itemLoot->ID = 93;
					itemLoot->quantity = 4;
					itemLoot->name = "Wood";
					lootTableID.push_back(*itemLoot);
					itemLoot->ID = 94;
					itemLoot->quantity = 1;
					itemLoot->name = "Copper";
					lootTableID.push_back(*itemLoot);
					itemLoot->ID = 98;
					itemLoot->quantity = 4;
					itemLoot->name = "Straw";
					lootTableID.push_back(*itemLoot);
					break;
				case 1:
					itemLoot->ID = 85;
					itemLoot->quantity = 1;
					itemLoot->name = "Iron";
					lootTableID.push_back(*itemLoot);
					itemLoot->ID = 94;
					itemLoot->quantity = 1;
					itemLoot->name = "Copper";
					lootTableID.push_back(*itemLoot);
					itemLoot->ID = 100;
					itemLoot->quantity = 3;
					itemLoot->name = "Leather";
					lootTableID.push_back(*itemLoot);
					break;
				case 2:

					itemLoot->ID = 101;
					itemLoot->quantity = 1;
					itemLoot->name = "Onyx";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 100;
					itemLoot->quantity = 5;
					itemLoot->name = "Leather";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 98;
					itemLoot->quantity = 3;
					itemLoot->name = "Straw";

					lootTableID.push_back(*itemLoot);

					break;
				case 3:
					itemLoot->ID = 101;
					itemLoot->quantity = 2;
					itemLoot->name = "Onyx";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 102;
					itemLoot->quantity = 3;
					itemLoot->name = "Amber";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 99;
					itemLoot->quantity = 5;
					itemLoot->name = "Rope";

					lootTableID.push_back(*itemLoot);

					break;
				case 4:
					itemLoot->ID = 102;
					itemLoot->quantity = 1;
					itemLoot->name = "Amber";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 85;
					itemLoot->quantity = 2;
					itemLoot->name = "Iron";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 99;
					itemLoot->quantity = 4;
					itemLoot->name = "Rope";

					lootTableID.push_back(*itemLoot);

					break;
				}

				break;
			case ChestDungeon::BEASTS:
				random = rand() % 4;
				switch (random)
				{
				case 0:
					itemLoot->ID = 107;
					itemLoot->quantity = 2;
					itemLoot->name = "Coral";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 102;
					itemLoot->quantity = 1;
					itemLoot->name = "Amber";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 101;
					itemLoot->quantity = 1;
					itemLoot->name = "Onyx";

					lootTableID.push_back(*itemLoot);
					break;
				case 1:
					itemLoot->ID = 97;
					itemLoot->quantity = 2;
					itemLoot->name = "Steel";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 95;
					itemLoot->quantity = 3;
					itemLoot->name = "Iron";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 99;
					itemLoot->quantity = 2;
					itemLoot->name = "Rope";

					lootTableID.push_back(*itemLoot);
					break;
				case 2:
					itemLoot->ID = 93;
					itemLoot->quantity = 2;
					itemLoot->name = "Wood";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 95;
					itemLoot->quantity = 2;
					itemLoot->name = "Iron";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 98;
					itemLoot->quantity = 2;
					itemLoot->name = "Rope";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 101;
					itemLoot->quantity = 2;
					itemLoot->name = "Onyx";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 102;
					itemLoot->quantity = 2;
					itemLoot->name = "Amber";

					lootTableID.push_back(*itemLoot);

					break;
				case 3:
					itemLoot->ID = 107;
					itemLoot->quantity = 2;
					itemLoot->name = "Coral";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 96;
					itemLoot->quantity = 2;
					itemLoot->name = "Steel";

					lootTableID.push_back(*itemLoot);

					break;
				}

				break;
			}
		break;

		case ChestTypes::RARE:

			switch (dungeon)
			{
			case ChestDungeon::TERRORS:

				itemLoot->ID = 108;
				itemLoot->quantity = 1;
				itemLoot->name = "Pearl";

				lootTableID.push_back(*itemLoot);

				itemLoot->ID = 106;
				itemLoot->quantity = 1;
				itemLoot->name = "Amethyst";

				lootTableID.push_back(*itemLoot);

				itemLoot->ID = 102;
				itemLoot->quantity = 2;
				itemLoot->name = "Amber";

				lootTableID.push_back(*itemLoot);

				itemLoot->ID = 101;
				itemLoot->quantity = 2;
				itemLoot->name = "Onyx";

				lootTableID.push_back(*itemLoot);

				itemLoot->ID = 95;
				itemLoot->quantity = 3;
				itemLoot->name = "Iron";

				lootTableID.push_back(*itemLoot);

				itemLoot->ID = 94;
				itemLoot->quantity = 5;
				itemLoot->name = "Copper";

				lootTableID.push_back(*itemLoot);

				break;
			case ChestDungeon::BEASTS:
				random = rand() % 2;
				switch (random)
				{
				case 0:
					itemLoot->ID = 105;
					itemLoot->quantity = 1;
					itemLoot->name = "Jade";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 104;
					itemLoot->quantity = 1;
					itemLoot->name = "Diamond";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 103;
					itemLoot->quantity = 1;
					itemLoot->name = "Emerald";

					lootTableID.push_back(*itemLoot);

					break;
				case 1:
					itemLoot->ID = 97;
					itemLoot->quantity = 2;
					itemLoot->name = "Titanium";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 112;
					itemLoot->quantity = 1;
					itemLoot->name = "Gold";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 108;
					itemLoot->quantity = 2;
					itemLoot->name = "Pearl";

					lootTableID.push_back(*itemLoot);

					itemLoot->ID = 106;
					itemLoot->quantity = 2;
					itemLoot->name = "Amethyst";

					lootTableID.push_back(*itemLoot);
					break;
				}
				break;
			}
		break;
		}
		return true;
	}

	bool Update(float dt);

	void GiveLoot()
	{
		if (type == ChestTypes::COMBAT)
		{
			LOG("Combat");
			app->audio->PlayFx(app->hTerrors->combatfx);
			if(app->hTerrors->active)
			{
				app->combat->PreLoadCombat(app->hTerrors->name);
			}
			else if (app->BeastT->active)
			{
				app->combat->PreLoadCombat(app->BeastT->name);
			}
			else
			{
				app->combat->PreLoadCombat(app->scene->name);
			}
			app->SaveGameRequest();

			if (dungeon == ChestDungeon::TERRORS)
			{
				app->fade->FadingToBlack((Module*)app->hTerrors, (Module*)app->combat, 45);
			}
			else
			{
				app->fade->FadingToBlack((Module*)app->BeastT, (Module*)app->combat, 45);
			}
			app->puzzleManager->Disable();
			app->hTerrors->steps_I = 0;
		}
		for (int i = 0; i < lootTableID.size(); i++)
		{
			app->itemManager->AddQuantity(lootTableID[i].ID, lootTableID[i].quantity);
		}
	}

	void PrintLoot()
	{
		for (int i = 0; i < lootTableID.size(); i++)
		{
			app->render->TextDraw(lootTableID[i].name.GetString(), 400, 200 + (35 * i), 30, Font::TEXT, { 255, 246, 240 });
		}
	}

	bool CleanUp()
	{
		lootTableID.clear();

		return true;
	}

public:

	vector <ItemLoot> lootTableID;
	int random; //offset + (rand() % range);
	ItemLoot* itemLoot = new ItemLoot;
	ChestTypes type; ChestDungeon dungeon;
};

class Chest
{
	public:
		Chest(int x_, int y_, ChestTypes type_, ChestDungeon dungeon_, SString texturePath_, int Id)
		{
			x = x_; y = y_;
			path = texturePath_;
			type = type_;
			dungeon = dungeon_;
			ID = Id;
		};
		~Chest() {};
		bool Start()
		{
			texture = app->tex->Load(path.GetString());
			hitbox = app->physics->CreateRectangle(x + 32, y + 32, 64, 50, bodyType::STATIC);
			sensor = app->physics->CreateRectangleSensor((x + 32), (y + 32), 80, 80, bodyType::STATIC, ID);
			sensor->ctype = ColliderType::LOOT;
			lootTable = new LootTable(type, dungeon);
			lootTable->Start();
			if (type == ChestTypes::RARE)
			{
				//Animations
				iddleAnim.PushBack({ 1, 10, 62, 55 });
				openAnim.PushBack({ 1, 10, 62, 55 });
				openAnim.PushBack({ 0, 84, 65, 44 });
				openAnim.PushBack({ 3, 128, 55, 62 });
				openAnim.PushBack({ 3, 128, 55, 62 });
				openAnim.PushBack({ 3, 128, 55, 62 });
				openAnim.PushBack({ 0, 19, 63, 66 });
				openAnim.PushBack({ 0, 19, 63, 66 });
				openAnim.PushBack({ 0, 19, 63, 66 });
			}
			else
			{
				//Animations
				iddleAnim.PushBack({ 193, 268, 61, 52 });
				openAnim.PushBack({ 193, 268, 61, 52 });
				openAnim.PushBack({ 191, 341, 66, 42 });
				openAnim.PushBack({ 197, 383, 51, 65 }); 
				openAnim.PushBack({ 197, 383, 51, 65 }); 
				openAnim.PushBack({ 194, 449, 60, 63 });
				openAnim.PushBack({ 194, 449, 60, 63 });
				openAnim.PushBack({ 194, 449, 60, 63 });
			}
			openAnim.loop = false;
			openAnim.speed = 0.1f;
			iddleAnim.loop = true;
			iddleAnim.speed = 0.1f;
			currentAnimation = &iddleAnim;
			return true;
		}
		bool Update(float dt)
		{
			if (dungeon == ChestDungeon::TERRORS && app->hTerrors->active==true)
			{
				currentAnimation->Update();
				SDL_Rect rect = currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(texture, x, y, &rect);
			}else if (dungeon == ChestDungeon::BEASTS && app->BeastT->active == true)
			{
				currentAnimation->Update();
				SDL_Rect rect = currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(texture, x, y, &rect);
			}
			if (currentAnimation == &openAnim && currentAnimation->HasFinished())
			{
				used = true;
				CleanUp();
			}
			if (currentAnimation == &openAnim && !currentAnimation->HasFinished())
			{
				lootTable->PrintLoot();
			}
			return true;
		}

		void UseChest()
		{
			if (type == ChestTypes::COMBAT)
			{
				used = true;
			}
			lootTable->GiveLoot();
			currentAnimation = &openAnim;
		}

		bool CleanUp()
		{
			if (hitbox != nullptr)
			{
				hitbox->body->GetWorld()->DestroyBody(hitbox->body);
				hitbox = nullptr;
			}
			if (sensor != nullptr)
			{
				sensor->id = -1;
				sensor->body->GetWorld()->DestroyBody(sensor->body);
				sensor = nullptr;
			}
			lootTable->CleanUp();
			app->tex->UnLoad(texture);
			currentAnimation = nullptr;
			return true;
		}

	public:

		int x, y;
		int ID;
		bool used = false;

		ChestTypes type;
		ChestDungeon dungeon;
		SDL_Texture* texture;
		SString path;
		PhysBody* hitbox;
		PhysBody* sensor;

		Animation* currentAnimation = nullptr;
		Animation openAnim;
		Animation iddleAnim;
		LootTable* lootTable;
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
		bool CleanUpDos();
		bool LoadState();
		bool SaveState(pugi::xml_node& data);
	public:
		vector <Chest*> chests;
};

#endif // __LOOTMANAGER_H__
