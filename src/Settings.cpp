#include "Settings.h"
#include "INIReader.h"
#include <glm/detail/func_trigonometric.inl>

Settings::Settings()
= default;

void Settings::Load()
{
	// init reader for ini files
    INIReader reader("assets/settings.ini");

	windowWidth = reader.GetInteger("window", "width", 1280);
	windowHeight = reader.GetInteger("window", "height", 720);
	isFullscreen = reader.GetBoolean("window", "fullscreen", false);
	title = reader.Get("window", "title", "Snowform");
	maxFps = reader.GetInteger("window", "refresh_rate", 60);
    brightness = static_cast<float>(reader.GetReal("window", "brightness", 1.f));
	
	cameraFov = static_cast<float>(glm::radians(reader.GetReal("camera", "fov", 60.f)));
	cameraNear = static_cast<float>(reader.GetReal("camera", "near", 0.1f));
	cameraFar = static_cast<float>(reader.GetReal("camera", "far", 100.1f));
}

int Settings::getWindowWidth() const
{
	return windowWidth;
}

int Settings::getWindowHeight() const
{
	return windowHeight;
}

bool Settings::getFullscreen() const
{
	return isFullscreen;
}

std::string Settings::getTitle() const
{
	return title;
}

int Settings::getMaxFps() const
{
	return maxFps;
}

float Settings::getBrightness() const
{
    return brightness;
}

float Settings::getCameraFov() const
{
	return cameraFov;
}
float Settings::getCameraNear() const
{
	return cameraNear;
}
float Settings::getCameraFar() const
{
	return cameraFar;
}