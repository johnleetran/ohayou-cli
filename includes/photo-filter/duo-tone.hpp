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
    //does not work, not sure how to implement using LUT
    cv::Mat exponential_function(Mat img, float exp)
    {
        std::vector<int> table;
        return img;
    }

    cv::Mat tone(Mat img, int number, float exp)
    {
        Mat planes[3];
        cv::split(img, planes);
        for(int i=0; i<3; i++){
            if(i == number){
                planes[i] *= exp;//exponential_function(planes[i], exp);
            }else{
                planes[i] *= 0;
            }
        }
        Mat out;
        cv::merge(planes, 3, out);
        return out;
    }

    cv::Mat apply_duo_tone(Mat img, int channel_number)
    {
        cv::Mat output_img = tone(img, channel_number, 1.05);

        return output_img;
    }
};
#endif //_OHAYOU_DUO_TONE_H
