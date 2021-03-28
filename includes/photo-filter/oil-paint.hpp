#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <opencv2/xphoto/oilpainting.hpp>

#ifndef _OHAYOU_OIL_PAINT_H
#define _OHAYOU_OIL_PAINT_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat apply_oil_paint(Mat img, int size)
    {
        cv::Mat output_img;
        xphoto::oilPainting(img, output_img, size, 1, cv::COLOR_BGR2Lab);
        return output_img;
    }
};
#endif //_OHAYOU_SPLASH_H
