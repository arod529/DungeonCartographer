#include "settings.h"

int Tileset::count;

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
	//ONES IN ID REPRESENT WALL PLACEMENT AS SWNE

	//tileset definition
	int count = 17;
	Tileset tileset[count];
	tileset[0].count = count;
	tileset[0b10000] = (Tileset){0b10000,"Background",(char *)"./tiles/bg.svg",NULL};

	tileset[0b1111] = (Tileset){0b1111,"NSEW Wall",(char *)"./tiles/nsewWall.svg",NULL};

	tileset[0b1110] = (Tileset){0b1110,"NSW Wall",(char *)"./tiles/nswWall.svg",NULL};
	tileset[0b1101] = (Tileset){0b1101,"SEW Wall",(char *)"./tiles/sewWall.svg",NULL};
	tileset[0b1011] = (Tileset){0b1011,"NSE Wall",(char *)"./tiles/nseWall.svg",NULL};
	tileset[0b0111] = (Tileset){0b0111,"NEW Wall",(char *)"./tiles/newWall.svg",NULL};

	tileset[0b1010] = (Tileset){0b1010,"NS Wall",(char *)"./tiles/nsWall.svg",NULL};
	tileset[0b0101] = (Tileset){0b0101,"EW Wall",(char *)"./tiles/ewWall.svg",NULL};
	tileset[0b0011] = (Tileset){0b0011,"NE Wall",(char *)"./tiles/neWall.svg",NULL};
	tileset[0b0110] = (Tileset){0b0110,"NW Wall",(char *)"./tiles/nwWall.svg",NULL};
	tileset[0b1100] = (Tileset){0b1100,"SW Wall",(char *)"./tiles/swWall.svg",NULL};
	tileset[0b1001] = (Tileset){0b1001,"SE Wall",(char *)"./tiles/seWall.svg",NULL};

	tileset[0b0001] = (Tileset){0b0001,"E Wall",(char *)"./tiles/eWall.svg",NULL};
	tileset[0b0010] = (Tileset){0b0010,"N Wall",(char *)"./tiles/nWall.svg",NULL};
	tileset[0b0100] = (Tileset){0b0100,"W Wall",(char *)"./tiles/wWall.svg",NULL};
	tileset[0b1000] = (Tileset){0b1000,"S Wall",(char *)"./tiles/sWall.svg",NULL};

	tileset[0b0000] = (Tileset){0b0000,"Open",(char *)"./tiles/open.svg",NULL};	


	//open file
	std::ofstream file;
	file.open("./tiles/defaultSet");
	
	//write to file
	file << count << '\n';
	for(int i = 0; i < count; i++)
		file << tileset[i].id << ',' << tileset[i].name << ',' << tileset[i].filePath << '\0' <<'\n';
	
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
		g_object_unref(tileset[i].pixbuf);
	}

	delete[] tileset;
}

/*!
	Load a tileset from a file

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
	for(int i = 0; i < count; i++)
	{
		file >> tileset[i].id;
		std::getline(file, tmp, ','); //eat comma
		std::getline(file, tileset[i].name, ',');
		std::getline(file, tmp);
		tileset[i].filePath = new char[tmp.size()];
		tmp.copy(tileset[i].filePath,tmp.size(),0);
	}

	//close file
	file.close();

	//create pixbufs
	GError *err = NULL;
	for(int i = 0; i < count; i++)
		tileset[i].pixbuf = gdk_pixbuf_new_from_file(tileset[i].filePath, &err);
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
		tile.emplace_back(i, this, &tileset[0b10000]);

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