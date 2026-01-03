cmake_minimum_required(VERSION 3.10)
project(HogwartsLegacy_Xbox_To_Epic)

set(CMAKE_CXX_STANDARD 17)

# Source files
set(SOURCES
    src/HL_save_converter.cpp
    src/HL_save_utils.cpp
)

# Header files (optional, for IDEs)
set(HEADERS
    src/HL_save_utils.h
)

add_executable(HogwartsLegacy_Xbox_To_Epic ${SOURCES} ${HEADERS})

# Add additional libraries here if needed, e.g. filesystem for C++17
if (MSVC)
    target_compile_options(HogwartsLegacy_Xbox_To_Epic PRIVATE /W4)
else()
    target_compile_options(HogwartsLegacy_Xbox_To_Epic PRIVATE -Wall -Wextra -pedantic)
endif()
