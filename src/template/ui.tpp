#ifndef UI_TPP
#define UI_TPP

template <class T>
void UI::getWidget(std::string name, T*& widget)
{
  builder->get_widget(name, widget);
}

#endif
