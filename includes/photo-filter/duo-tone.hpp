#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include <algorithm>
#include <cmath>
#ifndef _OHAYOU_DUO_TONE_H
#define _OHAYOU_DUO_TONE_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat exponential_function(Mat channel, float exp)
    {   
        Mat table(1, 256, CV_8U);
        for(int i=0; i<256; i++){
            table.at<uchar>(i) = std::min((int)std::pow(i, exp), 255);
        }
        cv::LUT(channel, table, channel);
        return channel;
    }

    cv::Mat tone(Mat img, int number, float exp = 1.05)
    {
        Mat channels[3];
        cv::split(img, channels);
        for(int i=0; i<3; i++){
            if(i == number){
                channels[i] = exponential_function(channels[i], exp);
            }else{
                channels[i] *= 0;
            }
        }
        Mat out;
        cv::merge(channels, 3, out);
        return out;
    }

    cv::Mat apply_duo_tone(Mat img, int channel_number, float exp = 1.05)
    {
        cv::Mat output_img = tone(img, channel_number, exp);

        return output_img;
    }
};
#endif //_OHAYOU_DUO_TONE_H
