#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#ifndef _OHAYOU_MOTION_H
#define _OHAYOU_MOTION_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat set_horizontal_motion_blur_kernal(cv::Mat kernal, int kernal_size)
    {
        int row = int(kernal_size / 2) + 1;
        for (int i = 0; i < kernal_size; i++){
            kernal.at<float>(row, i)++;
        }

        return kernal;
    }

    cv::Mat horizontal_motion_blur(Mat img, int kernal_size)
    {
        std::vector<float> data(kernal_size * kernal_size, 0.0f);
        cv::Mat kernal = cv::Mat(kernal_size, kernal_size, CV_32F, data.data());

        kernal = set_horizontal_motion_blur_kernal(kernal, kernal_size) / kernal_size;
        cv::Mat image_blurred;
        cv::filter2D(img, image_blurred, -1, kernal);
        return image_blurred;
    }
};
#endif //_OHAYOU_MOTION_H
