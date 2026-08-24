// src/graphic_lib/shapes/shapes.hpp
#pragma once

#include "shape_base.hpp"

class Cube : public ShapeBase3D
{
public:
    Cube();
    void render() override;
};

class CoordinateSystem : public ShapeBase3D
{
public:
    CoordinateSystem();
    void render() override;
};

class Pyramid : public ShapeBase3D
{
public:
    Pyramid();
    void render() override;
};

class Gradient : public ShapeBase
{
public:
    Gradient() { layoutIndex = 1; }
    void render() override;
};

class Square : public ShapeBase
{
public:
    Square() { layoutIndex = 2; }
    void render() override;
};


class Spline2D : public ShapeBase {
public:
    Spline2D();

    void render() override;

private:
    struct ControlPoint {
        double x;
        double y;
    };

    std::vector<ControlPoint> points;

    void generatePoints(int width, int height);
    void drawSpline();
};


class RayTracedSphere : public ShapeBase {
public:
    RayTracedSphere();

    void render() override;

private:
    struct Vec3 {
        double x;
        double y;
        double z;

        Vec3 operator+(const Vec3& other) const;
        Vec3 operator-(const Vec3& other) const;
        Vec3 operator*(double value) const;

        double dot(const Vec3& other) const;
        double length() const;
        Vec3 normalized() const;
    };

    struct Ray {
        Vec3 origin;
        Vec3 direction;
    };

    struct Sphere {
        Vec3 center;
        double radius;
    };

    Sphere sphere;

    Vec3 camera;
    Vec3 light;

    bool intersect(const Ray& ray,const Sphere& sphere,double& distance) const;

    Pixel calculatePixel(const Vec3& point,const Vec3& normal) const;
};


