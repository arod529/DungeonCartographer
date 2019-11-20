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

#include "tile.h"

#include <gdkmm/general.h> //on_draw()::set_source_pixbuf()

/*!
  Initilizes a Tile, adds mouse click mask, and makes visible.
**/
Tile::Tile(Tileset* tileset, uint16 tileId, int gridId, int gridWidth, int gridHeight)
: tileset{tileset}
, tileId{tileId}
, gridId{gridId}
, gridWidth{gridWidth}
, gridHeight{gridHeight}
, roomId{-1}
{
  //set drawing area properties
  set_can_focus(true);
  set_size_request(25,25);

  //event masks
  add_events(Gdk::EventMask::BUTTON_PRESS_MASK); //mouse click
  add_events(Gdk::EventMask::ENTER_NOTIFY_MASK); //enter

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
  int c = gridId%gridWidth; // this tile's column number
  int r = gridId/gridWidth; // this tile's row number

  bool tmp[] = {
    (c > 0),            //west tile
    (r < gridHeight-1), //south tile
    (c < gridWidth-1),  //east tile
    (r > 0),            //north tile
    (r > 0 && c > 0),                      //northwest tile
    (c > 0 && r < gridHeight-1),           //southwest tile
    (c < gridWidth-1 && r < gridHeight-1), //southeast tile
    (c < gridWidth-1 && r > 0)             //northeast tile
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
    gridId-1,           //west tile
    gridId+gridWidth,   //south tile
    gridId+1,           //east tile
    gridId-gridWidth,   //north tile
    gridId-gridWidth-1, //northwest tile
    gridId+gridWidth-1, //southwest tile
    gridId+gridWidth+1, //southeast tile
    gridId-gridWidth+1  //northeast tile
  };

  std::copy(tmp, tmp+8, adjacentIndex);
}

void Tile::print(Cairo::RefPtr<Cairo::Context>& cr)
{
  //get tile size
  int tilesize = tileset->tile[tileId].pixbuf->get_width();
  //get origin coordinats of this tile
  int x = (gridId%gridWidth)*tilesize;
  int y = (gridId/gridWidth)*tilesize;

  //paint tile at orign
  Gdk::Cairo::set_source_pixbuf(cr, tileset->tile[tileId].pixbuf, x, y);
  cr->rectangle(x, y, tilesize, tilesize);
  cr->fill();
}

/*!
  Resets the tile to a default background tile.
**/
void Tile::reset()
{
  tileId = BACKGROUND;
  roomId = -1;
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

bool Tile::on_enter_notify_event(GdkEventCrossing* crossing_event)
{
  printf("gridId: %d\t tileId: %X\t roomId: %d\n", gridId, tileId, roomId);

  return false;
}

//---------------------
//----- Overloads -----
//---------------------

/*!
  Copies the tileId and roomId.

  @param[in] sourceTile The tile to copy from
**/
void Tile::operator=(const Tile& sourceTile)
{
  tileId = sourceTile.tileId;
  roomId = sourceTile.roomId;
}

std::ostream& operator<<(std::ostream& out, const Tile& tile)
{
  if(tile.gridId%tile.gridWidth != 0)
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
