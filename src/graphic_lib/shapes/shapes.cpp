
#include "shapes.hpp"
#include "../../kernel/kernel.hpp"
#include <random>

void Gradient::render()
{
    auto [width, height] = Kernel::SCREEN()->getSize();

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
            Kernel::SCREEN()->addShape(pixel, this->layoutIndex);
        }
    }
}

void Square::render()
{
    auto [width, height] = Kernel::SCREEN()->getSize();
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

    int size = 15;
    int offsetX = 100; // Center offset on the X-axis
    int offsetY = 30;  // Center offset on the Y-axis
    int offsetZ = 10;  // Center offset on the Z-axis

    points = {
        Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, size + offsetY, size + offsetZ),
        Point3D(-size + offsetX, size + offsetY, size + offsetZ),
        Point3D(-size + offsetX, -size + offsetY, -size + offsetZ),
        Point3D(size + offsetX, -size + offsetY, -size + offsetZ),
        Point3D(size + offsetX, size + offsetY, -size + offsetZ),
        Point3D(-size + offsetX, size + offsetY, -size + offsetZ)};

    layoutIndex = 1;

    mode = isVisible::ON;
}

static inline bool randomBool()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::bernoulli_distribution dist(0.5);

    return dist(gen);
}

void Cube::render()
{
    Point3D::setProjectionMethod(&Point3D::perspectiveProjection);
    // Point3D::setProjectionMethod(&Point3D::orthographicProjection);
    double angle = 2.0 * M_PI / 180.0;

    if (randomBool())
        rotateX(angle);
    if (randomBool())
        rotateY(angle);
    if (randomBool())
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
        Point3D(0, 0, 0),
        Point3D(1000, 0, 0),
        Point3D(0, 1000, 0),
        Point3D(0, 0, 10000000)};
    layoutIndex = 998;

    mode = isVisible::ON;
}

void CoordinateSystem::render()
{
    Point3D::setProjectionMethod(&Point3D::orthographicProjection);

    line3D(points[0], points[1], Color::Gray()); // X-axis
    line3D(points[0], points[2], Color::Gray()); // Y-axis
    line3D(points[0], points[3], Color::Gray()); // Z-axis
}

Pyramid::Pyramid()
{
    int size = 30;

    int offsetX = 150;
    int offsetY = 50;
    int offsetZ = 0;

    // Base square + top point
    points = {
        // Base
        Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, size + offsetY, size + offsetZ),
        Point3D(-size + offsetX, size + offsetY, size + offsetZ),

        // Apex
        Point3D(offsetX, offsetY, -size + offsetZ)};

    layoutIndex = 2;
    mode = isVisible::ON;
}

void Pyramid::render()
{
    Point3D::setProjectionMethod(&Point3D::perspectiveProjection);

    double angle = 2.0 * M_PI / 180.0;

    rotateX(angle);
    rotateY(angle);
    rotateZ(angle);

    Color color = Color::White();

    // Base square
    line3D(points[0], points[1], color);
    line3D(points[1], points[2], color);
    line3D(points[2], points[3], color);
    line3D(points[3], points[0], color);

    // Pyramid sides
    line3D(points[0], points[4], color);
    line3D(points[1], points[4], color);
    line3D(points[2], points[4], color);
    line3D(points[3], points[4], color);
}