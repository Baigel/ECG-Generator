/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>
#include <stdio.h>
#include <sstream>

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Lib/BSlogger.hpp"

class GUI_Noise : public Gtk::Box {
 public:
    GUI_Noise();

 private:
    bool m_syncingNoiseScale = false;

    // GUI Elements
    Gtk::Box m_sliderBox;
    Gtk::Switch m_onOffSwitch;
    Gtk::Scale m_noiseMagnitudeScale{Gtk::Orientation::HORIZONTAL};
    Glib::RefPtr<Gtk::Adjustment> m_noiseMagnitudeAdjustment;
    Gtk::Entry m_noiseNumText;

    // Signal Handlers
    bool m_onSwitchChanged(bool state);
    void m_onSliderChanged(void);
    void m_onTextChanged(void);
};
