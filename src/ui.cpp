#include "ui.h"

#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filefilter.h>
#include <gtkmm/grid.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/menu.h>
#include <gtkmm/overlay.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/viewport.h>

/*!
	Initilizes the program UI.

	@param[in] settings The settings to use for initial set up.
**/
UI::UI(Settings* settings, Map* map)
: settings{settings}
, map{map}
{
	//-------------------------
  //----- Create Window -----
  //-------------------------

  //icons
  auto icontheme = Gtk::IconTheme::get_default();
  icontheme->append_search_path ("./ui/icons") ;

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
  iconList.emplace_back(Gdk::Pixbuf::create_from_file("./ui/icons/icon_dc.png"));
  set_icon_list(iconList);
  maximize();
  show_all_children();

	//------------------------------------
	//----- Load Default UI Settings -----
	//------------------------------------


	//---------------------------
	//----- Connect Signals -----
	//---------------------------
	
	//tool buttons
	Gtk::ToolButton* btn;
  builder->get_widget("btn_new", btn);
  btn->signal_clicked().connect(sigc::bind<Tileset*, uint>(sigc::mem_fun(*map, &Map::newMap), &settings->tileset, settings->mapSize));

  builder->get_widget("btn_open", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &UI::open));

  builder->get_widget("btn_save", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &UI::save));
  builder->get_widget("btn_saveAs", btn);
  btn->signal_clicked().connect(sigc::mem_fun(*this, &UI::saveAs));

  builder->get_widget("btn_zoomIn", btn);
  btn->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &UI::zoom), -1));
  builder->get_widget("btn_zoomOut", btn);
  btn->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &UI::zoom), 1));
  builder->get_widget("btn_zoomFit", btn);
  btn->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &UI::zoom), 0));

  builder->get_widget("btn_grid", gridToggle);
  gridToggle->signal_toggled().connect(sigc::mem_fun(*this, &UI::toggleGrid));

  //menus
  Gtk::MenuItem* menu;
  builder->get_widget("menu_new", menu);
  menu->signal_activate().connect(sigc::bind<Tileset*, uint>(sigc::mem_fun(*map, &Map::newMap), &settings->tileset, settings->mapSize));

  builder->get_widget("menu_open", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &UI::open));

  builder->get_widget("menu_save", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &UI::save));
  builder->get_widget("menu_saveAs", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &UI::saveAs));

  builder->get_widget("menu_print", menu);
  menu->signal_activate().connect(sigc::mem_fun(*map, &Map::print));

	builder->get_widget("menu_quit", menu);
  menu->signal_activate().connect(sigc::mem_fun(*this, &UI::hide));

  builder->get_widget("menu_reset", menu);
  menu->signal_activate().connect(sigc::bind<int>(sigc::mem_fun(*map, &Map::resetLevel), currPage));

  builder->get_widget("gridColor", gridColor);
  gridColor->signal_color_changed().connect(sigc::mem_fun(*this, &UI::setGridColor));

  //notebook
  pageSwitchCon = notebook->signal_switch_page().connect(sigc::mem_fun(*this, &UI::pageSwitch));

  //map signals
  map->signal_levelCreated.connect(sigc::mem_fun(*this, &UI::addTab));
  map->signal_mapCleared.connect(sigc::mem_fun(*this, &UI::clearTabs));

	//--------------------------
	//----- Make a New Map -----
	//--------------------------
  map->newMap(&settings->tileset, settings->mapSize);
}

UI::~UI()
{
  //This damn signal crashes the program on exit if not disconnected first.
  pageSwitchCon.disconnect();
}

