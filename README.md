# ECG Generator

## What is this?
This program allows for the streaming of an ECG signal through a network socket. The database of ECGs is simply a folder containing csv files of the ECG signals and has a structure described below.

## Features
The generator has the following custom features:
 - Independent ECG Generator engine (SigGen.cpp/hpp) that can be used on its own.
 - Socket Selection
 - ECG selector in the GUI to select different ECG pathologies
 - Changeable sampling frequency (50Hz up to 1000Hz)
 - Adjustable noise
 - Adjustable basic filtering
 - Example receiving code is provided

## Build Instructions
```
git clone Baigel/ECG-Generator-GUI
cd ECG-Generator-GUI
make
./ECG-Generator
```

## How do I receive the data?
Example code is provided for receiving the data. Python and C/C++ examples are provided. The examples provided just open a pre-defined network port that can be changed in the GUI.

## Database Structure
An example database is provided here in the repo. It is a subset of the PTB-XL dataset. Note that the data rate is assumed to be 1000Hz (the GUI provides a list of frequencies from 50 Hz to 1000Hz). If the database is not present in the program directory, the GUI will not launch.
```
./
├── ECG_Database/ # Root folder, must have this name
│   ├── Catergory_1/
│   │   ├── ECG_1/
│   │   │   ├── LeadI.csv
│   │   │   ├── LeadII.csv
│   │   │   └── ....csv
│   │   ├── ECG_2/
│   │   │   ├── LeadI.csv
│   │   │   ├── LeadII.csv
│   │   │   └── ....csv
│   └── Catergory_2/
│   │   ├── ECG_1/
│   │   │   ├── LeadI.csv
│   │   │   ├── LeadII.csv
│   │   │   └── ....csv
```

## The Engine
SigGen.cpp and SigGen.hpp contain a singleton class that is used to load, generate, and transmit the ECG over the network. You can take this library on its own and use it independently of the GUI. Note that the SigGen library expects to fine filter.c/h and BSlogger.hpp in the ./Core/Lib directory.

## Example Usage
![Demo](./ECG%20Generator%20Demo.png "Demo")
