/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI.hpp"

/* Constructor and Destructor Setup */

/*
 * Description: Constructor, define box orientations and setup heirachy of boxes
 * Args: None
 * Return: None
*/
Window::Window() :
        m_HBoxTop(Gtk::Orientation::HORIZONTAL, 10),
        m_VBoxSettings(Gtk::Orientation::VERTICAL, 10),
        m_boxTransmit(Gtk::Orientation::HORIZONTAL, 10),
        m_boxECGSelect(Gtk::Orientation::HORIZONTAL, 10),
        m_boxSampleFreq(Gtk::Orientation::HORIZONTAL, 10),
        m_boxNoise(Gtk::Orientation::HORIZONTAL, 10),
        m_boxFilter(Gtk::Orientation::HORIZONTAL, 10),
        m_boxQuit(Gtk::Orientation::HORIZONTAL, 10),
        m_boxSignalView(Gtk::Orientation::HORIZONTAL, 10),
        m_buttonQuit("Quit") {
    // Window Setup
    set_title("ECG Generator");
    set_default_size(800, 800);
    set_child(m_HBoxTop);


    /*
    Heirachy for Layout
    m_HBoxTop
        m_VBoxSettings
            m_boxTransmit
            m_boxECGSelect
            m_boxSampleFreq
            m_boxNoise
            m_boxFilter
            m_Box_HR?
            m_Box_Lead?
            m_boxQuit
        m_Separator
        m_Box_Display
    */
    // Define Box Heirachy
    m_HBoxTop.append(m_VBoxSettings);
    m_HBoxTop.append(m_separatorSettings);
        m_VBoxSettings.append(m_boxTransmit);
        m_boxTransmit.set_margin(5);
        m_VBoxSettings.append(m_separatorTransmit);
        m_VBoxSettings.append(m_boxECGSelect);
        m_boxECGSelect.set_margin(5);
        m_VBoxSettings.append(m_separatorECGSelect);
        m_VBoxSettings.append(m_boxSampleFreq);
        m_boxSampleFreq.set_margin(5);
        m_VBoxSettings.append(m_separatorSampleFreq);
        m_VBoxSettings.append(m_boxNoise);
        m_boxNoise.set_margin(5);
        m_VBoxSettings.append(m_separatorNoise);
        m_VBoxSettings.append(m_boxFilter);
        m_boxFilter.set_margin(5);
        m_VBoxSettings.append(m_separatorFilter);
        m_VBoxSettings.append(m_boxQuit);
        m_boxQuit.set_margin(5);
    m_HBoxTop.append(m_boxSignalView);

    // Set Box Parameters

    // Signal View
    m_boxSignalView.append(m_signalView);

    // Transmit Settings
    m_boxTransmit.append(m_transmitSettings);

    // ECG Select
    m_boxECGSelect.append(m_ECGSelect);

    // Sampling Frequency
    m_boxSampleFreq.append(m_sampleFrequency);

    // Noise Settings
    m_boxNoise.append(m_noise);

    // Filter Settings
    m_boxFilter.append(m_filter);

    // Quit Box
    m_boxQuit.append(m_buttonQuit);
    m_buttonQuit.signal_clicked().connect(sigc::mem_fun(*this,
        &Window::m_onButtonQuit));
    m_buttonQuit.set_margin(10);
    set_default_widget(m_boxQuit);

    // Connect transmit data signal to redraw plot
    m_signalDataTransmitted.connect(sigc::mem_fun(m_signalView,
                                    &GUI_Signal_View::m_redrawPlot));
}

/*
 * Description: Destructor
 * Args: None
 * Return: None
*/
Window::~Window() {
}

/* Signal Handling Functions */

/*
 * Description: Handle quit button // TODO: this be in the GUI_Quit.cpp file?
 * Args: None
 * Return: None
*/
void Window::m_onButtonQuit() {
    SIGGEN.m_endTransmission();
    set_visible(false);
}
