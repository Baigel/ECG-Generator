/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_Transmit_Settings.hpp"

sigc::signal<void(int)> m_signalDataTransmitted; // TODO: properly document why this is global

/*
 * Description: Transmit GUI constructor
*/
GUI_Transmit_Settings::GUI_Transmit_Settings() :
    Gtk::Box(Gtk::Orientation::VERTICAL, 10),
    m_HBox(Gtk::Orientation::HORIZONTAL, 10),
    m_socketBox(Gtk::Orientation::HORIZONTAL, 10),
    m_buttonStart("Start"), m_buttonStop("Stop") {

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Transmit settings GUI setup\n";

    append(*Gtk::make_managed<Gtk::Label>("Transmit Settings", 10));
    append(m_HBox);
        m_HBox.append(m_buttonStart);
        m_HBox.append(m_buttonStop);

    append(m_socketBox);
        m_socketBox.append(*Gtk::make_managed<Gtk::Label>("Socket:", 0));
        m_socketBox.append(socketEntry);
    append(socketAddress);

    socketEntry.set_text("12345");
    m_setSocket();
    
    socketEntry.signal_changed().connect(sigc::mem_fun(*this, &GUI_Transmit_Settings::m_setSocket));

    // Connect button signals
    m_buttonStart.signal_clicked().connect(sigc::mem_fun(*this, &GUI_Transmit_Settings::m_onStartClicked));
    m_buttonStop.signal_clicked().connect(sigc::mem_fun(*this, &GUI_Transmit_Settings::m_onStopClicked));
}

/* GUI Setup */


/* Signal Handling Functions */

/*
 * Description: Handle start button pressed
 * Args: None
 * Return: None
*/
void GUI_Transmit_Settings::m_onStartClicked(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handle Start clicked\n";

    SIGGEN.m_beginTransmission();
}

void transmitDataCallback(int data) {
    m_signalDataTransmitted.emit(data);
}

/*
 * Description: Handle stop button pressed
 * Args: None
 * Return: None
*/
void GUI_Transmit_Settings::m_onStopClicked(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handle Stop clicked\n";

    SIGGEN.m_endTransmission();
}

/*
 * Description: Handle socket text change
 * Args: None
 * Return: None
*/
void GUI_Transmit_Settings::m_setSocket(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handle setting socket\n";

    // Get entry as a int
    int val;
    std::istringstream ss("0" + socketEntry.get_text());
    ss >> val;
    SIGGEN.m_setSocket(val);

    // Set on screen text
    socketAddress.set_text("Address: 127.0.0.1:" + socketEntry.get_text());
}
