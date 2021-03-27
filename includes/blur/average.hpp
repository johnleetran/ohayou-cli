#include <opencv2/opencv.hpp>
#include <iostream>

#ifndef _OHAYOU_AVERAGE_BLUR_H
#define _OHAYOU_AVERAGE_BLUR_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat average_blur(Mat img, int radius)
    {
        Mat image_blurred;
        cv::blur(img, image_blurred, Size(radius, radius));
        return image_blurred;
    }
};
#endif //_OHAYOU_AVERAGE_BLUR_H
