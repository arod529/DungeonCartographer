#include "ui.h"
#include "settings.h"
#include "map.h"

int main(int argc, char **argv)
{
	//initilize gtk
	gtk_init(&argc, &argv);

	Settings settings; //create settings object
	Map map(&settings); //create default map
	try
	{
		UI ui(&settings, &map); //make ui
	}
	catch(const std::exception& e) //ah rats, its the aliens
	{
		g_printerr(e.what());
		return 1;
	}

	gtk_main(); //have the time of your life
	return 0;
}
