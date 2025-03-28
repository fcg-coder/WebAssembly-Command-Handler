#include "../screen.hpp"
#include "shapes.hpp"

extern InputOutputHandler* IOH;

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

Cube::Cube()
{
    int size = 250;
    int offsetX = 500; // Center offset on the X-axis
    int offsetY = 500; // Center offset on the Y-axis
    int offsetZ = 500; // Center offset on the Z-axis

    points = {
        new Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
        new Point3D(size + offsetX, -size + offsetY, size + offsetZ),
        new Point3D(size + offsetX, size + offsetY, size + offsetZ),
        new Point3D(-size + offsetX, size + offsetY, size + offsetZ),
        new Point3D(-size + offsetX, -size + offsetY, -size + offsetZ),
        new Point3D(size + offsetX, -size + offsetY, -size + offsetZ),
        new Point3D(size + offsetX, size + offsetY, -size + offsetZ),
        new Point3D(-size + offsetX, size + offsetY, -size + offsetZ)};

    layoutIndex = 1;
}

void Cube::render()
{
    Point3D::setProjectionMethod(&Point3D::perspectiveProjection);
    // Point3D::setProjectionMethod(&Point3D::orthographicProjection);
    double angle = 2.0 * M_PI / 180.0;
    rotateX(angle);
    rotateY(angle);
    rotateZ(angle);

    line3D(points[0], points[1], Color::White());
    line3D(points[1], points[2], Color::White());
    line3D(points[2], points[3], Color::White());
    line3D(points[3], points[0], Color::White());

    line3D(points[4], points[5], Color::White());
    line3D(points[5], points[6], Color::White());
    line3D(points[6], points[7], Color::White());
    line3D(points[7], points[4], Color::White());

    line3D(points[0], points[4], Color::White());
    line3D(points[1], points[5], Color::White());
    line3D(points[2], points[6], Color::White());
    line3D(points[3], points[7], Color::White());
}

CoordinateSystem::CoordinateSystem()
{

    points = {
        new Point3D(0, 0, 0),
        new Point3D(1000, 0, 0),
        new Point3D(0, 1000, 0),
        new Point3D(0, 0, 10000000)};
    layoutIndex = 998;
}

void CoordinateSystem::render()
{
    Point3D::setProjectionMethod(&Point3D::perspectiveProjection);

    line3D(points[0], points[1], Color::Gray()); // X-axis
    line3D(points[0], points[2], Color::Gray()); // Y-axis
    line3D(points[0], points[3], Color::Gray()); // Z-axis
}
