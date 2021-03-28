#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <opencv2/xphoto/white_balance.hpp>

#ifndef _OHAYOU_WHITE_BALANCE_H
#define _OHAYOU_WHITE_BALANCE_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat apply_white_balance(Mat img, float threshold = 0.99)
    {
        Mat output_img;
        auto wb = cv::xphoto::createGrayworldWB();
        wb->setSaturationThreshold(threshold);
        wb->balanceWhite(img, output_img);
        return output_img;
    }
};
#endif //_OHAYOU_WHITE_BALANCE_H
