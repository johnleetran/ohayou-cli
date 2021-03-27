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
#include "includes/photo-filter/sepia.hpp"
#include "includes/photo-filter/splash.hpp"
#include "includes/photo-filter/duo-tone.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]){
    std::string image_path = samples::findFile("/Users/john/Downloads/lion.jpg");
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    // Mat output_image = ohayou::average_blur(img, 9);
    // Mat output_image = ohayou::gaussian_blur(img, 9);
    // Mat output_image = ohayou::horizontal_motion_blur(img, 21);
    // Mat output_image = ohayou::apply_sepia(img);
    // Mat output_image = ohayou::apply_splash(img,
    //                                         cv::Scalar(155, 30, 30),
    //                                         cv::Scalar(315, 255, 255));

    //second parameter to 0 for blue, 1 for green and 2 for red screen
    Mat output_image = ohayou::apply_duo_tone(img, 1, 1.05);
    imshow("Display window", output_image);
    (void) waitKey(0); // Wait for a keystroke in the window

    return 0;
}

