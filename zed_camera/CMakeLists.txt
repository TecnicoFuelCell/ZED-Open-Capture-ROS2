cmake_minimum_required(VERSION 3.5)
project(zed_camera)

# Add required definitions
add_definitions(-DVIDEO_MOD_AVAILABLE)
add_definitions(-DSENSORS_MOD_AVAILABLE)
add_definitions(-DSL_OC_COMPIL)

# Check if we're on ARM (Jetson)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    add_definitions(-DSL_JETSON)
endif()

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(rclcpp REQUIRED)
find_package(OpenCV REQUIRED)


set(ZED_OPEN_CAPTURE_ROOT "/opt/share/zed-open-capture")

# Add include directory
include_directories(
    ${ZED_OPEN_CAPTURE_ROOT}/include
    ${ZED_OPEN_CAPTURE_ROOT}/src/include
    ${ZED_OPEN_CAPTURE_ROOT}/examples/include
)

add_executable(zed_publisher src/zed_publisher.cpp)

# Link libraries (ZED Open Capture and OpenCV)
target_link_libraries(zed_publisher
    ${ZED_OPEN_CAPTURE_ROOT}/build/libzed_open_capture.so
    ${OpenCV_LIBRARIES}
)

ament_target_dependencies(zed_publisher rclcpp std_msgs sensor_msgs cv_bridge OpenCV)

install(TARGETS
  zed_publisher
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
