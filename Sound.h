#pragma once

class Sound
{
public:
	static Sound& instance();
	void playBackground();
	void playExplosive();

private:
	Sound();
	Sound(const Sound& other);

};