#include "ui.h"

#include "event.h"

#include <gtkmm/cssprovider.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>

/*!

**/
UI::UI()
{
	//css style
	auto css = Gtk::CssProvider::create();
	css->load_from_path(cssFile);
	auto styleContext = get_style_context();
	styleContext->add_provider_for_screen(get_screen(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	//gtk ui builder object
	auto builder = Gtk::Builder::create_from_file(uiFile);

	Gtk::Box* content;
	builder->get_widget("contentBox", content);
	add(*content);

	builder->get_widget("layout", layout);

	//window
	set_title("Dungeon Cartographer");
	maximize();
	show_all_children();
}

void UI::addLevel(Level* level)
{
	layout->add(*level);
	level->show();
}
