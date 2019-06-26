#ifndef UI_H
#define UI_H

#include "level.h"
#include "map.h"
#include "refgrid.h"
#include "settings.h"

#include <string>
#include <vector>

#include <glibmm/refptr.h>

#include <gtkmm/builder.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/overlay.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/window.h>

class UI : public Gtk::Window
{
public:
	UI(Settings* settings, Map* map);
	~UI();

	void open();
	void save();
	void saveAs();
	
	// Utility
	double getScrollSpeed();

private:
	//access to static window elements
	Glib::RefPtr<Gtk::Builder> builder;
	//notebook
	Gtk::Notebook* notebook;
	//icon
	std::vector<Glib::RefPtr<Gdk::Pixbuf>> iconList;
	
	//page switch signal
	sigc::connection pageSwitchCon;

	Map* map;
	Settings* settings;
	std::vector<RefGrid> refgrid;
	// int zoomSpeed{5};

	const char* uiFile  = (char*)"./ui/dungeonCartographer.ui";
	const char* uiTab   = (char*)"./ui/tab.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
	
	void addTab(int levelId, Level* level);
	void clearTabs();
	void pageSwitch(Gtk::Widget* page, uint pageNum);
	void scroll(double dx, double dy);
	bool scrollEvent(GdkEventScroll* scroll_event);
	void zoom(int scrollDir);
};

#endif
