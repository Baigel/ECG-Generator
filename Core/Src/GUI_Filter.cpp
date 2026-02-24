/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_Filter.hpp"

// Reference: https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
template <typename T>
std::string to_string_with_precision(const T a_value, int n)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

/*
 * Description: Filter Constructor
*/
GUI_Filter::GUI_Filter() :
    Gtk::Box(Gtk::Orientation::VERTICAL, 10),
    m_LPBox(Gtk::Orientation::HORIZONTAL, 10),
    m_HPBox(Gtk::Orientation::HORIZONTAL, 10) {

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Setting Up Filter GUI\n";

    m_LPFilterAdjustment = Gtk::Adjustment::create(100, 0.1, 100.0, 0.1, 10.0,
            0.0);
    m_LPFilterScale.set_adjustment(m_LPFilterAdjustment);
    m_LPFilterScale.set_digits(0);  // No decimals
    m_LPFilterScale.set_hexpand(true);
    m_LPFilterScale.signal_value_changed().connect(sigc::mem_fun(*this,
            &GUI_Filter::m_onLPScaleChanged));
    m_LPNumText.signal_changed().connect(sigc::mem_fun(*this,
            &GUI_Filter::m_onLPTextChanged));

    m_HPFilterAdjustment = Gtk::Adjustment::create(0, 0.1, 100.0, 0.1, 10.0,
            0.0);
    m_HPFilterScale.set_adjustment(m_HPFilterAdjustment);
    m_HPFilterScale.set_digits(0);  // No decimals
    m_HPFilterScale.set_hexpand(false);
    m_HPFilterScale.signal_value_changed().connect(sigc::mem_fun(*this,
            &GUI_Filter::m_onHPScaleChanged));
    m_HPNumText.signal_changed().connect(sigc::mem_fun(*this,
            &GUI_Filter::m_onHPTextChanged));

    append(*Gtk::make_managed<Gtk::Label>("Filter", 10));

    // Switch
    m_onOffSwitch.set_hexpand(false);
    m_onOffSwitch.set_halign(Gtk::Align::START);  // Or CENTER/END
    append(m_onOffSwitch);
    m_onOffSwitch.signal_state_set().connect(sigc::mem_fun(*this,
            &GUI_Filter::m_onSwitchChanged), false);

    // LP Slider:
    append(m_LPBox);
        m_LPBox.append(*Gtk::make_managed<Gtk::Label>("Low Pass (f):", 0));
        m_LPBox.append(m_LPNumText);
    append(m_LPFilterScale);
    append(m_HPBox);
        m_HPBox.append(*Gtk::make_managed<Gtk::Label>("High Pass (f):", 0));
        m_HPBox.append(m_HPNumText);
    append(m_HPFilterScale);

    m_onLPScaleChanged();  // Initialize label with starting value
    m_onHPScaleChanged();
}

/* GUI Setup */


/* Signal Handling Functions */

/*
 * Description: Handle switch
 * Args: not sure why this is needed  // TODO
 * Return: not sure why there is any return needed here  // TODO
*/
bool GUI_Filter::m_onSwitchChanged(bool state) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handling on/off switch change\n";

    SIGGEN.m_filterOn = m_onOffSwitch.get_active();
    m_onHPScaleChanged();
    m_onLPScaleChanged();
    return false;
}

/*
 * Description: Handle scale moved for LP filter
 * Args: None
 * Return: None
*/
void GUI_Filter::m_onLPScaleChanged(void) {
    if (m_SyncingLPScale) return;

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handling LP Scale changed\n";

    m_SyncingLPScale = true;
    double value = static_cast<float>(m_LPFilterScale.get_value());
    m_LPNumText.set_text(to_string_with_precision(value, 1));
    m_SyncingLPScale = false;

    // Update SigGen var
    SIGGEN.m_setFilterLow(value);
}

/*
 * Description: Handle text changed for LP filter
 * Args: None
 * Return: None
*/
void GUI_Filter::m_onLPTextChanged(void) { // TODO: still need to sanitize input and ensure that digit only characters are used
    if (m_SyncingLPScale) return;

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handling LP text changed\n";

    m_SyncingLPScale = true;
    // Convert str from Gtk::Entry to int
    int value = 0;
    std::istringstream ss("0" + m_LPNumText.get_text());
    ss >> value;
    // Check bounds and then update slider
    if (value >= m_LPFilterAdjustment->get_lower() && value <=
            m_LPFilterAdjustment->get_upper()) {
        m_LPFilterScale.set_value(value);
    }
    m_SyncingLPScale = false;

    // Update SigGen var
    SIGGEN.m_setFilterLow(value);
}

/*
 * Description: Handle scale moved for HP filter
 * Args: None
 * Return: None
*/
void GUI_Filter::m_onHPScaleChanged(void) {
    if (m_SyncingHPScale) return;

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handling HP scale changed\n";

    m_SyncingHPScale = true;
    double value = static_cast<float>(m_HPFilterScale.get_value());
    m_HPNumText.set_text(to_string_with_precision(value, 1));
    m_SyncingHPScale = false;

    // Update SigGen var
    SIGGEN.m_setFilterHigh(value);
}

/*
 * Description: Handle text changed for HP filter
 * Args: None
 * Return: None
*/
void GUI_Filter::m_onHPTextChanged(void) { // TODO: still need to sanitize input and ensure that digit only characters are used
    if (m_SyncingHPScale) return;

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handling HP text changed\n";

    m_SyncingHPScale = true;
    // Convert str from Gtk::Entry to int
    int value = 0;
    std::istringstream ss("0" + m_HPNumText.get_text());
    ss >> value;
    // Check bounds and then update slider
    if (value >= m_HPFilterAdjustment->get_lower() && value <=
            m_HPFilterAdjustment->get_upper()) {
        m_HPFilterScale.set_value(value);
    }
    m_SyncingHPScale = false;

    // Update SigGen var
    SIGGEN.m_setFilterHigh(value);
}

