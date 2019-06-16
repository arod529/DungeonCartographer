#ifndef SETTINGS_H
#define SETTINGS_H

#include <gtk/gtk.h>

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

class Level;
class Settings;

/*!
  Tileset data structure. Contains the id, name, file path, and pixbuf of the
  tile image. An array of tileset structs defines a complete tileset.

  \bug variables should be private, and read only aside from initialization.
**/
struct Tileset
{
public:
	Tileset();
	Tileset(uint _id, std::string _name, std::string _filePath, GdkPixbuf* _pixbuf);
	~Tileset();

	uint getId() const;
	std::string getName() const;
	std::string getFilePath() const;

	GdkPixbuf* pixbuf;
private:
	uint id;
	std::string	name;
	std::string filePath;
};

/*!
  Tile data structure. Contains the grid id, the containing Level, and Tileset
  of the tile. Internal functions update the tiles, queue a draw, and examine the
  adjacent tiles for existence and index.

  \bug gridId and tileLvl should be private and read only after initialization.
  \bug locked should be private.
  \bug getTileExists(), getAdjacentIndex() updateCornerBits(), queDraw() should be private
**/
class Tile
{
public:
	int 		 gridId; 			//id of the tile's grid
	Level* 	 tileLvl; 		//pointer to tile owning level
	Tileset* tileTileset;	//tileset that defines the tile characteristics

	bool locked = false; //protection against multiple recursion

	Tile(int _gridId, Level* _tileLvl, Tileset* _tileTileset);
	void updateTile();

	void getTileExists(bool* _tileExists);
	void getAdjacentIndex(int* _adjacentIndex);
	void updateCornerBits(bool _propagate);
	void queDraw();
};

/*!
  Level data structure. Tileset and grid size for new levels default to the global
  values in Map().

  \bug tileset should be pointer
**/
class Level
{
	public:
		std::unordered_map<uint, Tileset>* tileset;											//default level tileset
		std::vector<Tile> tile; 							//tile for map
		std::vector<GtkWidget*>	drawingArea; 	//drawing areas for map

		Level(int _size, std::unordered_map<uint, Tileset>* _tileset);

		//accessors
		int getSize() const;

	private:
		int size; //default level size
};

/*!
	Map data structure. Tileset, and grid size for new maps default to the the global
	values in Settings.

	\bug make level private
	\bug tileset should be pointer
	\bug incomplete definition
	\bug add map file
	\bug should be in own header with Level, Tile, and Tileset?
**/
class Map
{
	public:
		Map(Settings*);						//default map
		Map(std::string mapFile);	//map from file
		~Map();										//close a map

		//utility
		void loadTileset(std::string tileSetFile);

		//accessors
		int getSize();

		//mutators
		void setTileset(std::string tileSetFile);
		void setSize(int s);

		std::vector<Level> level;		//array of levels for a map

	private:
		int size; //default level size
		std::unordered_map<uint, Tileset> tileset; //default level tileset
};

class Settings
{
	public:
		char* uiFile = (char*)"./ui/dungeonCartographer.ui";
		char* cssFile = (char*)"./ui/dungeonCartographer.css";
		char* settingsFile = (char*)"./ui/config";

		Settings();

		//utiltiy
		void writeDefaultTilesetFile();
		void writeDefaultSettingsFile();

		//accessors
		int getMapSize() const;
		int getZoomSpeed() const;
		int getScrollSpeed() const;
		bool getZoomFollowsMouse() const;

		//mutators
		void setMapSize(int _mapSize);
		void setZoomSpeed(int _zoomSpeed);
		void setScrollSpeed(int _scrollSpeed);
		void setZoomFollowsMouse(bool _zoomFollowsMouse);

	private:
		int mapSize;
		int zoomSpeed;
		int scrollSpeed;

		bool zoomFollowsMouse;

		int tabCount;
};

#endif
