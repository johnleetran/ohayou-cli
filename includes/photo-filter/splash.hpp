#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#ifndef _OHAYOU_SPLASH_H
#define _OHAYOU_SPLASH_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat getSplashMask(cv::Mat img, cv::Scalar lower_bound, cv::Scalar upper_bound)
    {
        cv::Mat hsv_img;
        cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

        cv::Mat hsv;
        cv::inRange(hsv_img, lower_bound, upper_bound, hsv);

        return std::move(hsv);
    }

    //In Opencv their maximum values of Hue range from 0–180 and of Saturation and Brightness from 0–255
    cv::Mat apply_splash(Mat img, cv::Scalar lower_bound, cv::Scalar upper_bound)
    {
        cv::Mat mask = getSplashMask(img, lower_bound, upper_bound);

        cv::Mat inv_mask;
        cv::bitwise_not(mask, inv_mask);
        
        cv::Mat gray; 
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        cv::Mat res_img;
        cv::bitwise_and(img, img, res_img, mask);

        cv::Mat res_gray;
        cv::bitwise_and(gray, gray, res_gray, inv_mask);

        cv::Mat output_img;
        cv::Mat res_gray_color;
        cv::cvtColor(res_gray, res_gray_color, cv::COLOR_GRAY2BGR);
        cv::bitwise_or(res_img, res_gray_color, output_img);
        return output_img;
    }
};
#endif //_OHAYOU_SPLASH_H
