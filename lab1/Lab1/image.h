#pragma once

#include <cstddef>
#include <limits>
#include <ostream>
#include <random>
#include <stdexcept>
#include <type_traits>

namespace lab1 {

    template <typename T>
    class Image {
    public:
        static constexpr double kEpsilon = 1e-9;

        Image(std::size_t width, std::size_t height, bool randomize);
        Image(const Image& other);
        Image& operator=(const Image& other);
        ~Image();

        T& operator()(std::size_t row, std::size_t col);
        const T& operator()(std::size_t row, std::size_t col) const;

        std::size_t width() const noexcept;
        std::size_t height() const noexcept;

        Image operator+(const Image& other) const;
        Image operator*(const Image& other) const;
        Image operator*(T scalar) const;
        Image operator+(T scalar) const;
        Image operator!() const;

        double fillRatio() const;

        bool operator==(const Image& other) const;
        bool operator!=(const Image& other) const;

    private:
        T* data_;
        std::size_t width_;
        std::size_t height_;

        static T randomValue();
    };

    template <typename T>
    Image<T>::Image(std::size_t width, std::size_t height, bool randomize)
        : data_(nullptr), width_(width), height_(height) {
        if (width == 0 || height == 0) {
            throw std::invalid_argument("Image dimensions must be positive");
        }
        data_ = new T[width_ * height_];
        for (std::size_t i = 0; i < width_ * height_; ++i) {
            data_[i] = randomize ? randomValue() : T{};
        }
    }

