/* Copyright: © 2025 Ahmed Baig <ahmedhafeezbaig@gmail.com> */

#include "./Core/Inc/GUI_Signal_View.hpp"

/*
 * Description: 
 * Args: None
 * Return: None
*/
GUI_Signal_View::GUI_Signal_View() :
    Gtk::Box(Gtk::Orientation::VERTICAL, 10) {

    LOG_INIT_CERR();
    _log(LOG_INFO) << "Signal view GUI setup\n";

    append(*Gtk::make_managed<Gtk::Label>("Signal View", 10));
    append(m_signalPlot);

    m_signalPlot.set_content_width(DRAW_WIDTH / DRAW_WIDTH_SQUISH);
    m_signalPlot.set_content_height(DRAW_HEIGHT);
    m_signalPlot.set_draw_func(sigc::mem_fun(*this, &GUI_Signal_View::m_onDraw));
}

/* GUI Setup */


/* Signal Handling Functions */

/*
 * Description: Push frame buffer to display
 * Args:
 * Return: None
*/
void GUI_Signal_View::m_onDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    if (SIGGEN.m_ECGData.empty()) { return; }

    cr->set_source(m_FrameBuffer, 0, 0);
    cr->paint();
}

/*
 * Description: Draw plot
 * Args: takes the new data point
 * Return: None
*/
void GUI_Signal_View::m_redrawPlot(int data) {
    // Add data to data array
    m_plottingData[m_plottingDataIndex] = data;
    m_plottingDataIndex++;
    m_plottingDataIndex %= DRAW_WIDTH;

    if (m_plottingDataIndex % (50 / SIGGEN.m_getMsDelay())) return; // Only draw every nth frame

    // Clear buffer if SIGGEN.m_getMsDelay() has changed
    static int old_m_msDelay = 0;
    if (old_m_msDelay != SIGGEN.m_getMsDelay()) { // has changed
        std::fill(m_plottingData.begin(), m_plottingData.end(), 0);
    }
    old_m_msDelay = SIGGEN.m_getMsDelay();

    /*
     * Note: frame buffering here is done because it makes the plot look much smoother.
     * If we plot directly in the m_onDraw() function, the plot is much more jittery
     */

    // Frame Buffer + Draw
    m_FrameBuffer = Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, DRAW_WIDTH / DRAW_WIDTH_SQUISH, DRAW_HEIGHT);

    // Draw into the buffer
    auto buf_ctx = Cairo::Context::create(m_FrameBuffer);

    buf_ctx->set_source_rgb(0.2, 0.4, 0.8);
    buf_ctx->set_line_width(2.0);

    for (int i = 0; i < DRAW_WIDTH; i++) {
        buf_ctx->move_to((DRAW_WIDTH - (i * SIGGEN.m_getMsDelay())) / DRAW_WIDTH_SQUISH, DRAW_HEIGHT - (0.5 * m_plottingData[(m_plottingDataIndex - i + DRAW_WIDTH) % DRAW_WIDTH] + DRAW_HEIGHT / 2));
        buf_ctx->line_to((DRAW_WIDTH - ((i + 1) * SIGGEN.m_getMsDelay())) / DRAW_WIDTH_SQUISH, DRAW_HEIGHT - (0.5 * m_plottingData[(m_plottingDataIndex - (i + 1) + DRAW_WIDTH) % DRAW_WIDTH] + DRAW_HEIGHT / 2));
    }

    buf_ctx->stroke();

    m_signalPlot.queue_draw();
}


