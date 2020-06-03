cmake_minimum_required(VERSION 3.16)
project(raidstream
    LANGUAGES CXX
)
set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#find_package(schifra REQUIRED)
set(RAIDSTREAM_SOURCES
        src/RaidStream/crc32/crc32.cpp
        src/RaidStream/crc32/crc32.hpp
        src/RaidStream/crc64/crc64.hpp
        src/RaidStream/crc64/crc64.cpp
        src/RaidStream/RaidStream.hpp
        src/RaidStream/RaidStream.cpp
        src/RaidStream/RaidConfiguration.hpp
        src/RaidStream/RaidConfiguration.cpp
        src/RaidStream/RaidFile.hpp
        src/RaidStream/RaidFile.cpp
        src/RaidStream/RaidFileBlock.hpp
        src/RaidStream/RaidFileBlock.cpp
        src/RaidStream/RaidFileStripe.hpp
        src/RaidStream/RaidFileStripe.cpp
        src/RaidStream/RaidSuperBlock.hpp
        src/RaidStream/RaidSuperBlock.cpp
        src/RaidStream/RaidSuperStripe.hpp
        src/RaidStream/RaidSuperStripe.cpp
        src/RaidStream/test.cpp
)
add_library(raidstream-object OBJECT ${RAIDSTREAM_SOURCES})
add_library(raidstream-static STATIC $<TARGET_OBJECTS:raidstream-object>)
add_library(raidstream-shared SHARED $<TARGET_OBJECTS:raidstream-object>)
target_include_directories(raidstream-object SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(raidstream-static SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(raidstream-shared SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Assume platform is linux until proven otherwise.
set(CMAKE_PLATFORM_TEST "IS_LINUX")
#if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#    set(CMAKE_PLATFORM_TEST "IS_LINUX")
#endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_PLATFORM_TEST "IS_MACOS")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_PLATFORM_TEST "IS_WINDOWS")
endif()
target_compile_definitions(raidstream-object PUBLIC "${CMAKE_PLATFORM_TEST}")
target_compile_definitions(raidstream-static PUBLIC "${CMAKE_PLATFORM_TEST}")
target_compile_definitions(raidstream-shared PUBLIC "${CMAKE_PLATFORM_TEST}")

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/src/sole/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(src/sole)
#target_include_directories(raidstream-object SYSTEM PUBLIC ${schifra_INCLUDE_DIR})
#message(STATUS ${schifra})

target_link_libraries(raidstream-object sole)
target_include_directories(raidstream-object SYSTEM PUBLIC src/sole)

add_executable(raidstream-test src/RaidStream/test.cpp)
target_link_libraries(raidstream-test raidstream-static)
target_include_directories(raidstream-test SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")