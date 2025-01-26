#include "Filters.h"

template <typename T>
std::vector<T> filter::Filter::GetPixel(const std::vector<std::vector<T>>& matrix, const Image& image,
                                        const std::size_t x, const std::size_t y) const {
    T red = 0;
    T blue = 0;
    T green = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            red += image
                       .GetColor(std::clamp(i + x - 1, static_cast<size_t>(0), image.GetWidth() - 1),
                                 std::clamp(j + y - 1, static_cast<size_t>(0), image.GetHeight() - 1))
                       .red *
                   matrix[i][j];
            green += image
                         .GetColor(std::clamp(i + x - 1, static_cast<size_t>(0), image.GetWidth() - 1),
                                   std::clamp(j + y - 1, static_cast<size_t>(0), image.GetHeight() - 1))
                         .green *
                     matrix[i][j];
            blue += image
                        .GetColor(std::clamp(i + x - 1, static_cast<size_t>(0), image.GetWidth() - 1),
                                  std::clamp(j + y - 1, static_cast<size_t>(0), image.GetHeight() - 1))
                        .blue *
                    matrix[i][j];
        }
    }
    return {blue, green, red};
}

uint8_t filter::Clamp(int color, const uint8_t& max_color) {
    if (color < 0) {
        return 0;
    } else if (color > max_color) {
        return max_color;
    }
    return static_cast<uint8_t>(color);
}

Image filter::Sharpening::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_data;
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t i = 1; i < image.GetHeight() - 1; ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 1; j < image.GetWidth() - 1; ++j) {
            std::vector<int> colors = GetPixel(matrix, image, i, j);
            row[j] = {Clamp(colors[2]), Clamp(colors[1]), Clamp(colors[0])};
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

Image filter::Negative::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_data;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            row[j] = {static_cast<uint8_t>(MAX_COLOR - image.GetColor(i, j).blue),
                      static_cast<uint8_t>(MAX_COLOR - image.GetColor(i, j).green),
                      static_cast<uint8_t>(MAX_COLOR - image.GetColor(i, j).red)};
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

Image filter::Grayscale::Apply(const Image& image) const {
    std::vector<std::vector<Color>> new_data;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        std::vector<Color> row(image.GetWidth());
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            row[j] = {static_cast<uint8_t>(GRAY_ONE * image.GetColor(i, j).red + GRAY_TWO * image.GetColor(i, j).green +
                                           GRAY_THREE * image.GetColor(i, j).blue),
                      static_cast<uint8_t>(GRAY_ONE * image.GetColor(i, j).red + GRAY_TWO * image.GetColor(i, j).green +
                                           GRAY_THREE * image.GetColor(i, j).blue),
                      static_cast<uint8_t>(GRAY_ONE * image.GetColor(i, j).red + GRAY_TWO * image.GetColor(i, j).green +
                                           GRAY_THREE * image.GetColor(i, j).blue)};
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

Image filter::Crop::Apply(const Image& image) const {
    if (width_ <= 0 || height_ <= 0) {
        throw std::invalid_argument("Broken dimensions");
    }
    if (width_ >= image.GetWidth() || height_ >= image.GetHeight()) {
        return image;
    }
    size_t height = image.GetHeight();
    size_t width = image.GetWidth();
    std::vector<std::vector<Color>> new_data;
    for (size_t i = 0; i < height; i--) {
        std::vector<Color> row(width);
        for (size_t j = 0; j < width; ++j) {
            row[j] = image.GetColor(image.GetHeight() - i - 1, j);
        }
        new_data.push_back(row);
    }
    return Image(new_data);
}

std::unique_ptr<filter::Filter> filter::CreateFilter(const parser::Token& token) {
    const std::string& filter_name = token.name;

    if (filter_name == "-sharp") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid arguments for filter");
        }
        return std::make_unique<Sharpening>();
    } else if (filter_name == "-neg") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid arguments for filter");
        }
        return std::make_unique<Negative>();
    } else if (filter_name == "-neg") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid arguments for filter");
        }
        return std::make_unique<Negative>();
    } else if (filter_name == "-gs") {
        if (!token.args.empty()) {
            throw std::invalid_argument("Invalid arguments for filter");
        }
        return std::make_unique<Grayscale>();
    } else if (filter_name == "-crop") {
        if (token.args.size() != 2) {
            throw std::invalid_argument("Invalid arguments for filter");
        }
        size_t width = static_cast<size_t>(std::stoi(token.args[0]));
        size_t height = static_cast<size_t>(std::stoi(token.args[1]));
        return std::make_unique<filter::Crop>(width, height);
    } else {
        throw std::invalid_argument("Invalid filter name" + filter_name);
    }
}
