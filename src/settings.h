#ifndef SETTINGS_H
#define SETTINGS_H

#include "tileset.h"

#include <string>


class Settings
{
	public:
		Tileset tileset;
		std::string tilesetFile{""};

		int mapSize{0};
		
		int zoomSpeed{0};
		int scrollSpeed{0};
		bool zoomFollowsMouse{0};
		int tabCount{0};

		Settings();

		//utility
		bool loadSettingsFile(std::string fileName);
		bool writeDefaultSettingsFile();

		//overloads
		friend std::ostream& operator<<(std::ostream& out, const Settings& settings);
		friend std::istream& operator>>(std::istream& in, Settings& settings);

	private:
		const std::string defaultSettingsFile = "./ui/config"; 
		const std::string defaultTilesetFile = "./tiles/defaultSet";
};

#endif
