# This makefile generates the program the runs the ECG-Generator GUI

# Compiler Flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS = -std=c++17 -Wall -pthread
DEBUG_LEVEL = -g0
MACOS_COMPILATION_FLAGS = -Wl,-rpath,/opt/homebrew/lib -Wl,-macos_version_min,15.0 -framework Cocoa

# Libraries
INC = -I./
//LIB_INC = -I./Core/Lib/
DIR = ./Core/Src/
LIB_DIR = ./Core/Lib/
SOURCE = $(DIR)SigGen.cpp $(DIR)GUI.cpp $(DIR)GUI_ECG_Select.cpp $(DIR)GUI_Noise.cpp $(DIR)GUI_Filter.cpp $(DIR)GUI_Quit.cpp $(DIR)GUI_Transmit_Settings.cpp $(DIR)GUI_Signal_View.cpp $(DIR)GUI_Sample_Frequency.cpp $(LIB_DIR)filter.c
TARGET = ECG-Generator

all: $(TARGET)

$(TARGET):
	g++ $(CFLAGS) ./Core/$(TARGET).cpp -o $(TARGET) $(DEBUG_LEVEL) $(SOURCE) $(INC) $(LIB_INC) `pkg-config --cflags --libs gtkmm-4.0` $(MACOS_COMPILATION_FLAGS)

# -DLAPACK_LIBRARIES=-lopenblas -DBLAS_LIBRARIES=-lopenblasp

clean:
	$(RM) $(TARGET)
