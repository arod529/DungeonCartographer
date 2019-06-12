#include "settings.h"
#include "constants.h"

//--------------------------
//----- Tileset Struct -----
//--------------------------
int Tileset::count = 0;

/*!
  Default empty initilizers for tileset struct
**/
Tileset::Tileset()
{
	id = 0;
	name = "";
	filePath = NULL;
	pixbuf = NULL;
}

/*!
  Assignment initilizer for tileset struct
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
	/* 
	id:
		bit[0-3] = wall bits
		bit[4-6] = corner bits
	*/

	//tileset definition
	int count = BACKGROUND+1;
	Tileset tileset[count];
	tileset[0].count = count;
	tileset[E_WALL] = (Tileset){E_WALL, "E Wall", (char *)"./tiles/eWall.svg", NULL};
	tileset[E_HALL] = (Tileset){E_HALL, "E Hall", (char *)"./tiles/eHall.svg", NULL};
	tileset[N_WALL] = (Tileset){N_WALL, "N Wall", (char *)"./tiles/nWall.svg", NULL};
	tileset[N_HALL] = (Tileset){N_HALL, "N Hall", (char *)"./tiles/nHall.svg", NULL};
	tileset[W_WALL] = (Tileset){W_WALL, "W Wall", (char *)"./tiles/wWall.svg", NULL};
	tileset[W_HALL] = (Tileset){W_HALL, "W Hall", (char *)"./tiles/wHall.svg", NULL};
	tileset[S_WALL] = (Tileset){S_WALL, "S Wall", (char *)"./tiles/sWall.svg", NULL};
	tileset[S_HALL] = (Tileset){S_HALL, "S Hall", (char *)"./tiles/sHall.svg", NULL};

	tileset[NE_WALL] = (Tileset){NE_WALL, "NE Wall", (char *)"./tiles/neWall.svg", NULL};
	tileset[NE_HALL] = (Tileset){NE_HALL, "NE Hall", (char *)"./tiles/neHall.svg", NULL};
	tileset[SE_WALL] = (Tileset){SE_WALL, "SE Wall", (char *)"./tiles/seWall.svg", NULL};
	tileset[SE_HALL] = (Tileset){SE_HALL, "SE Hall", (char *)"./tiles/seHall.svg", NULL};
	tileset[NW_WALL] = (Tileset){NW_WALL, "NW Wall", (char *)"./tiles/nwWall.svg", NULL};
	tileset[NW_HALL] = (Tileset){NW_HALL, "NW Hall", (char *)"./tiles/nwHall.svg", NULL};
	tileset[SW_WALL] = (Tileset){SW_WALL, "SW Wall", (char *)"./tiles/swWall.svg", NULL};
	tileset[SW_HALL] = (Tileset){SW_HALL, "SW Hall", (char *)"./tiles/swHall.svg", NULL};
	
	tileset[WE_WALL] = (Tileset){WE_WALL, "EW Wall", (char *)"./tiles/ewWall.svg", NULL};
	tileset[NS_HALL] = (Tileset){NS_HALL, "NS Wall", (char *)"./tiles/nsWall.svg", NULL};

	tileset[NEW_WALL] = (Tileset){NEW_WALL, "NEW Wall", (char *)"./tiles/newWall.svg", NULL};
	tileset[NSE_WALL] = (Tileset){NSE_WALL, "NSE Wall", (char *)"./tiles/nseWall.svg", NULL};
	tileset[SEW_WALL] = (Tileset){SEW_WALL, "SEW Wall", (char *)"./tiles/sewWall.svg", NULL};
	tileset[NSW_WALL] = (Tileset){NSW_WALL, "NSW Wall", (char *)"./tiles/nswWall.svg", NULL};

	tileset[NSEW_WALL] = (Tileset){NSEW_WALL, "NSEW Wall", (char *)"./tiles/nsewWall.svg", NULL};

	tileset[OPEN]   	 = (Tileset){OPEN, 	 		"Open",		(char *)"./tiles/open.svg", NULL};
	tileset[E_OPEN] 	 = (Tileset){E_OPEN, 		"E Open", (char *)"./tiles/nseHall.svg", NULL};
	tileset[N_OPEN] 	 = (Tileset){N_OPEN, 		"N Open", (char *)"./tiles/newHall.svg", NULL};
	tileset[W_OPEN] 	 = (Tileset){W_OPEN, 		"W Open", (char *)"./tiles/nswHall.svg", NULL};
	tileset[S_OPEN] 	 = (Tileset){S_OPEN, 		"S Open", (char *)"./tiles/sewHall.svg", NULL};
	tileset[NSEW_OPEN] = (Tileset){NSEW_OPEN, "NSEW Open", (char *)"./tiles/nsewHall.svg", NULL};

	tileset[BACKGROUND] = (Tileset){BACKGROUND, "Background", (char *)"./tiles/bg.svg", NULL};

	//open file
	std::ofstream file;
	file.open("./tiles/defaultSet");
	
	//write to file
	file << count << '\n';
	for(int i = 0; i < count; i++) {
		if(tileset[i].filePath != NULL) {
			file << tileset[i].id << ',' << tileset[i].name << ',' << tileset[i].filePath << '\0' <<'\n';
		}
	}
	
	//close file
	file.close();
}

//---------------------
//----- Map Class -----
//---------------------
/*!
	Initilizes a map with defaults from settings
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
	Free memmory of map
**/
Map::~Map()
{
	for(int i = 0; i < tileset[0].count; i++)
	{
		delete[] tileset[i].filePath;
		if(tileset[i].pixbuf != NULL) {
			g_object_unref(tileset[i].pixbuf);
		}
	}

	delete[] tileset;
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

	//allocate tileset
	tileset = new Tileset[count];
	tileset[0].count = count;

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
	for(int i = 0; i < count; i++) {
		if(tileset[i].filePath != NULL) {
			tileset[i].pixbuf = gdk_pixbuf_new_from_file(tileset[i].filePath, &err);
		}
	}
}

//-----------------------
//----- Level Class -----
//-----------------------
Level::Level(int _size, Tileset* _tileset)
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