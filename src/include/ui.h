#ifndef UI_H
#define UI_H

#include "settings.h"

#include <string>
#include <vector>

#include <glibmm/refptr.h>

#include <gtkmm/builder.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/window.h>

class UI : public Gtk::Window
{
public:
	friend class Map;

	UI(Settings& settings);

	std::string saveAs();
	std::string openFile();

	void addTab(uint pageNum, Gtk::Widget* widget);
	void clearTabs();
	double getScrollSpeed();
	void scroll(double dx, double dy);
	void zoom(int scrollDir, int gridSize, Gtk::Widget* widget);


	template <class T> void getWidget(std::string name, T*& widget);

private:
	//access to static window elements
	Glib::RefPtr<Gtk::Builder> builder;
	//notebook
	Gtk::Notebook* notebook;
	//icon
	std::vector<Glib::RefPtr<Gdk::Pixbuf>> iconList;
	
	// Settings settings;
	// int zoomSpeed{5};

	const char* uiFile  = (char*)"./ui/dungeonCartographer.ui";
	const char* uiTab   = (char*)"./ui/tab.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#include "ui.tpp"


#endif
