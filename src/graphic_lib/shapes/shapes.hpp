#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>

// forward_decls
class Screen;
class Pixel;
struct Color;
/**
 * @brief
 *
 */
class ShapeBase
{
public:
    int layoutIndex;
    virtual void render() = 0;
    virtual ~ShapeBase() = default;

    /**
     * @brief Draws a line between two points (x0, y0) and (x1, y1) using Bresenham's algorithm.

     *
     */
    void line(int x0, int y0, int x1, int y1, const Color& color);
};

constexpr int VIEWER_DISTANCE = 1000000; // Distance from the viewer to the screen in pixels
class Point3D
{
public:
    int x;
    int y;
    int z;

    Point3D(int x, int y, int z)
        : x(x), y(y), z(z) {}
    Point3D()
        : x(0), y(0), z(0) {}

    static std::pair<int, int> (Point3D::*projectionMethod)();

    static void setProjectionMethod(std::pair<int, int> (Point3D::*func)())
    {
        projectionMethod = func;
    }

    std::pair<int, int> projection()
    {
        return (this->*projectionMethod)();
    }

    /***
     * @brief Projects the 3D point onto a 2D plane using PERSPECTIVE PROJECTION.
     * @return A pair of integers representing the projected x and y coordinates.
     */
    std::pair<int, int> perspectiveProjection()
    {
        int projectedX = (x * VIEWER_DISTANCE) / (z + VIEWER_DISTANCE);
        int projectedY = (y * VIEWER_DISTANCE) / (z + VIEWER_DISTANCE);
        return {projectedX, projectedY};
    }

    /***
     * @brief
     */
    std::pair<int, int> orthographicProjection()
    {
        return {x, y + 5};
    }
};

class ShapeBase3D : public ShapeBase
{
public:
    std::vector<Point3D*> points;
    virtual void render() override = 0;
    void line3D(Point3D* p1, Point3D* p2, const Color& color)
    {
        auto [x1, y1] = p1->projection();
        auto [x2, y2] = p2->projection();
        line(x1, y1, x2, y2, color);
    };

    void rotateX(double angle)
    {
        for (auto& point : points)
        {
            double y = point->y;
            double z = point->z;

            point->y = y * cos(angle) - z * sin(angle);
            point->z = y * sin(angle) + z * cos(angle);
        }
    };

    void rotateY(double angle)
    {
        for (auto& point : points)
        {
            double x = point->x;
            double z = point->z;

            point->x = x * cos(angle) + z * sin(angle);
            point->z = -x * sin(angle) + z * cos(angle);
        }
    };

    void rotateZ(double angle)
    {
        for (auto& point : points)
        {
            double x = point->x;
            double y = point->y;

            point->x = x * cos(angle) - y * sin(angle);
            point->y = x * sin(angle) + y * cos(angle);
        }
    }
};

class Cube : public ShapeBase3D
{
public:
    Cube()
    {
        int size = 50;
        int offsetX = 100; // Center offset on the X-axis
        int offsetY = 100; // Center offset on the Y-axis
        int offsetZ = 100; // Center offset on the Z-axis

        points = {
            new Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
            new Point3D(size + offsetX, -size + offsetY, size + offsetZ),
            new Point3D(size + offsetX, size + offsetY, size + offsetZ),
            new Point3D(-size + offsetX, size + offsetY, size + offsetZ),
            new Point3D(-size + offsetX, -size + offsetY, -size + offsetZ),
            new Point3D(size + offsetX, -size + offsetY, -size + offsetZ),
            new Point3D(size + offsetX, size + offsetY, -size + offsetZ),
            new Point3D(-size + offsetX, size + offsetY, -size + offsetZ)};
    }

    void render() override;
};

class Gradient : public ShapeBase
{
public:
    Gradient()
    {
        layoutIndex = 999;
    }
    void render() override;
};

class Square : public ShapeBase

{

public:
    Square()
    {
        layoutIndex = 1;
    }
    void render() override;
};