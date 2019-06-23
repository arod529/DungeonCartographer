#include "ui.h"

/*!

**/
UI::UI(Settings& settings)
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
  \bug change to get tab from notebook; remove currTab var;
**/
int UI::getCurrTab() const
	{return notebook->get_current_page();}

/*!
  \bug change to get zoomSlider value; remove zoomSpeed var;
**/
int UI::getZoomSpeed() const
	{return zoomSpeed;}
	