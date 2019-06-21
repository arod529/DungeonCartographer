#include "ui.h"

#include "event.h"

#include <gtkmm/cssprovider.h>
#include <gtkmm/box.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/dialog.h>
#include <gtkmm/notebook.h>

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

	Gtk::Box* content;
	builder->get_widget("contentBox", content);
	add(*content);

	//this needs to be made dynamically;
	layout.emplace_back();
	builder->get_widget("layout", layout.back());

	//window
	set_title("Dungeon Cartographer");
	maximize();
	show_all_children();
}

/*!
  \bug change to get tab from notebook; remove currTab var;
**/
int UI::getCurrTab() const
	{return currTab;}

/*!
  \bug change to get zoomSlider value; remove zoomSpeed var;
**/
int UI::getZoomSpeed() const
	{return zoomSpeed;}


void UI::addLevel(Level* level)
{
	//create layout
	auto newLayout = Gtk::Layout();
	layout.emplace_back(&newLayout);
	layout.back()->set_hexpand(true);
	layout.back()->set_vexpand(true);
	layout.back()->show();

	//add level to layout
	layout.back()->add(*level);
	level->show();

	//add layout to notebook
	Gtk::Notebook* notebook;
	builder->get_widget("notebook", notebook);
	notebook->append_page(*layout.back());
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