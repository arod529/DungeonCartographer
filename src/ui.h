#ifndef UI_H
#define UI_H

#include "settings.h"
#include "level.h"

#include <string>
#include <vector>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/layout.h>

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
	Glib::RefPtr<Gtk::Builder> builder;
	std::vector<Gtk::Layout*> layout;
	Settings* settings;
	int currTab{0};
	int zoomSpeed{5};

	const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#include "ui.tpp"

#endif
