#include "map.h"
#include "settings.h"
#include "ui.h"

#include <gtkmm/application.h>
#include <glibmm/error.h>

#include <iostream>

int main(int argc, char **argv)
{
	//initilize gtk
	auto app = Gtk::Application::create(argc, argv);

  Settings settings;       // create settings object
  UI ui(settings);         // make ui
  Map map(&settings, &ui); // create default map

	return app->run(ui); //have the time of your life
}
