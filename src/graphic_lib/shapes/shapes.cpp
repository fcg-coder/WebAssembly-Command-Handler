#include "shapes.hpp"
#include "../../kernel/kernel.hpp"
#include <random>
#include <algorithm>
#include <cmath>
#include <algorithm>


namespace {
    int cubeWidth = 0;
    int cubeHeight = 0;
    int pyramidWidth = 0;
    int pyramidHeight = 0;
}

void Gradient::render() {
    auto [width, height] = Kernel::SCREEN()->getSize();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
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

void Square::render() {
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


static inline bool randomBool() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::bernoulli_distribution dist(0.5);

    return dist(gen);
}

/*
 * @todo !
 */
Cube::Cube() {
    auto [width, height] = Kernel::SCREEN()->getSize();

    cubeWidth = width;
    cubeHeight = height;

    int size = std::min(width, height) / 6;
    int offsetX = width / 5;
    int offsetY = height / 2;
    int offsetZ = 10;

    points = {
        Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, size + offsetY, size + offsetZ),
        Point3D(-size + offsetX, size + offsetY, size + offsetZ),
        Point3D(-size + offsetX, -size + offsetY, -size + offsetZ),
        Point3D(size + offsetX, -size + offsetY, -size + offsetZ),
        Point3D(size + offsetX, size + offsetY, -size + offsetZ),
        Point3D(-size + offsetX, size + offsetY, -size + offsetZ)
    };

    layoutIndex = 1;
    mode = isVisible::ON;
}



/*
 * @todo !
 */
void Cube::render() {
    auto [width, height] = Kernel::SCREEN()->getSize();

    if (width != cubeWidth || height != cubeHeight) {
        cubeWidth = width;
        cubeHeight = height;

        int size = std::min(width, height) / 6;
        int offsetX = width / 5;
        int offsetY = height / 2;
        int offsetZ = 10;

        points = {
            Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
            Point3D(size + offsetX, -size + offsetY, size + offsetZ),
            Point3D(size + offsetX, size + offsetY, size + offsetZ),
            Point3D(-size + offsetX, size + offsetY, size + offsetZ),
            Point3D(-size + offsetX, -size + offsetY, -size + offsetZ),
            Point3D(size + offsetX, -size + offsetY, -size + offsetZ),
            Point3D(size + offsetX, size + offsetY, -size + offsetZ),
            Point3D(-size + offsetX, size + offsetY, -size + offsetZ)
        };
    }

    Point3D::setProjectionMethod(&Point3D::perspectiveProjection);

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

CoordinateSystem::CoordinateSystem() {
    points = {
        Point3D(0, 0, 0),
        Point3D(1000, 0, 0),
        Point3D(0, 1000, 0),
        Point3D(0, 0, 10000000)
    };

    layoutIndex = 998;
    mode = isVisible::ON;
}

void CoordinateSystem::render() {
    Point3D::setProjectionMethod(&Point3D::orthographicProjection);

    line3D(points[0], points[1], Color::Gray());
    line3D(points[0], points[2], Color::Gray());
    line3D(points[0], points[3], Color::Gray());
}

/*
 * @todo !
 */
Pyramid::Pyramid() {
    auto [width, height] = Kernel::SCREEN()->getSize();

    pyramidWidth = width;
    pyramidHeight = height;

    int size = std::min(width, height) / 5;
    int offsetX = width / 2;
    int offsetY = height / 5;
    int offsetZ = 0;

    points = {
        Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, -size + offsetY, size + offsetZ),
        Point3D(size + offsetX, size + offsetY, size + offsetZ),
        Point3D(-size + offsetX, size + offsetY, size + offsetZ),
        Point3D(offsetX, offsetY, -size + offsetZ)
    };

    layoutIndex = 2;
    mode = isVisible::ON;
}

