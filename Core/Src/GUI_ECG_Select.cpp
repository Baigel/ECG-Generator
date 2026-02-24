/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_ECG_Select.hpp"

std::vector<Glib::ustring> Folder_List = { Glib::ustring{""} };

std::vector<Glib::ustring> ECG_List = { Glib::ustring{""} };

std::vector<Glib::ustring> Lead_List = { Glib::ustring{""} };

namespace fs = std::filesystem;

/*
 * Description: ECG Selector Constructor
*/
GUI_ECG_Select::GUI_ECG_Select() :
    Gtk::Box(Gtk::Orientation::VERTICAL, 10),
    m_folderDropdownBox(Gtk::Orientation::HORIZONTAL, 10),
    m_ECGDropdownBox(Gtk::Orientation::HORIZONTAL, 10),
    m_leadDropdownBox(Gtk::Orientation::HORIZONTAL, 10) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Setting Up ECG Select GUI\n";

    append(*Gtk::make_managed<Gtk::Label>("ECG Select", 10));
    append(m_folderDropdownBox);
    append(m_ECGDropdownBox);
    append(m_leadDropdownBox);

    m_folderDropdownBox.append(*Gtk::make_managed<Gtk::Label>("ECG Categories:"
            , 0));
    m_folderDropdownBox.append(m_dropdownPositionFolder);
    m_ECGDropdownBox.append(*Gtk::make_managed<Gtk::Label>("ECG List:", 0));
    m_ECGDropdownBox.append(m_dropdownPositionECG);
    m_leadDropdownBox.append(*Gtk::make_managed<Gtk::Label>("Lead List:", 0));
    m_leadDropdownBox.append(m_dropdownPositionLead);

    m_ECGSelectGUISetup();
}


/*
 * Description: Setup of ECG Signal Selector GUI Box
 * Args: None
 * Return: None
*/
void GUI_ECG_Select::m_ECGSelectGUISetup(void) {
    // Clear all
    Folder_List.clear();
    while (m_folderStringList->get_n_items() > 0) {
        m_folderStringList->remove(0);
    }
    // Get folder names from database
    std::string path = "./ECG_Database";
    for (const auto& entry : fs::directory_iterator(path)) {
        Folder_List.push_back(entry.path().string());
    }
    // Create the StringList
    m_dropdownPositionFolder.set_model(m_folderStringList);
    // Fill the DropDown's list model
    m_folderStringList->append("");
    for (const auto& Folder : Folder_List) {
        // Remove first 15 charswhich removes the './ECG_Database/'
        m_folderStringList->append(Folder.substr(15));
    }
    m_connectionFolder.disconnect();
    m_connectionFolder = m_dropdownPositionFolder.property_selected()
            .signal_changed().connect(sigc::mem_fun(*this,
            &GUI_ECG_Select::m_onFolderDropdown));

    m_dropdownPositionECG.set_selected(0);
    m_dropdownPositionLead.set_selected(0);
    Lead_List.clear();
    while (m_leadStringList->get_n_items() > 0) {
        m_leadStringList->remove(0);
    }
}

/* Signal Handling Functions */

/*
 * Description: Handle catagory folder drop down
 * Args: None
 * Return: None
*/
void GUI_ECG_Select::m_onFolderDropdown(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Categories dropdown handler\n";

    int index = m_dropdownPositionFolder.get_selected();
    if (index == GTK_INVALID_LIST_POSITION) {
        _log(LOG_ERR) << "Invalid position\n";
        return;
    }

    if (m_folderStringList->get_string(index).raw().length() < 1) {
        _log(LOG_ERR) << "Too short\n";
    }

    if (index >= 0) {  // Make this 1 instead of 0 because first entry is blank
        // Clear all
        ECG_List.clear();
        while (m_ECGStringList->get_n_items() > 0) {
            m_ECGStringList->remove(0);
        }
        // Get folder names from database
        std::string path = "./ECG_Database/" + m_folderStringList->get_string(
                index).raw();
        _log(LOG_INFO) << "Categories Path: " << path << "\n";
        for (const auto& entry : fs::directory_iterator(path)) {
            ECG_List.push_back(entry.path().string());
        }
        // Create the StringList:
        m_dropdownPositionECG.set_model(m_ECGStringList);
        // Fill the DropDown's list model
        m_ECGStringList->append("");
        for (const auto& ECG : ECG_List) {
            // Remove first n chars which removes the './ECG_Database/xx,xxx,xx'
            m_ECGStringList->append(ECG.substr(Folder_List[index-1]
                    .length() + 1));
        }

        m_connectionECG.disconnect();
        m_connectionECG = m_dropdownPositionECG.property_selected()
                .signal_changed().connect(sigc::mem_fun(*this,
                &GUI_ECG_Select::m_onECGDropdown));

        m_dropdownPositionECG.set_expand(false);
        m_dropdownPositionECG.set_hexpand(false);

        m_dropdownPositionECG.set_selected(0);
        m_dropdownPositionLead.set_selected(0);
    }
}

