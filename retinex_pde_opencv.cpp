/* 
 * File:   retinex_pde_opencv.cpp
 * Author: David Stutz <david.stutz@rwth-aachen.de>
 *
 * Created on January 27, 2015, 2:54 PM
 */

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "retinex_pde_lib_wrapper.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("input", boost::program_options::value<std::string>(), "input image")
        ("threshold", boost::program_options::value<int>()->default_value(10), "threshold in (0,255]");

    boost::program_options::positional_options_description positionals;
    positionals.add("input", 1);
    positionals.add("threshold", 1);
    
    boost::program_options::variables_map parameters;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(positionals).run(), parameters);
    boost::program_options::notify(parameters);

    if (parameters.find("help") != parameters.end()) {
        std::cout << desc << std::endl;
        return 1;
    }
    
    boost::filesystem::path inputFile(parameters["input"].as<std::string>());
    if (!boost::filesystem::is_regular_file(inputFile)) {
        std::cout << "Input file not found ..." << std::endl;
        return 1;
    }
    
    float threshold = parameters["threshold"].as<int>();
    if (threshold <= 0 || threshold > 255) {
        std::cout << "Threshold must be in (0, 255], see below!" << std::endl;
        std::cout << desc << std::endl;
    }
    
    cv::Mat image = cv::imread(inputFile.string());
    cv::Mat reflectance = retinex::RetinexPDE(image, threshold);
    cv::Mat shading = retinex::RetinexPDE_Shading(image, reflectance);
    
    int position = inputFile.filename().string().find(inputFile.filename().extension().string());
    std::string reflectanceFile = inputFile.filename().string().substr(0, position) + "_reflectance.png";
    std::string shadingFile = inputFile.filename().string().substr(0, position) + "_shading.png";
    
    cv::imwrite(reflectanceFile, reflectance);
    cv::imwrite(shadingFile, shading);
    
    return 0;
}

