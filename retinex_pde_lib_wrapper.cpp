/* 
 * File:   retinex_pde_lib_wrapper.cpp
 * Author: David Stutz <david.stutz@rwth-aachen.de>
 *
 * Created on January 27, 2015, 1:28 PM
 */

#include "retinex_pde_lib_wrapper.h"
#include "retinex_pde_lib.h"
#include "norm.h"
#include <cstddef>

namespace retinex {
    float* Mat2RetinexPDEArray(const cv::Mat mat) {
        float* output = new float[mat.rows*mat.cols*mat.channels()];
        
        // Retinex PDE Lib expects a one-dimensional array where the image is
        // row by row and channel by channel.
        for (int c = 0; c < mat.channels(); ++c) {
            for (int i = 0; i < mat.rows; ++i) {
                for (int j = 0; j < mat.cols; ++j) {
                    if (mat.channels() > 1) {
                        output[c*mat.rows*mat.cols + i*mat.cols + j] = mat.at<cv::Vec3f>(i, j)[c];
                    }
                    else {
                        output[c*mat.rows*mat.cols + i*mat.cols + j] = mat.at<float>(i, j);
                    }
                }
            }
        }
        
        return output;
    }
    
    cv::Mat RetinexPDEArray2Mat(float* array, std::size_t rows, std::size_t cols, std::size_t channels) {
        cv::Mat output(rows, cols, CV_32FC3);
        
        for (int c = 0; c < channels; ++c) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (channels > 1) {
                        output.at<cv::Vec3f>(i, j)[c] = array[c*rows*cols + i*cols + j];
                    }
                    else {
                        output.at<float>(i, j) = array[c*rows*cols + i*cols + j];
                    }
                }
            }
        }
        
        return output;
    }
    
    cv::Mat RetinexPDE(const cv::Mat& image, float threshold) {
        std::size_t rows = image.rows;
        std::size_t cols = image.cols;
        std::size_t channels = image.channels();
        
        // Assert that image has three or one channel with 8 bit each.
        assert(image.type() == CV_8UC3 || image.type() == CV_8UC1);
        
        // Assert the threshold to be in (0, 255].
        assert(threshold > 0 && threshold <= 255);
        
        // Create the output image of same size as image.
        // but float content.
        cv::Mat output = image.clone();
        cv::Mat input = image.clone();
        if (image.type() == CV_8UC3) {
            output.convertTo(output, CV_32FC3);
            input.convertTo(input, CV_32FC3);
        }
        else if (image.type() == CV_8UC1) {
            output.convertTo(output, CV_32FC1);
            input.convertTo(input, CV_32FC1);
        }
        
        // Unfortunately matrices are stored differently from io_png images.
        float* output_data = Mat2RetinexPDEArray(output);
        float* input_data = Mat2RetinexPDEArray(input);
        
        for (std::size_t c = 0; c < channels; ++c) {
            float* success = retinex_pde(output_data + c*cols*rows, cols, rows, threshold);
            if (NULL == success) {
                // Error, something went wrong.
                assert(0);
            }
            
            // Normalize this channel.
            normalize_mean_dt(output_data + c*cols*rows, input_data + c*cols*rows, rows*cols);
        }
        
        output = RetinexPDEArray2Mat(output_data, rows, cols, channels);
        output.convertTo(output, CV_8UC3);
        return output;
    }
    
    cv::Mat RetinexPDE_Shading(const cv::Mat& image, const cv::Mat& reflectance) {
        std::size_t rows = image.rows;
        std::size_t cols = image.cols;
        std::size_t channels = image.channels();
        
        // Assert dimensions are OK.
        assert(reflectance.rows == rows && reflectance.cols == cols && reflectance.channels() == channels);
        
        // Assert the type is OK.
        assert((image.type() == CV_8UC3 || image.type() == CV_8UC1) && image.type() == reflectance.type());
        
        cv::Mat output(rows, cols, CV_8UC3);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (image.type() == CV_8UC1) {
                    output.at<unsigned char>(i, j) = image.at<unsigned char>(i ,j) - reflectance.at<unsigned char>(i, j);
                }
                else if (image.type() == CV_8UC3) {
                    output.at<cv::Vec3b>(i, j) = image.at<cv::Vec3b>(i ,j) - reflectance.at<cv::Vec3b>(i, j);
                }
            }
        }
        
        return output;
    }
}