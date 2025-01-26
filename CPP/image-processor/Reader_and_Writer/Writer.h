#ifndef CPP_HSE_WRITER_H
#define CPP_HSE_WRITER_H

#include <string>
#include <utility>
#include <fstream>
#include <cerrno>
#include <algorithm>

#include "../Image/Image.h"
#include "Utils.h"
#include "Reader.h"

namespace reading_and_writing {
    class Writer {
    public:
        explicit Writer(std::string path);

        void Write(const Image &image);

    private:
        void WriteBMPHeader(unsigned char *bmp_header, size_t size_of_file);

        void WriteDIBHeader(unsigned char *dib_header, size_t width, size_t height);

        std::string filename_;

        template <typename T>
        void NumberToBytes(T number, unsigned char *bytes) const;
    };
}  // namespace reading_and_writing

#endif  // CPP_HSE_WRITER_H
