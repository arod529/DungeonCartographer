#include "settings.h"
#include "constants.h"

#include <cmath>

//--------------------------
//----- Tileset Struct -----
//--------------------------

/*!
  Default empty initializers for tileset struct.
**/
Tileset::Tileset()
{
	id = 0;
	name = "";
	filePath = NULL;
	pixbuf = NULL;
}

/*!
  Assignment initializer for tileset struct.

  @param[in] _id The id of the tile struct; a binary number that represents the
  								walls and corner bits that the tile represents. \n
  								|se|sw|nw|ne|s|w|n|e| \n
  								bit[0-3] = wall bits \n
									bit[4-7] = corner bits
	@param[in] _name The display name of the tile.
	@param[in] _filePath Path to the svg file of the tile.
	@param[in] _pixbuf The pixbuf for the tile.
**/
Tileset::Tileset(uint _id, std::string _name, char* _filePath, GdkPixbuf* _pixbuf)
{
	id = _id;
	name = _name;
	filePath = _filePath;
	pixbuf = _pixbuf;
}

//----------------------
//----- Tile Class -----
//----------------------
/*!
  Initializer for the Tile class.

  @param[in] _gridId The id of the grid square this tile represents. This is a
  										single dimension array that represents a square grid.
	@param[in] _tileLvl The Level that the tile resides in.
	@param[in] _tileTileset The Tileset struct that describes the tile image.
**/
Tile::Tile(int _gridId, Level* _tileLvl, Tileset* _tileTileset)
{
	gridId = _gridId;
	tileLvl = _tileLvl;
	tileTileset = _tileTileset;
}

/*!
  Get an array of adjacent tile exists truths. Each value in the array describes
  whether the tile adjacent to the calling tile exists. The directions are: \n
  {ne, se, sw, nw, n, e, s, w}

  @param [in,out] _tileExists Truths of tile exists (size 8)
**/
void Tile::getTileExists(bool* _tileExists)
{
	int gridSize = tileLvl->getSize();
	int c = gridId%gridSize;  //this tile's column number
	int r = gridId/gridSize;  //this tile's row number

	bool tmp[] = {
		(c > 0),					//west tile
		(r < gridSize-1), //south tile
		(c < gridSize-1), //east tile
		(r > 0),					//north tile
		(r > 0 && c > 0),	 					 				//northwest tile
		(c > 0 && r < gridSize-1),  				//southwest tile
		(c < gridSize-1 && r < gridSize-1), //southeast tile
		(c < gridSize-1 && r > 0)				 		//northeast tile
	};

	std::copy(tmp, tmp+8, _tileExists);
}

/*!
  Get an array of adjacent tile indexes. Each value in the array is the grid index
  of the adjacent tile. The directions are: \n
  {ne, se, sw, nw, n, e, s, w}

  @param [in,out] _adjacentIndex The indexes of the adjacent tiles (size 8)
**/
void Tile::getAdjacentIndex(int* _adjacentIndex)
{
	int gridSize = tileLvl->getSize();

	int tmp[] = {
		gridId-1,					 //west tile
		gridId+gridSize,	 //south tile
		gridId+1,					 //east tile
		gridId-gridSize, 	 //north tile
		gridId-gridSize-1, //northwest tile
		gridId+gridSize-1, //southwest tile
		gridId+gridSize+1, //southeast tile
		gridId-gridSize+1  //northeast tile
	};

	std::copy(tmp, tmp+8, _adjacentIndex);
}

