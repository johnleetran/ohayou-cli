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
const int slider_max = 255;
int slider = 0;
int gCurrentSlider = slider;
Mat gImage, gDepthImg, gImposeImg;
Point gImposeImagePosition;
std::string gWindowName = "image";
void update_impose_position(int event, int x, int y, int f, void *);

void render(){
    int val = gCurrentSlider;
    Mat output_image;
    Mat src_img = gImage;
    Mat result = src_img.clone();
    Mat depth_img = gDepthImg;
    Mat impose_img = gImposeImg;

    int startRow = gImposeImagePosition.y;
    int startCol = gImposeImagePosition.x;
    for(int row = 0; row < impose_img.rows; row++){
        for(int col = 0; col < impose_img.cols; col++){
            int c = col+startCol;
            int r = row+startRow;
            int pixelValue = (int)depth_img.at<uchar>(r, c);
            if(pixelValue >= val){
                Vec3b color = impose_img.at<Vec3b>(Point(col, row));
                result.at<Vec3b>(Point(c,r))[0] = color[0];
                result.at<Vec3b>(Point(c,r))[1] = color[1];
                result.at<Vec3b>(Point(c,r))[2] = color[2];
                // result.at<Vec4b>(Point(c,r))[3] = color[3];
            }
        }
    }
    // show result
    imshow("image", result);
    cv::setMouseCallback(gWindowName, update_impose_position, NULL);
}

void on_trackbar(int x, void *data)
{
    gCurrentSlider = x;
    render();


}

void execute(Mat &src_img,  Mat &depth_img, Mat &impose_img)
{
    cv::namedWindow(gWindowName);
    cv::createTrackbar("val", gWindowName, &slider, slider_max, on_trackbar);

    float val = cv::getTrackbarPos("val", gWindowName);
    gImage = src_img;
    gDepthImg = depth_img;
    gImposeImg = impose_img;
    on_trackbar(val, 0);

    cv::waitKey(0);
}

void update_impose_position(int event, int x, int y, int f, void *)
{
    switch (event)
    {
    case cv::EVENT_LBUTTONDOWN:
        std::cout << "Release x: " << x << std::endl;
        std::cout << "Release y: " << y << std::endl;
        std::cout << "Release f: " << f << std::endl;
        gImposeImagePosition.x = x;
        gImposeImagePosition.y = y;
        //cv::rectangle(gImage, startROI, endROI, cv::Scalar(0, 255, 0), -1); break;
    default:
        break;
    }
    render();
}

int main(int argc, char *argv[])
{
    std::string src_arg = argv[1];
    std::string depth_arg = argv[2];
    std::string impose_arg = argv[3];

    std::string src_path = samples::findFile(src_arg);
    std::string depth_path = samples::findFile(depth_arg);
    std::string impose_path = samples::findFile(impose_arg);

    Mat src_img = imread(src_path, cv::IMREAD_COLOR);
    Mat depth_img = imread(depth_path, cv::IMREAD_GRAYSCALE);
    Mat impose_img = imread(impose_path, cv::IMREAD_COLOR);

    execute(src_img, depth_img, impose_img);

    return 0;
}
