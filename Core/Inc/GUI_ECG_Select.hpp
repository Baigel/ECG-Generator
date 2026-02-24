/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>
#include <string>
#include <vector>
#include <numeric>

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Lib/filter.h"
#include "./Core/Lib/BSlogger.hpp"

class GUI_ECG_Select : public Gtk::Box {
 public:
    GUI_ECG_Select();

 private:
    // GUI Elements
    Gtk::Box m_folderDropdownBox, m_ECGDropdownBox, m_leadDropdownBox;
    Gtk::DropDown m_dropdownPositionFolder;
    Gtk::DropDown m_dropdownPositionECG;
    Gtk::DropDown m_dropdownPositionLead;

    // Dropdown String lists
    Glib::RefPtr<Gtk::StringList> m_folderStringList =
            Gtk::StringList::create({});
    Glib::RefPtr<Gtk::StringList> m_ECGStringList =
            Gtk::StringList::create({});
    Glib::RefPtr<Gtk::StringList> m_leadStringList =
            Gtk::StringList::create({});

    // Dropdown connections
    sigc::connection m_connectionFolder;
    sigc::connection m_connectionECG;
    sigc::connection m_connectionLead;

    // GUI Setup
    void m_ECGSelectGUISetup(void);

    // Signal handlers
    bool m_onSwitchChanged(bool state);
    void m_onFolderDropdown(void);
    void m_onECGDropdown(void);
    void m_onDropdownPosition();
};
