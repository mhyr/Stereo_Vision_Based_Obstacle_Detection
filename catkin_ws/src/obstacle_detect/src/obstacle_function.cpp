#include "obstacle.hpp"

ObjectDetect::ObjectDetect()
{


}

ObjectDetect::~ObjectDetect()
{

}

void ObjectDetect::Detection(void)
{
  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub1 = it.subscribe("/obstacle_detect/frame1", 1, &ObjectDetect::imageCallbackL,this);
  image_transport::Subscriber sub2 = it.subscribe("/obstacle_detect/frame2", 1, &ObjectDetect::imageCallbackR,this);
  ros::spin();
}


void ObjectDetect::imageCallbackL(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    m_imgOriginalL_big = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
    Read_Image_Flag = true;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void ObjectDetect::imageCallbackR(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    m_imgOriginalR_big = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
    if (Read_Image_Flag)
    {
    ObjectDetect::findObstacle();
    }
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void ObjectDetect::Detection(int photo_series)
{
  ////---Here I bypassed the publisher and read a list of preprapared images. should be removed in the last version----------------
  int start_s=clock();

  for(int i=50+20*photo_series;i<100;i++)
      {
      oss.str("");
      oss << "images/wide1"<<photo_series<<"/wide_left-0" << i << ".jpg";
      m_imgOriginalL_big = imread(oss.str());          // open image1
      if (m_imgOriginalL_big.empty())						// if unable to open image1
        cout << "Cannot open image1"<< endl;

      oss.str("");
      oss << "images/wide1"<<photo_series<<"/wide_right-0" << i << ".jpg";
      m_imgOriginalR_big = imread(oss.str());          // open image2
      if (m_imgOriginalR_big.empty())						// if unable to open image2
        cout << "Cannot open image2" << endl;

      ObjectDetect::findObstacle();
  } //

  for(int i=100;i<286+5*photo_series;i++)
      {
      oss.str("");
      oss << "images/wide1"<<photo_series<<"/wide_left-" << i << ".jpg";
      m_imgOriginalL_big = imread(oss.str());          // open image1
      if (m_imgOriginalL_big.empty())						// if unable to open image1
        cout << "Cannot open image1"<< endl;

      oss.str("");
      oss << "images/wide1"<<photo_series<<"/wide_right-" << i << ".jpg";
      m_imgOriginalR_big = imread(oss.str());          // open image2
      if (m_imgOriginalR_big.empty())						// if unable to open image2
        cout << "Cannot open image2" << endl;

      ObjectDetect::findObstacle();
  }
  int stop_s=clock();
  cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
  return;
}

void ObjectDetect::findObstacle()
{

  ObjectDetect::imageCrop();;
  ObjectDetect::DisparityMap();
  ObjectDetect::applyMask();
  ObjectDetect::linearRegression();
  ObjectDetect::isolateObject();
  ObjectDetect::findObjectChar();
  ObjectDetect::showResults();

  return;
}

void ObjectDetect::imageCrop(){
    ///This is not exactly ROI but reduces image size in accordance to ROI so the calculations will be faster
    m_imgOriginalL = m_imgOriginalL_big( Rect(ROI_X4, ROI_Y1, (ROI_X3-ROI_X4), (ROI_Y3-ROI_Y1)));
    m_imgOriginalR = m_imgOriginalR_big( Rect(ROI_X4, ROI_Y1, (ROI_X3-ROI_X4), (ROI_Y3-ROI_Y1)));
    return;
}

void ObjectDetect::DisparityMap()
{
  ///-- 1. convert to grayscale
  Mat gray_imageL, gray_imageR;
  cvtColor( m_imgOriginalL, gray_imageL, CV_BGR2GRAY );
  cvtColor( m_imgOriginalR, gray_imageR, CV_BGR2GRAY );
  
  ///-- 2. create the image in which we will save our disparities
  imgDisparity32F = Mat( gray_imageL.rows, gray_imageL.cols, CV_32F );
  imgDisparity8U = Mat( gray_imageL.rows, gray_imageL.cols, CV_8UC1 );

  ///-- 3. Call the constructor for StereoBM
  int ndisparities = 16*5;   /**< Range of disparity 16*5*/
  int SADWindowSize = 23; /**< Size of the block window. Must be odd 23*/
  StereoBM sbm(StereoBM::BASIC_PRESET, ndisparities, SADWindowSize );

  ///-- 4. Calculate the disparity image
  sbm(gray_imageL, gray_imageR, imgDisparity32F, CV_32F);

  ///-- 5. Check its extreme values
  //minMaxLoc( imgDisparity32F, &minVal, &maxVal );

  ///-- 6. Display it as a CV_8UC1 image
  //imgDisparity32F.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));

return;
}

void ObjectDetect::applyMask()
{
  ///-- Apply a mask to remove areas outside ROI
  Mat mask = Mat( imgDisparity32F.rows, imgDisparity32F.cols, CV_8UC1, Scalar(0,0,0));
  grayScaleDrawing = imgDisparity32F.clone();
  Point mask_points[1][4];
  mask_points[0][0]  = Point(ROI_X1-ROI_X4, 0);
  mask_points[0][1]  = Point(ROI_X2-ROI_X4, ROI_Y2-ROI_Y1);
  mask_points[0][2]  = Point(ROI_X3-ROI_X4, ROI_Y3-ROI_Y1);
  mask_points[0][3]  = Point(0, ROI_Y4-ROI_Y1);
  const Point* ppt[1] = { mask_points[0] };
  int npt[] = { 4 };
  int lineType = 8;
  fillPoly( mask, ppt, npt, 1, Scalar( 1, 0, 0 ), lineType );

  for(int i = 0; i<mask.rows; i++)
    for(int j=0; j<mask.cols; j++)
      if(!mask.at<uchar>(Point(j,i)))
	grayScaleDrawing.at<float>(Point(j,i))=0;
return;
}

void ObjectDetect::linearRegression(){


//---------------------fitting from a set of points supposedly we know these point are on the ground-------------------
 vector<Point> NewPoints;
 Vec4f fitedLine;
 double newSlope, newYIntercept;
 for(int i=0;i<planePoints.size();i++)
     NewPoints.push_back(Point(planePoints[i].x,grayScaleDrawing.at<float>(Point(planePoints[i].y,planePoints[i].x))));

  fitLine(NewPoints,fitedLine,CV_DIST_L1,0,0.01,0.01);

  newSlope = fitedLine[1]/fitedLine[0];
  newYIntercept = fitedLine[3]-slope*fitedLine[2];
  if(newSlope>0.95*slope && newSlope<1.05*slope) slope = newSlope;    //some measurement to make sure we don't dtetct other points as the groud plane.
  if(newYIntercept>0.95*yIntercept && newYIntercept<1.05*yIntercept) yIntercept=newYIntercept; // the steepest railway in europe does't exceed 5% slope so more than that should be an error

return;
}

void ObjectDetect::isolateObject(){

  ///-- Remove pixels which belongs to tha plane
  for(int i = 0; i<grayScaleDrawing.rows; i++)
    for(int j=0; j<grayScaleDrawing.cols; j++)
      if(grayScaleDrawing.at<float>(Point(j,i)) < yIntercept+slope*i+margine)
        grayScaleDrawing.at<float>(i, j)=0;
return;
}


void ObjectDetect::findObjectChar(){

  ///--Find distance and center of the mass
  distance = 0;
  if (countNonZero(grayScaleDrawing) > 0) {

  Scalar sum_of_disparity = sum(grayScaleDrawing);
  int countNonZero_of_disparity = countNonZero(grayScaleDrawing);
  distance = sum_of_disparity[0]/ (double)countNonZero_of_disparity;
  distance = 24*500/distance; // D = b*f/d where D= distance in cm and f= Focal length = 500 px and b= Base line = 24 cm and d =disparity in 32F format

  Moments m = moments(grayScaleDrawing, false);
  centerOfMass.x = (m.m10/m.m00)+ROI_X4;  centerOfMass.y = (m.m01/m.m00)+ROI_Y1;
  }

return;
}

void ObjectDetect::showResults(){

//--1. Draw the results on the image
  Mat finalDrawing(1320, 1320,CV_8UC3,Scalar(0,0,0));
  Rect modImg_ROI(240, 0,480,640);
  Mat modImg = m_imgOriginalL_big.clone();
  Mat objectImage;



  if(distance){
  circle(modImg, centerOfMass, 20, Scalar(128,0,0), -1);
  circle(modImg, centerOfMass, 5, Scalar(0,0,128), -1);
  char str[200];
  ostringstream oss1;
  oss1.str("");
  oss1 << "Distance = " << distance<<"cm";
  putText(modImg, oss1.str(), Point2f(100,100), FONT_HERSHEY_SIMPLEX, 1,  Scalar(0,0,255,255));
  putText(modImg, oss.str(), Point2f(20,20), FONT_HERSHEY_PLAIN,0.5,  Scalar(0,0,255,255));
}


//--2. some extra code to show what computer sees as an object  && more general disparity image
  minMaxLoc( grayScaleDrawing, &minVal, &maxVal );
  grayScaleDrawing.convertTo( objectImage, CV_8UC1, 255/(maxVal - minVal));
  threshold( objectImage, objectImage, 20, 255,0 );

  minMaxLoc( imgDisparity32F, &minVal, &maxVal );
  imgDisparity32F.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));


//--2. Show the results
  namedWindow("Obstacle",CV_WINDOW_NORMAL);
  cvSetWindowProperty("Obstacle", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
  imshow( "Obstacle", modImg);
  imshow("disparity",imgDisparity8U);

//--4. If user Press Esc Key, jumps out of the program
  char key = (char)waitKey(1);
  if( key  == ESC_KEY )
   throw std::invalid_argument( "received ESC KEY" );
  return;
}
