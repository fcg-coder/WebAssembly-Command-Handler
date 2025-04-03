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
        
        layoutIndex = 2;
    }
    void render() override;
};