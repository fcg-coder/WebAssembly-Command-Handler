#include "../screen.hpp"
#include "shapes.hpp"

extern InputOutputHandler* IOH;
void ShapeBase::line(int x0, int y0, int x1, int y1, const Color& color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        if (x0 >= 0 && x0 < Screen::getInstance()->getSize().first &&
            y0 >= 0 && y0 < Screen::getInstance()->getSize().second)
        {
            Pixel pixel(color.red, color.green, color.blue, color.alpha);
            pixel.x = x0;
            pixel.y = y0;
            Screen::getInstance()->addShape(pixel, this->layoutIndex);
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
void Gradient::render()
{
    auto [width, height] = Screen::getInstance()->getSize();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            uint8_t r = static_cast<uint8_t>(255 * (0.5f + 0.5f * sin(x * 0.02f)));
            uint8_t g = static_cast<uint8_t>(255 * (0.5f + 0.5f * cos(y * 0.02f)));
            uint8_t b = static_cast<uint8_t>(255 * (0.5f + 0.5f * sin((x + y) * 0.015f)));

            Pixel pixel(r, g, b);
            pixel.x = x;
            pixel.y = y;
            Screen::getInstance()->addShape(pixel, this->layoutIndex);
        }
    }
}

void Square::render()
{
    auto [width, height] = Screen::getInstance()->getSize();
    const int size = 100;
    const int half = size / 2;

    int centerX = width / 5;
    int centerY = height / 5;

    Color borderColor = Color::Red();

    line(centerX - half, centerY - half, centerX + half, centerY - half, borderColor);
    line(centerX - half, centerY + half, centerX + half, centerY + half, borderColor);

    line(centerX - half, centerY - half, centerX - half, centerY + half, borderColor);
    line(centerX + half, centerY - half, centerX + half, centerY + half, borderColor);
}