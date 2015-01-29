/* 
 * File:   retinex_pde_lib_wrapper.cpp
 * Author: David Stutz <david.stutz@rwth-aachen.de>
 *
 * Created on January 27, 2015, 1:28 PM
 */

#include "retinex_pde_lib_wrapper.h"
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif
  #include "retinex_pde_lib.h"
  #include "norm.h"
#ifdef __cplusplus
}
#endif 



namespace retinex {
    float* Mat2RetinexPDEArray(const cv::Mat mat) {
        assert(mat.type() == CV_8UC3);
        float* output = new float[mat.rows*mat.cols*mat.channels()];
        
        // Retinex PDE Lib expects a one-dimensional array where the image is
        // row by row and channel by channel.
        for (int c = 0; c < mat.channels(); ++c) {
            for (int i = 0; i < mat.rows; ++i) {
                for (int j = 0; j < mat.cols; ++j) {
                      output[c*mat.rows*mat.cols + i*mat.cols + j] = (float) mat.at<cv::Vec3b>(i, j)[c];
                }
            }
        }
        
        return output;
    }
    
    void RetinexPDEArray2Mat(float* array, cv::Mat& reflectance) {
        assert(reflectance.type() == CV_8UC3);
        
        std::size_t rows = reflectance.rows;
        std::size_t cols = reflectance.rows;
        std::size_t channels = reflectance.channels();
        
        for (int c = 0; c < channels; ++c) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    // Remember to clip to avoid overflows.
                    reflectance.at<cv::Vec3b>(i, j)[c] = std::max(0.f, std::min(255.f, array[c*rows*cols + i*cols + j]));
                }
            }
        }
    }
    
    void RetinexPDE(const cv::Mat& image, float threshold, cv::Mat* reflectance) {
        std::size_t rows = image.rows;
        std::size_t cols = image.cols;
        std::size_t channels = image.channels();
        
        assert(image.type() == CV_8UC3);
        assert(threshold > 0 && threshold <= 255);
        
        // Unfortunately matrices are stored differently from io_png images.
        float* output_data = Mat2RetinexPDEArray(image);
        float* input_data = Mat2RetinexPDEArray(image);
        
        for (std::size_t c = 0; c < channels; ++c) {
            float* success = retinex_pde(output_data + c*cols*rows, cols, rows, threshold);
            if (NULL == success) {
                // Error, something went wrong.
                assert(0);
            }
            
            // Normalize this channel.
            normalize_mean_dt(output_data + c*cols*rows, input_data + c*cols*rows, rows*cols);
        }
        
        RetinexPDEArray2Mat(output_data, *reflectance);
        cv::imwrite("test.png", *reflectance);
        // Remember to clean up!
        delete[] output_data;
        delete[] input_data;
    }
    
    void RetinexPDE_Shading(const cv::Mat& image, const cv::Mat& reflectance, cv::Mat* shading) {
        std::size_t rows = image.rows;
        std::size_t cols = image.cols;
        
        assert(reflectance.rows == rows && reflectance.cols == cols && reflectance.channels() == image.channels());
        assert(image.type() == CV_8UC3 && image.type() == reflectance.type());
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                shading->at<cv::Vec3b>(i, j) = image.at<cv::Vec3b>(i ,j) - reflectance.at<cv::Vec3b>(i, j);
            }
        }
    }
}