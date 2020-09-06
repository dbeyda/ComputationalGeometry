
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <vector>

using namespace std;

class CCanvas : public Gtk::DrawingArea
{
public:
    // points to to drawn {{xCoord, yCoord},}
    vector<vector<double>> *points;
    // circles to be drawn {{xCenter, yCenter, radius, rColor, gColor, bColor}}
    vector<vector<double>> *circles;
    CCanvas() : points(nullptr), circles(nullptr) {};
    virtual ~CCanvas() {};

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
private:
    void drawCenterMark(int xc, int yc, Cairo::RefPtr<Cairo::Context> const & cr);
    void drawPoint(int x, int y, const Cairo::RefPtr<Cairo::Context>& cr);
    void drawCircle(int x, int y, int radius, float r, float g, float b, Cairo::RefPtr<Cairo::Context> const & cr);

}; // CCanvas