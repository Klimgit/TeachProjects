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
        CPP/image-processor/Filters/Filters.cpp
        CPP/buffered-channel/buffered-channel.h
        CPP/cow-vector/cow-vector.h
        CPP/intrusive-list/intrusive-list.h
        CPP/lru-cache/lru-cache.h
        CPP/lru-cache/lru-cache.cpp
        CPP/mpsc-stack/mpsc-stack.h
        CPP/queue/queue.h
        CPP/queue/queue.cpp
        CPP/rw-lock/rw-lock.h
        CPP/rw-spinlock/rw-spinlock.h
        CPP/semaphore/sema.h
        CPP/stack/stack.h
        CPP/stack/stack.cpp
        CPP/string-view/string-view.h
        CPP/timerqueue/timerqueue.h
        CPP/compressed-pair/compressed-pair.h
        CPP/any/any.h
        Algorithms_and_Data_Structures/Aho-Corasic/aho-corasic.cpp)
