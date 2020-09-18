
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <vector>

using namespace std;

class CCanvas : public Gtk::DrawingArea
{
public:
    // points to to drawn {{xCoord, yCoord},}
    vector<vector<double>> *points;
    vector<vector<double>> *diagonals;
    CCanvas() : points(nullptr), diagonals(nullptr) {};
    virtual ~CCanvas() {};

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
    int height, width;
    void drawCenterMark(double xc, double yc, const Cairo::RefPtr<Cairo::Context>& cr);
    void drawPoint(double x, double y, const Cairo::RefPtr<Cairo::Context>& cr);
    void drawLine(double x1, double y1, double x2, double y2, const Cairo::RefPtr<Cairo::Context>& cr);
    void drawPoly(const Cairo::RefPtr<Cairo::Context>& cr);

}; // CCanvas