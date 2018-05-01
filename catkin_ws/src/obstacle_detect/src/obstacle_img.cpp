/*!
 * \file	obstacle_detect
 * \author	Abish Asphandiar <abish@uni-bremen.de> 3020632
 * \modify	Mahyar Pedram ,pedram@uni-bremen.de> 3020826
 * \date	19-April-2016
 * \brief Publishing images
 */

#include "obstacle_img.hpp"

Obstacle_IMG::Obstacle_IMG(const string &path1, const string &path2):
  loop_rate(30)
{
  image_transport::ImageTransport it(n);
  frame_pub1 = it.advertise("/obstacle_detect/frame1", 1);
  frame_pub2 = it.advertise("/obstacle_detect/frame2", 1);
  Mat frame1 = imread(path1);          // open image1
  if (frame1.empty())						// if unable to open image1
    cout << "Cannot open image1" << endl;
  Mat frame2 = imread(path2);          // open image2
  if (frame2.empty())						// if unable to open image2
    cout << "Cannot open image2" << endl;

  while(!frame1.empty() && !frame2.empty() && ros::ok())
  {
    sensor_msgs::ImagePtr im_msg1 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame1).toImageMsg();
    sensor_msgs::ImagePtr im_msg2 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame2).toImageMsg();
    frame_pub1.publish(im_msg1);
    frame_pub2.publish(im_msg2);
    ros::spinOnce();
    loop_rate.sleep();
  }
}

Preselected_IMG::Preselected_IMG():
  loop_rate(30)
{
  image_transport::ImageTransport it(n);
  frame_pub1 = it.advertise("/obstacle_detect/frame1", 1);
  frame_pub2 = it.advertise("/obstacle_detect/frame2", 1);
  ostringstream ossL, ossR;
  string sstmL, sstmR;
  Mat frame1,frame2;


  while(ros::ok())
  {
    for(int i=70;i<81;i++)
      {
      ossL.str("");
      ossL << "wide_left-0" << i << ".jpg";
      sstmL = ossL.str();
      frame1 = imread(sstmL);          // open image1
      if (frame1.empty())						// if unable to open image1
        cout << "Cannot open "<<sstmL << endl;
      ossR.str("");
      ossR << "wide_right-0" << i << ".jpg";
      sstmR = ossR.str();
      frame2 = imread(sstmR);          // open image2
      if (frame2.empty())						// if unable to open image2
        cout << "Cannot open image2" << endl;
      
      sensor_msgs::ImagePtr im_msg1 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame1).toImageMsg();
      sensor_msgs::ImagePtr im_msg2 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame2).toImageMsg();
      frame_pub1.publish(im_msg1);
      frame_pub2.publish(im_msg2);
      ros::spinOnce();
      loop_rate.sleep();
      }
  }
}
