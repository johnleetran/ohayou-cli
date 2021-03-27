#include <iostream>

//opencv
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

//ohayou libs
#include "includes/blur/gaussian.hpp"
#include "includes/blur/average.hpp"
#include "includes/blur/motion.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]){
    std::string image_path = samples::findFile("/Users/john/Downloads/shinigami.png");
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    Mat horizontal_motion_img = ohayou::horizontal_motion_blur(img, 35);

    imshow("Display window", horizontal_motion_img);
    (void) waitKey(0); // Wait for a keystroke in the window

    return 0;
}

