cmake_minimum_required(VERSION 3.15)
project(OrbbecSDKSamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Where the SDK lives ────────────────────────────────────────────────────
set(OrbbecSDK_DIR "/home/timnogga/OrbbecSDK" CACHE PATH "")
list(APPEND CMAKE_PREFIX_PATH "${OrbbecSDK_DIR}")

find_package(OrbbecSDK REQUIRED CONFIG)      # => OrbbecSDK::OrbbecSDK
find_package(OpenCV    REQUIRED)

# ── Helper library: wraps Orbbec sample "SensorControl.cpp" ────────────────
add_library(OrbbecPropertyHelpers STATIC
    "${OrbbecSDK_DIR}/examples/cpp/Sample-SensorControl/SensorControl.cpp"
)
target_include_directories(OrbbecPropertyHelpers PUBLIC
    "${OrbbecSDK_DIR}/examples/cpp/Sample-SensorControl"
)

# ── Convenience macro: every tool links the same libs, goes in bin/ ────────
function(add_orbbec_tool target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${target} PRIVATE
        OrbbecSDK::OrbbecSDK
        OrbbecPropertyHelpers      # ← gives us get/setPropertyValue
        ${OpenCV_LIBS}
    )
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endfunction()

# ── Your executables ───────────────────────────────────────────────────────
add_orbbec_tool(OrbbecSDKTest       main.cpp                src/FrameSaver.cpp)
add_orbbec_tool(DeviceInfoExample   DeviceInfoExample.cpp   src/FrameSaver.cpp)
add_orbbec_tool(SetDeviceMode       SetDeviceMode.cpp)
add_orbbec_tool(SyncCams            SyncCams.cpp            src/FrameSaver.cpp)
add_orbbec_tool(GetParameters       GetParameters.cpp)
