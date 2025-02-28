
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <vector>

using namespace std;

class CCanvas : public Gtk::DrawingArea
{
public:
    // points to to drawn {{xCoord, yCoord},}
    vector<vector<double>> *points;
    vector<vector<double>> *hull;
    CCanvas() : points(nullptr), hull(nullptr) {};
    virtual ~CCanvas() {};

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
    int height, width;
    void drawCenterMark(double xc, double yc, const Cairo::RefPtr<Cairo::Context>& cr);
    void drawPoint(double x, double y, const Cairo::RefPtr<Cairo::Context>& cr,
                   double r = 0l, double g = 0l, double b = 0l);
    void drawLine(double x1, double y1, double x2, double y2, const Cairo::RefPtr<Cairo::Context>& cr);
    void drawPoly(const Cairo::RefPtr<Cairo::Context>& cr, vector<vector<double>>& v,
                  double r = 0l, double g = 0l, double b = 0l);
}; // CCanvas