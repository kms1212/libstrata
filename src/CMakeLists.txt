cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)

target_sources(libstrata_static PRIVATE handle.c)

if (BUILD_SHARED_LIBS)
    target_sources(libstrata PRIVATE handle.c)
endif()
