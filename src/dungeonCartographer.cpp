#include "ui.h"
#include "settings.h"
#include "map.h"

#include <gtkmm/application.h>
#include <glibmm/error.h>

#include <iostream>

int main(int argc, char **argv)
{
	//initilize gtk
	auto app = Gtk::Application::create(argc, argv);

	Settings settings; //create settings object
	Map map(&settings); //create default map
	UI ui(&settings, &map); //make ui
	
	return app->run(ui); //have the time of your life
}
