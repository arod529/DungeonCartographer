#include "ui.h"
#include "constants.h"
#include "tilesettile.h"

#include <cmath>

//-----------------------------
//-----STATIC DECLARATIONS-----
//-----------------------------
int EventData::currTab = 0;

//-----------------
//-----UTILITY-----
//-----------------

/*!
	Centers the level on the canvas.

	@param[in] *layout  pointer to the layout of the current tab
	@param[in] *grid    pointer to the grid of the current tab
**/
void centerLevel(GtkWidget* layout, GtkWidget* grid)
{
	//get location and dimension properties
	GtkAllocation allocG;
	gtk_widget_get_allocation(grid, &allocG);
	GtkAllocation allocL;
	gtk_widget_get_allocation(layout, &allocL);

	//center grid in layout
	gtk_layout_move((GtkLayout*)layout, grid, allocL.width/2 - allocG.width/2, allocL.height/2 - allocG.height/2);
}

void zoomToFit(Level* level, GtkWidget* layout, GtkWidget* drawingArea0)
{
  //get location and dimension properties
  GtkAllocation allocL;
  gtk_widget_get_allocation(layout, &allocL);

  //get smallest dimension of layout
  int s = allocL.height;
  if(allocL.width < s) s = allocL.height;
  s /= level->getSize();

  //set new size
  gtk_widget_set_size_request(drawingArea0,s,s);
}

/*!
	Zooms the level in and out.

	@param[in] *settings 			pointer to the settings
	@param[in] *level 				pointer to the level of the current tab
	@param[in] *layout 				pointer to the layout of the current tab
	@param[in] *grid 					pointer to the grid of the current tab
	@param[in] *drawingArea0 	pointer to the drawing area at index 0 of the current tab
	@param[in] scrollDir 			the direction in which to zoom
**/
void zoom(Settings* settings, Level* level, GtkWidget* layout, GtkWidget* grid, GtkWidget* drawingArea0, double scrollDir)
{
	int xG, yG; //grid position
	int dx = 0;	//delta change in position
	int dy = 0;

	//get grid stats
	GtkAllocation allocG;
	gtk_widget_get_allocation(grid, &allocG);
	xG = allocG.x;
	yG = allocG.y;

	//-----Zoom Adjustment-----//
	//get current size of grid tile
	int s = gtk_widget_get_allocated_width(drawingArea0);

	//delta for size change
	int ds = settings->zoomSpeed;

	//change grid tile size
	if(scrollDir < 0) //scroll up
	{
		gtk_widget_set_size_request(drawingArea0,s+ds,s+ds);
		dx = -ds;
		dy = -ds;
	}
	else if(scrollDir > 0 && ds < s) //scroll down
	{
		gtk_widget_set_size_request(drawingArea0,s-ds,s-ds);
		dx = ds;
		dy = ds;
	}

	//-----Position Adjustment-----//
	// if(_settings->getZoomFollowsMouse())
	// {
	// 	//get mouse coords in canvas
	// 	double xMouse,yMouse;
	// 	gdk_event_get_coords(event, &xMouse, &yMouse);

	// 	//get layout size
		GtkAllocation allocL;
		gtk_widget_get_allocation(layout, &allocL);

	// 	//nudge grid center towards the mouse
	// 	dx -= ((xG + allocG.width/2) - xMouse)/ds;
	// 	dy += ((yG + allocG.height/2) - yMouse)/ds;
	// }

	//move the grid
	dx *= level->getSize()/2;
	dy *= level->getSize()/2;
	gtk_layout_move((GtkLayout*)layout, grid, xG + dx, yG + dy);
}

//----------------
//-----EVENTS-----
//----------------
/*!
  Zooms the level in when on zoom on button press

  @param[in] *btn        the button calling the event
  @param[in] *eventData  pointer to event data
**/
void eventBtn_zoomIn(GtkButton* btn, void* eventData)
{
	EventData* ed = (EventData*)eventData;
	int eI = ed->currTab;
	zoom(ed->settings, ed->level[eI], ed->layout[eI], ed->grid[eI], ed->drawingArea0[eI], -1);
}

/*!
  Zooms the level out when on zoom on button press

  @param[in] *btn        the button calling the event
  @param[in] *eventData  pointer to event data
**/
void eventBtn_zoomOut(GtkButton* btn, void* eventData)
{
	EventData* ed = (EventData*)eventData;
	int eI = ed->currTab;
	zoom(ed->settings, ed->level[eI], ed->layout[eI], ed->grid[eI], ed->drawingArea0[eI], 1);
}

/*!
  Centers the level and zooms to fit on button press

  @param[in] *btn        the button calling the event
  @param[in] *eventData  pointer to event data
**/
void eventBtn_zoomFit(GtkButton* btn, void* eventData)
{
  EventData* ed = (EventData*)eventData;  //event data
  int eI = ed->currTab;                   //tab index

	//fit level to canvas
  zoomToFit(ed->level[eI], ed->layout[eI], ed->drawingArea0[eI]);

  //center level
  centerLevel(ed->layout[eI], ed->grid[eI]);
}

