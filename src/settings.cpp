#include "settings.h"

/*!
	Initialize settings from default settings file.
**/
Settings::Settings()
{
	//try loading default settings file
	if(!loadSettingsFile(defaultSettingsFile))
		{writeDefaultSettingsFile();}

	//init global tileset
	tileset.loadFile(tilesetFile);
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
	//set default settings
	tilesetFile = defaultTilesetFile;
	mapSize = 25;
	zoomSpeed = 5;
	scrollSpeed = 5;
	zoomFollowsMouse = true;

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
			<< "MapSize=" << settings.mapSize << '\n'
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
	in.ignore(MAX, '='); in >> settings.mapSize;
	in.ignore(MAX, '='); in >> settings.zoomSpeed;
	in.ignore(MAX, '='); in >> settings.scrollSpeed;
	in.ignore(MAX, '='); in >> settings.zoomFollowsMouse;
	return in;
}
