cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)

project("libstrata" VERSION 0.1.0 LANGUAGES C)

# Project configurations
option(BUILD_SHARED_LIBS "Build shared library" ON)

# Target Configurations
add_library(libstrata_static STATIC)
set_target_properties(libstrata_static PROPERTIES OUTPUT_NAME "strata")
if (BUILD_SHARED_LIBS)
    add_library(libstrata SHARED)
    set_target_properties(libstrata PROPERTIES OUTPUT_NAME "strata")
endif()

# Compile Options
target_compile_features(libstrata_static PUBLIC c_std_11)
target_compile_options(libstrata_static PUBLIC 
    $<$<COMPILE_LANGUAGE:C>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors
        -nostdlib
        -ffreestanding
    >
)
if (BUILD_SHARED_LIBS)
    target_compile_features(libstrata PUBLIC c_std_11)
    target_compile_options(libstrata PUBLIC 
        $<$<COMPILE_LANGUAGE:C>:
            -Werror
            -Wall
            -Wno-unused-function
            -Wno-unused-variable
            -Wno-unused-but-set-variable
            -pedantic
            -pedantic-errors
            -nostdlib
            -ffreestanding
        >
    )
endif()

target_include_directories(libstrata_static PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(libstrata_static PRIVATE "${CMAKE_BINARY_DIR}")

if (BUILD_SHARED_LIBS)
    target_include_directories(libstrata PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_include_directories(libstrata PRIVATE "${CMAKE_BINARY_DIR}")
endif()

# Subdirectories
add_subdirectory(src)

# Install
if (BUILD_SHARED_LIBS)
    install(TARGETS libstrata_static libstrata RUNTIME DESTINATION lib)
else()
    install(TARGETS libstrata_static RUNTIME DESTINATION lib)
endif()
install(DIRECTORY include/strata/ DESTINATION include/strata FILES_MATCHING PATTERN "*.h")
