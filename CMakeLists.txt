cmake_minimum_required(VERSION 3.13)
project(TeachProjects)

set(CMAKE_CXX_STANDARD 14)

include_directories(CPP/smart-ptrs/unique)

add_executable(TeachProjects
        CPP/smart-ptrs/unique/unique.h
        CPP/smart-ptrs/unique/compressed_pair.h
        CPP/smart-ptrs/shared/shared.h
        CPP/smart-ptrs/intrusive/intrusive.h
        CPP/image-processor/image_processor.cpp
        CPP/image-processor/image_processor.h
        CPP/image-processor/Filters/Filters.cpp)