/*!
  Updates the tile by either making it a room or a background tile based
  on its previous state. Updates the shared walls of the adjacent tiles
  and calls a non-propagating updateCornerBits() on them. Calls a
  propagating updateCornerBits() on self.

<pre>
  1) Sets the new tile id to an enclosed room.
  	1a) Changes the new tile id to background if it was already a room.
	2) For each adjacent tile in cardinal directions.
		2a) If the adjacent tile exists and is a room.
			2a1.1) If this tile was a room (is now background), add shared wall to adjacent tile.
			2a1.2) If the tile was background (is now a room), remove shared wall of this tile
					 		and adjacent tile
			2a2) Add adjacent tile to draw queue.
	^<-
	3) Update corner bits of this tile.
</pre>
**/
void Tile::updateTile()
{
	Tile* aTile = NULL; //adjacent tile

	//adjacent tile exist truths
	bool tileExists[8];
	getTileExists(tileExists);

	//adjacent tile index
	int a[8];
	getAdjacentIndex(a);

	uint compare = E; 				//compare value for wall
	uint newTilesetId = NSEW;	//new base tile id for this tile is closed room

	if(tileTileset->id < BACKGROUND) //this tile is already a room
		{newTilesetId = BACKGROUND;}	 //set to background

	//check for and update adjacent tile walls
	//cycles through each adjacent tile starting with west tile
	//compare and adjacent tile are opposites
	for(int i = 0; i < 4; i++)
	{
		if(tileExists[i])	//adjacent tile exists
		{
			aTile = &tileLvl->tile[a[i]];	//get adjacent tile

			if (aTile->tileTileset->id < BACKGROUND) //adjacent tile is a room
			{
				if(tileTileset->id < BACKGROUND) //this tile was a room
				{
					//add previously shared wall of adjacent tile and remove all corner bits
					aTile->tileTileset = &tileLvl->tileset[(aTile->tileTileset->id | compare)&NSEW];
				}
				else //this tile was not a room
				{
					//remove shared wall of adjacent tile and all corner bits
					aTile->tileTileset = &tileLvl->tileset[(aTile->tileTileset->id & ~compare)&NSEW];
					newTilesetId ^= (1 << (i+2)%4); //remove shared wall of this tile
				}
				aTile->updateCornerBits(false); //re-add valid corner bits
			}
		}
		compare <<= 1; //shift compare to next wall
	}

	tileTileset = &tileLvl->tileset[newTilesetId]; //update this tile's tileset id
	updateCornerBits(true); //draw of this tile will occur here
}

/*!
	Updates the corner bits of the calling tile, recursively updates the corner bits
	of the diagonal tiles, and non-recursively updates the tiles cardinally adjacent.
	Locks the tile when called to prevent recursion double back. Calls queDraw() at end.

<pre>
  1) Checks if the calling object has already been recursed by the locked property,
  		returns if locked, locks on continue.
  2.1) If there are no walls or tile is background, 4 corners must be checked.
  2.2) If remainder is 0, the tile is a single wall tile, 2 corners must be checked.
  2.3) If the second remainder is 0 AND the sum of the powers is even, the tile
  			is a corner tile, 1 corner must be checked.
	2.4) No corners must be checked.
	3) For each corner to be checked.
		3a) Calculate starting corner bit from largest power with offset, use lowest power for se corner tile.
		3b) If the corner tile exists.
			3b1) If this tile is background and the adjacent tile is not background.
			 	3b1a) Update the corner bits of the adjacent tile (recurse).
	^<-
			3b2) If this tile is not a background tile.
			 	3b2a.1) If the adjacent tile is background, add corner bit.
 	^<-
			 	3b2a.2) If the adjacent tile is a room, add corner bit, update corner bits of adjacent tile (recurse).
 	^<-
	4)If tile is not background, open room, or closed room.
		4a) For each tile adjacent to open walls, if exists, update corner bits (non-recursive).
		^<-
	5) Queue draw for this tile.
	6) Unlock tile.
</pre>
**/
void Tile::updateCornerBits(bool _propagate)
{
	if(locked) return; //don't run if this tile already part of recursion chain
	locked = true; //lock the function for this tile

	Tile* aTile = NULL; //adjacent tile
	uint newTilesetId = tileTileset->id; //new tile set id to use, current as bases
	uint walls = newTilesetId & NSEW; //the walls of this tile

	//adjacent tile exists truths
	bool tileExists[8];
	getTileExists(tileExists);

	//adjacent tile indexes
	int a[8];
	getAdjacentIndex(a);

	double pow[2] = {-1, -1}; //the power of the highest bit aka index bit for tile opposite of wall
	int corners = 0; //the number of corners to check

	if(walls == O || newTilesetId == BACKGROUND) //is background tile or open room
		{corners = 4;}
	else if(modf(log(walls)/log(2), &pow[0]) == 0) //is single wall tile
		{corners = 2;}
	else if (modf(log(walls-exp2(pow[0]))/log(2), &pow[1]) == 0 && (int)(pow[0]+pow[1])%2) //is corner tile
		{corners = 1;}
	else //is double wall (parallel), triple wall, or closed room
		{
			corners = 0;
			newTilesetId &= NSEW; //remove all corner bits
			tileTileset = &tileLvl->tileset[newTilesetId];
		}

	//check corner tiles
	for(int i = CORNER_BIT_OFFSET; i < CORNER_BIT_OFFSET+corners; i++) //i is index offset for corners
	{
		int j = (int)(pow[0]+i)%CORNER_BIT_OFFSET+CORNER_BIT_OFFSET; //the bit for for the corner
		if(pow[1] == 0 && pow[0] == 3) //bit adjustment for se corner tile
			{j = (int)(pow[1]+i)%CORNER_BIT_OFFSET+CORNER_BIT_OFFSET;}

		if(tileExists[j]) //corner tile exists
		{
			aTile = &tileLvl->tile[a[j]];

			//don't update this tile if it is background AND
			//update adjacent if not a background tile
			if(newTilesetId == BACKGROUND && aTile->tileTileset->id != BACKGROUND)
			{
				if(_propagate) aTile->updateCornerBits(true);
			}
			else
			{
				if(aTile->tileTileset->id == BACKGROUND) //corner tile is background
				{
					newTilesetId |= (1<<j); //add corner bit
					tileTileset = &tileLvl->tileset[newTilesetId];
				}
				else //corner tile is room
				{
					newTilesetId &= ~(1<<j); //remove corner bit
					tileTileset = &tileLvl->tileset[newTilesetId];
					if(_propagate) aTile->updateCornerBits(true);
				}
			}
		}
	}

	//check tiles opposing walls
	if(corners <=2 && walls != NSEW) //tile is not background, open room, or closed room
	{
		pow[0] = -1; pow[1] = -1;
		uint openWalls = ~walls & NSEW;
		while(openWalls != 0) //make sure there are open walls left
		{
			modf(log(openWalls)/log(2), &pow[0]); //get highest power of of empty wall

			if(tileExists[(int)(pow[0]+2)%4]) //the tile adjacent to the empty wall exists
			{
				aTile = &tileLvl->tile[a[(int)(pow[0]+2)%4]]; //get the adjacent tile
				if(_propagate) aTile->updateCornerBits(false);
			}
			openWalls -= exp2(pow[0]);
		}
	}

	queDraw();
	locked = false;
}

