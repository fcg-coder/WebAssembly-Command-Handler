#include "../screen.hpp"
#include "shape_base.hpp"

extern InputOutputHandler* IOH;
std::pair<int, int> (Point3D::*Point3D::projectionMethod)() = nullptr;

void ShapeBase::line(int x0, int y0, int x1, int y1, const Color& color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int prevX = -1, prevY = -1;

    while (true)
    {
        if (x0 >= 0 && x0 < Screen::getInstance()->getSize().first &&
            y0 >= 0 && y0 < Screen::getInstance()->getSize().second)
        {
            if (x0 != prevX || y0 != prevY)
            {
                Pixel pixel(color.red, color.green, color.blue, color.alpha);
                pixel.x = x0;
                pixel.y = y0;
                Screen::getInstance()->addShape(pixel, this->layoutIndex);
                prevX = x0;
                prevY = y0;
            }
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}
