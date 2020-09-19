
#include "canvas.h"
#define POINT_SIZE 4
#define CENTER_MARK_SIZE 10
#define POINTS_SCALE 1

bool CCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();
    const int lesser = MIN(width, height);

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(1);

    if (points)
        for(auto& p : *points)
            drawPoint(p[0], p[1], cr);
    if (hull)
        drawPoly(cr, *hull, 1.0);
        for(auto& p : *hull)
            drawPoint(p[0], p[1], cr, 1.0);
    return true;
}

void CCanvas::drawCenterMark(double xc, double yc, const Cairo::RefPtr<Cairo::Context>& cr)
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

void CCanvas::drawPoint(double x, double y, const Cairo::RefPtr<Cairo::Context>& cr, double r, double g, double b)
{
    x *= POINTS_SCALE;
    y *= POINTS_SCALE;
    cr->save();
    cr->rectangle(x - POINT_SIZE/2, height-y - POINT_SIZE/2, POINT_SIZE, POINT_SIZE);
    cr->set_source_rgb(r, g, b);
    cr->fill_preserve();
    cr->restore();
    cr->stroke();
}

void CCanvas::drawLine(double x1, double y1, double x2, double y2, const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->move_to(x1 * POINTS_SCALE, height - y1 * POINTS_SCALE);
    cr->line_to(x2 * POINTS_SCALE, height - y2 * POINTS_SCALE);
}

void CCanvas::drawPoly(const Cairo::RefPtr<Cairo::Context>& cr, vector<vector<double>>& v, double r, double g, double b)
{
    if(v.size() < 2) return;
    cr->save();
    cr->set_source_rgb(r, g, b);
    for(int i=0; i<v.size()-1; ++i)
        drawLine(v[i][0], v[i][1],
                 v[i+1][0], v[i+1][1], cr);
    cr->stroke();
    cr->restore();
}
