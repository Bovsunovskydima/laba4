cmake_minimum_required(VERSION 3.10)
project(ThreadSafeDataStructure)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/DataStructure.cpp
    src/FileGenerator.cpp
)

set(HEADERS
    include/DataStructure.h
    include/FileGenerator.h
)

include_directories(include)

add_executable(thread_safe_demo ${SOURCES} ${HEADERS})

find_package(Threads REQUIRED)
target_link_libraries(thread_safe_demo Threads::Threads)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(thread_safe_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
