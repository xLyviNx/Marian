#pragma once
#include <stdio.h>
#define LINE_LENGTH 32
#define SAVEFILE_LOCATION "data/save.txt"
#define TEMPFILE_LOCATION "data/save.tmp"
class SaveSystem
{
	void Start();
public:
	static FILE* saveFile;
	static SaveSystem** Instance;
	SaveSystem();
	char* readLine(int);
	int LoadLevel();
	int LoadCoins();
	int LoadShootSpeed();
	float LoadSpeed();
	void printAtLine(int, const char*);
	//void resetData();
	void restart();
	void setStartData();
	~SaveSystem();
	static void Create();
	static void Destroy();

};
bool trimText(char*, int);