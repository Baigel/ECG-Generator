/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_Sample_Frequency.hpp"

namespace {

struct PositionTypeStruct {
    Gtk::PositionType position;
    Glib::ustring text;
};

const std::array<Glib::ustring, 7> freqSelection = {
    Glib::ustring{"50"},
    Glib::ustring{"100"},
    Glib::ustring{"125"},
    Glib::ustring{"200"},
    Glib::ustring{"250"},
    Glib::ustring{"500"},
    Glib::ustring{"1000"}
};

}  // namespace

/*
 * Description: Sample frequency selection Constructor
*/
GUI_Sample_Frequency::GUI_Sample_Frequency() :
    Gtk::Box(Gtk::Orientation::VERTICAL, 10) {
    append(*Gtk::make_managed<Gtk::Label>("Sampling Frequency", 10));
    m_ECGFreqSelectSetup();
}

/* GUI Setup */

/*
 * Description: Setup of ECG Freq Selection GUI Box
 * Args: None
 * Return: None
*/
void GUI_Sample_Frequency::m_ECGFreqSelectSetup(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Setting Up Frequency Select GUI\n";

    m_dropdownFreq.set_model(m_freqList);
    // Fill the DropDown's list model:
    for (const auto& freq : freqSelection) {
        m_freqList->append(freq);
    }
    append(m_dropdownFreq);
    m_dropdownFreq.property_selected().signal_changed().connect(
        sigc::mem_fun(*this, &GUI_Sample_Frequency::m_onFreqSelect));
    m_dropdownFreq.set_selected(0);  // Top
    m_dropdownFreq.set_expand(false);

    m_onFreqSelect();
}

/* Signal Handling Functions */

/*
 * Description: Handle frequency dropdown selection
 * Args: None
 * Return: None
*/
void GUI_Sample_Frequency::m_onFreqSelect(void) {
    LOG_INIT_CERR();
    const auto selected = m_dropdownFreq.get_selected();
    if (selected == GTK_INVALID_LIST_POSITION) {
        _log(LOG_ERR) << "Invalid List Position\n";
        return;
    }

    int index = m_dropdownFreq.get_selected();

    int value = 0;
    std::istringstream ss("0" + m_freqList->get_string(index).raw());
    ss >> value;

    _log(LOG_INFO) << "Selected = " << value << "\n";

    SIGGEN.m_setSampleFrequency(value);
}