/*!
  Changes the zoom speed value on slider adjust

  @param[in] *range     the range data associated with the slider
  @param[in] *settings  settings data
**/
void event_zoomSpeed(GtkRange* range, Settings* settings)
{
	settings->zoomSpeed = gtk_range_get_value(range);
}

/*!
  Changes the scroll speed value on slider adjust

  @param[in] *range     the range data associated with the slider
  @param[in] *settings  settings data
**/
void event_scrollSpeed(GtkRange* range, Settings* settings)
{
	settings->scrollSpeed = gtk_range_get_value(range);
}

void event_closeTab()
{

}

void event_changeTab(GtkNotebook* _notebook, GtkWidget* _page, uint _pageNum, void* _userData)
{

}

/*!
	Draw refresh event triggered for a tile.

	@param[in] *drawingArea  the tile's drawing area
	@param[in] *cr 	         the tile's cairo_t
**/
// bool event_drawTile(GtkWidget* drawingArea, cairo_t* cr)
// {
// 	//get drawing area size
// 	int s = gtk_widget_get_allocated_width(drawingArea);
// 	//get current assigned pixbuf and scale
// 	Tile* _tile = (Tile*)g_object_get_data(G_OBJECT(drawingArea),"tile");
// 	GdkPixbuf* tmp = gdk_pixbuf_scale_simple(_tile->tilesetTile->pixbuf,s,s,GDK_INTERP_NEAREST);
// 	//draw the image
// 	gdk_cairo_set_source_pixbuf(cr,tmp,0,0);
// 	cairo_paint(cr);

// 	//free mem
// 	g_object_unref(tmp);

// 	return false;
// }

/*!
	The click event for a tile.

	@param[in] *drawingArea  Unused; the drawing area calling the event.
	@param[in] *event	       Unused; required for overload.
	@param[in] *tile			   The tile clicked on.
**/
void event_tileClick(GtkWidget* _drawingArea, GdkEvent* _event, void* _tile)
{
	Tile* tile = (Tile*)_tile;		//the clicked tile

	tile->updateTile(); //set tile to room or background, update shared walls of adjacent tiles
}


/*!
  Zooms the level in and out when the mouse wheel is scrolled on the canvas

  @param[in] *layout      the layout calling the event
  @param[in] *event       event properties; auto generated
  @param[in] *eventData   event data
**/
void event_canvasScroll(GtkWidget* layout, GdkEvent* event, void* eventData)
{
	//get scroll direction
	double dxScroll; double dyScroll;
	gdk_event_get_scroll_deltas (event, &dxScroll, &dyScroll);

	//call zoom function
	EventData* ed = (EventData*)eventData;
	int eI = ed->currTab;
	zoom(ed->settings, ed->level[eI], ed->layout[eI], ed->grid[eI], ed->drawingArea0[eI], dyScroll);
}

/*!

**/
UI::UI(Settings* _settings, Map* _map)
{
	//read ui file
	builder = gtk_builder_new();
	GError *err = NULL;
	if(gtk_builder_add_from_file(builder, _settings->uiFile, &err) == 0)
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
			drawLevel(_map, 0);
			eventData.currTab = 0;

	//SIGNALS
	eventData.settings = _settings;
	eventData.level[0] = &_map->level[0];
	eventData.layout[0] = (GtkWidget*)layout[0];
	eventData.grid[0] = grid[0];
	eventData.drawingArea0[0] = _map->level[0].drawingArea[0];

	g_signal_connect(zoom[0],"clicked",G_CALLBACK(eventBtn_zoomIn),&eventData);
	g_signal_connect(zoom[1],"clicked",G_CALLBACK(eventBtn_zoomOut),&eventData);
	g_signal_connect(zoomFit,"clicked",G_CALLBACK(eventBtn_zoomFit),&eventData);

	g_signal_connect((GtkRange*)zoomSlider,"value-changed",G_CALLBACK(event_zoomSpeed),_settings);
	g_signal_connect((GtkRange*)scrollSlider,"value-changed",G_CALLBACK(event_scrollSpeed),_settings);

	g_signal_connect(layout[0],"scroll-event",G_CALLBACK(event_canvasScroll),&eventData);


	//css style
	css = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css, _settings->cssFile, &err);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//show
	gtk_widget_show_all(GTK_WIDGET(window));
}

UI::~UI()
{

}

void UI::drawLevel(Map* _map, int i)
{
	drawLevel(grid[i], &_map->level[i]);
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
		g_signal_connect(da,"draw",G_CALLBACK(TilesetTile::event_drawTile),NULL);
		g_object_set_data(G_OBJECT(da),"tile",&_lvl->tile[i]);
		//attach drawing area to grid
		gtk_grid_attach(GTK_GRID(_grid),da,i%lvlSize,i/lvlSize,1,1);

		//MOUSE EVENTS
		//click
		gtk_widget_add_events(da, GDK_BUTTON_PRESS_MASK);
		g_signal_connect(G_OBJECT(da),"button-press-event",G_CALLBACK(event_tileClick),&_lvl->tile[i]);

	}
	//set initial size
	gtk_widget_set_size_request(_lvl->drawingArea[0],25,25);
}
