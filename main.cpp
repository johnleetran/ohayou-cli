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
const int slider_max = 50;
int slider = 1;
Mat gImage;

void on_trackbar(int x, void *data)
{
    int val = x;
    if(val != 0){
        int scale = 1;
        int saturation = val;
        Mat output_image, saturated;
        Mat gray, gray_blur, mask;
        Mat color_blur_output;
        Mat hsv_img_channels[3], hsv_img;
        cv::cvtColor(gImage, gray, cv::COLOR_BGR2GRAY);
        cv::medianBlur(gray, gray_blur, 5);
        cv::adaptiveThreshold(gray_blur, mask, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 5);

        cv::bilateralFilter(gImage, color_blur_output, 9, 200, 200);

        cv::bitwise_and(gImage, color_blur_output, output_image, mask);
        cv::cvtColor(output_image, output_image, cv::COLOR_BGR2HSV);
        cv::split(output_image, hsv_img_channels);
        hsv_img_channels[1] *= saturation * 0.5;
        cv::merge(hsv_img_channels, 3, hsv_img);

        cv::cvtColor(hsv_img, output_image, cv::COLOR_HSV2BGR);
        imshow("image", output_image);

        // s = s *satadj
        //     s = np.clip(s, 0, 255)
        //             imghsv = cv2.merge([ h, s, v ])
        //                          
    }
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
    gImage= img;
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
