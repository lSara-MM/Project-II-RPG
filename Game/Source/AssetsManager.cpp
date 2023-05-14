//#include "App.h"
//#include "AssetsManager.h"
//
//#include "Defs.h"
//#include "Log.h"
//
//#include <iostream>
//#include <sstream>
//
////Some links of utility
////https://icculus.org/physfs/
////https://icculus.org/physfs/docs/html/physfs_8h.html
////https://gist.github.com/Ybalrid/fb3453c997b4925d25dc9a1bf4f952b6
////https://gregfmartin.wordpress.com/2017/01/02/physicsfsphysfs-basic-tutorial/
////https://wiki.libsdl.org/SDL_RWops
//
//AssetsManager::AssetsManager() : Module()
//{
//	name = ("assetsManager");
//
//	//TODO 1: initialize physicsFS Library
//	PHYSFS_init(nullptr);
//
//	//This function create the default path to search, in this case the Output, that is the default program path too
//	if (PHYSFS_mount(".", nullptr, 1) == 0) {
//		{
//			LOG("Error message: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
//		}
//	}
//}
//
//AssetsManager::~AssetsManager()
//{
//	//TODO 1 (Solved): Close all files opened by physicsFS
//	PHYSFS_deinit();
//}
//
//bool AssetsManager::Awake(pugi::xml_node& config)
//{
//	//TODO 2 (Solved): Now add the path to search our Assets folder that is a .zip
//	//HINT: The path is the same that we actually are: Output -> Assets.zip
//
//	if (PHYSFS_addToSearchPath("Assets.zip", 1) == 0) 
//	{
//		LOG("Error message: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
//		return false;
//	}
//
//	return true;
//}
//
//SDL_RWops* AssetsManager::Load(const char* path) const
//{
//	char* buffer; //The buffer is the memory space that we will need in order to access our file
//	uint numbytes = LoadData(path, &buffer); //Here we get the size in memory of the data from our file in bytes
//
//	if (numbytes > 0)
//	{
//		SDL_RWops* RWop = SDL_RWFromConstMem(buffer, numbytes); //With the number of bytes and the buffer extracts the info from our file
//
//		return RWop;
//	}
//	else
//	{
//		return nullptr;
//	}
//}
//
//uint AssetsManager::LoadData(const char* path, char** buffer) const
//{
//	uint ret = 0;
//	
//	//TODO 3 (Solved): Here we will open the file in question
//	PHYSFS_file* fileData = PHYSFS_openRead(path);
//
//	if (fileData != nullptr)//Check if it has open the file correctly
//	{
//		uint file_lenght = PHYSFS_fileLength(fileData); //Now we extract the size of our file to give it to the buffer
//
//		*buffer = new char[file_lenght]; //Pass the size of the file to the buffer in order to alocate memory
//
//		//TODO 4 (Solved): Now we will read the data form the physFS file
//		uint numReaded = PHYSFS_read(fileData, *buffer, 1, file_lenght); //Read the data in question
//
//		if (numReaded != file_lenght) //Here we check if the two sizes are the same, theorically they have to be, but in case that not is an error
//		{
//			RELEASE(buffer); //If they are not the same clean the memory buffer
//			LOG("PhysFS: The size of the file and the number of things readed are not the same");
//		}
//		else 
//		{
//			ret = numReaded; //In case that all is alright return the number of the size readed to the function
//		}
//
//		PHYSFS_close(fileData); //Close the PhysFS file that we opened before
//	}
//
//	return ret;
//}
//
//bool AssetsManager::CleanUp()
//{
//	return false;
//}


