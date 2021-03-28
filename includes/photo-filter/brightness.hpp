#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#ifndef _OHAYOU_BRIGHTNESS_H
#define _OHAYOU_BRIGHTNESS_H
using namespace cv;
using namespace std;
namespace ohayou
{

    cv::Mat clamp_value(cv::Mat img, float intensity, float cap)
    {
        for(int i=0; i<img.rows; i++){
            for(int j=0; j<img.cols; j++){
                img.at<uchar>(i, j) = std::min(img.at<uchar>(i, j) * intensity, cap);
            }
        }
        return img;
    }

    cv::Mat apply_brightness_fancy(Mat img, float intensity)
    {
        cv::Mat hsv_img;
        cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);
        Mat channels[3];
        cv::split(hsv_img, channels);
        //channels[1] *= 1.0/intensity;
        //channels[1] = clamp_value(channels[1], intensity, 255.f);
        channels[2] = clamp_value(channels[2], intensity, 255.f);

        cv::Mat output_img;
        cv::merge(channels, 3, output_img);
        cv::cvtColor(output_img, output_img, cv::COLOR_HSV2BGR);
        return output_img;
    }

    cv::Mat apply_brightness(Mat img, float level = 0.0)
    {
        cv::Mat output_img;
        cv::convertScaleAbs(img, output_img, 1.0, level);
        return output_img;
    }
};
#endif //_OHAYOU_BRIGHTNESS_H
