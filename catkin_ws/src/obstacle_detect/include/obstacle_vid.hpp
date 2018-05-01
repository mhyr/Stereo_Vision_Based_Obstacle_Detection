#ifndef OBSTACLE_VID_H
#define OBSTACLE_VID_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include "ros/ros.h"
#include "cv_bridge/cv_bridge.h"
#include "obstacle_detect/Frame1.h"
#include "obstacle_detect/Frame2.h"

using namespace std;
using namespace cv;

class Obstacle_VID
{
public:
  Obstacle_VID(const string &path1, const string &path2);
  ~Obstacle_VID();

private:
  VideoCapture m_video1, m_video2;
  ros::NodeHandle n;
  ros::Rate loop_rate;
  ros::Publisher frame_pub1, frame_pub2;
  obstacle_detect::Frame1 msg_frame1;
  obstacle_detect::Frame2 msg_frame2;
};

#endif // OBSTACLE_VID_H
