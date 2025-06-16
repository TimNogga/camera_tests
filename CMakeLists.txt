cmake_minimum_required(VERSION 3.15)
project(OrbbecSDKTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Fixed SDK path ───────────────────────────────────────────────────────────
set(OrbbecSDK_DIR "/home/timnogga/OrbbecSDK" CACHE PATH "")
list(APPEND CMAKE_PREFIX_PATH "${OrbbecSDK_DIR}")

find_package(OrbbecSDK REQUIRED CONFIG)
find_package(OpenCV REQUIRED)   # grabs core + imgcodecs

# ── First executable: OrbbecSDKTest ──────────────────────────────────────────
add_executable(OrbbecSDKTest
    main.cpp
    src/FrameSaver.cpp
)

target_include_directories(OrbbecSDKTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(OrbbecSDKTest PRIVATE
    OrbbecSDK::OrbbecSDK
    ${OpenCV_LIBS}
)

set_target_properties(OrbbecSDKTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ── Second executable: DeviceInfoExample ─────────────────────────────────────
add_executable(DeviceInfoExample
    DeviceInfoExample.cpp
    src/FrameSaver.cpp            
)

target_include_directories(DeviceInfoExample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(DeviceInfoExample PRIVATE
    OrbbecSDK::OrbbecSDK
    ${OpenCV_LIBS}                # remove if you don’t actually use OpenCV here
)

set_target_properties(DeviceInfoExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Third Executable SetDeviceMode
add_executable(SetDeviceMode
    SetDeviceMode.cpp
)
target_link_libraries(SetDeviceMode PRIVATE
    OrbbecSDK::OrbbecSDK
    ${OpenCV_LIBS}              
)
set_target_properties(SetDeviceMode PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)