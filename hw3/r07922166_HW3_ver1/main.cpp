//
//  main.cpp
//  test
//
//  Created by 李淑貞 on 2018/9/12.
//  Copyright © 2018年 李淑貞. All rights reserved.
//

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace cv;
using namespace std;

Mat negImage(Mat inputMat){
    Mat result;
    inputMat.copyTo(result);
    for(int i = 0; i < result.rows; i++){
        for(int j =0; j < result.cols ; j++){
            result.at<uint8_t>(i,j)=255-result.at<uint8_t>(i,j);
        }
    }
    return result;
}

int SumOfMatrix(Mat mat){
    int total = 0;
    for(int i = 0; i < mat.rows; i++){
        for(int j =0; j < mat.cols ; j++){
            total += mat.at<uint8_t>(i,j);
        }
    }
    return total;
}

Mat grayScale(Mat inputMat){
    Mat result;
    inputMat.copyTo(result);
    for(int i = 0; i < inputMat.rows ; i++){
        for(int j = 0 ; j < inputMat.cols ; j++){
            if(inputMat.at<uint8_t>(i,j) < 128){
                result.at<uint8_t>(i,j)=0;
            }else{
                result.at<uint8_t>(i,j)=1;
            }
        }
    }
    return result;
}

// inputMat is binary scale(0,1) image
Mat erosion(Mat inputMat, Mat kernel){
    // erosion
    Mat erosion_image = Mat::zeros(inputMat.rows, inputMat.cols, CV_8UC1);
    int kernelSize = kernel.rows;
    int kernelSum = SumOfMatrix(kernel);
    for(int i = (kernelSize-1)/2; i < inputMat.rows-(kernelSize-1)/2 ; i++){
        for(int j = (kernelSize-1)/2 ; j < inputMat.cols-(kernelSize-1)/2 ; j++){
            // erosion
            Mat erosion_temp = inputMat(cv::Rect(j-(kernelSize-1)/2,i-(kernelSize-1)/2,kernelSize,kernelSize));
            Mat erosion_result = kernel.mul(erosion_temp);
            int erosion_sum = SumOfMatrix(erosion_result);
            if(erosion_sum == kernelSum){
                erosion_image.at<uint8_t>(i,j)=255;
            }else{
                erosion_image.at<uint8_t>(i,j)=0;
            }
            
        }
    }
    return erosion_image;
}

Mat dilation(Mat inputMat, Mat kernel){
    // erosion
    Mat dilation_image = Mat::zeros(inputMat.rows, inputMat.cols, CV_8UC1);
    int kernelSize = kernel.rows;
    for(int i = (kernelSize-1)/2; i < inputMat.rows-(kernelSize-1)/2 ; i++){
        for(int j = (kernelSize-1)/2 ; j < inputMat.cols-(kernelSize-1)/2 ; j++){
            // dilation
            if(inputMat.at<uint8_t>(i,j)==1){
                Mat dilation_temp = dilation_image(cv::Rect(j-(kernelSize-1)/2, i-(kernelSize-1)/2, kernelSize, kernelSize));
                add(dilation_temp, kernel, dilation_temp);
            }
        }
    }
    for(int i = 0 ; i < dilation_image.rows ; i++){
        for(int j = 0 ; j < dilation_image.cols ; j++){
            if(dilation_image.at<uint8_t>(i,j)!=0)
                dilation_image.at<uint8_t>(i,j)=255;
        }
    }
    return dilation_image;
}

int main(int argc, const char * argv[]) {
    // read image
    Mat image;
    image = imread("lena.bmp", IMREAD_COLOR); // Read the file
    // threshold 128 image
    Mat thresh_img ;
    image.copyTo(thresh_img);
    cv::cvtColor(thresh_img, thresh_img, CV_RGB2GRAY);
    
    thresh_img = grayScale(thresh_img);

    Mat kernel = (Mat_<uint8_t>(5,5)<<
                      0, 1, 1, 1, 0,
                      1, 1, 1, 1, 1,
                      1, 1, 1, 1, 1,
                      1, 1, 1, 1, 1,
                      0, 1, 1, 1, 0);
   
    Mat J_kernel = (Mat_<uint8_t>(5,5)<<
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    1, 1, 0, 0, 0,
                    0, 1, 0, 0, 0,
                    0, 0, 0, 0, 0);
    
    Mat K_kernel = (Mat_<uint8_t>(5,5)<<
                    0, 0, 0, 0, 0,
                    0, 1, 1, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0);
    
    Mat temp ;

    // erosion
    Mat erosionImage = erosion(thresh_img, kernel);
    imshow("erosion", erosionImage);
    waitKey(0);
    
    // dilation
    Mat dilationImage = dilation(thresh_img, kernel);
    imshow("dilation", dilationImage);
    waitKey(0);
    
    // opening
    Mat openingImage = dilation(grayScale(erosionImage), kernel);
    imshow("opening", openingImage);
    waitKey(0);
    
    // closing
    Mat closingImage = erosion(grayScale(dilationImage), kernel);
    imshow("closing", closingImage);
    waitKey(0);
    
    
    //upper right-hand corner
    Mat erosion_by_J;
    Mat erosion_by_K;
    Mat result;
    thresh_img.copyTo(erosion_by_J);
    erosion(erosion_by_J, J_kernel).copyTo(erosion_by_J);
    for(int i = 0 ; i < thresh_img.rows; i++){
        for(int j = 0 ; j < thresh_img.cols; j++){
            if(thresh_img.at<uint8_t>(i,j)!=0){
                thresh_img.at<uint8_t>(i,j)=0;
            }
            else{
                thresh_img.at<uint8_t>(i,j)=1;
            }
        }
    }
    thresh_img.copyTo(erosion_by_K);
    erosion(erosion_by_K, K_kernel).copyTo(erosion_by_K);
    grayScale(erosion_by_J).copyTo(erosion_by_J);
    grayScale(erosion_by_K).copyTo(erosion_by_K);
    erosion_by_J.copyTo(result);
    result = result.mul(erosion_by_K);
    for(int i = 0; i < result.rows; i++){
        for(int j = 0 ; j < result.cols; j++){
            if(result.at<uint8_t>(i,j) == 1){
                result.at<uint8_t>(i,j) = 255;
            }else{
                result.at<uint8_t>(i,j) = 0;
            }
        }
    }
    
    imshow("hit-and-miss", result);
    waitKey(0);
    imwrite("./corner.jpg", result);
    return 0;
}
