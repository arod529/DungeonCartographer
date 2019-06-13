/*!
  \bug clean up classes and structs,
  	make variables private,
  	write accessor and mutator functions
	\bug make sure objects are only copied if needed, otherwise point
**/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <gtk/gtk.h>

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

class Level;
class Settings;

struct Tileset
{
	Tileset();
	Tileset(uint, std::string, char*, GdkPixbuf*);

	/*!
	id: |se|sw|nw|ne|s|w|n|e|
		bit[0-3] = wall bits
		bit[4-7] = corner bits
	**/
	uint 				id;
	std::string	name;
	char* 			filePath;
	GdkPixbuf* 	pixbuf;
};

struct Tile
{
	int 		 gridId; 			//id of the tile's grid
	Level* 	 tileLvl; 		//pointer to tile owning level
	Tileset* tileTileset;	//tileset that defines the tile characteristics

	Tile(int _gridId, Level* _tileLvl, Tileset* _tileTileset);
};

/*!
	\bug make level private
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
		int 		 size; 		//default level size
		std::unordered_map<uint, Tileset> tileset;	//default level tileset
};

class Level
{
	public:
		std::unordered_map<uint, Tileset> tileset;											//default level tileset
		std::vector<Tile> tile; 							//tile for map
		std::vector<GtkWidget*>	drawingArea; 	//drawing areas for map

		Level(int size, std::unordered_map<uint, Tileset> tileset);
		~Level();

		//accessors
		int 			 getSize() const;
		GdkPixbuf* getTile(int i) const;
		GtkWidget* getDrawingArea(int i) const;

		//mutators
		void setTileset(std::string tileSetFile);
		void setTile(int i, int j);

	private:
		//settings
		int size; 		//default level size

};

class Settings
{
	public:
		char* uiFile = (char*)"./ui/dungeonCartographer.ui";
		char* cssFile = (char*)"./ui/dungeonCartographer.css";
		char* settingsFile = (char*)"./ui/config";

		Settings();
		~Settings();

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
