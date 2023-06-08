#pragma once
#include <stdio.h>
#define LINE_LENGTH 32
/**
 * @brief Lokalizacja pliku zapisu
 */
#define SAVEFILE_LOCATION "data/save.txt"
 /**
  * @brief Lokalizacja pliku tymczasowego
  */
#define TEMPFILE_LOCATION "data/save.tmp"
  /**
   * @brief Klasa SaveSystemu.
   * 
   * Klasa ta pozwala na zapis oraz odczyt danych gracza z i do pliku.
   */
class SaveSystem
{
	void Start();
public:
	static FILE* saveFile; /**< Zaladowany plik zapisu. */
	static SaveSystem** Instance; /**< Instancja klasy. */
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