    template <typename T>
    Image<T>::Image(const Image& other)
        : data_(nullptr), width_(other.width_), height_(other.height_) {
        data_ = new T[width_ * height_];
        for (std::size_t i = 0; i < width_ * height_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    template <typename T>
    Image<T>& Image<T>::operator=(const Image& other) {
        if (this == &other) {
            return *this;
        }
        T* newData = new T[other.width_ * other.height_];
        for (std::size_t i = 0; i < other.width_ * other.height_; ++i) {
            newData[i] = other.data_[i];
        }
        delete[] data_;
        data_ = newData;
        width_ = other.width_;
        height_ = other.height_;
        return *this;
    }

    template <typename T>
    Image<T>::~Image() {
        delete[] data_;
    }

    template <typename T>
    T& Image<T>::operator()(std::size_t row, std::size_t col) {
        if (row >= height_ || col >= width_) {
            throw std::out_of_range("Image index out of range");
        }
        return data_[row * width_ + col];
    }

    template <typename T>
    const T& Image<T>::operator()(std::size_t row, std::size_t col) const {
        if (row >= height_ || col >= width_) {
            throw std::out_of_range("Image index out of range");
        }
        return data_[row * width_ + col];
    }

    template <typename T>
    std::size_t Image<T>::width() const noexcept {
        return width_;
    }

    template <typename T>
    std::size_t Image<T>::height() const noexcept {
        return height_;
    }

    template <typename T>
    T Image<T>::randomValue() {
        static std::mt19937 generator(std::random_device{}());
        if constexpr (std::is_same_v<T, bool>) {
            std::uniform_int_distribution<int> dist(0, 1);
            return dist(generator) == 1;
        }
        else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            return static_cast<T>(dist(generator));
        }
        else {
            std::uniform_int_distribution<int> dist(
                static_cast<int>(std::numeric_limits<T>::min()),
                static_cast<int>(std::numeric_limits<T>::max()));
            return static_cast<T>(dist(generator));
        }
    }

    template <typename T>
    T addValues(T lhs, T rhs) {
        if constexpr (std::is_same_v<T, bool>) {
            return lhs || rhs;
        }
        else if constexpr (std::is_integral_v<T>) {
            long long sum = static_cast<long long>(lhs) + static_cast<long long>(rhs);
            if (sum > static_cast<long long>(std::numeric_limits<T>::max())) {
                return std::numeric_limits<T>::max();
            }
            if (sum < static_cast<long long>(std::numeric_limits<T>::min())) {
                return std::numeric_limits<T>::min();
            }
            return static_cast<T>(sum);
        }
        else {
            return lhs + rhs;
        }
    }

    template <typename T>
    T multiplyValues(T lhs, T rhs) {
        if constexpr (std::is_same_v<T, bool>) {
            return lhs && rhs;
        }
        else if constexpr (std::is_integral_v<T>) {
            long long product = static_cast<long long>(lhs) * static_cast<long long>(rhs);
            if (product > static_cast<long long>(std::numeric_limits<T>::max())) {
                return std::numeric_limits<T>::max();
            }
            if (product < static_cast<long long>(std::numeric_limits<T>::min())) {
                return std::numeric_limits<T>::min();
            }
            return static_cast<T>(product);
        }
        else {
            return lhs * rhs;
        }
    }

    template <typename T>
    Image<T> Image<T>::operator+(const Image& other) const {
        std::size_t newWidth = width_ > other.width_ ? width_ : other.width_;
        std::size_t newHeight = height_ > other.height_ ? height_ : other.height_;
        Image result(newWidth, newHeight, false);

        for (std::size_t row = 0; row < newHeight; ++row) {
            for (std::size_t col = 0; col < newWidth; ++col) {
                T lhs = (row < height_ && col < width_) ? (*this)(row, col) : T{};
                T rhs = (row < other.height_ && col < other.width_) ? other(row, col) : T{};
                result(row, col) = addValues(lhs, rhs);
            }
        }
        return result;
    }

    template <typename T>
    Image<T> Image<T>::operator*(const Image& other) const {
        std::size_t newWidth = width_ > other.width_ ? width_ : other.width_;
        std::size_t newHeight = height_ > other.height_ ? height_ : other.height_;
        Image result(newWidth, newHeight, false);

        for (std::size_t row = 0; row < newHeight; ++row) {
            for (std::size_t col = 0; col < newWidth; ++col) {
                T lhs = (row < height_ && col < width_) ? (*this)(row, col) : T{};
                T rhs = (row < other.height_ && col < other.width_) ? other(row, col) : T{};
                result(row, col) = multiplyValues(lhs, rhs);
            }
        }
        return result;
    }

    template <typename T>
    Image<T> Image<T>::operator*(T scalar) const {
        Image result(width_, height_, false);
        for (std::size_t row = 0; row < height_; ++row) {
            for (std::size_t col = 0; col < width_; ++col) {
                result(row, col) = multiplyValues((*this)(row, col), scalar);
            }
        }
        return result;
    }

    template <typename T>
    Image<T> Image<T>::operator+(T scalar) const {
        Image result(width_, height_, false);
        for (std::size_t row = 0; row < height_; ++row) {
            for (std::size_t col = 0; col < width_; ++col) {
                result(row, col) = addValues((*this)(row, col), scalar);
            }
        }
        return result;
    }

    template <typename T>
    Image<T> Image<T>::operator!() const {
        Image result(width_, height_, false);
        for (std::size_t row = 0; row < height_; ++row) {
            for (std::size_t col = 0; col < width_; ++col) {
                T value = (*this)(row, col);
                if constexpr (std::is_same_v<T, bool>) {
                    result(row, col) = !value;
                }
                else {
                    result(row, col) =
                        static_cast<T>(std::numeric_limits<T>::max() - value);
                }
            }
        }
        return result;
    }

    template <typename T>
    double Image<T>::fillRatio() const {
        if (width_ == 0 || height_ == 0) {
            return 0.0;
        }
        double sum = 0.0;
        for (std::size_t i = 0; i < width_ * height_; ++i) {
            sum += static_cast<double>(data_[i]);
        }
        double maxValue = static_cast<double>(std::numeric_limits<T>::max());
        if (maxValue == 0.0) {
            return 0.0;
        }
        return sum / (static_cast<double>(width_ * height_) * maxValue);
    }

    template <typename T>
    bool valuesEqual(T lhs, T rhs) {
        if constexpr (std::is_floating_point_v<T>) {
            double diff = static_cast<double>(lhs) - static_cast<double>(rhs);
            if (diff < 0.0) {
                diff = -diff;
            }
            return diff <= Image<T>::kEpsilon;
        }
        else {
            return lhs == rhs;
        }
    }

    template <typename T>
    bool Image<T>::operator==(const Image& other) const {
        if (width_ != other.width_ || height_ != other.height_) {
            return false;
        }
        for (std::size_t row = 0; row < height_; ++row) {
            for (std::size_t col = 0; col < width_; ++col) {
                if (!valuesEqual((*this)(row, col), other(row, col))) {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T>
    bool Image<T>::operator!=(const Image& other) const {
        return !(*this == other);
    }

}  // namespace lab1

template <typename T>
std::ostream& operator<<(std::ostream& os, const lab1::Image<T>& image) {
    for (std::size_t row = 0; row < image.height(); ++row) {
        for (std::size_t col = 0; col < image.width(); ++col) {
            T value = image(row, col);
            if constexpr (std::is_same_v<T, bool>) {
                os << (value ? '1' : '0');
            }
            else {
                os << value;
            }
            if (col + 1 < image.width()) {
                os << ' ';
            }
        }
        os << '\n';
    }
    return os;
}

template <typename T>
lab1::Image<T> operator*(T scalar, const lab1::Image<T>& image) {
    return image * scalar;
}

template <typename T>
lab1::Image<T> operator+(T scalar, const lab1::Image<T>& image) {
    return image + scalar;
}