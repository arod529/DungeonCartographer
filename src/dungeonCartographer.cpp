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

#include "map.h"
#include "settings.h"
#include "ui.h"

#include <gtkmm/application.h>
#include <glibmm/error.h>

#include <iostream>

int main(int argc, char **argv)
{
	//initilize gtk
	auto app = Gtk::Application::create(argc, argv);

  Settings settings;      // create settings
  Map map;                // create map
  UI ui(&settings, &map); // make ui

	return app->run(ui); //have the time of your life
}
