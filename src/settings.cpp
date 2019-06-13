#include "settings.h"
#include "constants.h"

//--------------------------
//----- Tileset Struct -----
//--------------------------

/*!
  Default empty initializers for tileset struct
**/
Tileset::Tileset()
{
	id = 0;
	name = "";
	filePath = NULL;
	pixbuf = NULL;
}

/*!
  Assignment initializer for tileset struct
**/
Tileset::Tileset(uint _id, std::string _name, char* _filePath, GdkPixbuf* _pixbuf)
{
	id = _id;
	name = _name;
	filePath = _filePath;
	pixbuf = _pixbuf;
}

//-----------------------
//----- Tile Struct -----
//-----------------------
Tile::Tile(int _gridId, Level* _tileLvl, Tileset* _tileTileset)
{
	gridId = _gridId;
	tileLvl = _tileLvl;
	tileTileset = _tileTileset;
}

//---------------------
//----- Map Class -----
//---------------------
/*!
	Initializes a map with defaults from settings
**/
Map::Map(Settings* _settings)
{
	//set default size
	size = _settings->getMapSize();

	//set default tileset
	loadTileset("./tiles/defaultSet");

	//create a level
	level.emplace_back(size, tileset);
}

/*!
	Free memory of map
**/
Map::~Map()
{
	for(auto iter = tileset.begin(); iter != tileset.end(); iter++)
	{
		auto i = iter->first;
		delete[] tileset[i].filePath;
		g_object_unref(tileset[i].pixbuf);
	}
}

/*!
	Load a tileset from a file

	\bug doesn't check if file exists

	@param[in] tileSetFile file name for the tile set to load
**/
void Map::loadTileset(std::string tileSetFile)
{
	//open file
	std::ifstream file;
	file.open(tileSetFile);

	//get number of tile in tileset
	int count;
	file >> count;

	//read tileset
	std::string tmp;
	uint _id;
	do
	{
		file >> _id;
		tileset[_id].id = _id;
		std::getline(file, tmp, ','); //eat comma
		std::getline(file, tileset[_id].name, ',');
		std::getline(file, tmp);
		tileset[_id].filePath = new char[tmp.size()];
		tmp.copy(tileset[_id].filePath, tmp.size(), 0);
	} while(!file.eof());

	//close file
	file.close();

	//create pixbufs
	GError *err = NULL;
	for(auto iter = tileset.begin(); iter != tileset.end(); iter++) {
		auto i = iter->first;
		tileset[i].pixbuf = gdk_pixbuf_new_from_file(tileset[i].filePath, &err);
		if(err != NULL) printf("%s\n", err->message);
	}
}

//-----------------------
//----- Level Class -----
//-----------------------
Level::Level(int _size, std::unordered_map<uint, Tileset> _tileset)
{
	//set map size and tileset
	size = _size;
	tileset = _tileset;

	//set all tiles to background tile
	for(int i = 0; i < size*size; i++)
		tile.emplace_back(i, this, &tileset[BACKGROUND]);

	//initilize drawing areas for map
	for(int i = 0; i < size*size; i++)
		drawingArea.emplace_back(gtk_drawing_area_new());
	for(int i = 0; i < size*size; i++)
		gtk_widget_set_can_focus (drawingArea[i],true);
}

Level::~Level()
{

}

int Level::getSize() const
{
	return size;
}

// GtkWidget* Level::getDrawingArea(int i) const
// {
// 	return drawingArea[i];
// }

//--------------------------
//----- Settings Class -----
//--------------------------
/*!
	Initilize settings from file
**/
Settings::Settings()
{
	std::fstream file;

	//try default file
	file.open(settingsFile, std::ios_base::in);
	if(file.is_open()) //file exist read settings
	{
		file >> mapSize
				 >> zoomSpeed
				 >> scrollSpeed
				 >> zoomFollowsMouse;
	}
	else //write and use default settings
	{
		mapSize = 25;
		zoomSpeed = 5;
		scrollSpeed = 5;
		zoomFollowsMouse = true;

		file.open(settingsFile, std::ios_base::out);
		file << mapSize << '\n'
				 << zoomSpeed << '\n'
				 << scrollSpeed << '\n'
				 << zoomFollowsMouse << '\n';
	}
}

Settings::~Settings()
{

}

/*!
	Returns the default map size

	@return int the global size for a map
**/
int Settings::getMapSize() const
	{return mapSize;}
/*!
	Returns the zoom speed

	@return int the zoom speed in pixels change per zoom action
**/
int Settings::getZoomSpeed() const
	{return zoomSpeed;}
