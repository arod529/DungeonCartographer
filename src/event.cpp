#include "event.h"

#include "map.h"
#include "tilesettile.h"

// //----------------
// //-----EVENTS-----
// //----------------
// void Event::save(GtkButton* btn, void* map)
// {
//   ((Map*)map)->saveToFile("./mapTestFile");
// }

// /*!
//   Zooms the level in on zoom in button press.

//   @param[in] *btn        The button calling the event.
//   @param[in] *eventData  EventData object.
// **/
// void Event::btn_zoomIn(GtkButton* btn, void* eventData)
// {
//   EventData* ed = (EventData*)eventData;
//   int eI = ed->currTab;
//   zoom(ed->settings, ed->level[eI], ed->layout[eI], ed->grid[eI], ed->drawingArea0[eI], -1);
// }

// /*!
//   Zooms the level out on zoom out button press.

//   @param[in] *btn        The button calling the event.
//   @param[in] *eventData  EventData object.
// **/
// void Event::btn_zoomOut(GtkButton* btn, void* eventData)
// {
//   EventData* ed = (EventData*)eventData;
//   int eI = ed->currTab;
//   zoom(ed->settings, ed->level[eI], ed->layout[eI], ed->grid[eI], ed->drawingArea0[eI], 1);
// }

// /*!
//   Centers the level and zooms to fit on button press.

//   @param[in] *btn        The button calling the event.
//   @param[in] *eventData  EventData object.
// **/
// void Event::btn_zoomFit(GtkButton* btn, void* eventData)
// {
//   EventData* ed = (EventData*)eventData;  //event data
//   int eI = ed->currTab;                   //tab index

//   //fit level to canvas
//   zoomToFit(ed->level[eI], ed->layout[eI], ed->drawingArea0[eI]);

//   //center level
//   centerLevel(ed->layout[eI], ed->grid[eI]);
// }

// /*!
//   Changes the zoom speed value on slider adjust.

//   @param[in] *range     The range data associated with the slider.
//   @param[in] *settings  Settings object.
// **/
// void Event::zoomSpeed(GtkRange* range, Settings* settings)
//   {settings->zoomSpeed = gtk_range_get_value(range);}

// /*!
//   Changes the scroll speed value on slider adjust.

//   @param[in] *range     The range data associated with the slider.
//   @param[in] *settings  Settings object.
// **/
// void Event::scrollSpeed(GtkRange* range, Settings* settings)
//   {settings->scrollSpeed = gtk_range_get_value(range);}

// // void event_closeTab()
// // {

// // }

// // void event_changeTab(GtkNotebook* _notebook, GtkWidget* _page, uint _pageNum, void* _userData)
// // {

// // }

/*!
  The click event for a tile.

  @param[in] *drawingArea  Unused; the drawing area calling the event.
  @param[in] *event        Unused; required for overload.
  @param[in] *tile         Tile clicked on.
**/
// void Event::tileClick(GtkWidget* drawingArea, GdkEvent* event, void* tile)
//   {((Tile*)tile)->updateTile();}

// bool Event::drawTile(Cairo::RefPtr<Cairo::Context> cr, Gtk::DrawingArea* da, Tile* tile)
// {
//   tile->tilesetTile->drawTile(da, cr);
//   return false; //keep signal handler
// }


// /*!
//   Zooms the level in and out when the mouse wheel is scrolled on the canvas.

//   @param[in] *layout      The layout calling the event.
//   @param[in] *event       Unused; required for overload.
//   @param[in] *eventData   EventData object.
// **/
// void Event::canvasScroll(GtkWidget* layout, GdkEvent* event, void* eventData)
// {
//   //get scroll direction
//   double dxScroll; double dyScroll;
//   gdk_event_get_scroll_deltas (event, &dxScroll, &dyScroll);

//   //call zoom function
//   EventData* ed = (EventData*)eventData;
//   int eI = ed->currTab;
//   zoom(ed->settings, ed->level[eI], ed->layout[eI], ed->grid[eI], ed->drawingArea0[eI], dyScroll);
// }

// // //-----------------
// // //-----UTILITY-----
// // //-----------------

/*!
  Centers the level on the canvas.

  @param[in] *layout  pointer to the layout of the current tab
  @param[in] *grid    pointer to the grid of the current tab
**/
void Event::centerLevel(Gtk::Layout* layout, Gtk::Grid* grid)
{
  //get location and dimension properties
  Gtk::Allocation allocG = grid->get_allocation();
  Gtk::Allocation allocL = layout->get_allocation();

  //center grid in layout
  layout->move(*grid, allocL.get_width()/2 - allocG.get_width()/2, allocL.get_height()/2 - allocG.get_height()/2);
}

// void Event::zoomToFit(Level* level, GtkWidget* layout, GtkWidget* drawingArea0)
// {
//   //get location and dimension properties
//   GtkAllocation allocL;
//   gtk_widget_get_allocation(layout, &allocL);

//   //get smallest dimension of layout
//   int s = allocL.height;
//   if(allocL.width < s) s = allocL.height;
//   s /= level->getSize();

//   //set new size
//   gtk_widget_set_size_request(drawingArea0,s,s);
// }

// /*!
//   Zooms the level in and out.

//   @param[in] *settings       pointer to the settings
//   @param[in] *level          pointer to the level of the current tab
//   @param[in] *layout         pointer to the layout of the current tab
//   @param[in] *grid           pointer to the grid of the current tab
//   @param[in] *drawingArea0   pointer to the drawing area at index 0 of the current tab
//   @param[in] scrollDir       the direction in which to zoom
// **/
// void Event::zoom(Settings* settings, Level* level, GtkWidget* layout, GtkWidget* grid, GtkWidget* drawingArea0, double scrollDir)
// {
//   int xG, yG; //grid position
//   int dx = 0;  //delta change in position
//   int dy = 0;

//   //get grid stats
//   GtkAllocation allocG;
//   gtk_widget_get_allocation(grid, &allocG);
//   xG = allocG.x;
//   yG = allocG.y;

//   //-----Zoom Adjustment-----//
//   //get current size of grid tile
//   int s = gtk_widget_get_allocated_width(drawingArea0);

//   //delta for size change
//   int ds = settings->zoomSpeed;

//   //change grid tile size
//   if(scrollDir < 0) //scroll up
//   {
//     gtk_widget_set_size_request(drawingArea0,s+ds,s+ds);
//     dx = -ds;
//     dy = -ds;
//   }
//   else if(scrollDir > 0 && ds < s) //scroll down
//   {
//     gtk_widget_set_size_request(drawingArea0,s-ds,s-ds);
//     dx = ds;
//     dy = ds;
//   }

//   //-----Position Adjustment-----//
//   // if(_settings->getZoomFollowsMouse())
//   // {
//   //   //get mouse coords in canvas
//   //   double xMouse,yMouse;
//   //   gdk_event_get_coords(event, &xMouse, &yMouse);

//   //   //get layout size
//     GtkAllocation allocL;
//     gtk_widget_get_allocation(layout, &allocL);

//   //   //nudge grid center towards the mouse
//   //   dx -= ((xG + allocG.width/2) - xMouse)/ds;
//   //   dy += ((yG + allocG.height/2) - yMouse)/ds;
//   // }

//   //move the grid
//   dx *= level->getSize()/2;
//   dy *= level->getSize()/2;
//   gtk_layout_move((GtkLayout*)layout, grid, xG + dx, yG + dy);
// }
