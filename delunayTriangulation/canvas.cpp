
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
    {
        for(auto& p : *points)
            drawPoint(p[0], p[1], cr);
    }
    if (triangles)
    {
        cr->save();
        cr->set_source_rgb(1.0, 0.0, 0.0);
        for(int i=0; i < triangles->size(); ++i)
            drawPoly(triangles->at(i), cr);

        cr->stroke();
        cr->restore();
    }
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

void CCanvas::drawPoint(double x, double y, const Cairo::RefPtr<Cairo::Context>& cr)
{
    x *= POINTS_SCALE;
    y *= POINTS_SCALE;
    cr->save();
    cr->rectangle(x - POINT_SIZE/2, height-y - POINT_SIZE/2, POINT_SIZE, POINT_SIZE);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->fill_preserve();
    cr->restore();
    cr->stroke();
}

void CCanvas::drawLine(double x1, double y1, double x2, double y2, const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->move_to(x1 * POINTS_SCALE, height - y1 * POINTS_SCALE);
    cr->line_to(x2 * POINTS_SCALE, height - y2 * POINTS_SCALE);
}

void CCanvas::drawPoly(vector<double>& poly, const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(poly.size() < 2) return;
    cr->save();
    for(int i=0; i<poly.size()-2; i+=2)
    {
        drawLine(poly[i], poly[i+1],
                 poly[i+2], poly[i+3], cr);
    }
    // closing polygon
    drawLine(poly[poly.size()-2], poly[poly.size()-1],
                poly[0], poly[1], cr);
    cr->restore();
    cr->stroke();
}
