#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#ifndef _OHAYOU_SUMMER_H
#define _OHAYOU_SUMMER_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat summer_gamma_function(cv::Mat channel, float gamma)
    {
        Mat table(1, 256, CV_8U);
        float invGamma = 1.0/gamma;
        for (int i = 0; i < 256; i++)
        {
            table.at<uchar>(i) = std::pow(i / 255.0, invGamma) * 255.0;
        }
        cv::LUT(channel, table, channel);
        return channel;
    }

    cv::Mat apply_summer(Mat img)
    {
        cv::Mat summer_img;
        Mat summer_channels[3];
        cv::split(img, summer_channels);
        summer_channels[0] = summer_gamma_function(summer_channels[0], 0.75);
        summer_channels[2] = summer_gamma_function(summer_channels[2], 1.25);
        cv::merge(summer_channels, 3, summer_img);

        Mat hsv_channels[3];
        cv::split(summer_img, hsv_channels);

        cv::Mat hsv ; cv::cvtColor(summer_img, hsv, cv::COLOR_BGR2HSV);
        hsv_channels[1] = summer_gamma_function(hsv_channels[1], 1.2);
        cv::Mat output_img;
        cv::cvtColor(hsv, output_img, cv::COLOR_HSV2BGR);
         /*
        img[:, :, 0] = summer_gamma_function(img[:, :, 0], 0.75) # down scaling blue channel
        img[:, :, 2] = summer_gamma_function(img[:, :, 2], 1.25) # up scaling red channel
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        hsv[:, :, 1] = summer_gamma_function(hsv[:, :, 1], 1.2) # up scaling saturation channel
        img = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
        */
        return output_img;
    }
};
#endif //_OHAYOU_SUMMER_H
