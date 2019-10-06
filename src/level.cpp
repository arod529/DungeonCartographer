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

#include "level.h"

#include <limits> //operator>>
#include <string> //operator>>

/*!
  Initializes a level from a file
**/
Level::Level(Tileset* tileset, std::ifstream& file)
: tileset{tileset}
{
  //initialize properties
  propInit();

  //get level from file
  file >> *this;

  //fill with tiles
  for(int i = 0; i < width*height; i++)
  {
    createNewTile();
    file >> *tile.back();
  }
}

/*!
  Initializes a level and fills it with background tiles.
**/
Level::Level(Tileset* tileset, int id, int width, int height)
: tileset{tileset}
, id{id}
, width{width}
, height{height}
{
  //initialize properties
  propInit();

  //Fill level with background tiles
  for(int i = 0; i < width*height; i++)
  {
    createNewTile();
  }
}

//------------------
//----- PUBLIC -----
//------------------

/*!
  Centers the Level with an affinity for the NW corner if odd spacing.
**/
void Level::center()
{
  int extents[4];
  getExtents(extents);

  //calculate the x shift
  int x = (extents[2]+(width-1-extents[0]))/2 - extents[2];
  //calculate the y shift
  int y = -((extents[1]+(height-1-extents[3]))/2 - extents[1]);

  shift(x, y);
}

/*!
  Inserts columns into the Level.
  
  @param[in] colNum The number of the column at which to begin insertion.
  @param[in] count The number of columns to insert.
**/
void Level::insertColumns(int colNum, int count)
{
  //reset size request of tile 0
  int s;
  tile[0]->get_size_request(s, s);
  tile[0]->set_size_request();
  
  //update width
  width += count;

  //insert count number of columns into grid
  for(int j = 0; j < count; j++)
  {
    insert_column(colNum+j);
  }

  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width; j++)
    {
      int k = j+i*width; //the tile index

      //fill columns with new tiles
      if(j >= colNum && j < colNum + count)
      {
        createNewTile(k);   
      }
      //update preceding and following columns' tiles' gridWidth, gridId, and signals
      else
      {
        tile[k]->gridWidth = width;
        tile[k]->gridId = k;
        tile[k]->clickSig.disconnect();
        tile[k]->clickSig = tile[k]->signal_button_press_event().connect(sigc::bind<int>(sigc::mem_fun(*this, &Level::updateTile), tile[k]->gridId), false);
      } 
    }
  }

  //set size of tile 0
  tile[0]->set_size_request(s, s);
}

/*!
  Inserts rows into the Level.

  @param[in] rowNum The index of the row at which to begin insertion.
  @param[in] count The number of rows to insert.
**/
void Level::insertRows(int rowNum, int count)
{
  //reset size request of tile 0
  int s;
  tile[0]->get_size_request(s, s);
  tile[0]->set_size_request();

  //update height
  height += count;
  //insert count number of rows
  for(int j = 0; j < count; j++)
  {
    //insert row in grid
    insert_row(rowNum+j);
  }

  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width; j++)
    {
      int k = j+i*width; //the tile index

      //add new tiles to new row
      if(i >= rowNum && i < rowNum + count)
      {
        createNewTile(k);
      }
      //update tile's gridHeight, gridId, and signals
      else
      {
        tile[k]->gridHeight = height;

        if(i >= rowNum + count)
        {
          tile[k]->gridId = k;
          tile[k]->clickSig.disconnect();
          tile[k]->clickSig = tile[k]->signal_button_press_event().connect(sigc::bind<int>(sigc::mem_fun(*this, &Level::updateTile), tile[k]->gridId), false);
        }
      }
    }
  }

  //set size of tile 0
  tile[0]->set_size_request(s, s);
}

/*!
  Sets all the tiles back to background clearing the Level.

  @bug This should be replaced by a clear function, reset should reset to default size as well.
**/
void Level::reset()
{
  for(int i = 0; i < width*height; i++)
    tile[i]->tileId = BACKGROUND;

  queue_draw();
}

