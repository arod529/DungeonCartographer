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

	try
	{
		UI ui(&settings, &map); //make ui
		return app->run(*ui.window); //have the time of your life
	}
	catch(const Gtk::BuilderError& bErr) //ah rats, its the aliens
	{
		std::cerr << bErr.what();
		return 1;
	}
	catch(const Glib::Error& cssErr) //ah rats, its the aliens
	{
		std::cerr << cssErr.what();
		return 1;
	}
}
