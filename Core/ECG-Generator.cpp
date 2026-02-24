/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include <gtkmm/application.h>

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Inc/GUI.hpp"

int main(int argc, char *argv[]) {
  LOG_INIT_CERR();
  auto app = Gtk::Application::create("org.gtkmm.example");

  // Shows the window and returns when it is closed.
  return app->make_window_and_run<Window>(argc, argv);
}
