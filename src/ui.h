#ifndef UI_H
#define UI_H

#include "settings.h"
#include "map.h"

#include <gtkmm/window.h>

#include <vector>

class UI : public Gtk::Window
{
public:
	UI(Settings*, Map*);

	void drawLevel(int i);

private:
	Settings* settings;
	Map* map;

	const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#endif
