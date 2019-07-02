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

#include "refgrid.h"

RefGrid::RefGrid(int xTileNum, int yTileNum)
{
  set_hexpand(true);
  set_vexpand(true);
  set_sensitive(false);
  set_can_focus(false);

  setSize(xTileNum, yTileNum);
}

bool RefGrid::isActive() const
  {return active;}

void RefGrid::setActive(bool a)
{
  active = a;
  
  if(active)
    show();
  else
    hide();
}

void RefGrid::setRGB(double r, double g, double b)
{
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;

  queue_draw();
}

void RefGrid::setSize(int xTileNum, int yTileNum)
{
  width = xTileNum;
  height = yTileNum;
}

bool RefGrid::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  //prevent show when inactive when parent container calls show all
  if(!active)
  {
    hide();
    return true;
  }

  int w = get_allocated_width();
  int h = get_allocated_height();

  //set color
  cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);
  //set line width
  cr->set_line_width(lWidth);

  double x{0.0}, y{0.0}; //origin
  double X{0.0}, Y{0.0}; //end
  double ds = w/width;   //size of a tile

  //draw vertical lines
  for(int i = 1; i < width; i++)
  {
    x = i*ds;
    X = x;
    y = 0;
    Y = h;
    
    cr->move_to(x, y);
    cr->line_to(X, Y);
    cr->stroke();
  }

  for(int i = 1; i < height; i++)
  {
    y = i*ds;
    Y = y;
    x = 0;
    X = w;

    //horizontal
    cr->move_to(x, y);
    cr->line_to(X, Y);
    cr->stroke();
  }

  return true;
}