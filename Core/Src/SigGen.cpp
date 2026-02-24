/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/SigGen.hpp"

/* Class Function Definitions */

/*
 * Description: Constructor
 * Args: None
 * Return: None
*/
SigGen::SigGen() {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Initiating Signal Generator Engine\n";

    m_socketOpen = false;

    // Create filters
    m_LPFilterScale = create_bw_low_pass_filter(2, 250, 100); // Cutoff f here is fairly arbitrary as will almost immediately be written over when filter is turned on and adjusted
    m_HPFilterScale = create_bw_high_pass_filter(2, 250, 1);
}

/*
 * Description: Destructor
 * Args: None
 * Return: None
*/
SigGen::~SigGen() {
    // cleanup filters   
    free_bw_high_pass(m_HPFilterScale);
    free_bw_low_pass(m_LPFilterScale);
}

/* ##########  Network/Transmit Funtions  ########## */

/*
 * Description: Setup transmission and start thread that is called every 1 ms
 * Args: None
 * Return: None
*/
void SigGen::m_beginTransmission(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Opening socket\n";

    // Exit if socket is already open
    if (m_socketOpen) {
        _log(LOG_INFO) << "Socket already open, doing nothing.\n";
        return;
    }

    // Create a UDP socket (IPv4, datagram, UDP protocol)
    m_streamSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_streamSocket < 0) {
        _log(LOG_ERR) << "Unable to open socket\n";
        return;
    }

    m_socketOpen = true;

    // Set up destination address structure
    m_streamAddr.sin_family = AF_INET; // IPv4
    m_streamAddr.sin_port = htons(m_socketNum); // Destination port, converted to network byte order
    inet_pton(AF_INET, "127.0.0.1", &m_streamAddr.sin_addr); // Convert IP string to binary form

    // Start a thread
    std::thread t1(&SigGen::m_thread, this);
    t1.detach();
}

/*
 * Description: Send signal to close thread and close socket
 * Args: None
 * Return: None
*/
void SigGen::m_endTransmission(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Send signal to close socket\n";

    m_socketOpen = false;
    // Note that closing of the circuit is handled by the transmit thread, once it has finished transmitting
}

/*
 * Description: Calls the transmitData function every 1 ms
 * Args: None
 * Return: None
*/
bool SigGen::m_thread(void) {
    LOG_INIT_CERR();

    std::chrono::steady_clock::time_point next = std::chrono::steady_clock::now();
    while (m_socketOpen) {
        next += std::chrono::milliseconds(1);
        m_transmitData();
        std::this_thread::sleep_until(next);
    }

    // If the m_socketOpen var has gone false, exit and close the port
    _log(LOG_INFO) << "Closing socket.\n";    
    close(m_streamSocket);

    _log(LOG_INFO) << "Exiting transmit thread...\n";

    return true;
}

/*
 * Description: Delays for the appropriate number of ms and then sends data
 * Args: None
 * Return: None
*/
void SigGen::m_transmitData(void) {
    // Increment Data
    m_incrementData();

    // Delay for the number of ms in order to get correct timing
    static int freqDelay = 0;
    freqDelay++;
    if (freqDelay % m_msDelay) { return; }
    
    int data = m_getNextData();

    // Update graph view of data
    transmitDataCallback(data);

    // Write data to port
    // flawfinder:ignore // can safely do this as we are just storing an int
    char msg[12];
    snprintf(msg, sizeof(msg), "%d", data);
    sendto(m_streamSocket, msg, strlen(msg), 0, (sockaddr*)&m_streamAddr, sizeof(m_streamAddr));
}

//__weak void transmitDataCallback(int data) { }  // TODO: need to have this __weak definition so that the SigGen library can be used in different applications

/*
 * Description: 
 * Args: None
 * Return: None
*/
void SigGen::m_setSocket(int socket) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Set network socket\n";

    m_socketNum = socket;
}

/* ##########  Sample Frequency Funtions  ########## */