void Pyramid::render() {
    auto [width, height] = Kernel::SCREEN()->getSize();

    if (width != pyramidWidth || height != pyramidHeight) {
        pyramidWidth = width;
        pyramidHeight = height;

        int size = std::min(width, height) / 5;
        int offsetX = width / 2;
        int offsetY = height / 5;
        int offsetZ = 0;

        points = {
            Point3D(-size + offsetX, -size + offsetY, size + offsetZ),
            Point3D(size + offsetX, -size + offsetY, size + offsetZ),
            Point3D(size + offsetX, size + offsetY, size + offsetZ),
            Point3D(-size + offsetX, size + offsetY, size + offsetZ),
            Point3D(offsetX, offsetY, -size + offsetZ)
        };
    }

    Point3D::setProjectionMethod(&Point3D::perspectiveProjection);

    double angle = 2.0 * M_PI / 180.0;

    rotateX(angle);
    rotateY(angle);
    rotateZ(angle);

    Color color = Color::White();

    line3D(points[0], points[1], color);
    line3D(points[1], points[2], color);
    line3D(points[2], points[3], color);
    line3D(points[3], points[0], color);

    line3D(points[0], points[4], color);
    line3D(points[1], points[4], color);
    line3D(points[2], points[4], color);
    line3D(points[3], points[4], color);
}

namespace {
    std::mt19937 splineGenerator(std::random_device{}());
}

Spline2D::Spline2D()
{
    layoutIndex = 3;
    mode = isVisible::ON;

    auto [width, height] = Kernel::SCREEN()->getSize();

    generatePoints(width, height);
}

void Spline2D::generatePoints(int width, int height)
{
    points.clear();

    constexpr int pointCount = 8;

    std::uniform_real_distribution<double> xDist(width * 0.1, width * 0.9);

    std::uniform_real_distribution<double> yDist(height * 0.1, height * 0.9);

    for (int i = 0; i < pointCount; ++i) {
        points.push_back({xDist(splineGenerator),yDist(splineGenerator)
        });
    }

    // Сортируем по X, чтобы spline шёл слева направо.
    std::sort(points.begin(), points.end(), [](const ControlPoint& a, const ControlPoint& b) { return a.x < b.x;});
}

void Spline2D::render()
{
    auto [width, height] = Kernel::SCREEN()->getSize();

    if (points.empty()) {
        generatePoints(width, height);
    }

    drawSpline();
}

void Spline2D::drawSpline()
{
    if (points.size() < 2)
        return;

    constexpr double step = 0.01;

    for (size_t i = 0; i + 1 < points.size(); ++i) {

        // Для Catmull-Rom нужны точки:
        //
        // P0 ---- P1 ---- P2 ---- P3
        //
        // Кривая строится между P1 и P2.

        const ControlPoint& p0 = (i == 0) ? points[i] : points[i - 1];


        const ControlPoint& p1 = points[i];

        const ControlPoint& p2 = points[i + 1];

        const ControlPoint& p3 = (i + 2 >= points.size()) ? points[i + 1] : points[i + 2];

        int prevX = static_cast<int>(p1.x);
        int prevY = static_cast<int>(p1.y);

        for (double t = step; t <= 1.0; t += step) {

            const double t2 = t * t;
            const double t3 = t2 * t;

            const double x = 0.5 * (2.0 * p1.x + (-p0.x + p2.x) * t + (2.0 * p0.x - 5.0 * p1.x +  4.0 * p2.x - p3.x) * t2 + (-p0.x + 3.0 * p1.x - 3.0 * p2.x + p3.x) * t3 );

            const double y =
                0.5 *
                (
                    2.0 * p1.y +
                    (-p0.y + p2.y) * t +
                    (2.0 * p0.y - 5.0 * p1.y +
                     4.0 * p2.y - p3.y) * t2 +
                    (-p0.y + 3.0 * p1.y -
                     3.0 * p2.y + p3.y) * t3
                );

            const int currentX = static_cast<int>(x);
            const int currentY = static_cast<int>(y);

            line(prevX,prevY,currentX,currentY, Color::White());
            prevX = currentX;
            prevY = currentY;
        }
    }
}


RayTracedSphere::Vec3 RayTracedSphere::Vec3::operator+(const Vec3& other) const
{
    return {x + other.x, y + other.y,
        z + other.z
    };
}

