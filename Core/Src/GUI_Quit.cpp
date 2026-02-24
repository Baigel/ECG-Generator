/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_Quit.hpp"

/*
 * Description: Quit button constructor
*/
GUI_Quit::GUI_Quit() :
    Gtk::Box(Gtk::Orientation::HORIZONTAL, 10)
{
    append(*Gtk::make_managed<Gtk::Label>("---", 10));
}

