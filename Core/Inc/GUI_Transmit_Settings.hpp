/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>
#include <cstdio>

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Inc/GUI_Signal_View.hpp"
#include "./Core/Lib/BSlogger.hpp"

class GUI_Transmit_Settings : public Gtk::Box {
 public:
    GUI_Transmit_Settings();

 private:
    // GUI Elements
    Gtk::Box m_HBox;
    Gtk::Box m_socketBox;
    Gtk::Button m_buttonStart, m_buttonStop;
    Gtk::Entry socketEntry;
    Gtk::Label socketAddress;

    // Signal handling functions
    void m_onStartClicked();
    void m_onStopClicked();
    void m_setSocket(void);

    sigc::signal<void(int)> m_signalSendData;
    sigc::connection m_connectionSendData;
};

extern sigc::signal<void(int)> m_signalDataTransmitted;
