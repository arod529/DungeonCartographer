#ifndef UI_H
#define UI_H

#include "event.h"
#include "settings.h"
#include "level.h"
#include "map.h"

#include <glibmm/refptr.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/scale.h>
#include <gtkmm/notebook.h>
#include <gtkmm/layout.h>
#include <gtkmm/grid.h>


#include <vector>

class UI
{
	public:
		Gtk::Window* window;

		UI(Settings*, Map*);
		~UI();

		void drawLevel(int i);

	private:
		//------------------------------
		//----- Program Properties -----
		//------------------------------
		Settings* settings;
		Map* map;


		//--------------------------
		//----- Gtk Properties -----
		//--------------------------
		//utils
		Glib::RefPtr<Gtk::Builder> builder;
		Glib::RefPtr<Gtk::CssProvider> css;

		//UI


			//toolbar
			Gtk::ToolButton* save;
			Gtk::ToolButton* zoom[2];
			Gtk::ToolButton* zoomFit;
			Gtk::Scale* zoomSlider;
			Gtk::Scale* scrollSlider;

			//notebook (tabbed window display)
			Gtk::Notebook* notebook;

				//layout (infinite scroll area)
				std::vector<Gtk::Layout*> layout;
					//grid widget
					std::vector<Gtk::Grid*> grid;

    // std::vector<GObject*> tabContent;
    // std::vector<GObject*> tabLabel;
    // std::vector<GObject*> tabButton;
};

#endif
