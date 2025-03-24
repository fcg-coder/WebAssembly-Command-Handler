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