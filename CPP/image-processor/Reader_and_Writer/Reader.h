#ifndef CPP_HSE_READER_H
#define CPP_HSE_READER_H

#include <string>
#include <fstream>
#include <cerrno>
#include "Utils.h"

#include "../Image/Image.h"

namespace reading_and_writing {
    class Reader {
    public:
        explicit Reader(const std::string& file_name);
        Image Read();

    private:
        std::string file_name_;
        static size_t BytesToNumber(const unsigned char* bytes);
    };

    const size_t GetPaddingSize(size_t width);
}  // namespace reading_and_writing

#endif  // CPP_HSE_READER_H
