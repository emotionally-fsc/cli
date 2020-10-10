cmake_minimum_required(VERSION 3.15)
project(emotions)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

# Affdex package
# ----------------------------------------------------------------------------
set(AFFDEX_DIR "lib/affdex-sdk/")
set(AFFDEX_FOUND FALSE)

if (DEFINED AFFDEX_DIR)
    find_path(AFFDEX_INCLUDE_DIR FrameDetector.h
            HINTS "${AFFDEX_DIR}/include")

    find_library(AFFDEX_LIBRARY NAMES affdex-native
            HINTS "${AFFDEX_DIR}/lib")

    set(AFFDEX_INCLUDE_DIRS "${AFFDEX_INCLUDE_DIR}")
    set(AFFDEX_LIBRARIES "${AFFDEX_LIBRARY}")

    if (AFFDEX_INCLUDE_DIR AND AFFDEX_LIBRARY)
        set(AFFDEX_FOUND TRUE)
    endif (AFFDEX_INCLUDE_DIR AND AFFDEX_LIBRARY)

    set(AFFDEX_DATA_DIR "${AFFDEX_DIR}/data")


    if (NOT AFFDEX_FOUND)
        message(FATAL_ERROR "Unable to find the Affdex found")
    endif (NOT AFFDEX_FOUND)

else (DEFINED AFFDEX_DIR)
    message(FATAL_ERROR "Please define AFFDEX_DIR")
endif (DEFINED AFFDEX_DIR)

include_directories(src/include/)
link_directories(src/include/rapidjson/)
include_directories(${AFFDEX_INCLUDE_DIRS})
link_directories(${AFFDEX_LIBRARIES})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_executable(emotions src/main.cpp src/utilities.cpp src/base64.cpp src/data_uri.cpp src/common/Visualizer.cpp src/common/PlottingImageListener.cpp)
target_include_directories(emotions PRIVATE ${Boost_INCLUDE_DIRS} ${AFFDEX_INCLUDE_DIRS})
target_link_libraries(emotions ${AFFDEX_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
