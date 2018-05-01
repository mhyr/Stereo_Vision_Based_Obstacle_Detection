#ifndef OBSTACLE_IMG_H
#define OBSTACLE_IMG_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include "ros/ros.h"
#include "cv_bridge/cv_bridge.h"
#include <image_transport/image_transport.h>
#include "obstacle_detect/Frame1.h"
#include "obstacle_detect/Frame2.h"

using namespace std;
using namespace cv;


class Obstacle_IMG
{
public:
  Obstacle_IMG(const string &path1, const string &path2);

private:
  ros::NodeHandle n;
  ros::Rate loop_rate;
  image_transport::Publisher frame_pub1, frame_pub2;
  obstacle_detect::Frame1 msg_frame1;
  obstacle_detect::Frame2 msg_frame2;
};

class Preselected_IMG
{
public:
  Preselected_IMG();

private:
  ros::NodeHandle n;
  ros::Rate loop_rate;
  image_transport::Publisher frame_pub1, frame_pub2;
  obstacle_detect::Frame1 msg_frame1;
  obstacle_detect::Frame2 msg_frame2;
};

#endif // OBSTACLE_IMG_H
