#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "Z:/CS32/BoulderBlast/Assets" or "/Users/fred/cs32/BoulderBlast/Assets"

const string assetDirectory = "Assets"; 

class GameWorld;

GameWorld* createStudentWorld(string assetDir = "");

int main(int argc, char* argv[])
{
	
	{
		string path = assetDirectory;
		if (!path.empty())
			path += '/';
		const string someAsset = "frack1.tga";
		ifstream ifs(path + someAsset);
		if (!ifs)
		{
			cout << "Cannot find " << someAsset << " in ";
			cout << (assetDirectory.empty() ? "current directory"
											: assetDirectory) << endl;
			return 1;
		}
	}
	
	srand(static_cast<unsigned int>(time(nullptr)));
	
	GameWorld* gw = createStudentWorld(assetDirectory);
	Game().run(argc, argv, gw, "FrackMan");
	/*
	if (_CrtDumpMemoryLeaks())
		std::cerr << "Memory Leak!" << std::endl;
	else
		std::cerr << "No Memory Leak!" << std::endl;
	*/
}
