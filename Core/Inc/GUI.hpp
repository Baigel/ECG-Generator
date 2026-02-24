/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>
#include <array>
#include <iostream>

// Include GUI Elements
#include "./Core/Inc/GUI_Signal_View.hpp"
#include "./Core/Inc/GUI_Transmit_Settings.hpp"
#include "./Core/Inc/GUI_ECG_Select.hpp"
#include "./Core/Inc/GUI_Sample_Frequency.hpp"
#include "./Core/Inc/GUI_Noise.hpp"
#include "./Core/Inc/GUI_Filter.hpp"
#include "./Core/Inc/GUI_Quit.hpp"
#include "./Core/Lib/BSlogger.hpp"

class Window : public Gtk::Window {
 public:
    Window();
    ~Window() override;

 private:
    // Box Defintions
    Gtk::Box m_HBoxTop, m_VBoxSettings, m_boxTransmit, m_boxECGSelect,
            m_boxSampleFreq, m_boxFormat, m_boxNoise, m_boxFilter,
            m_boxQuit, m_boxSignalView;

    // Define some separators
    Gtk::Separator m_separatorSettings, m_separatorTransmit,
            m_separatorECGSelect, m_separatorSampleFreq, m_separatorFormat,
            m_separatorNoise, m_separatorFilter;

    // Signal View Widgets
    GUI_Signal_View m_signalView;

    // Transmit Settings Widgets
    GUI_Transmit_Settings m_transmitSettings;

    // ECG Select GUI
    GUI_ECG_Select m_ECGSelect;

    // Sampling Frequency Widgets
    GUI_Sample_Frequency m_sampleFrequency;

    // Noise Settings Widgets
    GUI_Noise m_noise;

    // Filter Settings Widgets
    GUI_Filter m_filter;

    // Quit Settings
    void m_onButtonQuit();
    Gtk::Button m_buttonQuit;
};