RayTracedSphere::Vec3 RayTracedSphere::Vec3::operator-(const Vec3& other) const
{
    return {
        x - other.x,
        y - other.y,
        z - other.z
    };
}

RayTracedSphere::Vec3
RayTracedSphere::Vec3::operator*(double value) const
{
    return {
        x * value,
        y * value,
        z * value
    };
}

double RayTracedSphere::Vec3::dot(const Vec3& other) const
{
    return
        x * other.x +
        y * other.y +
        z * other.z;
}

double RayTracedSphere::Vec3::length() const
{
    return std::sqrt(dot(*this));
}

RayTracedSphere::Vec3
RayTracedSphere::Vec3::normalized() const
{
    const double len = length();

    if (len == 0.0)
        return {0.0, 0.0, 0.0};

    return {
        x / len,
        y / len,
        z / len
    };
}



RayTracedSphere::RayTracedSphere()
{
    layoutIndex = 4;
    mode = isVisible::ON;

    auto [width, height] = Kernel::SCREEN()->getSize();

    sphere = {
        {
            width / 2.0,
            height / 2.0,
            0.0
        },
        std::min(width, height) * 0.3
    };

    camera = {
        width / 2.0,
        height / 2.0,
        -500.0
    };

    light = {
        width / 3.0,
        height / 3.0,
        -300.0
    };
}


bool RayTracedSphere::intersect(
    const Ray& ray,
    const Sphere& sphere,
    double& distance
) const
{
    const Vec3 oc = ray.origin - sphere.center;

    const double a = ray.direction.dot(ray.direction);
    const double b = 2.0 * oc.dot(ray.direction);

    const double c =
        oc.dot(oc) -
        sphere.radius * sphere.radius;

    const double discriminant =
        b * b - 4.0 * a * c;

    if (discriminant < 0.0)
        return false;

    const double sqrtDiscriminant =
        std::sqrt(discriminant);

    const double t1 =
        (-b - sqrtDiscriminant) / (2.0 * a);

    const double t2 =
        (-b + sqrtDiscriminant) / (2.0 * a);

    if (t1 > 0.0) {
        distance = t1;
        return true;
    }

    if (t2 > 0.0) {
        distance = t2;
        return true;
    }

    return false;
}


Pixel RayTracedSphere::calculatePixel(
    const Vec3& point,
    const Vec3& normal
) const
{
    Vec3 lightDirection =
        (light - point).normalized();

    double intensity =
        normal.dot(lightDirection);

    intensity = std::max(0.0, intensity);

    // Небольшое ambient-освещение,
    // чтобы тёмная сторона шара не была полностью чёрной.
    intensity =
        0.15 +
        intensity * 0.85;

    intensity =
        std::min(1.0, intensity);

    const uint8_t value =
        static_cast<uint8_t>(255.0 * intensity);

    return Pixel(
        value,
        value,
        value
    );
}


void RayTracedSphere::render()
{
    auto [width, height] = Kernel::SCREEN()->getSize();

    sphere.center.x = width / 2.0;
    sphere.center.y = height / 2.0;

    sphere.radius =
        std::min(width, height) * 0.3;

    camera.x = width / 2.0;
    camera.y = height / 2.0;

    for (int y = 0; y < height; ++y) {

        for (int x = 0; x < width; ++x) {

            Ray ray;

            ray.origin = camera;

            ray.direction = Vec3 {
                static_cast<double>(x) - camera.x,
                static_cast<double>(y) - camera.y,
                500.0
            }.normalized();

            double distance;

            if (!intersect(ray, sphere, distance))
                continue;

            const Vec3 hitPoint =
                ray.origin +
                ray.direction * distance;

            const Vec3 normal =
                (hitPoint - sphere.center).normalized();

            Pixel pixel = calculatePixel(hitPoint, normal);

            pixel.x = x;
            pixel.y = y;

            Kernel::SCREEN()->addShape(
                pixel,
                layoutIndex
            );
        }
    }
}

