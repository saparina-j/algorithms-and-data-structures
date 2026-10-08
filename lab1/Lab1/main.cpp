#include <cstddef>
#include <iostream>

#include "image.h"

template <typename T>
void drawFilledCircle(lab1::Image<T>& image,
    double centerX,
    double centerY,
    double radius,
    T fillValue) {
    const double radiusSquared = radius * radius;

    for (std::size_t row = 0; row < image.height(); ++row) {
        for (std::size_t col = 0; col < image.width(); ++col) {
            const double dx = static_cast<double>(col) - centerX;
            const double dy = static_cast<double>(row) - centerY;
            if (dx * dx + dy * dy <= radiusSquared) {
                image(row, col) = fillValue;
            }
        }
    }
}

int main() {
    std::size_t width = 0;
    std::size_t height = 0;
    double centerX = 0.0;
    double centerY = 0.0;
    double radius = 0.0;
    int fillValue = 0;

    std::cout << "Enter image width and height: ";
    std::cin >> width >> height;

    std::cout << "Enter circle center (x y) and radius: ";
    std::cin >> centerX >> centerY >> radius;

    std::cout << "Enter fill value: ";
    std::cin >> fillValue;

    lab1::Image<short> image(width, height, false);

    drawFilledCircle(image, centerX, centerY, radius,
        static_cast<short>(fillValue));

    std::cout << "Result:\n" << image;

    return 0;
}
