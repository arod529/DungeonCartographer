#include "ui.h"

#include <gtkmm/drawingarea.h>

/*!

**/
UI::UI(Settings* settings, Map* map)
: settings(settings), map(map)
{
	//gtk ui builder object
	builder = Gtk::Builder::create_from_file(settings->uiFile);

	//window
	builder->get_widget("window", window);
	window->show_all_children(true);
	window->maximize();

	//css style
	css = Gtk::CssProvider::create();
	css->load_from_path(settings->cssFile);
	auto styleContext = window->get_style_context();
	styleContext->add_provider_for_screen(window->get_screen(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//toolbar buttons
	builder->get_widget("saveMap", save);
	// save.signal_clicked()
	builder->get_widget("zoom[0]", zoom[0]);
	builder->get_widget("zoom[1]", zoom[1]);
	builder->get_widget("zoomFit", zoomFit);
	builder->get_widget("zoomSpeedSlider", zoomSlider);
	builder->get_widget("scrollSpeedSlider", scrollSlider);

	//noteboook
	builder->get_widget("notebook", notebook);

		//layout
		layout.emplace_back(new Gtk::Layout());
		builder->get_widget("layout[0]", layout[0]);

		//grid
		grid.emplace_back(new Gtk::Grid());
		builder->get_widget("grid[0]", grid[0]);

			//draw initial default map
			// drawLevel(0);
			// eventData.currTab = 0;

	//SIGNALS


	// eventData.settings = settings;
	// eventData.level[0] = &map->level[0];
	// eventData.layout[0] = (GtkWidget*)layout[0];
	// eventData.grid[0] = grid[0];
	// eventData.drawingArea0[0] = map->level[0].drawingArea[0];

	// g_signal_connect(save,"clicked",G_CALLBACK(Event::save),map);
	// g_signal_connect(zoom[0],"clicked",G_CALLBACK(Event::btn_zoomIn),&eventData);
	// g_signal_connect(zoom[1],"clicked",G_CALLBACK(Event::btn_zoomOut),&eventData);
	// g_signal_connect(zoomFit,"clicked",G_CALLBACK(Event::btn_zoomFit),&eventData);

	// g_signal_connect((GtkRange*)zoomSlider,"value-changed",G_CALLBACK(Event::zoomSpeed),settings);
	// g_signal_connect((GtkRange*)scrollSlider,"value-changed",G_CALLBACK(Event::scrollSpeed),settings);

	// g_signal_connect(layout[0],"scroll-event",G_CALLBACK(Event::canvasScroll),&eventData);
}

UI::~UI()
{
	delete window;
}

void UI::drawLevel(int i)
{
	auto _level = map->level[i];

	//size var for convenience and call reduction
	int lvlSize = _level.getSize();
	int tileNum = lvlSize*lvlSize;
	Gtk::DrawingArea* da;

	for(int i = 0; i < tileNum; i++)
	{
		da = _level.drawingArea[i];

		//create callback for render
		// da->signal_draw().connect(sigc::ptr_fun(&Event::drawTile));
// g_signal_connect(da,"draw",G_CALLBACK(Event::drawTile),NULL);
// g_object_set_data(G_OBJECT(da),"tile",&_lvl->tile[i]);

		//attach drawing area to grid
		// grid[i]->attach(da->gobj(), i%lvlSize, i/lvlSize);
// gtk_grid_attach(GTK_GRID(_grid),da,i%lvlSize,i/lvlSize,1,1);

		//MOUSE EVENTS
		//click
// gtk_widget_add_events(da, GDK_BUTTON_PRESS_MASK);
// g_signal_connect(G_OBJECT(da),"button-press-event",G_CALLBACK(Event::tileClick),&_lvl->tile[i]);

	}
	//set initial size
	da->set_size_request(25,25);
// gtk_widget_set_size_request(_lvl->drawingArea[0],25,25);
}
