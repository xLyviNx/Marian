#include "saveSystem.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "saveSystem.h"

SaveSystem** SaveSystem::Instance = NULL;
FILE* SaveSystem::saveFile = NULL;
/**
 * @brief Destruktor SaveSystemu.
 */
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
/**
 * @brief Funkcja tworzaca SaveSystem.
 */
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
	}
	else {
		printf("============SAVE SYSTEM: %p, %p\n", SaveSystem::Instance, (*SaveSystem::Instance));

	}
}
/**
 * @brief Funkcja niszczaca SaveSystem.
 */
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
/**
 * @brief Konstruktor SaveSystemu.
 */
SaveSystem::SaveSystem()
{
	this->Start();
}
/**
 * @brief Funkcja startowa SaveSystemu ustawiajaca instancje oraz wykonujaca podstawowe pierwsze operacje.
 */
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
/**
 * @brief Funkcja odczytujaca linie pliku zapisu.
 * @param lineNumber Linia do odczytania.
 * @return Odczytany tekst.
 */
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
/**
 * @brief Funkcja przycinajaca tekst.
 * @param text Tekst do przyciecia.
 * @param n Ilosc znakow.
 * @return Czy skrocono.
 */
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
/**
 * @brief Odczytywanie poziomu z pliku zapisu.
 * @return Poziom.
 */
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
/**
 * @brief Odczytywanie coinow z pliku zapisu.
 * @return Liczba coinow.
 */
int SaveSystem::LoadCoins()
{
	int tret = 1;
	char* loadS = readLine(2);
	tret = atoi(loadS);
	printf("[SAVE SYSTEM] Loading COINS: %d\n", tret);
	free(loadS);
	return tret;
}
/**
 * @brief Odczytywanie szybkostrzelnosci gracza z pliku zapisu.
 * @return Szybkostrzelnosc gracza.
 */
int SaveSystem::LoadShootSpeed()
{
	int tret = 1;
	char* loadS = readLine(3);
	tret = atoi(loadS);
	printf("[SAVE SYSTEM] Loading COINS: %d\n", tret);
	free(loadS);
	return tret;
}
/**
 * @brief Odczytywanie predkosci gracza z pliku zapisu.
 * @return Predkosc gracza.
 */
float SaveSystem::LoadSpeed()
{
	float tret = 6.0;
	char* loadS = readLine(4);
	printf("[SAVE SYSTEM] Attempting to convert '%s' to FLOAT SPEED\n", loadS);
	tret = atof(loadS);
	printf("[SAVE SYSTEM] Loading SPEED: %lf\n", tret);
	free(loadS);
	return tret;
}
/**
 * @brief Zapis do pliku zapisu.
 * @param lineNumber Linia w pliku do zapisania.
 * @param content Tekst do zapisania.
 */
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
/**
 * @brief Restart SaveSystemu.
 */
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
/**
 * @brief Ustawienie domyslnych wartosci dla pliku zapisu.
 */
void SaveSystem::setStartData()
{
	printAtLine(1, "1");
	printAtLine(2, "0");
	printAtLine(3, "0");
	printAtLine(4, "6.00");
}