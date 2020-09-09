
#include "canvas.h"
#define POINT_SIZE 4
#define CENTER_MARK_SIZE 10
#define POINTS_SCALE 0.5

bool CCanvas::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    const int lesser = MIN(width, height);

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(1);

    drawCenterMark(xc, yc, cr);

    if (points)
        for(auto& p : *points)
            drawPoint(xc + p[0] * POINTS_SCALE, yc - p[1] * POINTS_SCALE, cr);

    if (circles)
        for(auto& c : *circles)
            drawCircle(xc + c[0] * POINTS_SCALE,    // x center coord
                       yc - c[1] * POINTS_SCALE,    // y center coord
                       c[2] * POINTS_SCALE,         // radius
                       c[3], c[4], c[5],            // r, g, b
                       cr);

    return true;
}

void CCanvas::drawCenterMark(int xc, int yc, Cairo::RefPtr<Cairo::Context> const & cr)
{
    cr->save();
    float offset = CENTER_MARK_SIZE / 2.0;
    cr->move_to(xc-offset, yc-offset);
    cr->line_to(xc+offset, yc+offset);
    cr->move_to(xc+offset, yc-offset);
    cr->line_to(xc-offset, yc+offset);
    cr->restore();
    cr->stroke();
}

void CCanvas::drawPoint(int x, int y, Cairo::RefPtr<Cairo::Context> const & cr)
{
    cr->save();
    cr->rectangle(x - POINT_SIZE/2, y-POINT_SIZE/2, POINT_SIZE, POINT_SIZE);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->fill_preserve();
    cr->restore();
    cr->stroke();
}

void CCanvas::drawCircle(int x, int y, int radius, float r, float g, float b, Cairo::RefPtr<Cairo::Context> const & cr)
{
    cr->save();
    cr->arc(x, y, radius, 0.0, 2.0 * M_PI);
    cr->set_source_rgba(r, g, b, 0.4);
    cr->fill_preserve();
    cr->restore();
    cr->stroke();
}