/*!
  Queue a redraw for the tile.
**/
void Tile::queDraw()
{
	g_object_set_data(G_OBJECT(tileLvl->drawingArea[gridId]),"tile",this); //update this tile's drawing area
	gtk_widget_queue_draw(tileLvl->drawingArea[gridId]);									 //queue redraw of this tile
}

//---------------------
//----- Map Class -----
//---------------------
/*!
	Initializes a map with defaults from Settings. Creates a starting Level.

	@param[in] _settings The program settings
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
	Free memory of map used by the Tileset file paths, and pixbufs.

	\bug Tileset memory management should be done by the tileset
	\bug will need to free memory of optional Tileset
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

	@param[in] tileSetFile File path for the tile set to load.

	\bug doesn't check if file exists
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
/*!
  Initializes a new blank Level.

  @param[in] _size The size of the level.
  @param[in] _tileset The Tileset to use for the level
**/
Level::Level(int _size, std::unordered_map<uint, Tileset> _tileset)
{
	//set map size and tileset
	size = _size;
	tileset = _tileset;

	//set all tiles to background tile
	for(int i = 0; i < size*size; i++)
		tile.emplace_back(i, this, &tileset[BACKGROUND]);

	//initialize drawing areas for map
	for(int i = 0; i < size*size; i++)
		drawingArea.emplace_back(gtk_drawing_area_new());
	for(int i = 0; i < size*size; i++)
		gtk_widget_set_can_focus (drawingArea[i],true);
}

/*!
  Returns the size of the level.

  @return The size of the Level.
**/
int Level::getSize() const
{
	return size;
}

//--------------------------
//----- Settings Class -----
//--------------------------
/*!
	Initialize settings from default settings file.
**/
Settings::Settings()
{
	std::fstream file;

	//try default file
	file.open(settingsFile, std::ios_base::in);
	if(file.is_open()) //file exists read settings
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
		file << tileset[i].id << ',' << tileset[i].name << ',' << tileset[i].filePath << '\0' << '\n';
	}

	//close file
	file.close();
}
