#ifndef UI_H
#define UI_H

#include "settings.h"

#include <string>
#include <vector>

#include <glibmm/refptr.h>

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/dialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/layout.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/window.h>

class UI : public Gtk::Window
{
public:
	friend class Map;

	UI(Settings& settings);

	std::string saveAs();
	// void addLevel(Gtk::Grid* level);
	
	int getCurrTab() const;
	int getZoomSpeed() const;

	template <class T> void getWidget(std::string name, T*& widget);

private:
	//access to static window elements
	Glib::RefPtr<Gtk::Builder> builder;
	//vector for containing level containers
	std::vector<Gtk::Layout*> layout;
	//notebook
	Gtk::Notebook* notebook;
	
	Settings settings;
	int zoomSpeed{5};

	const char* uiFile  = (char*)"./ui/dungeonCartographer.ui";
	const char* uiTab   = (char*)"./ui/tab.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#include "ui.tpp"

#endif
