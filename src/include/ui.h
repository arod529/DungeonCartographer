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

#ifndef UI_H
#define UI_H

#include "level.h"
#include "map.h"
#include "refgrid.h"
#include "settings.h"

#include <string>
#include <vector>

#include <glibmm/refptr.h>

#include <gdkmm/pixbuf.h>

#include <gtkmm/builder.h>
#include <gtkmm/colorselection.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/overlay.h>
#include <gtkmm/toggletoolbutton.h>
#include <gtkmm/window.h>

class UI : public Gtk::Window
{
public:
	UI(Settings* settings, Map* map);
	~UI();

	void centerLevel();
	void deleteColumns();
	void deleteRows();
	void insertColumns();
	void insertRows();
	void open();
	void resetLevel();
	void save();
	void saveAs();
	void shiftLevel();
	
	// Utility
	double getScrollSpeed();

private:
	Glib::RefPtr<Gtk::Builder> builder;              // access to static window elements
	Gtk::Notebook* notebook;                         // notebook
	Gtk::ToggleToolButton* gridToggle;               // grid toggle
	std::vector<Glib::RefPtr<Gdk::Pixbuf>> iconList; // window icon
	Gtk::ColorSelection* gridColor;                  // grid color selector
	
	sigc::connection pageSwitchCon; //page switch signal

	Map* map;
	Settings* settings;
	int currPage{0};
	std::vector<RefGrid> refgrid;

	const char* uiCSS          = (char*)"/DungeonCartographer/dungeonCartographer.css";
	const char* uiMain         = (char*)"/DungeonCartographer/dungeonCartographer.ui";
	const char* uiTab          = (char*)"/DungeonCartographer/tab.ui";
	const char* uiIconMain     = (char*)"/DungeonCartographer/icon_dc.png";
	const char* uiInsertColumn = (char*)"/DungeonCartographer/insertColumn.ui";
	const char* uiInsertRow    = (char*)"/DungeonCartographer/insertRow.ui";
	const char* uiShiftLevel   = (char*)"/DungeonCartographer/shiftLevel.ui";
	
	void addTab(int levelId, Level* level);
	void clearTabs();
	void pageSwitch(Gtk::Widget* page, uint pageNum);
	void scroll(double dx, double dy);
	bool scrollEvent(GdkEventScroll* scroll_event);
	void setGridColor();
	void toggleGrid();
	void zoom(int scrollDir);
};

#endif
