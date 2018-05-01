#ifndef OBSTACLEDETECT_H
#define OBSTACLEDETECT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"
#include "obstacle_detect/Obstacle.h"
#include "obstacle_detect/Frame1.h"
#include "obstacle_detect/Frame2.h"


#define ROI_X1 279
#define ROI_Y1 217
#define ROI_X2 300
#define ROI_Y2 217
#define ROI_X3 557
#define ROI_Y3 479
#define ROI_X4 5
#define ROI_Y4 479






using namespace cv;
using namespace std;

class ObjectDetect
{
public:
  ObjectDetect();
  ~ObjectDetect();
  void Detection(void);
  void Detection(int imageListNo);
  void imageCallbackL(const sensor_msgs::ImageConstPtr& msg);
  void imageCallbackR(const sensor_msgs::ImageConstPtr& msg);
  void findObstacle();
  void imageCrop();
  void DisparityMap();
  void applyMask();
  void linearRegression();
  void isolateObject();
  void findObjectChar();
  void showResults();
  
private:
  ros::NodeHandle n;
  Mat m_imgOriginalL_big, m_imgOriginalR_big;
  Mat m_imgOriginalL, m_imgOriginalR;
  bool Read_Image_Flag=false;
  Mat imgDisparity32F, imgDisparity8U;
  Mat grayScaleDrawing;
  Mat mask;
  double minVal; double maxVal;
  ostringstream oss;
  double slope=0.15831277995278, yIntercept=2.4315884661626, margine=4;
  double distance;
  Point centerOfMass;
  const char ESC_KEY = 27;
  const vector<Point> planePoints{{11,262}, {12,261}, {13,308}, {14,283}, {15,289}, {16,302}, {17,301}, {18,309}, {19,309}, {20,256}, {21,260}, {22,260}, {23,263}, {24,265}, {25,265}, {26,266}, {27,266}, {28,267}, {29,277}, {30,321}, {31,321}, {32,321}, {33,321}, {34,321}, {35,321}, {36,321}, {37,321}, {38,246}, {39,247}, {40,247}, {41,246}, {42,247}, {43,247}, {44,262}, {45,267}, {46,262}, {47,262}, {48,317}, {49,315}, {50,313}, {51,319}, {52,320}, {53,320}, {54,320}, {55,319}, {56,320}, {57,300}, {58,300}, {59,300}, {60,300}, {61,300}, {62,271}, {63,270}, {64,270}, {65,270}, {66,270}, {67,334}, {68,333}, {69,328}, {70,239}, {71,233}, {72,359}, {73,319}, {74,362}, {75,362}, {76,362}, {77,362}, {78,362}, {79,364}, {80,368}, {81,367}, {82,368}, {83,377}, {84,377}, {85,377}, {86,370}, {87,377}, {88,291}, {89,288}, {90,290}, {91,295}, {92,298}, {93,298}, {94,298}, {95,298}, {96,304}, {97,298}, {98,298}, {99,299}, {100,393}, {101,393}, {102,394}, {103,396}, {104,397}, {105,397}, {106,343}, {107,344}, {108,344}, {109,279}, {110,279}, {111,279}, {112,360}, {113,363}, {114,364}, {115,366}, {116,363}, {117,290}, {118,293}, {119,294}, {120,303}, {121,294}, {122,256}, {123,414}, {124,416}, {125,418}, {126,418}, {127,226}, {128,222}, {129,419}, {130,420}, {131,424}, {132,425}, {133,328}, {134,425}, {135,411}, {136,425}, {137,411}, {138,410}, {139,411}, {140,411}, {141,405}, {142,368}, {143,376}, {144,376}, {145,375}, {146,375}, {147,382}, {148,387}, {149,441}, {150,441}, {151,270}, {152,309}, {153,268}, {154,308}, {155,322}, {156,449}, {157,449}, {158,399}, {159,399}, {160,399}, {161,454}, {162,455}, {163,455}, {164,452}, {165,455}, {166,439}, {167,455}, {168,460}, {169,345}, {170,279}, {171,463}, {172,463}, {173,465}, {174,466}, {175,467}, {176,467}, {177,468}, {178,468}, {179,405}, {180,405}, {181,154}, {182,152}, {183,387}, {184,404}, {185,406}, {186,404}, {187,375}, {188,376}, {189,364}, {190,374}, {191,325}, {192,318}, {193,325}, {194,402}, {195,476}, {196,103}, {197,351}, {198,477}, {199,478}, {200,478}, {201,462}, {202,462}, {203,98}, {204,444}, {205,479}, {206,478}, {207,387}, {208,382}, {209,387}, {210,377}, {211,377}, {212,299}, {213,301}, {214,471}, {215,471}, {216,474}, {217,474}, {218,474}, {219,432}, {220,432}, {221,432}, {222,472}, {223,472}, {224,472}, {225,472}, {226,475}, {227,474}, {228,474}, {229,193}, {230,199}, {231,199}, {232,114}, {233,396}, {234,398}, {235,392}, {236,457}, {237,450}, {238,450}, {239,450}, {240,448}, {241,447}, {242,363}, {243,363}, {244,365}, {245,301}, {246,154}, {247,160}, {248,415}, {249,475}, {250,478}};


};

#endif // OBSTACLEDETECT_H
