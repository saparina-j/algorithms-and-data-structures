#include <cstddef>
#include <iostream>
#include <stdexcept>

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

template <typename T>
void demonstrateAllOperations() {
    const std::size_t width = 4;
    const std::size_t height = 3;

    lab1::Image<T> first(width, height, true);
    lab1::Image<T> second(width, height, true);

    std::cout << "First image:\n" << first;
    std::cout << "Second image:\n" << second;

    std::cout << "Width: " << first.width()
        << ", height: " << first.height() << '\n';

    std::cout << "first(0, 0) = " << first(0, 0) << '\n';

    std::cout << "first + second:\n" << (first + second);
    std::cout << "first * second:\n" << (first * second);

    const T scalar = static_cast<T>(2);
    std::cout << "first * scalar:\n" << (first * scalar);
    std::cout << "scalar * first:\n" << (scalar * first);
    std::cout << "first + scalar:\n" << (first + scalar);
    std::cout << "scalar + first:\n" << (scalar + first);

    std::cout << "!first:\n" << !first;

    std::cout << "fillRatio(first) = " << first.fillRatio() << '\n';

    std::cout << "first == second: " << (first == second) << '\n';
    std::cout << "first != second: " << (first != second) << '\n';

    lab1::Image<T> copy(first);
    std::cout << "copy == first: " << (copy == first) << '\n';

    lab1::Image<T> assigned(width, height, false);
    assigned = first;
    std::cout << "assigned == first: " << (assigned == first) << '\n';
}

int main() {
    std::cout << "===== Demonstration for short =====\n";
    demonstrateAllOperations<short>();

    std::cout << "\n===== Demonstration for float =====\n";
    demonstrateAllOperations<float>();

    std::size_t width = 0;
    std::size_t height = 0;
    double centerX = 0.0;
    double centerY = 0.0;
    double radius = 0.0;
    int fillValue = 0;

    std::cout << "\n===== Filled circle =====\n";
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