/*!
	Returns the scroll speed

	@return int the scroll speed in pixels change per scroll action
**/
int Settings::getScrollSpeed() const
	{return scrollSpeed;}
/*!
	Returns the zoom follows mouse setting

	@return bool does zoom follow the mouse
**/
bool Settings::getZoomFollowsMouse() const
	{return zoomFollowsMouse;}
/*!
	Sets the default map size

	@param[in] int the global size for a map
**/
void Settings::setMapSize(int _mapSize)
	{mapSize = mapSize;}
/*!
	Sets the zoom speed

	@param[in] int the zoom speed in pixels change per zoom action
**/
void Settings::setZoomSpeed(int _zoomSpeed)
	{zoomSpeed = _zoomSpeed;}
/*!
	Sets the scroll speed

	@param[in] int the scroll speed in pixels change per scroll action
**/
void Settings::setScrollSpeed(int _scrollSpeed)
	{scrollSpeed = _scrollSpeed;}
/*!
	Set the zoom follows mouse setting

	@param[in] bool does zoom follow the mouse
**/
void Settings::setZoomFollowsMouse(bool _zoomFollowsMouse)
	{zoomFollowsMouse = _zoomFollowsMouse;}


void Settings::writeDefaultTilesetFile()
{
	std::unordered_map<uint, Tileset> tileset;

	tileset[E] = 			 (Tileset){E, "E Wall", (char *)"./tiles/eWall.svg", NULL};
	tileset[E_NW] = 	 (Tileset){E_NW, "E Wall NW Corner", (char *)"./tiles/eWall_nwCorner.svg", NULL};
	tileset[E_SW] = 	 (Tileset){E_SW, "E Wall SW Corner", (char *)"./tiles/eWall_swCorner.svg", NULL};
	tileset[E_NW_SW] = (Tileset){E_NW_SW, "E Wall NW-SW Corner", (char *)"./tiles/eWall_nw-swCorner.svg", NULL};

	tileset[N] =			 (Tileset){N, "N Wall", (char *)"./tiles/nWall.svg", NULL};
	tileset[N_SW] = 	 (Tileset){N_SW, "N Wall SE Corner", (char *)"./tiles/nWall_seCorner.svg", NULL};
	tileset[N_SE] = 	 (Tileset){N_SE, "N Wall SW Corner", (char *)"./tiles/nWall_swCorner.svg", NULL};
	tileset[N_SW_SE] = (Tileset){N_SW_SE, "N Wall SE-SW Corner", (char *)"./tiles/nWall_sw-seCorner.svg", NULL};

	tileset[W] = 			 (Tileset){W, "W Wall", (char *)"./tiles/wWall.svg", NULL};
	tileset[W_NE] =		 (Tileset){W_NE, "W Wall NE Corner", (char *)"./tiles/wWall_neCorner.svg", NULL};
	tileset[W_SE] =		 (Tileset){W_SE, "W Wall SE Corner", (char *)"./tiles/wWall_seCorner.svg", NULL};
	tileset[W_NE_SE] = (Tileset){W_NE_SE, "W Wall NE-SE Corner", (char *)"./tiles/wWall_ne-seCorner.svg", NULL};

	tileset[S] = 			 (Tileset){S, "S Wall", (char *)"./tiles/sWall.svg", NULL};
	tileset[S_NE] = 	 (Tileset){S_NE, "S Wall NE Corner", (char *)"./tiles/sWall_neCorner.svg", NULL};
	tileset[S_NW] = 	 (Tileset){S_NW, "S Wall NW Corner", (char *)"./tiles/sWall_nwCorner.svg", NULL};
	tileset[S_NE_NW] = (Tileset){S_NE_NW, "S Wall NE-NW Corner", (char *)"./tiles/sWall_ne-nwCorner.svg", NULL};

	tileset[NE] = 	 (Tileset){NE, "NE Wall", (char *)"./tiles/neWall.svg", NULL};
	tileset[NE_SW] = (Tileset){NE_SW, "NE Wall SW Corner", (char *)"./tiles/neWall_swCorner.svg", NULL};
	tileset[SE] = 	 (Tileset){SE, "SE Wall", (char *)"./tiles/seWall.svg", NULL};
	tileset[SE_NW] = (Tileset){SE_NW, "SE Wall NW Corner", (char *)"./tiles/seWall_nwCorner.svg", NULL};
	tileset[NW] = 	 (Tileset){NW, "NW Wall", (char *)"./tiles/nwWall.svg", NULL};
	tileset[NW_SE] = (Tileset){NW_SE, "NW Wall SE Corner", (char *)"./tiles/nwWall_seCorner.svg", NULL};
	tileset[SW] =		 (Tileset){SW, "SW Wall", (char *)"./tiles/swWall.svg", NULL};
	tileset[SW_NE] = (Tileset){SW_NE, "SW Wall NE Corner", (char *)"./tiles/swWall_neCorner.svg", NULL};

	tileset[WE] = (Tileset){WE, "EW Wall", (char *)"./tiles/ewWall.svg", NULL};
	tileset[NS] = (Tileset){NS, "NS Wall", (char *)"./tiles/nsWall.svg", NULL};

	tileset[NEW] = (Tileset){NEW, "NEW Wall", (char *)"./tiles/newWall.svg", NULL};
	tileset[NSE] = (Tileset){NSE, "NSE Wall", (char *)"./tiles/nseWall.svg", NULL};
	tileset[SEW] = (Tileset){SEW, "SEW Wall", (char *)"./tiles/sewWall.svg", NULL};
	tileset[NSW] = (Tileset){NSW, "NSW Wall", (char *)"./tiles/nswWall.svg", NULL};

	tileset[NSEW] = (Tileset){NSEW, "NSEW Wall", (char *)"./tiles/nsewWall.svg", NULL};

	tileset[O] = 						 (Tileset){O, "Open", (char *)"./tiles/open.svg", NULL};
	tileset[O_NE] = 				 (Tileset){O_NE, "Open NE Corner", (char *)"./tiles/open_neCorner.svg", NULL};
	tileset[O_NW] = 				 (Tileset){O_NW, "Open NW Corner", (char *)"./tiles/open_nwCorner.svg", NULL};
	tileset[O_NE_NW] = 			 (Tileset){O_NE_NW, "Open NE-NW Corner", (char *)"./tiles/open_ne-nwCorner.svg", NULL};
	tileset[O_SW] = 				 (Tileset){O_SW, "Open SW Corner", (char *)"./tiles/open_swCorner.svg", NULL};
	tileset[O_NE_SW] = 			 (Tileset){O_NE_SW, "Open NE-SW Corner", (char *)"./tiles/open_ne-swCorner.svg", NULL};
	tileset[O_NW_SW] =       (Tileset){O_NW_SW, "Open NW-SW Corner", (char *)"./tiles/open_nw-swCorner.svg", NULL};
	tileset[O_NE_NW_SW] =    (Tileset){O_NE_NW_SW, "Open NE-NW-SW Corner", (char *)"./tiles/open_ne-nw-swCorner.svg", NULL};
	tileset[O_SE] = 				 (Tileset){O_SE, "Open SE Corner", (char *)"./tiles/open_seCorner.svg", NULL};
	tileset[O_NE_SE] = 			 (Tileset){O_NE_SE, "Open NE-SE Corner", (char *)"./tiles/open_ne-seCorner.svg", NULL};
	tileset[O_NW_SE] = 			 (Tileset){O_NW_SE, "Open NW-SE Corner", (char *)"./tiles/open_nw-seCorner.svg", NULL};
	tileset[O_NE_NW_SE] = 	 (Tileset){O_NE_NW_SE, "Open NE-NW-SE Corner", (char *)"./tiles/open_ne-nw-seCorner.svg", NULL};
	tileset[O_SW_SE] = 			 (Tileset){O_SW_SE, "Open SW-SE Corner", (char *)"./tiles/open_sw-seCorner.svg", NULL};
	tileset[O_NE_SW_SE] = 	 (Tileset){O_NE_SW_SE, "Open NE-SW-SE Corner", (char *)"./tiles/open_ne-sw-seCorner.svg", NULL};
	tileset[O_NW_SW_SE] = 	 (Tileset){O_NW_SW_SE, "Open NW-SW-SE Corner", (char *)"./tiles/open_nw-sw-seCorner.svg", NULL};
	tileset[O_NE_NW_SW_SE] = (Tileset){O_NE_NW_SW_SE, "Open NE-NW-SW-SE Corner", (char *)"./tiles/open_ne-nw-sw-seCorner.svg", NULL};

	tileset[BACKGROUND] = (Tileset){BACKGROUND, "Background", (char *)"./tiles/bg.svg", NULL};

	//open file
	std::ofstream file;
	file.open("./tiles/defaultSet");

	//write to file
	file << tileset.size() << '\n';
	for(auto iter = tileset.begin(); iter != tileset.end(); iter++) {
		auto i = iter->first;
		file << tileset[i].id << ',' << tileset[i].name << ',' << tileset[i].filePath << '\0' <<'\n';
	}

	//close file
	file.close();
}