/*
 * Description: Handle ecg selection dropdown
 * Args: None
 * Return: None
*/
void GUI_ECG_Select::m_onECGDropdown(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "ECG dropdown handler\n";
    _log(LOG_INFO) << "Ecg list length: " << ECG_List.size() << "\n";

    // if ECG list have not yet been defined, exit
    if (ECG_List.size() == 0) {
        while (m_leadStringList->get_n_items() > 0) {
            m_leadStringList->remove(0);
        }
        while (m_ECGStringList->get_n_items() > 0) {
            m_ECGStringList->remove(0);
        }
        m_dropdownPositionLead.set_model(m_leadStringList);
        m_dropdownPositionECG.set_model(m_ECGStringList);
        return;
    }

    int index = m_dropdownPositionECG.get_selected();
    if (index == GTK_INVALID_LIST_POSITION) {
        _log(LOG_ERR) << "Invalid position.\n";
        return;
    }

    if (m_ECGStringList->get_string(index).raw().length() < 1) {
        // We have re-selected the "" (blank) entry in the list -> ignore
        _log(LOG_INFO) << "Selected blank entry, ignoring.\n";
        return;
    }

    if (index >= 0) {
        // Clear all
        Lead_List.clear();
        while (m_leadStringList->get_n_items() > 0) {
            m_leadStringList->remove(0);
        }

        // Get folder names from database
        // - 1 as the first element of "" was manually added
        std::string path = ECG_List[index - 1].raw();
        _log(LOG_INFO) << "ECG Path: " << path << "\n";
        for (const auto& entry : fs::directory_iterator(path)) {
            Lead_List.push_back(entry.path().string());
        }
        // Create the StringList
        m_dropdownPositionLead.set_model(m_leadStringList);
        // Fill the DropDown's list model
        m_leadStringList->append("");
        for (const auto& Lead : Lead_List) {
            m_leadStringList->append(Lead.substr(0, Lead.size() - 4).substr(
                    ECG_List[index - 1].length() + 1));  // Remove extra chars
        }
        m_connectionLead.disconnect();
        m_connectionLead = m_dropdownPositionLead.property_selected()
                .signal_changed().connect(sigc::mem_fun(*this,
                &GUI_ECG_Select::m_onDropdownPosition));

        m_dropdownPositionLead.set_expand(false);
        m_dropdownPositionLead.set_hexpand(false);
    }
}

/*
 * Description: Handle lead dropdown
 * Args: None
 * Return: None
*/
void GUI_ECG_Select::m_onDropdownPosition(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Leads dropdown handler\n";
    _log(LOG_INFO) << "Lead list length: " << Lead_List.size() << "\n";

    // if leads have not yet been defined, exit
    if (Lead_List.size() == 0) {
        return;
    }

    const auto selected = m_dropdownPositionLead.get_selected();
    if (selected == GTK_INVALID_LIST_POSITION) {
        _log(LOG_ERR) << "Invalid position" << ECG_List.size() << "\n";
        return;
    }

    int index = m_dropdownPositionLead.get_selected();
    if (index >= 0) {
        std::filesystem::path filePath = Lead_List[index-1].raw();  // - 1 as we
                // need to ignore the first blank entry in Lead_List (i.e. the
                // "" that was manually added)

        SIGGEN.m_loadDataFromFile(filePath);
    }
}
