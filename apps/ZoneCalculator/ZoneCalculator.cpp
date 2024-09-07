// ZoneCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include <ZoneTools.h>

using namespace cv;

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        std::cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << std::endl;
        return -1;
    }
    Mat image;
    image = imread(argv[1], IMREAD_COLOR); // Read the file
    if (image.empty()) // Check for invalid input
    {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    
    // TODO: User parameters
    double width = 100.0;
    ZoneInfo zone_info = {
        (Vec3d)image.at<Vec3b>(1, 300),
        (Vec3d)image.at<Vec3b>(1, 1),
        (Vec3d)image.at<Vec3b>(1, 600)
    };

    auto image_with_zones = CalcZones(image, width, zone_info);
    
    namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Display window", image_with_zones); // Show our image inside it.
    waitKey(0); // Wait for a keystroke in the window

    return 0;
}

