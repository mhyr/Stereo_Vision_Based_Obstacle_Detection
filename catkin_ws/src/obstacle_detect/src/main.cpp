#include "obstacle.hpp"

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "ObstacleDetectMain");
  try {
      ObjectDetect ObjectOnRail;
  if (argc>1)
  {
    int x = atoi(argv[1]);
    ObjectOnRail.Detection(x);
  }
  else
     ObjectOnRail.Detection();
  }
  catch( const std::invalid_argument& e ) {
  return -1;
}
  return 0;
}
