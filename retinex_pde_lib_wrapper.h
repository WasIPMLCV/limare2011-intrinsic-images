/* 
 * File:   retinex_pde_lib_wrapper.h
 * Author: David Stutz <david.stutz@rwth-aachen.de>
 *
 * Created on January 27, 2015, 1:28 PM
 */

#ifndef _RETINEX_PDE_LIB_WRAPPER_H
#define	_RETINEX_PDE_LIB_WRAPPER_H

#include <opencv2/opencv.hpp>

namespace retinex {
    
    /**
     * Wrapper for retinex_pde defined in retinex_pde_lib.c.
     * 
     * The method expects an image with one or three channels to be given.
     * Each channel is expected to contain 8 bit data.
     * 
     * @param image
     * @param threshold
     * @return 
     */
    cv::Mat RetinexPDE(const cv::Mat& image, float threshold);
    
    /**
     * Given the reflectance image from RetinexPDE, the shading can be determined
     * base don the original image.
     * 
     * @param reflectance
     * @return 
     */
    cv::Mat RetinexPDE_Shading(const cv::Mat& image, const cv::Mat& reflectance);
}

#endif	/* RETINEX_PDE_LIB_WRAPPER_H */

