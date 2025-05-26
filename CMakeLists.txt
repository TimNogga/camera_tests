cmake_minimum_required(VERSION 3.15)
project(OrbbecSDKTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fixed SDK path
set(OrbbecSDK_DIR "/home/timnogga/OrbbecSDK" CACHE PATH "")
list(APPEND CMAKE_PREFIX_PATH "${OrbbecSDK_DIR}")

find_package(OrbbecSDK REQUIRED CONFIG)

find_package(OpenCV REQUIRED)          # grabs core + imgcodecs

add_executable(${PROJECT_NAME}
    main.cpp
    src/FrameSaver.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OrbbecSDK::OrbbecSDK
    ${OpenCV_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
