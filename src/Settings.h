#pragma once
#include <string>
#include "INIReader.h"

class Settings
{
public:
	static Settings& getInstance()
	{
		static Settings instance;
		return instance;
	}

	// We want to make sure these two methods are unacceptable,
   // otherwise we may accidentally get copies of our singleton
	Settings(Settings const&) = delete;
	void operator=(Settings const&) = delete;

	void Load();
	int getWindowWidth() const;
	int getWindowHeight() const;
	bool getFullscreen() const;
	std::string getTitle() const;
	int getMaxFps() const;

	float getCameraFov() const;
	float getCameraNear() const;
	float getCameraFar() const;

private:
	Settings();

	int windowWidth;
	int windowHeight;
	bool isFullscreen;
	string title;
	int maxFps;

	float cameraFov;
	float cameraNear;
	float cameraFar;
};

#define sSettings Settings::getInstance()
