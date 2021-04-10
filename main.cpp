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
#include "includes/photo-filter/white-balance.hpp"

// command-line-parser
#include "includes/command-line-parser/CLI11.hh"

//ohayou preset
#include "includes/preset/summer.hpp"


using namespace cv;
using namespace std;
const int slider_max = 100;
int slider = 1;
Mat img;

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
    CLI::App app{"Ohayou-cli - useless imaging CLI that shows off cool effects"};

    std::string input_filename = "input.jpg";
    std::string output_filename = "output.jpg";
    int blur_amount = 1;

    CLI::App *blur_cmd = app.add_subcommand("blur", "applies blur on the input image");
    blur_cmd->add_option("-i,--input", input_filename, "the path to the input image")->required();
    blur_cmd->add_option("-o,--output", output_filename, "the path to where you want to save the output");
    blur_cmd->add_option("-a,--amount", blur_amount, "how much to blur the image")->required();
    blur_cmd->add_subcommand("average");
    blur_cmd->add_subcommand("gaussian");
    blur_cmd->add_subcommand("motion");

    CLI11_PARSE(app, argc, argv);

    std::string image_path = samples::findFile(input_filename);
    Mat image = imread(image_path, IMREAD_COLOR);

    if (app.got_subcommand(blur_cmd)){
        Mat output_image;
        if (blur_cmd->got_subcommand("average")){
            output_image = ohayou::average_blur(image, blur_amount);
        }
        else if (blur_cmd->got_subcommand("gaussian")){
            output_image = ohayou::gaussian_blur(image, blur_amount);
        }
        else{
            output_image = ohayou::horizontal_motion_blur(image, blur_amount);
        }
        imshow("image", output_image);
        cv::waitKey(0);
    }

    // std::string file_path = argv[1];
    // std::string image_path = samples::findFile(file_path);
    // Mat image = imread(image_path, IMREAD_COLOR);

    // img = image;
    //execute(image);
    return 0;
}

