/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>
#include <stdio.h>
#include <sstream>

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Lib/BSlogger.hpp"

class GUI_Filter : public Gtk::Box {
 public:
    GUI_Filter();

 private:
    bool m_SyncingLPScale = false;
    bool m_SyncingHPScale = false;

    // GUI Elements
    Gtk::Box m_LPBox;
    Gtk::Box m_HPBox;
    Gtk::Switch m_onOffSwitch;
    Gtk::Scale m_LPFilterScale{Gtk::Orientation::HORIZONTAL};
    Gtk::Scale m_HPFilterScale{Gtk::Orientation::HORIZONTAL};
    Glib::RefPtr<Gtk::Adjustment> m_LPFilterAdjustment;
    Glib::RefPtr<Gtk::Adjustment> m_HPFilterAdjustment;
    Gtk::Entry m_LPNumText;
    Gtk::Entry m_HPNumText;

    // Signal Handlers
    bool m_onSwitchChanged(bool state);
    void m_onLPScaleChanged(void);
    void m_onLPTextChanged(void);
    void m_onHPScaleChanged(void);
    void m_onHPTextChanged(void);
};
