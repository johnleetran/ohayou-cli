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

//ohayou preset
#include "includes/preset/summer.hpp"

// libvips
#include <vips/vips8>
using namespace vips;

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
    Mat output_image = ohayou::apply_sepia(img, val/100.0);
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

void execute(Mat& img)
{
    cv::namedWindow("image");
    cv::createTrackbar("val", "image", &slider, slider_max, on_trackbar);

    float val = cv::getTrackbarPos("val", "image");
    on_trackbar(val, 0);

    cv::waitKey(0);
}

int doVips(int argc, char **argv)
{
    if (VIPS_INIT(argv[0]))
        vips_error_exit(NULL);

    if (argc != 3)
        vips_error_exit("usage: %s input-file output-file", argv[0]);

    VImage in = VImage::new_from_file(argv[1],
                                      VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL));

    double avg = in.avg();

    printf("avg = %g\n", avg);
    printf("width = %d\n", in.width());

    in = VImage::new_from_file(argv[1],
                               VImage::option()->set("access", VIPS_ACCESS_SEQUENTIAL));

    VImage out = in.embed(0, 0, 1000, 1000,
                          VImage::option()->set("extend", "background")->set("background", 128));

    std::vector<double> ink = {0.0};
    out.draw_line(ink, 0, 0, 1000, 1000);
    out.write_to_file(argv[2]);

    vips_shutdown();

    return (0);
}

int main(int argc, char *argv[]){
    std::string file_path = argv[1];
    std::string image_path = samples::findFile(file_path);
    Mat image = imread(image_path, IMREAD_COLOR);
    if (image.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    Mat output_image = ohayou::apply_sepia(image, 1.0);
    cv::imwrite("./out.jpg", output_image);

    doVips(argc, argv);
    // imshow("Display window", output_image);
    // (void) waitKey(0); // Wait for a keystroke in the window
    //img = image;
    //execute(image);
    return 0;
}