/*
 * Description: Loads data from the full path to a file
 * Args: file path
 * Return: None
*/
void SigGen::m_loadDataFromFile(std::string filePath) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Lead Path: " << filePath << "\n";

    // Clear vector
    SIGGEN.m_ECGData.clear();
    SIGGEN.m_ECGDataIndex = 0;

    std::ifstream inputFile(filePath);

    std::vector<int> m_tempDataStore;
    
    // do the work of extraction, etc
    int i = 0;
    std::string line;
    while (std::getline(inputFile, line)) {
        
        int val = 0;
        std::istringstream ss("0" + line);
        ss >> val;

        m_tempDataStore.push_back(val);
        i++;
    }

    m_ECGDataSize = i - 1; // Minus one because last element of
    // data vector is always 0

    // Take out average of data
    int average = accumulate( m_tempDataStore.begin(), m_tempDataStore
            .end(), 0.0) / m_tempDataStore.size();

    std::transform(m_tempDataStore.begin(), m_tempDataStore.end(),
                std::back_inserter(m_tempDataStore),
                [=](int n) { return n - average; });

    m_ECGData = m_tempDataStore;

    // Reset the noise signal
    m_generateNoise();
}

/*
 * Description: Set sample Frequency
 * Args: sample f
 * Return: None
*/
void SigGen::m_setSampleFrequency(int freq) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Set frequency\n";
    m_msDelay = 1000 / freq; // 1000ms divided by freq to get ms delay between samples
}

/*
 * Description: Get ms Delay
 * Args: None
 * Return: None
*/
int SigGen::m_getMsDelay(void) {
    return m_msDelay;
}

/* ##########  Noise Funtions  ########## */

/*
 * Description: Generate a noise signal
 * Args: None
 * Return: None
*/
void SigGen::m_generateNoise(void) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Generate noise\n";

    if (m_NoiseData.size() == m_ECGDataSize) { return; }  // Don't remake all data if we already have it all

    // Random Number generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1,100);  // distribution in range [1, 100]

    for (int i = 0; i < m_ECGDataSize; i++) {  // TODO: why are we generating a whole array of noise? why cant we just add noise to the signal in real time
        m_NoiseData.push_back(dist100(rng) - 50); // -50 to +50
    }
}

/*
 * Description: Set Noise Magnitude
 * Args: None
 * Return: None
*/
void SigGen::m_setNoiseMagnitude(int noiseMag) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Set noise level\n";

    m_noiseMagnitude = noiseMag;
}

/* ##########  Filtering Funtions  ########## */

/*
 * Description: Recreate HP filter with new cutoff f
 * Args: cutoff f
 * Return: None
*/
void SigGen::m_setFilterHigh(int HPf) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Set HP filter\n";

    free_bw_high_pass(m_HPFilterScale);
    m_HPFilterScale = create_bw_high_pass_filter(2, 250, HPf);
}

/*
 * Description: Recreate LP filter with new cutoff f
 * Args: None
 * Return: None
*/
void SigGen::m_setFilterLow(int LPf) {
    LOG_INIT_CERR();
    _log(LOG_INFO) << "Set LP filter\n";

    free_bw_low_pass(m_LPFilterScale);
    m_LPFilterScale = create_bw_low_pass_filter(2, 250, LPf);
}

/* ##########  ECG Signal Funtions  ########## */

/*
 * Description: Increment data
 * Args: None
 * Return: None
*/
void SigGen::m_incrementData(void) {
    m_ECGDataIndex++;
}

/*
 * Description: Calculate and return next data point
 * Args: None
 * Return: None
*/
int SigGen::m_getNextData(void) {
    int data = m_ECGData[m_ECGDataIndex % m_ECGDataSize];

    if (m_noiseOn) {
        data += m_NoiseData[m_ECGDataIndex % m_ECGDataSize] * m_noiseMagnitude / 15;
    }

    if (m_filterOn) {
        data = bw_low_pass(m_LPFilterScale, bw_high_pass(m_HPFilterScale, data));
    }

    return data;
}