/*!
  Adds a tab to the interface and populates it with widget of type Level.

  @param[in] 
  @param[in] 
**/
void UI::addTab(int levelId, Level* level)
{
	//make a builder for the tab
  auto tabBuilder = Gtk::Builder::create_from_file(uiTab);

  //get scrolled window and viewport; connect scroll signal
  Gtk::ScrolledWindow* scrolledWindow;
  tabBuilder->get_widget("scrolledWindow", scrolledWindow);
  scrolledWindow->add_events(Gdk::EventMask::SCROLL_MASK);
  scrolledWindow->signal_scroll_event().connect(sigc::mem_fun(*this, &UI::scrollEvent), false);

  Gtk::Overlay* overlay;
  tabBuilder->get_widget("overlay", overlay);

  //add level to overlay as base
  overlay->add(*level);
  // map->setTileSize(levelId, 25);

  //add reference grid overlay
  refgrid.emplace_back();
  overlay->add_overlay(refgrid[levelId]);
  overlay->set_overlay_pass_through(refgrid[levelId], true);

  //create tab
  Gtk::Label* tabLabel;
  tabBuilder->get_widget("tabLabel", tabLabel);
  tabLabel->set_label("Level " + std::to_string(levelId+1));

  //add scrolled window and tab to notebook; insert in-place of new tab
  notebook->insert_page(*scrolledWindow, *tabLabel, levelId);
  
  //show all
  notebook->show_all_children();
  //set to active page
  notebook->set_current_page(levelId);
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
  Displays a open file dialog, and subsequently loads the file if a name is chosen.
**/
void UI::open()
{
	//make dialog
	auto dOpen = Gtk::FileChooserDialog(*this, "Open", Gtk::FileChooserAction::FILE_CHOOSER_ACTION_OPEN);
	dOpen.add_button("Open", 1)->grab_default();
	dOpen.add_button("Cancel", 0);

	//add file filters
	auto dcm = Gtk::FileFilter::create();
	dcm->add_pattern("*.dcm");
	dcm->set_name("Dungeon Cartographer Map (*.dcm)");
	dOpen.add_filter(dcm);
	auto all = Gtk::FileFilter::create();
	all->add_pattern("*");
	all->set_name("All Files (*)");
	dOpen.add_filter(all);
  
  //display dialog
  if(dOpen.run())
  {
  	std::string fPath = dOpen.get_filename();

  	if(fPath != "")
	  {
	  	//load map file
	    map->loadFile(fPath);
	  }
	}
}

void UI::pageSwitch(Gtk::Widget* page, uint pageNum)
{
  //triggered by tab change && (is only page || tab not newtab)
  if(!(page != NULL && (pageNum == 0 || ((Gtk::Label*)notebook->get_tab_label(*page))->get_text() != "+")))
    map->appendLevel();

  //update currpage
  currPage = notebook->get_current_page();

  //make sure grid toggle immitates grid visibility
  gridToggle->set_active(refgrid[pageNum].isActive());
}

/*!
  Saves to the file associated with the Map.
  If there is no file associated, displays save as dialog.
**/
void UI::save()
{
  std::string fPath = map->getFilePath();
  if(fPath == "") //there is no associated file
    saveAs();
  else //save the file
    map->saveToFile(fPath);
}

/*!
  Dipslays a save as dialog.

  \bug overwrite confirmation does not work
**/
void UI::saveAs()
{
  auto dSaveAs = Gtk::FileChooserDialog(*this, "Save As", Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
  dSaveAs.set_do_overwrite_confirmation(true);
  dSaveAs.add_button("Save", 1)->grab_default();
  dSaveAs.add_button("Cancel", 0);

  //add file filters
  auto dcm = Gtk::FileFilter::create();
  dcm->add_pattern("*.dcm");
  dcm->set_name("Dungeon Cartographer Map (*.dcm)");
  dSaveAs.add_filter(dcm);
  auto all = Gtk::FileFilter::create();
  all->add_pattern("*");
  all->set_name("All Files (*)");
  dSaveAs.add_filter(all);

  if(dSaveAs.run())
  {
    std::string fPath = dSaveAs.get_filename();

    if(fPath != "")
    {
      //append .dcm file extension if no extension was given and the dcm filter selected
      if(fPath.find('.') == std::string::npos && dSaveAs.get_filter() == dcm)
      fPath += ".dcm";

      //save file
      map->saveToFile(fPath);
    }
  }
}

/*!
  Scrolls the window based on the scroll speed slider value.

  @param[in] dx The chhange in direction x
  @param[in] dy The chhange in direction y
**/
void UI::scroll(double dx, double dy)
{
	auto scrolledWindow = (Gtk::ScrolledWindow*)notebook->get_nth_page(currPage);
  auto hAdjust = scrolledWindow->get_hadjustment();
  auto vAdjust = scrolledWindow->get_vadjustment();

  auto scrollSpeed = getScrollSpeed()*10;

  hAdjust->set_value(hAdjust->get_value()+dx*scrollSpeed);
  vAdjust->set_value(vAdjust->get_value()+dy*scrollSpeed);
}

/*!
  Zooms or scrolls the Level on mouse wheel input based on modifer keys.

  @param[in] scroll_event The event information.

  @return The signal has been fully handled.
**/
bool UI::scrollEvent(GdkEventScroll* scroll_event)
{
  //call zoom event if ctrl key is held
  if(scroll_event->state == Gdk::ModifierType::CONTROL_MASK)
  {
    zoom(scroll_event->delta_y);
  }
  else
  {
    if(scroll_event->state == Gdk::ModifierType::SHIFT_MASK)
      scroll(scroll_event->delta_y, 0);
    else
      scroll(0, scroll_event->delta_y);
  }
  return true; //don't pass on event
}

void UI::setGridColor()
{  
  auto rgba = gridColor->get_current_rgba();
  refgrid[currPage].setRGB(rgba.get_red(), rgba.get_green(), rgba.get_blue()); 
}

/*!
  Toggles the reference grid
**/
void UI::toggleGrid()
{
  refgrid[currPage].setActive(gridToggle->get_active());
}

/*!
  Zooms the grid by adjusting the tile size. Maintains focus.

  @param[in] scrollDir The direction to zoom; <0 zooms in; >0 zooms out; =0 zooms to fit.
  @param[in] gridSize The size of the grid.
  @param[in] widget The tile widget at (0,0).
**/
void UI::zoom(int scrollDir)
{
	int gridSize = map->getLevelSize(currPage);

	//get current active scrolled window
	auto scrolledWindow = (Gtk::ScrolledWindow*)notebook->get_nth_page(currPage);
	
	//get width and height of the scrolled window; get adjustments
	double wWidth = scrolledWindow->get_allocated_width();
	double wHeight = scrolledWindow->get_allocated_height();
	auto hAdjust = scrolledWindow->get_hadjustment();
	auto vAdjust = scrolledWindow->get_vadjustment();

	if(scrollDir == 0) //fit to window
	{
		//set tile size to min dimension/(number of tiles + padding of 4 tiles)
		int s = std::min(wWidth, wHeight)/(gridSize+4);
		if(s < 0) s = 1; //make sure s is not <= 0
		map->setTileSize(currPage, s);
	}
	else
	{
		//get the current width of the a tile
		double s = map->getTileSize(currPage);

		//get the zoom speed value from ui
		auto zoomAdjustment = (Gtk::Adjustment*)builder->get_object("zoomAdjustment").get();
		double ds = zoomAdjustment->get_value(); //size delta
		double dp = gridSize/2*ds; //position delta

		//change tile size
		if(scrollDir < 0) //scroll up, zoom in
		{
			map->setTileSize(currPage, s+ds);
			hAdjust->set_value(hAdjust->get_value()+dp);
			vAdjust->set_value(vAdjust->get_value()+dp);
		}
		else if(scrollDir > 0) //scroll down, zoom out
		{
			if(ds < s) //don't shrink to nothing
			{
				map->setTileSize(currPage, s-ds);
				hAdjust->set_value(hAdjust->get_value()-dp);
				vAdjust->set_value(vAdjust->get_value()-dp);
			}
		}
	}

	//refresh viewport to prevent draw artifacts on drop shadow
	scrolledWindow->get_child()->queue_draw();
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

