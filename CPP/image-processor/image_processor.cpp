#include "image_processor.h"
#include "Filters/Filters.h"
#include "Reader_and_Writer/Reader.h"
#include "Reader_and_Writer/Writer.h"
#include "Parsing/Parser.h"

namespace image_processing {

    Image ImageProcessor::ApplyFilters(const Image& image, const std::vector<FilterToken>& filter_tokens) const {
        Image result_image = image;

        for (const FilterToken& filter_token : filter_tokens) {
            std::unique_ptr<filter::Filter> filter = filter::CreateFilter(filter_token.token);
            if (filter) {
                result_image = filter->Apply(result_image);
            } else {
                std::cerr << "Broken_token: " << filter_token.token.name << std::endl;
            }
        }

        return Image(result_image);
    }

}  // namespace image_processing

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <filter1> [<filter2> ...]" << std::endl;
        return 1;
    }

    std::vector<parser::Token> tokens = parser::Parse(argc, argv);
    std::vector<image_processing::FilterToken> filter_tokens;
    for (size_t i = 2; i < tokens.size(); ++i) {
        image_processing::FilterToken filter_token;
        filter_token.token = tokens[i];
        filter_tokens.push_back(filter_token);
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    reading_and_writing::Reader reader(input_file);
    Image input_image = reader.Read();

    image_processing::ImageProcessor processor;
    Image processed_image = processor.ApplyFilters(input_image, filter_tokens);

    reading_and_writing::Writer writer(output_file);
    writer.Write(processed_image);
    std::cout << "Successfully." << std::endl;

    return 0;
}
