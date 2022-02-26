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
Mat gImage, gImposeImg, gDepthImg;

void on_trackbar(int x, void *data)
{
    int val = x;
    Mat output_image;
    Mat src_img = gImage;
    Mat result = src_img.clone();
    Mat impose_img = gImposeImg;
    Mat depth_img = gDepthImg;
    int startRow = 223;
    int startCol = 288;
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
}

void execute(Mat &src_img,  Mat &impose_img, Mat &depth_img)
{
    cv::namedWindow("image");
    cv::createTrackbar("val", "image", &slider, slider_max, on_trackbar);

    float val = cv::getTrackbarPos("val", "image");
    gImage = src_img;
    gImposeImg = impose_img;
    gDepthImg = depth_img;
    on_trackbar(val, 0);

    cv::waitKey(0);
}

int main(int argc, char *argv[])
{
    std::string src_arg = argv[1];
    std::string impose_arg = argv[2];
    std::string depth_arg = argv[3];

    std::string src_path = samples::findFile(src_arg);
    std::string impose_path = samples::findFile(impose_arg);
    std::string depth_path = samples::findFile(depth_arg);

    Mat src_img = imread(src_path, cv::IMREAD_UNCHANGED);
    Mat impose_img = imread(impose_path, cv::IMREAD_COLOR);
    Mat depth_img = imread(depth_path, cv::IMREAD_GRAYSCALE);

    execute(src_img, impose_img, depth_img);

    return 0;
}
