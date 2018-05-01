/*!
 * \file	obstacle_detect
 * \author	Abish Asphandiar <abish@uni-bremen.de> 3020632
 * \modify	Mahyar Pedram ,pedram@uni-bremen.de> 3020826
 * \date	19-April-2016
 * \brief Publishing  videos
 */

#include "obstacle_vid.hpp"

Obstacle_VID::Obstacle_VID(const string &path1, const string &path2):
  loop_rate(30)
{
  frame_pub1 = n.advertise<obstacle_detect::Frame1>("/obstacle_detect/frame1", 1);
  frame_pub2 = n.advertise<obstacle_detect::Frame2>("/obstacle_detect/frame2", 1);
  Mat frame1, frame2;

  m_video1.open(path1);
  if (m_video1.isOpened() == false)
    cerr << "error: Video1 not accessed successfully" << endl;

  m_video2.open(path2);
  if (m_video2.isOpened() == false)
    cerr << "error: Video2 not accessed successfully" << endl;

  while(m_video1.isOpened() && m_video2.isOpened() && ros::ok())
  {
    if (!m_video1.read(frame1))
    {
      cerr << "error: frame not read from video1" << endl;
    }

    if (!m_video2.read(frame2))
    {
      cerr << "error: frame not read from video2" << endl;
    }

    msg_frame1.timestamp.stamp = ros::Time::now();
    msg_frame2.timestamp.stamp = ros::Time::now();
    sensor_msgs::ImagePtr im_msg1 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame1).toImageMsg();
    sensor_msgs::ImagePtr im_msg2 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame2).toImageMsg();
    msg_frame1.orig_image = *im_msg1;
    msg_frame2.orig_image = *im_msg2;
    frame_pub1.publish(msg_frame1);
    frame_pub2.publish(msg_frame2);
    ros::spinOnce();
    loop_rate.sleep();
  }
}

Obstacle_VID::~Obstacle_VID()
{
  m_video1.release();
  m_video2.release();
}
