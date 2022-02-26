#include <iostream>
#include <algorithm>

//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

//ohayou blurs
#include "includes/blur/gaussian.hpp"
#include "includes/blur/average.hpp"
#include "includes/blur/motion.hpp"

//ohayou photo-filters
#include "includes/photo-filter/brightness.hpp"
#include "includes/photo-filter/duo-tone.hpp"
#include "includes/photo-filter/sepia.hpp"
#include "includes/photo-filter/splash.hpp"
#include "includes/photo-filter/oil-paint.hpp"
#include "includes/photo-filter/white-balance.hpp"

// command-line-parser
#include "includes/command-line-parser/CLI11.hh"

//ohayou preset
#include "includes/preset/summer.hpp"

using namespace cv;
using namespace std;
const int slider_max = 1000;
int slider = 1;
Mat gImage, gSaturationImg, gVibranceImg;

void on_trackbar(int x, void *data)
{
    int val = x;
    Mat output_image;
    Mat result;
    Mat img = gImage;
    const double exponential_e = std::exp(1.0);
    // Create Look-up table for color curve effect
    Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++)
    {
        float x = (float)i / 256.0;
        lut.at<uchar>(i) = cvRound(256 * (1 / (1 + pow(exponential_e, -((x - 0.5) / 0.1)))));
    }

    // Split the image channels and apply curve transform only to red channel
    vector<Mat> bgr;
    split(img, bgr);
    LUT(bgr[2], lut, bgr[2]);
    // merge result
    merge(bgr, result);

    // Create image for halo dark
    float halo_val = val / 1000.0;
    Mat halo(img.rows, img.cols, CV_32FC3, Scalar(halo_val, halo_val, halo_val));
    // Create circle
    circle(halo, Point(img.cols / 2, img.rows / 2), img.cols / 3, Scalar(1, 1, 1), -1);
    blur(halo, halo, Size(img.cols / 3, img.cols / 3));

    // Convert the result to float to allow multiply by 1 factor
    Mat resultf;
    result.convertTo(resultf, CV_32FC3);

    // Multiply our result with halo
    multiply(resultf, halo, resultf);

    // convert to 8 bits
    resultf.convertTo(result, CV_8UC3);

    // show result
    imshow("image", result);
}

void execute(Mat &img)
{
    cv::namedWindow("image");
    cv::createTrackbar("val", "image", &slider, slider_max, on_trackbar);

    float val = cv::getTrackbarPos("val", "image");
    gImage = img;
    on_trackbar(val, 0);

    cv::waitKey(0);
}

int main(int argc, char *argv[])
{
    std::string file_path = argv[1];
    std::string image_path = samples::findFile(file_path);
    Mat img = imread(image_path, IMREAD_COLOR);

    execute(img);

    return 0;
}
