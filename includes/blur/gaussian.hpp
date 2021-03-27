#include <opencv2/opencv.hpp>
#include <iostream>

#ifndef _OHAYOU_GAUSSIAN_H
#define _OHAYOU_GAUSSIAN_H
using namespace cv;
using namespace std;
namespace ohayou{
    cv::Mat gaussian_blur(Mat img, int radius){
        Mat image_blurred;
        cv::GaussianBlur(img, image_blurred, Size(radius, radius), 0);
        return image_blurred;
    }
};
#endif //_OHAYOU_GAUSSIAN_H
