/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#pragma once

#include <gtkmm.h>
#include <vector>
#include <cmath>
#include <algorithm>  // For std::fill

#include "./Core/Inc/SigGen.hpp"
#include "./Core/Lib/BSlogger.hpp"


#define DRAW_HEIGHT 900
#define DRAW_WIDTH  3200
#define DRAW_WIDTH_SQUISH 4  // squish the graph horizontally so it doesn't take
                             // up too much space

class GUI_Signal_View : public Gtk::Box {
 public:
    GUI_Signal_View();

 private:
    // GUI Elements
    Gtk::DrawingArea m_signalPlot;

    // Data
    std::array<int, DRAW_WIDTH> m_plottingData = {0};
    int m_plottingDataIndex = 0;
    Cairo::RefPtr<Cairo::ImageSurface> m_FrameBuffer;

    // Call back functions
    void m_onDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                                                           int height);

 public:
    // Functions
    void m_redrawPlot(int sample_freq);
};
