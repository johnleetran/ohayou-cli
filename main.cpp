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
const int slider_max = 100;
int slider = 1;
Mat gImage, gSaturationImg, gVibranceImg;

void on_trackbar(int x, void* data)
{
    int val = x;
    /** EDGES **/
    // Apply median filter to remove possible noise
    Mat imgMedian;
    medianBlur(gImage, imgMedian, 7);

    // Detect edges with canny
    Mat imgCanny;
    Canny(imgMedian, imgCanny, val, 150);

    // Dilate the edges
    Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
    dilate(imgCanny, imgCanny, kernel);

    // Scale edges values to 1 and invert values
    imgCanny = imgCanny / 255;
    imgCanny = 1 - imgCanny;

    // Use float values to allow multiply between 0 and 1
    Mat imgCannyf;
    imgCanny.convertTo(imgCannyf, CV_32FC3);

    // Blur the edgest to do smooth effect
    blur(imgCannyf, imgCannyf, Size(5, 5));

    /** COLOR **/
    // Apply bilateral filter to homogenizes color
    Mat imgBF;
    bilateralFilter(gImage, imgBF, 9, 150.0, 150.0);

    // truncate colors
    Mat result = imgBF / 25;
    result = result * 25;

    /** MERGES COLOR + EDGES **/
    // Create a 3 channles for edges
    Mat imgCanny3c;
    Mat cannyChannels[] = {imgCannyf, imgCannyf, imgCannyf};
    merge(cannyChannels, 3, imgCanny3c);

    // Convert color result to float
    Mat resultf;
    result.convertTo(resultf, CV_32FC3);

    // Multiply color and edges matrices
    multiply(resultf, imgCanny3c, resultf);

    // convert to 8 bits color
    resultf.convertTo(result, CV_8UC3);

    // Show image
    imshow("image", result);
    // Mat output_image = ohayou::gaussian_blur(img, 9);
    // Mat output_image = ohayou::horizontal_motion_blur(img, val);
    // Mat output_image = ohayou::apply_sepia(img, val/100.0);
    // Mat output_image = ohayou::apply_splash(img,
    //                                         cv::Scalar(155, 30, 30),
    //                                         cv::Scalar(315, 255, 255));

    //Mat output_image = ohayou::apply_duo_tone(img, 1, 1 + val / 100.0);
    //Mat output_image = ohayou::horizontal_motion_blur(img, val);
    //Mat output_image = ohayou::apply_brightness(img, val / 2.0);
    //Mat output_image = ohayou::apply_summer(img);
    //Mat output_image = ohayou::apply_oil_paint(img, val);
    //Mat output_image = ohayou::apply_white_balance(img, val/100.0);

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