/*!
  Shifts the Level by n tiles in the specified direction and magnitude.

  @param[in] x The direction and magnitude to shift horizontally.
  @param[in] y The direction and magnitude to shift vertically.
**/
void Level::shift(int x, int y)
{
  int i{0}; //tile grid index
  int j{0}; //column offset

  //shift the level in the x axis 
  if(x != 0)
  {
    do //shift by copying the tiles in column offset by x
    {
      i = (x > 0) ? width-j-1 : j;

      do //scan down column
      {
        tile[i]->tileId = tile[i-x]->tileId; //copy from the column offset by x

        i += width;
      } while(i < width*height);

      j++;
    } while(j < width-abs(x)); //the column to copy is in bounds

    
    do //set the remaining tiles to background
    {
      i = (x > 0) ? width-j-1 : j; //tile grid index 

      do
      {
        tile[i]->tileId = BACKGROUND;
        i += width;
      } while( i < width*height);

      j++;
    } while(j < width); //the column is in bounds
  }

  //shift the level in the y axis
  if(y != 0)
  {
    j = (y > 0) ? 0 : height-1; //row offset

    do
    {
      for(i = width*j; i < width*j+width; i++)
      {
        tile[i]->tileId = tile[i+width*y]->tileId; 
      }

      (y > 0) ? j++ : j--;
    } while(j+y < height && j+y >= 0); //the row to copy is in bounds

    do //set the remaining tiles to background
    {
      for(i = width*j; i < width*j+width; i++)
      {
        tile[i]->tileId = BACKGROUND;
      }

      (y > 0) ? j++ : j--;
    } while(j < height && j >= 0);
  }

  queue_draw();
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
               and adjacent tile.
      2a2) Add adjacent tile to draw queue.
  ^<-
  3) Update corner bits of this tile.
</pre>

  @param[in] btn unused; gtk_signal requirement.
  @param[in] gridId The grid id of the tile to update.

  @return Indicates that the event has been fully handled; gtk_signal requirement.
**/
bool Level::updateTile(GdkEventButton* btn, int gridId)
{
  Tile* tile = this->tile[gridId].get(); //the tile to update
  Tile* aTile = NULL;                    //adjacent tile

  //adjacent tile exist truths
  bool tileExists[8];
  tile->getTileExists(tileExists);

  //adjacent tile index
  int a[8];
  tile->getAdjacentIndex(a);

  uint compare = E;      //compare value for wall

  if(tile->tileId < BACKGROUND)  //this tile is already a room
    {tile->tileId = BACKGROUND;} //set to background
  else
    {tile->tileId = NSEW;}        //new base tile id is closed room

  //check for and update adjacent tile walls
  //cycles through each adjacent tile starting with west tile
  //compare and adjacent tile are opposites
  for(int i = 0; i < 4; i++)
  {
    if(tileExists[i]) //adjacent tile exists
    {
      aTile = this->tile[a[i]].get(); //get adjacent tile

      if (aTile->tileId < BACKGROUND) //adjacent tile is a room
      {
        if(tile->tileId == BACKGROUND) //this tile was a room
        {
          //add previously shared wall of adjacent tile and remove all corner bits
          aTile->tileId = (aTile->tileId | compare)&NSEW;
        }
        else //this tile was not a room
        {
          //remove shared wall of adjacent tile and all corner bits
          aTile->tileId = (aTile->tileId & ~compare)&NSEW;
          tile->tileId ^= (1 << (i+2)%4); //remove shared wall of tile
        }
        updateCornerBits(aTile->gridId, false); //re-add valid corner bits
      }
    }
    compare <<= 1; //shift compare to next wall
  }
  updateCornerBits(tile->gridId, true); //add valid corner bits
  return true; //the event has been fully handled.
}

