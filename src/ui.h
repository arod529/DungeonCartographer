#ifndef UI_H
#define UI_H

#include "level.h"
#include "settings.h"

#include <string>
#include <vector>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/layout.h>
#include <gtkmm/notebook.h>
#include <gtkmm/window.h>

//forward declaration
class Level;

class UI : public Gtk::Window
{
public:
	UI(Settings* settings);

	std::string saveAs();
	void addLevel(Level* level);
	
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
	
	Settings* settings;
	int currTab{0};
	int zoomSpeed{5};

	const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
	const char* uiTab = (char*)"./ui/tab.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#include "ui.tpp"

#endif
