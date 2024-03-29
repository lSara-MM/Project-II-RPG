
#include "App.h"
#include "Render.h"
#include "Window.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "Pathfinding.h"
#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();
    mapBackground = config.child("mapbackground").attribute("path").as_string();

    mapDungeonFileName = config.child("mapdungeonfile").attribute("path").as_string();
    mapDungeonFolder = config.child("mapdungeonfolder").attribute("path").as_string();
    mapDungeonBackground = config.child("mapdungeonbackground").attribute("path").as_string();

    mapCircusFileName = config.child("mapcircusfile").attribute("path").as_string();
    mapCircusFolder = config.child("mapcircusfolder").attribute("path").as_string();
    mapCircusBackground = config.child("mapcircusbackground").attribute("path").as_string();

    mapPracticeFileName = config.child("mappracticefile").attribute("path").as_string();
    mapPracticeFolder = config.child("mappracticefolder").attribute("path").as_string();
    mapPracticeBackground = config.child("mappracticebackground").attribute("path").as_string();

    return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<MapLayer*>* item;
    item = mapData.maplayers.start;

    for (item = mapData.maplayers.start; item != NULL; item = item->next)
    {
        MapLayer* layer = item->data;

        if (!layer->properties.GetProperty("Navigation")->value)
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);

                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;
                if (tileId == 246) {
                    map[i] = 1;
                }

                else {
                    map[i] = 0;
                }
               
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}

void Map::Draw()
{
    if(mapLoaded == false)
        return;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;
 
    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value == true) {
            
            app->render->DrawTexture(backGround, 0, 0);
        }
        mapLayerItem = mapLayerItem->next;
    }
    
}

void Map::DrawPlatformCollider() {

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    ListItem<PhysBody*>* bodyItem;
    bodyItem = listBodies.start;

    while (mapLayerItem != NULL) {

        if( mapLayerItem->data->properties.GetProperty("collider") != NULL && mapLayerItem->data->properties.GetProperty("collider")->value == true)
        {
            for (int x1 = 0; x1 < mapLayerItem->data->width; x1++) 
            {
                for (int y1 = 0; y1 < mapLayerItem->data->height; y1++)
                {
                    // L05: DONE 9: Complete the draw function
                    int gid = mapLayerItem->data->Get(x1, y1);

                    //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    iPoint pos = MapToWorld(x1, y1);

                    if(gid == 1 && gid!=NULL)
                    {
                        int w, h;
                        w = 64;
                        h = 64;

                        PhysBody* co = new PhysBody;
                        co = app->physics->CreateRectangle(pos.x + w / 2, pos.y + h / 2, w, h, STATIC);
                        co->ctype = ColliderType::PLATFORM;
                        listBodies.Add(co);  
                        
                    }else if (gid == 2 && gid != NULL)
                    {
                        int w, h;
                        w = 64;
                        h = 64;

                        PhysBody* co = new PhysBody;
                        co = app->physics->CreateRectangleSensor(pos.x + w / 2, pos.y + h / 2, w, h, STATIC);
                        co->ctype = ColliderType::PORTAL;
                        co->id = portalID;
                        listBodies.Add(co);
                        if (mapLayerItem->data->Get(x1+1, y1) != 2)
                        {
                            if (mapLayerItem->data->Get(x1, y1 + 1) != 2)
                            {
                                portalID++;
                            }
                        }
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;
    }
}

void Map::DrawPaths()
{
    if (mapLoaded == false)
        return;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);

                    if (gid != 0)
                    {
                        TileSet* tileset = GetTilesetFromTileId(gid);

                        SDL_Rect r = tileset->GetTileRect(gid);
                        iPoint pos = MapToWorld(x, y);

                        app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r);
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;
    }
}


iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    ret.x = x / mapData.tileWidth;

    ret.y = y / mapData.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

   
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    ListItem<TileSet*>* item;
    item = mapData.tilesets.start;
    app->tex->UnLoad(backGround);

    while (item != NULL)
    {
        app->tex->UnLoad(item->data->texture);
        RELEASE(item->data);
        item = item->next;
    }
    mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }
    mapData.maplayers.Clear();

    ListItem<PhysBody*>* bodyItem;
    bodyItem = listBodies.start;

    while (bodyItem != NULL)
    {
        app->render->name;
        bodyItem->data->body->GetWorld()->DestroyBody(bodyItem->data->body);
        app->render->active;
        delete bodyItem->data;
        bodyItem = bodyItem->next;
    }
    listBodies.Clear();

    portalID = 0;
    mapPendingtoDelete = false;
    app->render->active;
    return true;
}

