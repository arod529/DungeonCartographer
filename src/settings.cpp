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

#include "settings.h"

/*!
	Initialize settings from default settings file.
**/
Settings::Settings()
{
	//try loading default settings file
	if(!loadSettingsFile(defaultSettingsFile))
		{writeDefaultSettingsFile();}

	//try loading default tileset file
	if(!tileset.loadFile(tilesetFile))
	{
		tileset.writeDefaultTilesetFile(tilesetFile);
		tileset.loadFile(tilesetFile);
	}
}

/*!
  Load Settings from a file on disk.

	@param[in] fPath The path to the settings file.

  @return Load was successful.
**/
bool Settings::loadSettingsFile(const std::string fPath)
{
	//open file
	std::ifstream file(fPath);

	//Make sure file can be opened
	if(!file.is_open())
	{
		fprintf(stderr, "ERROR: Dungeon Cartographer @ loadSettingsFile: The file could not be opened: %s\n", fPath.c_str());
		return false;
	}

	//read file
	file >> *this;
	file.close();

	return true;
}

/*!
  Writes a default Settings file to disk.

  @return File write success.
**/
bool Settings::writeDefaultSettingsFile()
{
	tilesetFile = defaultTilesetFile;
	
	//open file
	std::ofstream file(defaultSettingsFile);

	//Make sure file can be opened
	if(!file.is_open())
	{
		fprintf(stderr, "ERROR: Dungeon Cartographer @ writeDefaultSettingsFile: The file could not be opened: %s\n", defaultSettingsFile.c_str());
		return false;
	}

	//write to file
	file << *this;
	file.close();

	return true;
}

//---------------------
//----- Overloads -----
//---------------------
std::ostream& operator<<(std::ostream& out, const Settings& settings)
{
	out << "Tileset=" << settings.tilesetFile << '\n'
			<< "MapWidth=" << settings.mapWidth << '\n'
			<< "MapHeight=" << settings.mapHeight << '\n'
			<< "ZoomSpeed=" << settings.zoomSpeed << '\n'
			<< "ScrollSpeed=" << settings.scrollSpeed << '\n'
			<< "ZoomFollowsMouse=" << settings.zoomFollowsMouse << '\n';
	 out.flush();
	 return out;
}

std::istream& operator>>(std::istream& in, Settings& settings)
{
	static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

	in.ignore(MAX, '='); in >> settings.tilesetFile;
	in.ignore(MAX, '='); in >> settings.mapWidth;
	in.ignore(MAX, '='); in >> settings.mapHeight;
	in.ignore(MAX, '='); in >> settings.zoomSpeed;
	in.ignore(MAX, '='); in >> settings.scrollSpeed;
	in.ignore(MAX, '='); in >> settings.zoomFollowsMouse;
	return in;
}
