#include "ui.h"

#include "event.h"

#include <gtkmm/cssprovider.h>
#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/label.h>
#include <gtkmm/toolbutton.h>

/*!

**/
UI::UI(Settings* settings)
: settings{settings}
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
	maximize();
	show_all_children();
}

/*!
  \bug change to get tab from notebook; remove currTab var;
**/
int UI::getCurrTab() const
	{return notebook->get_current_page();}

/*!
  \bug change to get zoomSlider value; remove zoomSpeed var;
**/
int UI::getZoomSpeed() const
	{return zoomSpeed;}


void UI::addLevel(Level* level)
{
	//make a builder for the tab
	auto tabBuilder = Gtk::Builder::create_from_file(uiTab);

	//get the notebook
	int currTab = getCurrTab();

	//create layout
	layout.emplace_back();
	tabBuilder->get_widget("layout", layout.back());	

	//add level to layout
	layout.back()->add(*level);
	level->show();

	//create tab
	Gtk::Box* tab;
	tabBuilder->get_widget("tab", tab);
	//set default tab label
	Gtk::Label* tabLabel;
	tabBuilder->get_widget("tabLabel", tabLabel);
	tabLabel->set_label("Level " + std::to_string(currTab+1));


	//add layout and tab to notebook; insert inplace of new tab
	notebook->insert_page(*layout.back(), *tab, currTab);
	//show all
	notebook->show_all_children();
	//set to active page
	notebook->set_current_page(currTab);
}

/*!
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