// Load new map
bool Map::Load(int ID)
{
    bool ret = true;

    mapPendingtoDelete = false;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result;

    switch (ID)
    {
    case 0:
         result = mapFileXML.load_file(mapFileName.GetString());
        break;
    case 1:
        result = mapFileXML.load_file(mapDungeonFileName.GetString());
        break;
    case 2:
        result = mapFileXML.load_file(mapCircusFileName.GetString());
        break;
    case 3:
       result = mapFileXML.load_file(mapPracticeFileName.GetString());
        break;
    }
    

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML, ID);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML, ID);
    }

    if (ret == true)
    { 
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
  
    if(ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    
    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    int w, h;
    uchar* data = NULL;

    bool retWalkMap = CreateWalkabilityMap(w, h, &data);
    if (retWalkMap) app->pathfinding->SetMap(w, h, data);
    DrawPlatformCollider();

    RELEASE_ARRAY(data);

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile, int ID)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        switch (ID)
        {
        case 0:
            backGround = app->tex->Load(mapBackground);
            break;
        case 1:
            backGround = app->tex->Load(mapDungeonBackground);
            break;
        case 2:
            backGround = app->tex->Load(mapCircusBackground);
            break;
        case 3:
            backGround = app->tex->Load(mapPracticeBackground);
            break;
        }
        
        portalID = 0;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile, int ID){

    bool ret = true; 

    pugi::xml_node tileset;
    switch (ID)
    {
    case 0:
        for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
        {
            TileSet* set = new TileSet();

            set->name = tileset.attribute("name").as_string();
            set->firstgid = tileset.attribute("firstgid").as_int();
            set->margin = tileset.attribute("margin").as_int();
            set->spacing = tileset.attribute("spacing").as_int();
            set->tileWidth = tileset.attribute("tilewidth").as_int();
            set->tileHeight = tileset.attribute("tileheight").as_int();
            set->columns = tileset.attribute("columns").as_int();
            set->tilecount = tileset.attribute("tilecount").as_int();


            SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
            set->texture = app->tex->Load(tmp.GetString());

            mapData.tilesets.Add(set);
        }
        break;
    case 1:
        for (tileset = mapFile.child("dungeon").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
        {
            TileSet* set = new TileSet();

            set->name = tileset.attribute("name").as_string();
            set->firstgid = tileset.attribute("firstgid").as_int();
            set->margin = tileset.attribute("margin").as_int();
            set->spacing = tileset.attribute("spacing").as_int();
            set->tileWidth = tileset.attribute("tilewidth").as_int();
            set->tileHeight = tileset.attribute("tileheight").as_int();
            set->columns = tileset.attribute("columns").as_int();
            set->tilecount = tileset.attribute("tilecount").as_int();


            SString tmp("%s%s", mapDungeonFolder.GetString(), tileset.child("image").attribute("source").as_string());
            set->texture = app->tex->Load(tmp.GetString());

            mapData.tilesets.Add(set);
        }
        break;

    case 2:
        for (tileset = mapFile.child("circus").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
        {
            TileSet* set = new TileSet();

            set->name = tileset.attribute("name").as_string();
            set->firstgid = tileset.attribute("firstgid").as_int();
            set->margin = tileset.attribute("margin").as_int();
            set->spacing = tileset.attribute("spacing").as_int();
            set->tileWidth = tileset.attribute("tilewidth").as_int();
            set->tileHeight = tileset.attribute("tileheight").as_int();
            set->columns = tileset.attribute("columns").as_int();
            set->tilecount = tileset.attribute("tilecount").as_int();


            SString tmp("%s%s", mapCircusFolder.GetString(), tileset.child("image").attribute("source").as_string());
            set->texture = app->tex->Load(tmp.GetString());

            mapData.tilesets.Add(set);
        }
        break;

    case 3:
        for (tileset = mapFile.child("practice").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
        {
            TileSet* set = new TileSet();

            set->name = tileset.attribute("name").as_string();
            set->firstgid = tileset.attribute("firstgid").as_int();
            set->margin = tileset.attribute("margin").as_int();
            set->spacing = tileset.attribute("spacing").as_int();
            set->tileWidth = tileset.attribute("tilewidth").as_int();
            set->tileHeight = tileset.attribute("tileheight").as_int();
            set->columns = tileset.attribute("columns").as_int();
            set->tilecount = tileset.attribute("tilecount").as_int();


            SString tmp("%s%s", mapPracticeFolder.GetString(), tileset.child("image").attribute("source").as_string());
            set->texture = app->tex->Load(tmp.GetString());

            mapData.tilesets.Add(set);
        }
        break;
    }
    

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();


    LoadProperties(node, layer->properties);

    //LoadGidValue(node, layer->gids);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); 
        
        properties.list.Add(p);
    }

    return ret;
}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}

