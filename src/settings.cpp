#include "settings.h"

#include "constants.h"

#include <fstream>
#include <cstdio>
#include <limits>

/*!
	Initialize settings from default settings file.
**/
Settings::Settings()
{
	// try loading default settings file
	if(!loadSettingsFile(defaultSettingsFile))
		{writeDefaultSettingsFile();}
}

/*!
	Returns the default map size.

	@return The global size for a map.
**/
int Settings::getMapSize() const
	{return mapSize;}
/*!
	Returns the zoom speed.

	@return The zoom speed in pixels change per zoom action
**/
int Settings::getZoomSpeed() const
	{return zoomSpeed;}
/*!
	Returns the scroll speed.

	@return The scroll speed in pixels change per scroll action.
**/
int Settings::getScrollSpeed() const
	{return scrollSpeed;}
/*!
	Returns the zoom follows mouse setting.

	@return Does zoom follow the mouse.
**/
bool Settings::getZoomFollowsMouse() const
	{return zoomFollowsMouse;}
/*!
	Sets the default map size.

	@param[in] _mapSize The global size for a map
**/
void Settings::setMapSize(int _mapSize)
	{mapSize = mapSize;}
/*!
	Sets the zoom speed.

	@param[in] _zoomSpeed The zoom speed in pixels change per zoom action.
**/
void Settings::setZoomSpeed(int _zoomSpeed)
	{zoomSpeed = _zoomSpeed;}
/*!
	Sets the scroll speed.

	@param[in] _scrollSpeed The scroll speed in pixels change per scroll action.
**/
void Settings::setScrollSpeed(int _scrollSpeed)
	{scrollSpeed = _scrollSpeed;}
/*!
	Set the zoom follows mouse setting.

	@param[in] _zoomFollowsMouse Does zoom follow the mouse.
**/
void Settings::setZoomFollowsMouse(bool _zoomFollowsMouse)
	{zoomFollowsMouse = _zoomFollowsMouse;}

