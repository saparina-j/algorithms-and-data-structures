#include <cstddef>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace lab1 {

    template <typename T>
    class Image {
    public:
        Image(std::size_t width, std::size_t height, bool randomize);
        Image(const Image& other);
        Image& operator=(const Image& other);
        ~Image();

    private:
        T* data_;
        std::size_t width_;
        std::size_t height_;
    };

}  // namespace lab1