#include "saveSystem.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "saveSystem.h"
SaveSystem** SaveSystem::Instance = NULL;
FILE* SaveSystem::saveFile = NULL;
SaveSystem::~SaveSystem()
{
	if (SaveSystem::Instance && *(SaveSystem::Instance) == this) {
		(*SaveSystem::Instance) = NULL;
		if (SaveSystem::saveFile) {
			fclose(SaveSystem::saveFile);
			SaveSystem::saveFile = NULL;
			std::cout << "[SAVE SYSTEM] DESTROYED." << std::endl;
		}
	}
}
void SaveSystem::Create()
{
	if (!SaveSystem::Instance)
	{
		SaveSystem::Instance = (SaveSystem**)malloc(sizeof(SaveSystem*));
		if (SaveSystem::Instance)
		{
			(*SaveSystem::Instance) = NULL;
		}
	}
	if (SaveSystem::Instance && *SaveSystem::Instance == NULL)
	{
		SaveSystem* sS = new SaveSystem();
		//SaveSystem::Instance = &sS;
	}
	else {
		printf("============SAVE SYSTEM: %p, %p\n", SaveSystem::Instance, (*SaveSystem::Instance));

	}
}
void SaveSystem::Destroy()
{
	if (SaveSystem::Instance && *(SaveSystem::Instance) != NULL)
	{
		SaveSystem* ss = *SaveSystem::Instance;
		*SaveSystem::Instance = NULL;
		printf("DESTROYING %p\n", ss);
		delete(ss);

	}
}
SaveSystem::SaveSystem()
{
	this->Start();
}
void SaveSystem::Start()
{
	if (!SaveSystem::Instance)
	{
		SaveSystem::Instance = (SaveSystem**)malloc(sizeof(SaveSystem*));
	}
	if (SaveSystem::Instance)
	{
		if (!*SaveSystem::Instance && SaveSystem::saveFile)
		{
			*SaveSystem::Instance = this;
		}
		if ((!(*SaveSystem::Instance) && !SaveSystem::saveFile) || (*SaveSystem::Instance == this && !SaveSystem::saveFile))
		{
			fopen_s(&(SaveSystem::saveFile), SAVEFILE_LOCATION, "r+");
			if (SaveSystem::saveFile) {
				printf("[SAVE SYSTEM] Save System Initialized\n");
			}
			else
			{
				printf("[SAVE SYSTEM] Save File Not Found! Creating One.\n");
				fopen_s(&(SaveSystem::saveFile), SAVEFILE_LOCATION, "w+");
				if (SaveSystem::saveFile)
				{
					(*SaveSystem::Instance) = this;
					setStartData();
				}
				else
				{
					printf("[SAVE SYSTEM] Save System Initialization FAILED!\n");

				}

			}
			if (!SaveSystem::saveFile)
			{
				printf("[SAVE SYSTEM] Save System Initialization FAILED, %p!\n", this);
				
			}
			else
			{
				(*SaveSystem::Instance) = this;

			}
			
		}
		else
		{
			printf("[SAVE SYSTEM] Save System ALREADY INITIALIZED! (SAVE FILE: %p, OBJ: %p (%p) vs %p)\n", SaveSystem::saveFile, SaveSystem::Instance, *SaveSystem::Instance, this);
			if ((*SaveSystem::Instance) != this)
			{
				delete(this);
			}
		}
		printf("Instance: %p, me: %p\n", *SaveSystem::Instance, this);
	}
	else
	{
		printf("[SAVE SYSTEM] Everything is broken!");
	}
}
char* SaveSystem::readLine(int lineNumber)
{
	char* buf = (char*)malloc(LINE_LENGTH * sizeof(char));
	if (SaveSystem::saveFile && buf) {
		std::cout << " SAVE INSTANCE : " << (SaveSystem::Instance != NULL) << " , BUF : " << (buf != NULL) << std::endl;
		fseek(SaveSystem::saveFile, 0, SEEK_SET);
		bool keepread = true;
		int currLine = 1;
		do
		{
			fgets(buf, LINE_LENGTH, SaveSystem::saveFile);
			trimText(buf, LINE_LENGTH);
			printf("Checking (%d): %s\n", currLine, buf);
			if (currLine == lineNumber)
			{
				keepread = false;
				printf("[SAVE SYSTEM] Returning Line %d: '%s'\nREMEMBER TO FREE YOUR MEMORY AFTER FINISHING OPERATIONS\n", currLine, buf);
			}
			if (keepread)
			{
				if (feof(SaveSystem::saveFile))
				{
					keepread = false;
					buf[0] = '\0';
					printf("[SAVE SYSTEM] Could not find the line.\n");
				}
			}
			currLine++;

		} while (keepread);
	}
	return buf != NULL ? buf : NULL;

}
bool trimText(char* text, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (text[i] == '\n' && text[i + 1] == '\0')
		{
			text[i] = '\0';
			return true;
		}
	}
	return false;
}
int SaveSystem::LoadLevel()
{
	int tret = 1;
	char* loadS = readLine(1);
	std::cout << "[SAVE SYSTEM] Attempting to convert '" << loadS << "' to INT.\n";
	tret = atoi(loadS);
	printf("[SAVE SYSTEM] Loading Level: %d\n", tret);
	free(loadS);
	return tret;
}
void SaveSystem::printAtLine(int lineNumber, const char* content)
{

	if (SaveSystem::Instance && *SaveSystem::Instance == this) {
		if (!SaveSystem::saveFile)
		{
			printf("[SAVE SYSTEM] ERROR! Not initialized (saveLine)\n");
			return;
		}
		fseek(SaveSystem::saveFile, 0, SEEK_SET);
		int currLine = 1;
		char buf[LINE_LENGTH] = "";
		//strcpy_s(stringToPut, LINE_LENGTH, content);
		FILE* tempfile = NULL;
		fopen_s(&tempfile, TEMPFILE_LOCATION, "w");
		if (tempfile)
		{
			while (!feof(SaveSystem::saveFile) && fgets(buf, LINE_LENGTH, SaveSystem::saveFile) != NULL)
			{
				if (currLine == lineNumber)
				{
					strcpy_s(buf, LINE_LENGTH * sizeof(char), content);
					//printf("line: %d, '%s'\n", currLine, buf);
				}
				//printf("Putting '%s'\n", buf);
				fputs(buf, tempfile);
				int lastl = strlen(buf) - 1;
				if (buf[lastl] != '\n' && buf[lastl] != '\0' && !feof(SaveSystem::saveFile)) {
					fputc('\n', tempfile);
					//printf("CBUF: '%c' (+1: %d)\n", buf[lastl], buf[lastl+1]);
				}
				currLine++;
			}
			bool atEnd = false;
			while (currLine <= lineNumber && currLine > 1)
			{
				atEnd = true;
				fputc('\n', tempfile);
				currLine++;
			}
			if (atEnd || currLine == 1)
			{
				strcpy_s(buf, LINE_LENGTH * sizeof(char), content);
				fputs(buf, tempfile);
				int lastl = strlen(buf) - 1;
					if (buf[lastl] != '\n' && buf[lastl] != '\0' && !feof(SaveSystem::saveFile)) {
					fputc('\n', tempfile);
					//printf("CBUF: '%c' (+1: %d)\n", buf[lastl], buf[lastl + 1]);
				}
			}
			puts("[SAVE SYSTEM] Reloading the SAVE SYSTEM (replacing files)");
			fclose(tempfile);
			fclose(SaveSystem::saveFile);
			(SaveSystem::saveFile) = NULL;
			remove(SAVEFILE_LOCATION);
			rename(TEMPFILE_LOCATION, SAVEFILE_LOCATION);
			remove(TEMPFILE_LOCATION);
			fopen_s(&(SaveSystem::saveFile), SAVEFILE_LOCATION, "r+");
			if (!(SaveSystem::saveFile))
			{
				std::cout << "[SAVE SYSTEM] Couldn't load file again. Destroying Instance.\n";
				SaveSystem::Destroy();
			}

		}
		else
		{
			puts("[SAVE SYSTEM] ERROR CREATING TEMP FILE.");
		}
	}
}
/*void SaveSystem::resetData()
{
	puts("[SAVE SYSTEM] RESETTING DATA");

	if (SaveSystem::Instance && *SaveSystem::Instance == this) {
		SaveSystem::Destroy();
		remove(SAVEFILE_LOCATION);
		SaveSystem::Create();
	}

}*/
void SaveSystem::restart()
{

	if (SaveSystem::Instance && *SaveSystem::Instance == this) {
		puts("[SAVE SYSTEM] RESTARTING");
		if (SaveSystem::saveFile)
		{
			fclose(saveFile);
		}
			SaveSystem::Destroy();
		
		*SaveSystem::Instance = NULL;
		SaveSystem::Create();
	}
}
void SaveSystem::setStartData()
{
	printAtLine(1, "1");
}