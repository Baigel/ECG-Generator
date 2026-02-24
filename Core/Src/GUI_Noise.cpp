/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_Noise.hpp"

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
 * Description: Noise GUI Constructor
 * Args: None
 * Return: None
*/
GUI_Noise::GUI_Noise() :
    Gtk::Box(Gtk::Orientation::VERTICAL, 10),
    m_sliderBox(Gtk::Orientation::HORIZONTAL, 10) {

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Noise GUI setup\n";

    m_noiseMagnitudeAdjustment = Gtk::Adjustment::create(1.0, 1.0, 100.0, 1, 10.0, 0.0);
    m_noiseMagnitudeScale.set_adjustment(m_noiseMagnitudeAdjustment);
    m_noiseMagnitudeScale.set_digits(0);  // No decimals
    m_noiseMagnitudeScale.set_hexpand(true);
    m_noiseMagnitudeScale.signal_value_changed().connect(sigc::mem_fun(*this, &GUI_Noise::m_onSliderChanged));
    m_noiseNumText.signal_changed().connect(sigc::mem_fun(*this, &GUI_Noise::m_onTextChanged));

    append(*Gtk::make_managed<Gtk::Label>("Noise", 10));

    // Switch
    m_onOffSwitch.set_hexpand(false);
    m_onOffSwitch.set_halign(Gtk::Align::START);  // Or CENTER/END
    append(m_onOffSwitch);
    m_onOffSwitch.signal_state_set().connect(sigc::mem_fun(*this, &GUI_Noise::m_onSwitchChanged), false);

    // Noise Slider:
    append(m_sliderBox);
        m_sliderBox.append(*Gtk::make_managed<Gtk::Label>("Noise Magnitude:", 0));
        m_sliderBox.append(m_noiseNumText);
    append(m_noiseMagnitudeScale);

    m_onSliderChanged();  // Initialize label with starting value
}

/* GUI Setup */

/* Signal Handling Functions */

/*
 * Description: Handle switch for noise
 * Args: not sure why this is needed  // TODO
 * Return: not sure why there is any return needed here  // TODO
*/
bool GUI_Noise::m_onSwitchChanged(bool state) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "handle on/off switch\n";

    SIGGEN.m_noiseOn = m_onOffSwitch.get_active();
    SIGGEN.m_generateNoise();
    return false;
}

/*
 * Description: Handle slider changed for noise level
 * Args: None
 * Return: None
*/
void GUI_Noise::m_onSliderChanged(void) {
    if (m_syncingNoiseScale) return;

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handle slider change\n";

    m_syncingNoiseScale = true;
    double value = static_cast<float>(m_noiseMagnitudeScale.get_value());
    m_noiseNumText.set_text(to_string_with_precision(value, 0));
    m_syncingNoiseScale = false;

    // Update Generator
    SIGGEN.m_setNoiseMagnitude(value);
}

/*
 * Description: Handle noise value text changed
 * Args: None
 * Return: None
*/
void GUI_Noise::m_onTextChanged(void) { // TODO: still need to sanitize input and ensure that digit only characters are used
    if (m_syncingNoiseScale) return;

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Handle text change\n";

    m_syncingNoiseScale = true;
    // Convert str from Gtk::Entry to int
    int value = 0;
    std::istringstream ss("0" + m_noiseNumText.get_text());
    ss >> value;
    // Check bounds and then update slider
    if (value >= m_noiseMagnitudeAdjustment->get_lower() && value <= m_noiseMagnitudeAdjustment->get_upper()) {
        m_noiseMagnitudeScale.set_value(value);
    }
    m_syncingNoiseScale = false;

    // Update Generator
    SIGGEN.m_setNoiseMagnitude(value);
}