//-------------------
//----- Utility -----
//-------------------

/*!
  Gets the render size of the tile.

  @return The render size of the tile in pixels.
**/
int Level::getTileSize() const
  {return tile[0]->get_allocated_width();}

/*!
  Sets the render size of the tile.

  @param[in] tileSize The render size of the tile in pixels.
**/
void Level::setTileSize(int tileSize)
  {tile[0]->set_size_request(tileSize, tileSize);}

//-------------------
//----- PRIVATE -----
//-------------------

/*!
  Creates a tile, attaches it to the grid, and connects the button press event.

  The tile is placed in the Level at the given index. If no index is given, it
  is appended.

  @param[in] i The grid index of the tile to create
**/
void Level::createNewTile(int i)
{
  if(i == -1) i = tile.size();
  auto iter = tile.begin() + i;
  
  tile.emplace(iter, std::make_unique<Tile>(tileset, BACKGROUND, i, width, height));
  attach(*tile[i], i%width, i/width, 1, 1);
  tile[i]->clickSig = tile[i]->signal_button_press_event().connect(sigc::bind<int>(sigc::mem_fun(*this, &Level::updateTile), tile[i]->gridId), false);
}

/*!
  Searches for the Level extents by scanline.

  @param[in/out] extents The extents of the Level; size of 4.
**/
void Level::getExtents(int* extents)
{
  //level extents {e, n, w, s}
  int ext[4]{-1, -1, -1, -1};

  int j = 1; //column offset
  int i; 

  //find max east extents by scanning east to west
  do
  {
    i = width-j; //tile gridId
    do
    {
      //the tile is a room, save the row number and end search
      if(tile[i]->tileId != BACKGROUND)
      {
        ext[0] = i%width;
        break;
      } 

      i += width;
    } while(i < width*height);

    j++;
  } while(ext[0] == -1 && j <= width); 

  if(ext[0] == -1) return; //there are no rooms, stop search

  //find max north extents by scanning north to south
  for(i = 0; i < width*height; i++)
  {
    //the tile is a room, save the row number and end search
    if(tile[i]->tileId != BACKGROUND)
    {
      ext[1] = i/width;
      break;
    } 
  }

  //find max west column by scanning west to east
  j = 0;
  do
  {
    i = j;
    do
    {
      //the tile is a room, save the row number and end search
      if(tile[i]->tileId != BACKGROUND)
      {
        ext[2] = i%width;
        break;
      }

      i += width;
    } while(i < width*height);

    j++;
  } while(ext[2] == -1 && j < width);

  //find max south extents by scanning south to north
  for(i = width*height-1; i >= 0; i--)
  {
    //the tile is a room, save the row number and end search
    if(tile[i]->tileId != BACKGROUND)
    {
      ext[3] = i/width;
      break;
    }
  }

  std::copy(ext, ext+4, extents);
}

void Level::print(Cairo::RefPtr<Cairo::PdfSurface>& surface, Cairo::RefPtr<Cairo::Context>& cr)
{
  //set page size to tile size * num of tiles for width and height
  int tileSize = tileset->tile[BACKGROUND].getSize();
  surface->set_size(tileSize*width, tileSize*height);

  //print tiles
  for(int i = 0; i < width*height; i++)
  {
    tile[i]->print(cr);
  }
}

