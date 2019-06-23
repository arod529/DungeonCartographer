#include "tile.h"

#include "level.h"

#include <gdkmm/general.h> //on_draw()::set_source_pixbuf()

/*!
  Initilizes a Tile, adds mouse click mask, and makes visible.
**/
Tile::Tile(Tileset* tileset, uint tileId, int gridId, int gridSize)
: tileset{tileset}, tileId{tileId}, gridId{gridId}, gridSize{gridSize}
{
  //set drawing area properties
  set_can_focus(true);
  set_size_request(25,25);

  //mouse click mask
  add_events(Gdk::EventMask::BUTTON_PRESS_MASK);

  //make visible
  show();
};

//-------------------
//----- Utility -----
//-------------------

/*!
  Get an array of adjacent tile exists truths. Each value in the array describes
  whether the tile adjacent to the calling tile exists. The directions are: \n
  {ne, se, sw, nw, n, e, s, w}

  @param [in,out] tileExists Truths of tile exists (size 8)
**/
void Tile::getTileExists(bool* tileExists)
{
  int c = gridId%gridSize; // this tile's column number
  int r = gridId/gridSize; // this tile's row number

  bool tmp[] = {
    (c > 0),          //west tile
    (r < gridSize-1), //south tile
    (c < gridSize-1), //east tile
    (r > 0),          //north tile
    (r > 0 && c > 0),                   // northwest tile
    (c > 0 && r < gridSize-1),          // southwest tile
    (c < gridSize-1 && r < gridSize-1), // southeast tile
    (c < gridSize-1 && r > 0)           // northeast tile
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
  int tmp[] = {
    gridId-1,          // west tile
    gridId+gridSize,   // south tile
    gridId+1,          // east tile
    gridId-gridSize,   // north tile
    gridId-gridSize-1, // northwest tile
    gridId+gridSize-1, // southwest tile
    gridId+gridSize+1, // southeast tile
    gridId-gridSize+1  // northeast tile
  };

  std::copy(tmp, tmp+8, adjacentIndex);
}

//--------------------------
//----- Event Handlers -----
//--------------------------

/*!
  The draw event of the tile.

  @param[in] cr The Cairo::Context associated with the draw event.
**/
bool Tile::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  int s = get_allocated_width();
  auto tmpPixbuf = tileset->tile[tileId].pixbuf->scale_simple(s, s, Gdk::INTERP_BILINEAR);
  Gdk::Cairo::set_source_pixbuf(cr, tmpPixbuf, 0, 0);
  cr->paint();

  return true;
}

//---------------------
//----- Overloads -----
//---------------------
std::ostream& operator<<(std::ostream& out, const Tile& tile)
{
  if(tile.gridId%tile.gridSize != 0)
  {
    out.seekp(-1, std::ios::cur);
    out << '|';
  }

  out << tile.tileId << '\n';
  out.flush();
  return out;
}

std::istream& operator>>(std::istream& in, Tile& tile)
{
  in >> tile.tileId;
  if(in.peek() == '|') in.get(); //scrap separator

  //leave stream in good state by discarding empty lines
  while(in.peek() == '\n')
    {in.get();}

  return in;
}
