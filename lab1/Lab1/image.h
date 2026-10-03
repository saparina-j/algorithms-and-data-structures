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
        Image(std::size_t width, std::size_t height, bool randomize);
        Image(const Image& other);
        Image& operator=(const Image& other);
        ~Image();

        T& operator()(std::size_t row, std::size_t col);
        const T& operator()(std::size_t row, std::size_t col) const;

        std::size_t width() const noexcept;
        std::size_t height() const noexcept;

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

}  