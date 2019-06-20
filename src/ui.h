#ifndef UI_H
#define UI_H

#include "settings.h"
#include "level.h"

#include <gtkmm/window.h>
#include <gtkmm/layout.h>

#include <vector>

class UI : public Gtk::Window
{
public:
	UI();

	void addLevel(Level* level);

private:
	Gtk::Layout* layout;

	const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#endif
