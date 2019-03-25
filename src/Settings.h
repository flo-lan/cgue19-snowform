#pragma once

class Settings
{
public:
	static Settings& getInstance()
	{
		static Settings instance;
		return instance;
	}

private:
	Settings();

};

#define sSettings Settings::getInstance()