/*!
  Initializes Level Grid properties
**/
void Level::propInit()
{
  //set grid properties
  set_row_homogeneous(true); set_column_homogeneous(true);
  set_row_spacing(0); set_column_spacing(0);
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

  @param[in] gridId The grid id of the tile to update.
  @param[in] propagate Weather the update should propagate to other tiles.
**/
void Level::updateCornerBits(int gridId, bool propagate)
{
  Tile* tile = this->tile[gridId].get();

  if(tile->locked) return; //don't run if this tile already part of recursion chain
  tile->locked = true;     //lock the function for this tile

  Tile* aTile = NULL;               //adjacent tile
  uint walls = tile->tileId & NSEW; //the walls of this tile

  //adjacent tile exists truths
  bool tileExists[8];
  tile->getTileExists(tileExists);

  //adjacent tile indexes
  int a[8];
  tile->getAdjacentIndex(a);

  double pow[2] = {-1, -1}; //the power of the highest bit aka index bit for tile opposite of wall
  int corners = 0;          //the number of corners to check

  if(walls == O || tile->tileId == BACKGROUND) //is background tile or open room
    {corners = 4;}
  else if(modf(log(walls)/log(2), &pow[0]) == 0) //is single wall tile
    {corners = 2;}
  else if (modf(log(walls-exp2(pow[0]))/log(2), &pow[1]) == 0 && (int)(pow[0]+pow[1])%2) //is corner tile
    {corners = 1;}
  else //is double wall (parallel), triple wall, or closed room
  {
    corners = 0;
    tile->tileId &= NSEW; //remove all corner bits
  }

  //check corner tiles
  for(int i = CORNER_BIT_OFFSET; i < CORNER_BIT_OFFSET+corners; i++) //i is index offset for corners
  {
    int j = (int)(pow[0]+i)%CORNER_BIT_OFFSET+CORNER_BIT_OFFSET; //the bit for for the corner
    if(pow[1] == 0 && pow[0] == 3) //bit adjustment for se corner tile
      {j = (int)(pow[1]+i)%CORNER_BIT_OFFSET+CORNER_BIT_OFFSET;}

    if(tileExists[j]) //corner tile exists
    {
      aTile = this->tile[a[j]].get();

      //don't update this tile if it is background AND
      //update adjacent if not a background tile
      if(tile->tileId == BACKGROUND && aTile->tileId != BACKGROUND)
      {
        if(propagate) updateCornerBits(aTile->gridId, true);
      }
      else
      {
        if(aTile->tileId == BACKGROUND) //corner tile is background
          {tile->tileId |= (1<<j);} //add corner bit
        else //corner tile is room
        {
          tile->tileId &= ~(1<<j); //remove corner bit
          if(propagate) updateCornerBits(aTile->gridId, true);
        }
      }
    }
  }

  //check tiles opposing walls
  if(corners <=2 && walls != NSEW) //tile is not background, open room, or closed room
  {
    pow[0] = -1; pow[1] = -1;
    uint openWalls = ~walls & NSEW;
    while(openWalls != 0) //there are open walls left
    {
      modf(log(openWalls)/log(2), &pow[0]); //get highest power of empty wall

      if(tileExists[(int)(pow[0]+2)%4]) //the tile adjacent to the empty wall exists
      {
        if(propagate)
        {
          aTile = this->tile[a[(int)(pow[0]+2)%4]].get(); //get the adjacent tile
          updateCornerBits(aTile->gridId, false);
        } 
      }
      openWalls -= exp2(pow[0]); //remove the tested open wall
    }
  }

  tile->queue_draw();
  tile->locked = false;
}

//---------------------
//----- Overloads -----
//---------------------
std::ostream& operator<<(std::ostream& out, const Level& level)
{
  out << "Level["
       << level.id << ','
       << level.width << ','
       << level.height << ','
       << level.tileset->filepath << ']' << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Level& level)
{
  static const std::streamsize MAX = std::numeric_limits<std::streamsize>::max();

  in.ignore(MAX, '[');
  in >> level.id;
  in.get(); //discard comma
  in >> level.width;
  in.get(); //discard comma
  in >> level.height;
  in.get(); //discard comma


  std::string tilesetFile = "";
  std::getline(in, tilesetFile, ']');
  if(!level.tileset->isTileset(tilesetFile))
    {level.tileset = new Tileset(tilesetFile);}

  in.ignore(MAX, '\n'); //getline doesn't eat this newline

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
