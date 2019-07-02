/*************************************************************************
| Dungeon Cartographer, a grid based map drawing program.                |
| Copyright (C) 2019  Alexander L. Hopper                                |
|                                                                        |
| This program is free software: you can redistribute it and/or modify   |
| it under the terms of the GNU General Public License as published by   |
| the Free Software Foundation, either version 3 of the License, or      |
| (at your option) any later version.                                    |
|                                                                        |
| This program is distributed in the hope that it will be useful,        |
| but WITHOUT ANY WARRANTY; without even the implied warranty of         |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          |
| GNU General Public License for more details.                           |
|                                                                        |
| You should have received a copy of the GNU General Public License      |
| along with this program.  If not, see <https://www.gnu.org/licenses/>. |
*************************************************************************/

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
