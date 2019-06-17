#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
	public:
		const std::string defaultTilesetFile = "./tiles/defaultSet";
		const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
		const char* cssFile = (char*)"./ui/dungeonCartographer.css";

		int mapSize;
		int zoomSpeed;
		int scrollSpeed;

		bool zoomFollowsMouse;

		int tabCount;

		Settings();

		//utility
		bool writeDefaultTilesetFile();
		bool writeDefaultSettingsFile();
		bool loadSettingsFile(std::string fileName);

		//overloads
		friend std::ostream& operator<<(std::ostream& out, const Settings& settings);
		friend std::istream& operator>>(std::istream& in, Settings& settings);

	private:
		const std::string defaultSettingsFile = "./ui/config";
};

#endif
