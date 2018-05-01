/*!
 * \file	obstacle_detect
 * \author	Abish Asphandiar <abish@uni-bremen.de> 3020632
 * \modify	Mahyar Pedram ,pedram@uni-bremen.de> 3020826
 * \date	19-April-2016
 * \brief Initializing the object to publish either images or videos based on the extension
 */

#include "obstacle_img.hpp"
#include "obstacle_vid.hpp"

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "ObstacleDetector");

  if (argc != 3)
  {
    cout << "Please enter two file names" << endl;
    Preselected_IMG();
    return -1;
  }

  string path1 =  string(argv[1]);
  string path2 =  string(argv[2]);

  if ((path1.find(".mp4") != std::string::npos) || (path1.find(".avi") != std::string::npos))
    Obstacle_VID vid_obstacle(path1, path2);
  else
    Obstacle_IMG img_obstacle(path1, path2);

  return 0;
}
