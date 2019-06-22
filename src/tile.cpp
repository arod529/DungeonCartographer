#include "tile.h"

#include "constants.h"

#include <cmath>
#include <fstream>

#include <glibmm/refptr.h>
//for set_source_pixbuf()
#include <gdkmm/general.h>

/*!
  Initializer for the Tile class.

  @param[in] gridId The id of the grid square this tile represents. This is a
                      single dimension array that represents a square grid.
  @param[in] tileLvl The Level that the tile resides in.
  @param[in] tilesetTile The TilesetTile struct that describes the tile image.
**/
Tile::Tile(int gridId, Level* tileLvl, TilesetTile* tilesetTile)
:gridId{gridId}, tileLvl{tileLvl}, tilesetTile{tilesetTile}
{
  set_can_focus(true);
  set_size_request(25,25);

  add_events(Gdk::EventMask::BUTTON_PRESS_MASK);

  show();
}

/*!
  Get an array of adjacent tile exists truths. Each value in the array describes
  whether the tile adjacent to the calling tile exists. The directions are: \n
  {ne, se, sw, nw, n, e, s, w}

  @param [in,out] tileExists Truths of tile exists (size 8)
**/
void Tile::getTileExists(bool* tileExists)
{
  int gridSize = tileLvl->getSize();
  int c = gridId%gridSize;  //this tile's column number
  int r = gridId/gridSize;  //this tile's row number

  bool tmp[] = {
    (c > 0),          //west tile
    (r < gridSize-1), //south tile
    (c < gridSize-1), //east tile
    (r > 0),          //north tile
    (r > 0 && c > 0),                      //northwest tile
    (c > 0 && r < gridSize-1),          //southwest tile
    (c < gridSize-1 && r < gridSize-1), //southeast tile
    (c < gridSize-1 && r > 0)             //northeast tile
  };

  std::copy(tmp, tmp+8, tileExists);
}

/*!
  Get an array of adjacent tile indexes. Each value in the array is the grid index
  of the adjacent tile. The directions are: \n
  {ne, se, sw, nw, n, e, s, w}

  @param [in,out] adjacentIndex The indexes of the adjacent tiles (size 8)
**/
void Tile::getAdjacentIndex(int* adjacentIndex)
{
  int gridSize = tileLvl->getSize();

  int tmp[] = {
    gridId-1,           //west tile
    gridId+gridSize,   //south tile
    gridId+1,           //east tile
    gridId-gridSize,    //north tile
    gridId-gridSize-1, //northwest tile
    gridId+gridSize-1, //southwest tile
    gridId+gridSize+1, //southeast tile
    gridId-gridSize+1  //northeast tile
  };

  std::copy(tmp, tmp+8, adjacentIndex);
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

  uint compare = E;         //compare value for wall
  uint newTilesetId = NSEW;  //new base tile id for this tile is closed room

  if(tilesetTile->getId() < BACKGROUND) //this tile is already a room
    {newTilesetId = BACKGROUND;}   //set to background

  //check for and update adjacent tile walls
  //cycles through each adjacent tile starting with west tile
  //compare and adjacent tile are opposites
  for(int i = 0; i < 4; i++)
  {
    if(tileExists[i])  //adjacent tile exists
    {
      aTile = &tileLvl->tile[a[i]];  //get adjacent tile

      if (aTile->tilesetTile->getId() < BACKGROUND) //adjacent tile is a room
      {
        if(tilesetTile->getId() < BACKGROUND) //this tile was a room
        {
          //add previously shared wall of adjacent tile and remove all corner bits
          aTile->tilesetTile = &tileLvl->tileset->tile[(aTile->tilesetTile->getId() | compare)&NSEW];
        }
        else //this tile was not a room
        {
          //remove shared wall of adjacent tile and all corner bits
          aTile->tilesetTile = &tileLvl->tileset->tile[(aTile->tilesetTile->getId() & ~compare)&NSEW];
          newTilesetId ^= (1 << (i+2)%4); //remove shared wall of this tile
        }
        aTile->updateCornerBits(false); //re-add valid corner bits
      }
    }
    compare <<= 1; //shift compare to next wall
  }

  tilesetTile = &tileLvl->tileset->tile[newTilesetId]; //update this tile's tilesetTile
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
void Tile::updateCornerBits(bool propagate)
{
  if(locked) return; //don't run if this tile already part of recursion chain
  locked = true; //lock the function for this tile

  Tile* aTile = NULL; //adjacent tile
  uint newTilesetId = tilesetTile->getId(); //new tile set id to use, current as bases
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
      tilesetTile = &tileLvl->tileset->tile[newTilesetId];
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
      if(newTilesetId == BACKGROUND && aTile->tilesetTile->getId() != BACKGROUND)
      {
        if(propagate) aTile->updateCornerBits(true);
      }
      else
      {
        if(aTile->tilesetTile->getId() == BACKGROUND) //corner tile is background
        {
          newTilesetId |= (1<<j); //add corner bit
          tilesetTile = &tileLvl->tileset->tile[newTilesetId];
        }
        else //corner tile is room
        {
          newTilesetId &= ~(1<<j); //remove corner bit
          tilesetTile = &tileLvl->tileset->tile[newTilesetId];
          if(propagate) aTile->updateCornerBits(true);
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
        if(propagate) aTile->updateCornerBits(false);
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
  // g_object_set_data(G_OBJECT(tileLvl->drawingArea[gridId]),"tile",this); //update this tile's drawing area
  queue_draw(); //queue redraw of this tile
}

bool Tile::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  int s = get_allocated_width();
  auto tmpPixbuf = tilesetTile->pixbuf->scale_simple(s, s, Gdk::INTERP_BILINEAR);
  Gdk::Cairo::set_source_pixbuf(cr, tmpPixbuf, 0, 0);
  cr->paint();

  return false;
}

bool Tile::on_button_press_event(GdkEventButton* btn)
{
  updateTile();
  return false;
}

/*!
  Writes a Tile to a file stream.
**/
std::ostream& operator<<(std::ostream& out, const Tile& tile)
{
  if(tile.gridId%tile.tileLvl->getSize() != 0)
  {
    out.seekp(-1, std::ios::cur);
    out << '|';
  }

  out << tile.tilesetTile->getId() << '\n';
  out.flush();
  return out;
}

/*!
  Reads a Tile from a file stream.

  The Tile must have a valid tileLvl, and therefore cannot be initialized with this.
**/
std::istream& operator>>(std::istream& in, Tile& tile)
{
  uint id = 0;

  in >> id;
  if(in.peek() == '|') in.get(); //scrap separator

  tile.tilesetTile = &tile.tileLvl->tileset->tile[id];

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
