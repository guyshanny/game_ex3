#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <stdio.h>
#include <string>

/*
* a class to load single-buffer sound files.
* Currently only .wav files are supported...
*/

class SoundLoader {
public:
	/*
	* Load sound data to openAL, and returns useful handles
	* filename - the path of the sound file
	* buffer - the openAL handle for the data buffer (note that only a single buffer
	* is supported in this class - for multiple buffers see AudioStream)
	* size, frequency, format - more data about the file.
	*
	*/
	virtual bool loadSoundFile(const std::string filename, ALuint* buffer,
		ALsizei* size, ALsizei* frequency,
		ALenum* format) = 0;
};

/*
* A class to load .wav sounds to openAL. Extends SoundLoader.
*/
class WaveSoundLoader : public SoundLoader {
public:
	virtual bool loadSoundFile(const std::string filename, ALuint* buffer,
		ALsizei* size, ALsizei* frequency,
		ALenum* format);
private:
	// all kinds of things related to the structure of a .wav file
	struct RIFF_Header {
		char chunkID[4];
		int chunkSize;//size not including chunkSize or chunkID
		char format[4];
	};

	char padding[4];

	struct WAVE_Format {
		char subChunkID[4];
		int subChunkSize;
		short audioFormat;
		short numChannels;
		int sampleRate;
		int byteRate;
		short blockAlign;
		short bitsPerSample;
	};

	struct WAVE_Data {
		char subChunkID[4]; //should contain the word data
		int subChunk2Size; //Stores the size of the data block
	};

};