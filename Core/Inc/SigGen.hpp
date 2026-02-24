/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>    // For inet_pton, sockaddr_in
#include <util.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <random>
#include <thread>         // For transmit thread
#include <string>

#include "./Core/Lib/filter.h"
#include "./Core/Lib/BSlogger.hpp"

#define SIGGEN SigGen::getInstance()

class SigGen {
 private:
    SigGen();
    ~SigGen();

    // Prevent copying of class (Singleton)
    SigGen(SigGen&) = delete;  // Copy constructor (Not Implemented)
    void operator=(SigGen const&) = delete;  // Copy assignment operator
                                             // (Not Implemented)

    // Signal
    int m_ECGDataIndex = 0;
    int m_ECGDataSize = 0;
    int m_msDelay;  // Defines sample frequency as a delay between each sample
                    // (in milliseconds)

    // Noise Variables
    std::vector<int> m_NoiseData;
    int m_noiseMagnitude;

    // Filters
    BWLowPass* m_LPFilterScale;
    BWHighPass* m_HPFilterScale;

    // Network
    void m_transmitData(void);
    bool m_thread(void);
    bool m_socketOpen = false;
    int m_socketNum;
    int m_streamSocket;
    sockaddr_in m_streamAddr{};

 public:
    // Get instance for singleton setup
    static SigGen& getInstance() {
         static SigGen instance;
         return instance;
    }

    // Signal
    std::vector<int> m_ECGData;
    void m_loadDataFromFile(std::string path);
    void m_incrementData(void);
    int m_getNextData(void);

    // Freq Selection
    void m_setSampleFrequency(int freq);
    int m_getMsDelay(void);

    // Noise Variables
    void m_generateNoise();
    void m_setNoiseMagnitude(int noiseMag);
    bool m_noiseOn = false;

    // Filtering
    void m_setFilterHigh(int HPf);
    void m_setFilterLow(int LPf);
    bool m_filterOn = false;

    // Network
    void m_beginTransmission(void);
    void m_endTransmission(void);
    void m_setSocket(int socket);
};

void transmitDataCallback(int data);
