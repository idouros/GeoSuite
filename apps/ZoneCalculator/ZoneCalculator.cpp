// ZoneCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
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

    ConfigParams configParams;
    boost::property_tree::ini_parser::read_ini(argv[1], configParams);
   
    // Input and output files
    std::filesystem::path configFilePath(argv[1]);
    auto image_in = getFileParameter(configFilePath, configParams, "files.image_in");
    auto image_out = getFileParameter(configFilePath, configParams, "files.image_out");
    auto zone_info_file = getFileParameter(configFilePath, configParams, "files.zone_info");

    // Zone info
    ZoneInfo zoneInfo;
    LOG_OUT("Reading config file...");
    boost::property_tree::json_parser::read_json(zone_info_file, zoneInfo);

    Mat image;
    image = imread(image_in, IMREAD_COLOR); 
    if (image.empty()) 
    {
        std::cout << "Could not open or find the image" << std::endl;
        return RETURN_CODE::ERR_FILE_NOT_FOUND;
    }
    
    cv::Mat image_with_zones;
    LOG_OUT("Calculating Zones...");
    EXEC_TIMED(image_with_zones = CalcZones(image, configParams, zoneInfo));

    imwrite(image_out, image);
    
    namedWindow("Display window", WINDOW_AUTOSIZE); 
    imshow("Display window", image_with_zones); 
    waitKey(0); 

    return RETURN_CODE::OK;
}

