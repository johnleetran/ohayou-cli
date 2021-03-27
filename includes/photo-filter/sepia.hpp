#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#ifndef _OHAYOU_SEPIA_H
#define _OHAYOU_SEPIA_H
using namespace cv;
using namespace std;
namespace ohayou
{
    cv::Mat getSepiaKernal()
    {
        cv::Mat kernal = (cv::Mat_<float>(3, 3)
                              << 
                                0.272, 0.534, 0.131,
                                0.349, 0.686, 0.168,
                                0.393, 0.769, 0.189);
         return kernal;
    }

    cv::Mat apply_sepia(Mat img)
    {
        cv::Mat kernal = getSepiaKernal();
        cv::Mat output_img;
        cv::transform(img, output_img, kernal);
        return output_img;
    }
};
#endif //_OHAYOU_SEPIA_H