/*!
  Writes a default Tileset file to disk.
**/
bool Settings::writeDefaultTilesetFile()
{
	//open file
	std::ofstream file;
	file.open(defaultTilesetFile.c_str());

	//Make sure file can be opened
	if(!file.is_open())
	{
		fprintf(stderr, "ERROR: Dungeon Cartographer @ writeDefaultTilesetFile: The file could not be opened: %s\n", defaultTilesetFile.c_str());
		return false;
	}

	//write to file (id,name,filePath\0\n) for each tile
	file << E << ',' << "E Wall" << ',' << "./tiles/eWall.svg" << '\n'
			 << E_NW << ',' << "E Wall NW Corner" << ',' << "./tiles/eWall_nwCorner.svg" << '\n'
			 << E_SW << ',' << "E Wall SW Corner" << ',' << "./tiles/eWall_swCorner.svg" << '\n'
			 << E_NW_SW << ',' << "E Wall NW-SW Corner" << ',' << "./tiles/eWall_nw-swCorner.svg" << '\n'

			 << N << ',' << "N Wall" << ',' << "./tiles/nWall.svg" << '\n'
			 << N_SW << ',' << "N Wall SE Corner" << ',' << "./tiles/nWall_seCorner.svg" << '\n'
			 << N_SE << ',' << "N Wall SW Corner" << ',' << "./tiles/nWall_swCorner.svg" << '\n'
			 << N_SW_SE << ',' << "N Wall SE-SW Corner" << ',' << "./tiles/nWall_sw-seCorner.svg" << '\n'

			 << W << ',' << "W Wall" << ',' << "./tiles/wWall.svg" << '\n'
			 << W_NE << ',' << "W Wall NE Corner" << ',' << "./tiles/wWall_neCorner.svg" << '\n'
			 << W_SE << ',' << "W Wall SE Corner" << ',' << "./tiles/wWall_seCorner.svg" << '\n'
			 << W_NE_SE << ',' << "W Wall NE-SE Corner" << ',' << "./tiles/wWall_ne-seCorner.svg" << '\n'

			 << S << ',' << "S Wall" << ',' << "./tiles/sWall.svg" << '\n'
			 << S_NE << ',' << "S Wall NE Corner" << ',' << "./tiles/sWall_neCorner.svg" << '\n'
			 << S_NW << ',' << "S Wall NW Corner" << ',' << "./tiles/sWall_nwCorner.svg" << '\n'
			 << S_NE_NW << ',' << "S Wall NE-NW Corner" << ',' << "./tiles/sWall_ne-nwCorner.svg" << '\n'

			 << NE << ',' << "NE Wall" << ',' << "./tiles/neWall.svg" << '\n'
			 << NE_SW << ',' << "NE Wall SW Corner" << ',' << "./tiles/neWall_swCorner.svg" << '\n'
			 << SE << ',' << "SE Wall" << ',' << "./tiles/seWall.svg" << '\n'
			 << SE_NW << ',' << "SE Wall NW Corner" << ',' << "./tiles/seWall_nwCorner.svg" << '\n'
			 << NW << ',' << "NW Wall" << ',' << "./tiles/nwWall.svg" << '\n'
			 << NW_SE << ',' << "NW Wall SE Corner" << ',' << "./tiles/nwWall_seCorner.svg" << '\n'
			 << SW << ',' << "SW Wall" << ',' << "./tiles/swWall.svg" << '\n'
			 << SW_NE << ',' << "SW Wall NE Corner" << ',' << "./tiles/swWall_neCorner.svg" << '\n'

			 << WE << ',' << "EW Wall" << ',' << "./tiles/ewWall.svg" << '\n'
			 << NS << ',' << "NS Wall" << ',' << "./tiles/nsWall.svg" << '\n'

			 << NEW << ',' << "NEW Wall" << ',' << "./tiles/newWall.svg" << '\n'
			 << NSE << ',' << "NSE Wall" << ',' << "./tiles/nseWall.svg" << '\n'
			 << SEW << ',' << "SEW Wall" << ',' << "./tiles/sewWall.svg" << '\n'
			 << NSW << ',' << "NSW Wall" << ',' << "./tiles/nswWall.svg" << '\n'
			 << NSEW << ',' << "NSEW Wall" << ',' << "./tiles/nsewWall.svg" << '\n'

			 << O << ',' << "Open" << ',' << "./tiles/open.svg" << '\n'
			 << O_NE << ',' << "Open NE Corner" << ',' << "./tiles/open_neCorner.svg" << '\n'
			 << O_NW << ',' << "Open NW Corner" << ',' << "./tiles/open_nwCorner.svg" << '\n'
			 << O_NE_NW << ',' << "Open NE-NW Corner" << ',' << "./tiles/open_ne-nwCorner.svg" << '\n'
			 << O_SW << ',' << "Open SW Corner" << ',' << "./tiles/open_swCorner.svg" << '\n'
			 << O_NE_SW << ',' << "Open NE-SW Corner" << ',' << "./tiles/open_ne-swCorner.svg" << '\n'
			 << O_NW_SW << ',' << "Open NW-SW Corner" << ',' << "./tiles/open_nw-swCorner.svg" << '\n'
			 << O_NE_NW_SW << ',' << "Open NE-NW-SW Corner" << ',' << "./tiles/open_ne-nw-swCorner.svg" << '\n'
			 << O_SE << ',' << "Open SE Corner" << ',' << "./tiles/open_seCorner.svg" << '\n'
			 << O_NE_SE << ',' << "Open NE-SE Corner" << ',' << "./tiles/open_ne-seCorner.svg" << '\n'
			 << O_NW_SE << ',' << "Open NW-SE Corner" << ',' << "./tiles/open_nw-seCorner.svg" << '\n'
			 << O_NE_NW_SE << ',' << "Open NE-NW-SE Corner" << ',' << "./tiles/open_ne-nw-seCorner.svg" << '\n'
			 << O_SW_SE << ',' << "Open SW-SE Corner" << ',' << "./tiles/open_sw-seCorner.svg" << '\n'
			 << O_NE_SW_SE << ',' << "Open NE-SW-SE Corner" << ',' << "./tiles/open_ne-sw-seCorner.svg" << '\n'
			 << O_NW_SW_SE << ',' << "Open NW-SW-SE Corner" << ',' << "./tiles/open_nw-sw-seCorner.svg" << '\n'
			 << O_NE_NW_SW_SE << ',' << "Open NE-NW-SW-SE Corner" << ',' << "./tiles/open_ne-nw-sw-seCorner.svg" << '\n'

			 << BACKGROUND << ',' << "Background" << ',' << "./tiles/bg.svg" << '\n';

	//close file
	file.close();

	return true;
}

/*!
  Load Settings from a file on disk.

  @return Whether or not the load was successful.
**/
bool Settings::loadSettingsFile(const std::string _fileName)
{
	//open file
	std::ifstream file;
	file.open(_fileName.c_str());

	//Make sure file can be opened
	if(!file.is_open())
	{
		fprintf(stderr, "ERROR: Dungeon Cartographer @ loadSettingsFile: The file could not be opened: %s\n", _fileName.c_str());
		return false;
	}

	//read file
	file >> *this;
	file.close();

	return true;
}

/*!
  Writes a default Settings file to disk.
**/
bool Settings::writeDefaultSettingsFile()
{
	//set default settings
	mapSize = 25;
	zoomSpeed = 5;
	scrollSpeed = 5;
	zoomFollowsMouse = true;

	//open file
	std::ofstream file(defaultSettingsFile.c_str());

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

std::ostream& operator<<(std::ostream& _out, const Settings& _settings)
{
	_out << "MapSize=" << _settings.mapSize << '\n'
			 << "ZoomSpeed=" << _settings.zoomSpeed << '\n'
			 << "ScrollSpeed=" << _settings.scrollSpeed << '\n'
			 << "ZoomFollowsMouse=" << _settings.zoomFollowsMouse << '\n';
	 _out.flush();
	 return _out;
}

std::istream& operator>>(std::istream& _in, Settings& _settings)
{
	static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

	_in.ignore(MAX, '='); _in >> _settings.mapSize;
	_in.ignore(MAX, '=');	_in >> _settings.zoomSpeed;
	_in.ignore(MAX, '=');	_in >> _settings.scrollSpeed;
	_in.ignore(MAX, '=');	_in >> _settings.zoomFollowsMouse;
	return _in;
}
