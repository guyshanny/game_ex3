#include "Sound.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <windows.h>

#define SOUNDS_FOLDER "sounds\\"

Sound& Sound::instance() 
{
	static Sound instance;
	return instance;
}

void Sound::playBackground()
{
	PlaySound(SOUNDS_FOLDER"background.wav", NULL, SND_ASYNC | SND_LOOP);
}

void Sound::playExplosive()
{
	int min = 1;
	int max = 4;
	int num = min + (rand() % (int)(max - min + 1));
	std::string path = std::string(SOUNDS_FOLDER) + 
					   std::string("explosive") + 
					   std::to_string(num) + 
					   std::string(".wav");
	PlaySound(path.c_str(), NULL, NULL);
}

Sound::Sound() { }