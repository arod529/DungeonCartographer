#ifndef UI_H
#define UI_H

#include "settings.h"
#include "level.h"

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/layout.h>

#include <string>
#include <vector>

class UI : public Gtk::Window
{
public:
	UI();

	void addLevel(Level* level);
	std::string saveAs();

	template <class T> void getWidget(std::string name, T*& widget);

private:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Layout* layout;

	const char* uiFile = (char*)"./ui/dungeonCartographer.ui";
	const char* cssFile = (char*)"./ui/dungeonCartographer.css";
};

#include "ui.tpp"

#endif
