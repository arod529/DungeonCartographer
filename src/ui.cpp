#include "ui.h"

//-----------------------------
//-----STATIC DECLARATIONS-----
//-----------------------------
int EventData::currTab = 0;

/*!

**/
UI::UI(Settings* settings, Map* map)
: settings(settings), map(map)
{
	//read ui file
	builder = gtk_builder_new();
	GError *err = NULL;
	if(gtk_builder_add_from_file(builder, settings->uiFile, &err) == 0)
	{
		std::string errMsg = "UI file error: ";
		errMsg += err->message;
		errMsg += '\n';
		throw std::runtime_error(errMsg);
	}

	//window
	window = gtk_builder_get_object(builder,"window");
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_maximize((GtkWindow*)window);

	//toolbar buttons
	save = gtk_builder_get_object(builder,"saveMap");
	zoom[0] = gtk_builder_get_object(builder,"zoom[0]");
	zoom[1] = gtk_builder_get_object(builder,"zoom[1]");
	zoomFit = gtk_builder_get_object(builder,"zoomFit");

	zoomSlider = gtk_builder_get_object(builder,"zoomSpeedSlider");
	scrollSlider = gtk_builder_get_object(builder,"scrollSpeedSlider");

	//noteboook

		//layout
		layout.emplace_back(gtk_builder_get_object(builder,"layout[0]"));

		//grid
		grid.emplace_back(GTK_WIDGET(gtk_builder_get_object(builder,"grid[0]")));

			//draw initial default map
			drawLevel(map, 0);
			eventData.currTab = 0;

	//SIGNALS
	eventData.settings = settings;
	eventData.level[0] = &map->level[0];
	eventData.layout[0] = (GtkWidget*)layout[0];
	eventData.grid[0] = grid[0];
	eventData.drawingArea0[0] = map->level[0].drawingArea[0];

	g_signal_connect(save,"clicked",G_CALLBACK(Event::save),map);
	g_signal_connect(zoom[0],"clicked",G_CALLBACK(Event::btn_zoomIn),&eventData);
	g_signal_connect(zoom[1],"clicked",G_CALLBACK(Event::btn_zoomOut),&eventData);
	g_signal_connect(zoomFit,"clicked",G_CALLBACK(Event::btn_zoomFit),&eventData);

	g_signal_connect((GtkRange*)zoomSlider,"value-changed",G_CALLBACK(Event::zoomSpeed),settings);
	g_signal_connect((GtkRange*)scrollSlider,"value-changed",G_CALLBACK(Event::scrollSpeed),settings);

	g_signal_connect(layout[0],"scroll-event",G_CALLBACK(Event::canvasScroll),&eventData);


	//css style
	auto css = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css, settings->cssFile, &err);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//show
	gtk_widget_show_all(GTK_WIDGET(window));
}

UI::~UI()
{

}

void UI::drawLevel(Map* map, int i)
{
	drawLevel(grid[i], &map->level[i]);
}

void UI::drawLevel(GtkWidget* _grid, Level* _lvl)
{
	//size var for convenience and call reduction
	int lvlSize = _lvl->getSize();
	int tileNum = lvlSize*lvlSize;
	GtkWidget* da;

	for(int i = 0; i < tileNum; i++)
	{
		da = _lvl->drawingArea[i];
		//create callback for render
		g_signal_connect(da,"draw",G_CALLBACK(Event::drawTile),NULL);
		g_object_set_data(G_OBJECT(da),"tile",&_lvl->tile[i]);
		//attach drawing area to grid
		gtk_grid_attach(GTK_GRID(_grid),da,i%lvlSize,i/lvlSize,1,1);

		//MOUSE EVENTS
		//click
		gtk_widget_add_events(da, GDK_BUTTON_PRESS_MASK);
		g_signal_connect(G_OBJECT(da),"button-press-event",G_CALLBACK(Event::tileClick),&_lvl->tile[i]);

	}
	//set initial size
	gtk_widget_set_size_request(_lvl->drawingArea[0],25,25);
}
