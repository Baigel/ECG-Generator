/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Lib/BSlogger.hpp"

class GUI_Sample_Frequency : public Gtk::Box {
 public:
    GUI_Sample_Frequency();

 private:
    // GUI Elements
    Gtk::DropDown m_dropdownFreq;

    // Stringlists
    Glib::RefPtr<Gtk::StringList> m_freqList = Gtk::StringList::create({});

    // GUI Setup
    void m_ECGFreqSelectSetup(void);

    // Signal Handlers
    void m_onFreqSelect(void);
};
