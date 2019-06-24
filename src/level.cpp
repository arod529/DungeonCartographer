#include "level.h"

#include <limits> //operator>>
#include <string> //operator>>

/*!
  Initilizes a level from a file
**/
Level::Level(Tileset* tileset, std::ifstream& file)
: tileset{tileset}
{
  //get level from file
  file >> *this;

  //fill with tiles
  for(int i = 0; i < size*size; i++)
  {
    createNewTile();
    file >> *tile.back();
  }
  //connect signals
  sigInit();
}

/*!
  Initilizes a level and fills it with background tiles.
**/
Level::Level(Tileset* tileset, int id, int size)
: tileset{tileset}, id{id}, size{size}
{
  //set grid properties
  set_row_homogeneous(true); set_column_homogeneous(true);
  set_row_spacing(0); set_column_spacing(0);
  get_style_context()->add_class("mainWindow");

  //Fill level with background tiles
  for(int i = 0; i < size*size; i++)
  {
    createNewTile();
  }
  //connect signals
  sigInit();
}

/*!
  Initilizes the signals managed by Level
**/
void Level::sigInit()
{
  for(int i = 0; i < size*size; i++)
    (*tile[i]).signal_button_press_event().connect(sigc::bind<int>(sigc::mem_fun(*this, &Level::updateTile), tile[i]->gridId), false);

}

//-------------------
//----- Utility -----
//-------------------

void Level::createNewTile()
{
  int i = tile.size();
  tile.emplace_back(std::make_unique<Tile>(tileset, BACKGROUND, i, size));
  attach(*tile[i], i%size, i/size, 1, 1);
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
  Tile* aTile = NULL;         //adjacent tile

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
  return true; //requred for Gtk::Grid::signal_button_press_event()
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
       << level.size << ','
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
  in >> level.size;
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
