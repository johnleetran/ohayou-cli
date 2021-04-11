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
const int slider_max = 100;
int slider = 1;

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

int blur_command(int argc, char *argv[])
{
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

    if (app.got_subcommand(blur_cmd))
    {
        Mat output_image;
        if (blur_cmd->got_subcommand("average"))
        {
            output_image = ohayou::average_blur(image, blur_amount);
        }
        else if (blur_cmd->got_subcommand("gaussian"))
        {
            output_image = ohayou::gaussian_blur(image, blur_amount);
        }
        else
        {
            output_image = ohayou::horizontal_motion_blur(image, blur_amount);
        }
        imshow("output_image", output_image);
        cv::waitKey(0);
    }
    return 0;
}

Mat energy_function(Mat img)
{
    Mat gray;
    Mat energy_image;
    Mat gradient_x;
    Mat gradient_y;
    Mat abs_grad_x, abs_grad_y;
    Mat img_blur;

    cv::GaussianBlur(img, img_blur, Size(3, 3), 0, 0, BORDER_DEFAULT);

    cv::cvtColor(img_blur, gray, COLOR_BGR2GRAY);

    cv::Scharr(gray, gradient_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
    cv::Scharr(gray, gradient_y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
    //cv::magnitude(gradient_x, gradient_y, output_image);

    // convert gradients to abosulte versions of themselves
    cv::convertScaleAbs(gradient_x, abs_grad_x);
    cv::convertScaleAbs(gradient_y, abs_grad_y);

    // total gradient (approx)
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gray);
    gray.convertTo(energy_image, CV_64F, 1.0 / 255.0);

    // Mat inverted_image;
    // cv::bitwise_not(output_image, inverted_image);
    return energy_image;
}

Mat energy_function_refinement(Mat energy_image){
    int rowsize = energy_image.rows;
    int colsize = energy_image.cols;

    Mat cumulative_energy_map = Mat(rowsize, colsize, CV_64F, double(0));
    // if (seam_direction == VERTICAL)
    energy_image.row(0).copyTo(cumulative_energy_map.row(0));
    for (int row = 1; row < rowsize; row++)
    {
        for (int col = 0; col < colsize; col++)
        {
            double a = cumulative_energy_map.at<double>(row - 1, max(col - 1, 0));
            double b = cumulative_energy_map.at<double>(row - 1, col);
            double c = cumulative_energy_map.at<double>(row - 1, min(col + 1, colsize - 1));

            cumulative_energy_map.at<double>(row, col) = energy_image.at<double>(row, col) + std::min(a, min(b, c));
        }
    }
    return cumulative_energy_map;
}

void showPath(Mat &energy_image, vector<int> path, std::string debug_text= "")
{

    for (int i = 0; i < energy_image.rows; i++)
    {
        energy_image.at<double>(i, path[i]) = 1;
    }
    cv::putText(energy_image,        //target image
                debug_text,          //text
                cv::Point(10, 50), //top-left position
                cv::FONT_HERSHEY_DUPLEX,
                1.0,
                cv::Scalar(255, 255, 255));
    // display the seam on top of the energy image
    namedWindow("Seam on Energy Image", WINDOW_AUTOSIZE);
    imshow("Seam on Energy Image", energy_image);
    waitKey(0);
}

vector<int> get_seam(Mat energy_image){
    vector<int> path;
    double min_val, max_val;
    Point min_pt, max_pt;
    
    // get the number of rows and columns in the cumulative energy map
    int rowsize = energy_image.rows;
    int colsize = energy_image.cols;

    // copy the data from the last row of the cumulative energy map
    Mat row = energy_image.row(rowsize - 1);

    // get min and max values and locations
    minMaxLoc(row, &min_val, &max_val, &min_pt, &max_pt);

    // initialize the path vector
    path.resize(rowsize);
    int min_index = min_pt.x;
    path[rowsize - 1] = min_index;

    // starting from the bottom, look at the three adjacent pixels above current pixel, choose the minimum of those and add to the path
    for (int i = rowsize - 2; i >= 0; i--)
    {
        int offset = 0;
        double a = energy_image.at<double>(i, max(min_index - 1, 0));
        double b = energy_image.at<double>(i, min_index);
        double c = energy_image.at<double>(i, min(min_index + 1, colsize - 1));

        if (min(a, b) > c)
        {
            offset = 1;
        }
        else if (min(a, c) > b)
        {
            offset = 0;
        }
        else if (min(b, c) > a)
        {
            offset = -1;
        }

        min_index += offset;
        min_index = min(max(min_index, 0), colsize - 1); // take care of edge cases
        path[i] = min_index;
    }
    return path;
}

void reduce(Mat &image, vector<int> path)
{
    clock_t start = clock();

    // get the number of rows and columns in the image
    int rowsize = image.rows;
    int colsize = image.cols;

    // create a 1x1x3 dummy matrix to add onto the tail of a new row to maintain image dimensions and mark for deletion
    Mat dummy(1, 1, CV_8UC3, Vec3b(0, 0, 0));

    for (int i = 0; i < rowsize; i++)
    {
        // take all pixels to the left and right of marked pixel and store them in appropriate subrow variables
        Mat new_row;
        Mat lower = image.rowRange(i, i + 1).colRange(0, path[i]);
        Mat upper = image.rowRange(i, i + 1).colRange(path[i] + 1, colsize);

        // merge the two subrows and dummy matrix/pixel into a full row
        if (!lower.empty() && !upper.empty())
        {
            hconcat(lower, upper, new_row);
            hconcat(new_row, dummy, new_row);
        }
        else
        {
            if (lower.empty())
            {
                hconcat(upper, dummy, new_row);
            }
            else if (upper.empty())
            {
                hconcat(lower, dummy, new_row);
            }
        }
        // take the newly formed row and place it into the original image
        new_row.copyTo(image.row(i));
    }
    // clip the right-most side of the image
    image = image.colRange(0, colsize - 1);
}

Mat seam_carving(Mat img)
{
    Mat output_image;
    int iterations = 100;
    for (int i = 0; i < iterations; i++)
    {

        Mat energy_image = energy_function(img);
        Mat interminate_energy_image = energy_function_refinement(energy_image);
        vector<int> path = get_seam(interminate_energy_image);
        showPath(energy_image, path, "Cur Iter: " + std::to_string(i + 1) + "/" + std::to_string(iterations));
        reduce(img, path);
    }
    return img;
}

int main(int argc, char *argv[]){

    std::string file_path = argv[1];
    std::string image_path = samples::findFile(file_path);
    Mat img = imread(image_path, IMREAD_COLOR);
    Mat output_image = seam_carving(img);
    cv::imshow("output_image", output_image);
    cv::waitKey(0);
    return 0;
}

