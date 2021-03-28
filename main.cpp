#include <iostream>

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

//ohayou preset
#include "includes/preset/summer.hpp"


using namespace cv;
using namespace std;
const int slider_max = 100;
int slider = 1;
Mat src1;

void on_trackbar(int x, void *data)
{
    int val = x;
    // Mat output_image = ohayou::average_blur(src1, 9);
    // Mat output_image = ohayou::gaussian_blur(src1, 9);
    // Mat output_image = ohayou::horizontal_motion_blur(src1, val);
    // Mat output_image = ohayou::apply_sepia(src1, val/100.0);
    // Mat output_image = ohayou::apply_splash(src1,
    //                                         cv::Scalar(155, 30, 30),
    //                                         cv::Scalar(315, 255, 255));

    //Mat output_image = ohayou::apply_duo_tone(src1, 1, 1 + val / 100.0);
    //Mat output_image = ohayou::horizontal_motion_blur(img, val);
    //Mat output_image = ohayou::apply_brightness(src1, val / 2.0);
    //Mat output_image = ohayou::apply_summer(src1);
    Mat output_image = ohayou::apply_oil_paint(src1, val);
    imshow("image", output_image);
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
    std::string image_path = samples::findFile("/Users/john/Downloads/shinigami.jpg");
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    // imshow("Display window", output_image);
    // (void) waitKey(0); // Wait for a keystroke in the window
    src1 = img;
    execute(img);

    return 0;
}

