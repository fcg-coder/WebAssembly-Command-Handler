#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>

// forward_decls
class Screen;
class Pixel;
class Point3D;
struct Color;
/**
 * @brief 2D shape base class
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

constexpr int VIEWER_DISTANCE = 10000; // Distance from the viewer to the screen in pixels
class Point3D
{
public:
    double x;
    double y;
    double z;

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
        long long denominator = z + VIEWER_DISTANCE;
        if (denominator <= 0)
            return {0, 0};

        long long projectedX = (static_cast<long long>(x) * VIEWER_DISTANCE) / denominator;
        long long projectedY = (static_cast<long long>(y) * VIEWER_DISTANCE) / denominator;

        return {static_cast<int>(projectedX), static_cast<int>(projectedY)};
    }

    /***
     * @brief
     */
    std::pair<int, int> orthographicProjection()
    {
        return {x, y};
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

        double centerX = 0, centerY = 0, centerZ = 0;
        for (auto& point : points)
        {
            centerX += point->x;
            centerY += point->y;
            centerZ += point->z;
        }
        centerX /= points.size();
        centerY /= points.size();
        centerZ /= points.size();

        for (auto& point : points)
        {
            point->x -= centerX;
            point->y -= centerY;
            point->z -= centerZ;
        }

        for (auto& point : points)
        {
            double y = point->y;
            double z = point->z;

            point->y = y * cos(angle) - z * sin(angle);
            point->z = y * sin(angle) + z * cos(angle);
        }

        for (auto& point : points)
        {
            point->x += centerX;
            point->y += centerY;
            point->z += centerZ;
        }
    }

    void rotateY(double angle)
    {
        double centerX = 0, centerY = 0, centerZ = 0;
        for (auto& point : points)
        {
            centerX += point->x;
            centerY += point->y;
            centerZ += point->z;
        }
        centerX /= points.size();
        centerY /= points.size();
        centerZ /= points.size();

        for (auto& point : points)
        {
            point->x -= centerX;
            point->y -= centerY;
            point->z -= centerZ;
        }

        for (auto& point : points)
        {
            double x = point->x;
            double z = point->z;

            point->x = x * cos(angle) + z * sin(angle);
            point->z = -x * sin(angle) + z * cos(angle);
        }

        for (auto& point : points)
        {
            point->x += centerX;
            point->y += centerY;
            point->z += centerZ;
        }
    }

    void rotateZ(double angle)
    {
        double centerX = 0, centerY = 0, centerZ = 0;
        for (auto& point : points)
        {
            centerX += point->x;
            centerY += point->y;
            centerZ += point->z;
        }
        centerX /= points.size();
        centerY /= points.size();
        centerZ /= points.size();

        for (auto& point : points)
        {
            point->x -= centerX;
            point->y -= centerY;
            point->z -= centerZ;
        }

        for (auto& point : points)
        {
            double x = point->x;
            double y = point->y;

            point->x = x * cos(angle) - y * sin(angle);
            point->y = x * sin(angle) + y * cos(angle);
        }

        for (auto& point : points)
        {
            point->x += centerX;
            point->y += centerY;
            point->z += centerZ;
        }
    }
};
