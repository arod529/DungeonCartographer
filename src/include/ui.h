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

	const char* uiFile  = (char*)"./ui/dungeonCartographer.ui";
	const char* uiTab   = (char*)"./ui/tab.ui";
	const char* uiShiftLevel = (char*)"./ui/shiftLevel.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
	
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
