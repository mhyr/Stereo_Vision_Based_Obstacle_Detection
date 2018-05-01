cmake_minimum_required(VERSION 2.8)
project(calibration)

find_package(catkin REQUIRED cv_bridge genmsg image_transport sensor_msgs)

catkin_package()

find_package(OpenCV)

include_directories(include ${OpenCV_INCLUDE_DIRS})

# add calibration
add_executable(calibration src/calibration.cpp)
target_link_libraries(calibration ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

