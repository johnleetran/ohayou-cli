#include <iostream>
#include <vector>

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
    Mat output_image = ohayou::apply_sepia(img, val / 100.0);
    // Mat output_image = ohayou::apply_splash(img,
    //                                         cv::Scalar(155, 30, 30),
    //                                         cv::Scalar(315, 255, 255));

    //Mat output_image = ohayou::apply_duo_tone(img, 1, 1 + val / 100.0);
    //Mat output_image = ohayou::horizontal_motion_blur(img, val);
    //Mat output_image = ohayou::apply_brightness(img, val / 2.0);
    //Mat output_image = ohayou::apply_summer(img);
    //Mat output_image = ohayou::apply_oil_paint(img, val);
    //Mat output_image = ohayou::apply_white_balance(img, val/100.0);

    imshow("image", output_image);
}

void execute(Mat &img)
{
    cv::namedWindow("image");
    cv::createTrackbar("val", "image", &slider, slider_max, on_trackbar);

    float val = cv::getTrackbarPos("val", "image");
    on_trackbar(val, 0);

    cv::waitKey(0);
}

cv::Mat getRegionOfInterestGrayScale(Mat img){
    int width = img.cols;
    int height = img.rows;

    Mat mask = cv::Mat::zeros(img.size(), img.type());
    cv::fillPoly(mask, std::vector<cv::Point>{
                           cv::Point{0, 0},
                           cv::Point{width, 0},
                           cv::Point{width, height},
                           cv::Point{0, height},
                       },
                 255, cv::LINE_8);
    Mat out;
    cv::bitwise_and(img, mask, out);
    return out;
}

int mainWebCam(int argc, char *argv[])
{
    std::string file_path = argv[1];
    std::string image_path = samples::findFile(file_path);
    // Mat image = imread(image_path, IMREAD_COLOR);
    // if (image.empty())
    // {
    //     std::cout << "Could not read the image: " << image_path << std::endl;
    //     return 1;
    // }

    // imshow("Display window", output_image);
    // (void) waitKey(0); // Wait for a keystroke in the window
    // img = image;
    //execute(image);

    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap(image_path);
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    // int deviceID = 0;        // 0 = open default camera
    // int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // // open selected camera using selected API
    // cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    while(true){
        // wait for a new frame from camera and store it into 'frame'
        Mat gray, out;
        cap >> frame;
        if (frame.empty())
            break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, out, 60, 60 * 3);
        // check if we succeeded
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        Mat live, roi;
        //roi = getRegionOfInterestGrayScale(out);
        //cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
        //cv::bitwise_or(frame, out, live);
        imshow("Live", out);
        if (waitKey(30) >= 0)
            break;
    }
    // When everything done, release the video capture object
    cap.release();
    // Closes all the frames
    destroyAllWindows();

    // Mat dst;
    // Mat src;
    // cv::cvtColor(img, src, cv::COLOR_BGR2GRAY);
    // cv::Canny(src, dst, 60, 60 * 3);

    // imshow("image", dst);
    // cv::imwrite("test.jpg", dst);
    // cv::waitKey(0);
    return 0;
}


int main(int argc, char* argv[]){
    std::string file_path1 = argv[1];
    std::string file_path2 = argv[2];
    std::string image_path1 = samples::findFile(file_path1);
    std::string image_path2 = samples::findFile(file_path2);

    Mat image1 = cv::imread(image_path1);
    Mat image2 = cv::imread(image_path2);
    Mat output_image;
    cv::hconcat(image1, image2, output_image);

    cv::imshow("output", output_image);
    cv::waitKey(0);
    return 0;
}