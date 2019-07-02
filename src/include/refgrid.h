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

#ifndef REFGRID_H
#define REFGRID_H

#include <gtkmm/box.h>

class RefGrid : public Gtk::Box
{
public:
  RefGrid(int xTileNum, int yTileNum);

  bool isActive() const;
  void setActive(bool a);
  void setRGB(double r, double g, double b);
  void setSize(int xTileNum, int yTileNum);

private:
  bool active{false};
  double lWidth{1};       //grid line width
  double rgb[3]{0, 0, 1}; //grid color
  int width{25};          //number of grid spaces wide
  int height{25};         //number of grid spaces heigh

  //signal overload
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif