#include "ui.h"

#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/viewport.h>

/*!
	Initilizes the program UI.

	@param[in] settings The settings to use for initial set up.
**/
UI::UI(Settings& settings)
{
	//css style
	auto css = Gtk::CssProvider::create();
	css->load_from_path(cssFile);
	get_style_context()->add_provider_for_screen(get_screen(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//gtk ui builder object
	builder = Gtk::Builder::create_from_file(uiFile);

	//add static window content
	Gtk::Box* content;
	builder->get_widget("contentBox", content);
	add(*content);

	//get notebook
	builder->get_widget("notebook", notebook);

	//window
	set_title("Dungeon Cartographer");
	iconList.emplace_back(Gdk::Pixbuf::create_from_file("./ui/dc_icon.png"));
	set_icon_list(iconList);
	maximize();
	show_all_children();
}

/*!
  Adds a tab to the interface and populates it with widget of type Level.

  @param[in] pageNum The page to add a tab at.
  @param[in] widget The widget to add to the tab.
**/
void UI::addTab(uint pageNum, Gtk::Widget* widget)
{
	//make a builder for the tab
  auto tabBuilder = Gtk::Builder::create_from_file(uiTab);

  //get scrolled window and viewport
  Gtk::ScrolledWindow* scrolledWindow;
  tabBuilder->get_widget("scrolledWindow", scrolledWindow);
  scrolledWindow->add_events(Gdk::EventMask::SCROLL_MASK);

  Gtk::Viewport* viewport;
  tabBuilder->get_widget("viewport", viewport);

  //add level viewport
  viewport->add(*widget);

  //create tab
  Gtk::Label* tabLabel;
  tabBuilder->get_widget("tabLabel", tabLabel);
  tabLabel->set_label("Level " + std::to_string(pageNum+1));

  //add scrolled window and tab to notebook; insert in-place of new tab
  notebook->insert_page(*scrolledWindow, *tabLabel, pageNum);
  //show all
  notebook->show_all_children();
  //set to active page
  notebook->set_current_page(pageNum);
}

/*!
  Removes all tabs except for the new Level tab.
**/
void UI::clearTabs()
{
	//remove notebook pages
  while(notebook->get_n_pages() > 1)
    {notebook->remove_page();}
}

/*!
  Dipslays a save as dialog.

  @return The file path to the chosen file.

  \bug overwrite confirmation does not work
**/
std::string UI::saveAs()
{
	auto dSaveAs = Gtk::FileChooserDialog(*this, "Save As", Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
	dSaveAs.set_do_overwrite_confirmation(true);
	dSaveAs.add_button("Save", 1);
	dSaveAs.add_button("Cancel", 0);
  auto response = dSaveAs.run();

  if(response)
  	return dSaveAs.get_filename();
  else
  	return "";
}

/*!
  Dipslays a save as dialog.

  @return The file path to the chosen file.

  \bug overwrite confirmation does not work
**/
std::string UI::openFile()
{
	auto dOpen = Gtk::FileChooserDialog(*this, "Open", Gtk::FileChooserAction::FILE_CHOOSER_ACTION_OPEN);
	dOpen.add_button("Open", 1);
	dOpen.add_button("Cancel", 0);
  auto response = dOpen.run();

  if(response)
  	return dOpen.get_filename();
  else
  	return "";
}

/*!
  Gets the scrollSpeed slider value.

  @return The value of the scroll speed slider.
**/
double UI::getScrollSpeed()
{
	auto scrollAdjustment = (Gtk::Adjustment*)builder->get_object("scrollAdjustment").get();
	return scrollAdjustment->get_value();
}

/*!
  Scrolls the window based on the scroll speed slider value.

  @param[in] dx The chhange in direction x
  @param[in] dy The chhange in direction y
**/
void UI::scroll(double dx, double dy)
{
	auto scrolledWindow = (Gtk::ScrolledWindow*)notebook->get_nth_page(notebook->get_current_page());
  auto hAdjust = scrolledWindow->get_hadjustment();
  auto vAdjust = scrolledWindow->get_vadjustment();

  auto scrollSpeed = getScrollSpeed()*10;

  hAdjust->set_value(hAdjust->get_value()+dx*scrollSpeed);
  vAdjust->set_value(vAdjust->get_value()+dy*scrollSpeed);
}

/*!
  Zooms the grid by adjusting the tile size. Maintains focus.

  @param[in] scrollDir The direction to zoom; <0 zooms in; >0 zooms out; =0 zooms to fit.
  @param[in] gridSize The size of the grid.
  @param[in] widget The tile widget at (0,0).
**/
void UI::zoom(int scrollDir, int gridSize, Gtk::Widget* widget)
{
	auto scrolledWindow = (Gtk::ScrolledWindow*)notebook->get_nth_page(notebook->get_current_page());
	
	//get width and height of the scrolled window
	double wWidth = scrolledWindow->get_allocated_width();
	double wHeight = scrolledWindow->get_allocated_height();
	auto hAdjust = scrolledWindow->get_hadjustment();
	auto vAdjust = scrolledWindow->get_vadjustment();

	//get the current width of the widget
	double s = widget->get_allocated_width();

	//get the zoom speed value from ui
	auto zoomAdjustment = (Gtk::Adjustment*)builder->get_object("zoomAdjustment").get();
	double ds = zoomAdjustment->get_value(); //size delta
	double dp = gridSize/2*ds; //position delta

	//change widget size
	if(scrollDir < 0) //scroll up, zoom in
	{
		widget->set_size_request(s+ds, s+ds);
		hAdjust->set_value(hAdjust->get_value()+dp);
		vAdjust->set_value(vAdjust->get_value()+dp);
	}
	else if(scrollDir > 0) //scroll down, zoom out
	{
		if(ds < s) //don't shrink to nothing
		{
			widget->set_size_request(s-ds, s-ds);
			hAdjust->set_value(hAdjust->get_value()-dp);
			vAdjust->set_value(vAdjust->get_value()-dp);
		}
	}
	else //fit to window
	{
		//set tile size to min dimension/(number of tiles + padding of 4 tiles)
		s = std::min(wWidth, wHeight)/(gridSize+4);
		if(s < 0) s = 1; //make sure s is not <= 0
		widget->set_size_request(s, s);
	}

	//refresh viewport to prevent draw artifacts on drop shadow
	scrolledWindow->get_child()->queue_draw();
}