#include "dcmat.hpp"

Settings settings;

void Settings::show() const
{
    std::cout << "h_view_lo: " << h_view_lo << "\n"
              << "h_view_hi: " << h_view_hi << "\n"
              << "v_view_lo: " << v_view_lo << "\n"
              << "v_view_hi: " << v_view_hi << "\n"
              << "float precision: " << float_precision << "\n"
              << "integral_steps: " << integral_steps << "\n"
              << "Draw Axis: " << (draw_axis ? "ON" : "OFF") << "\n"
              << "Erase Plot: " << (erase_plot ? "ON" : "OFF") << "\n"
              << "Connect Dots: " << (connect_dots ? "ON" : "OFF") << "\n";
}

void Settings::reset()
{
    h_view_lo = -6.5;
    h_view_hi = 6.5;
    v_view_lo = -3.5;
    v_view_hi = 3.5;
    float_precision = 6;
    integral_steps = 1000;
    draw_axis = true;
    erase_plot = true;
    connect_dots = false;
}
