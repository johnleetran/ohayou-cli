#include <iostream>
#include <algorithm>

//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn_superres.hpp>

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
Mat gImage;

void on_trackbar(int x, void *data)
{
    int val = x;
    // Mat output_image = ohayou::average_blur(img, 9);
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

    // imshow("image", output_image);
}

void execute(Mat& img)
{
    cv::namedWindow("image");
    cv::createTrackbar("val", "image", &slider, slider_max, on_trackbar);

    float val = cv::getTrackbarPos("val", "image");
    on_trackbar(val, 0);

    cv::waitKey(0);
}


int main(int argc, char *argv[]){

    std::string file_path = argv[1];
    std::string image_path = samples::findFile(file_path);
    Mat img = imread(image_path, IMREAD_COLOR);
    cv::dnn_superres::DnnSuperResImpl sr;
    string model_path = "/Users/john/Documents/code/imaging/opencv-code/ohayou-cli/models/super-resolution/EDSR_x4.pb";

    sr.readModel(model_path);

    //Set the desired model and scale to get correct pre- and post-processing
    sr.setModel("edsr", 4);

    //Upscale
    Mat img_new;
    sr.upsample(img, img_new);
    cv::imwrite("upscaled.png", img_new);
    imshow("result", img_new);
    waitKey(0);
    return 0;
}
