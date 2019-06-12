#include "ui.h"


int main(int argc, char **argv)
{
	//initilize gtk
	gtk_init(&argc, &argv);

	Settings settings;
	settings.writeDefaultTilesetFile();
	Map map(&settings);
	try
	{
		UI ui(&settings, &map);
	}
	catch(const std::exception& e)
	{
		g_printerr(e.what());
		return 1;
	}

	gtk_main();
	return 0;
}