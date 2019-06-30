#ifndef SETTINGS_H
#define SETTINGS_H
#include "tileset.h"

#include <fstream>
#include <limits>
#include <string>


class Settings
{
	public:
		Tileset tileset;             //global program tileset
		std::string tilesetFile{""}; //file associated with the tileset

		//global map defaults
		int mapWidth{25};
		int mapHeight{25};

		int zoomSpeed{5};
		int scrollSpeed{5};
		bool zoomFollowsMouse{true};

		Settings();

		//utility
		bool loadSettingsFile(std::string fileName);
		bool writeDefaultSettingsFile();

		//overloads
		friend std::ostream& operator<<(std::ostream& out, const Settings& settings);
		friend std::istream& operator>>(std::istream& in, Settings& settings);

	private:
		const std::string defaultSettingsFile = "./ui/config";
		const std::string defaultTilesetFile  = "./tiles/defaultSet.dct";
};

#endif
