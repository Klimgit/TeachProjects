#include "Reader.h"

reading_and_writing::Reader::Reader(const std::string& file_name) : file_name_(file_name) {
    file_name_ = file_name;
}

size_t reading_and_writing::Reader::BytesToNumber(const unsigned char* bytes) {
    size_t number = *bytes;
    for (size_t i = 0; i < image::utils::OFFSETS_OF_BYTES.size(); ++i) {
        number += *(bytes + i + 1) << image::utils::OFFSETS_OF_BYTES[i];
    }
    return number;
}

const size_t reading_and_writing::GetPaddingSize(size_t width) {
    return (image::utils::PADDING_SIZE - (width * image::utils::BYTES_PER_PIXEL) % image::utils::PADDING_SIZE) %
           image::utils::PADDING_SIZE;
}

Image reading_and_writing::Reader::Read() {
    unsigned char bmp_header[image::utils::BMP_HEADER_SIZE];
    unsigned char dib_header[image::utils::DIB_HEADER_SIZE];

    std::ifstream image_file;
    image_file.open(file_name_, std::ios::in | std::ios::binary);
    if (!image_file.is_open()) {
        throw std::invalid_argument(std::string("FILE ") + file_name_ + std::string(" NOT FOUND"));
    }
    if (errno == EACCES) {
        throw std::invalid_argument(std::string("NO PERMISSION TO READ FILE ") + file_name_);
    }
    image_file.read(reinterpret_cast<char*>(bmp_header), image::utils::BMP_HEADER_SIZE);
    if (bmp_header[0] != image::utils::FORMAT_BYTES[0] && bmp_header[1] != image::utils::FORMAT_BYTES[1]) {
        throw std::invalid_argument(std::string("FILE ") + file_name_ + std::string(" IS NOT BMP"));
    }

    try {
        image_file.read(reinterpret_cast<char*>(dib_header), image::utils::DIB_HEADER_SIZE);
        size_t width = BytesToNumber(dib_header + image::utils::HEADER_WIDTH_OFFSET);
        size_t height = BytesToNumber(dib_header + image::utils::HEADER_HEIGHT_OFFSET);

        Image image(height, width);

        std::vector<std::vector<Color>> data(height);
        std::vector<Color> row(width);

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                unsigned char pixel[image::utils::BYTES_PER_PIXEL];
                image_file.read(reinterpret_cast<char*>(pixel), image::utils::BYTES_PER_PIXEL);
                row[j].SetColors(static_cast<uint8_t>(pixel[0]), static_cast<uint8_t>(pixel[1]),
                                 static_cast<uint8_t>(pixel[2]));
            }
            data[i] = row;
            image_file.ignore(static_cast<std::streamsize>(GetPaddingSize(width)));
        }
        image_file.close();
        return Image(data);

    } catch (std::exception& e) {
        image_file.close();
        throw "ERROR WHILE READING FILE" + file_name_ + ", " + static_cast<std::string>(e.what());
    }
}
