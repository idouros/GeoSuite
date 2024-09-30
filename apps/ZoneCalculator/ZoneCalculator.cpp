// ZoneCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <filesystem>
#include <iostream>


#include <ZoneTools.h>

using namespace cv;

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        std::cout << " Usage: " << argv[0] << " ConfigFile" << std::endl;
        return -1;
    }

    boost::property_tree::ptree configParams;
    boost::property_tree::ini_parser::read_ini(argv[1], configParams);
   
    // Input and output files
    std::filesystem::path configFilePath(argv[1]);
    auto configFileFolder = configFilePath.parent_path();
    auto image_in = configFileFolder.append(configParams.get<std::string>("files.image_in")).string();
    configFileFolder = configFilePath.parent_path();
    auto image_out = configFileFolder.append(configParams.get<std::string>("files.image_out")).string();

    Mat image;
    image = imread(image_in, IMREAD_COLOR); 
    if (image.empty()) 
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

    imwrite(image_out, image);
    
    namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    imshow("Display window", image_with_zones); // Show our image inside it.
    waitKey(0); // Wait for a keystroke in the window

    return 0;
}

