//
//  main.cpp
//  test
//
//  Created by 李淑貞 on 2018/9/12.
//  Copyright © 2018年 李淑貞. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>


using namespace cv;
using namespace std;
Mat getKernel(Mat inputMat, int row, int col, int size);
Mat laplacian(Mat img, Mat kernel, int size, int threshold);
void gernerateDOG(Mat inputMat, int kernelSize, float sigma1, float sigma2);
int main(int argc, const char * argv[]) {

    Mat image;
    image = imread("lena.bmp", 1); // Read the file
    cv::cvtColor(image, image, CV_RGB2GRAY);
    
    Mat kernel1 = (Mat_<float>(3,3)<< 0, 1, 0, 1, -4, 1, 0, 1, 0);
    Mat kernel2 = (Mat_<float>(3,3)<< 1./3., 1./3., 1./3., 1./3., -8./3., 1./3., 1./3., 1./3., 1./3.);
    Mat kernel3 = (Mat_<float>(3,3)<< 2./3., -1./3., 2./3., -1./3., -4./3., -1./3., 2./3., -1./3., 2./3.);
    Mat kernel4 = (Mat_<float>(11,11)<<
        0, 0,  0, -1, -1, -2, -1, -1,  0,  0,  0,
        0, 0, -2, -4, -8, -9, -8, -4, -2,  0,  0,
        0,-2, -7,-15,-22,-23,-22,-15, -7, -2,  0,
        -1,-4,-15,-24,-14, -1,-14,-24,-15, -4, -1,
        -1,-8,-22,-14, 52,103, 52,-14,-22, -8, -1,
        -2,-9,-23, -1,103,178,103, -1,-23, -9, -2,
        -1,-8,-22,-14, 52,103, 52,-14,-22, -8, -1,
        -1,-4,-15,-24,-14, -1,-14,-24,-15, -4, -1,
        0,-2, -7,-15,-22,-23,-22,-15, -7, -2,  0,
        0, 0, -2, -4, -8, -9, -8, -4, -2,  0,  0,
        0, 0,  0, -1, -1, -2, -1, -1,  0,  0,  0 );
    
    
    Mat kernel5 = Mat::zeros(11, 11, CV_32F);
    gernerateDOG(kernel5, 11, 1., 3.);

    Mat img1 = laplacian(image, kernel1, 3, 20);
    Mat img2 = laplacian(image, kernel2, 3, 15);
    Mat img3 = laplacian(image, kernel3, 3, 12);
    Mat img4 = laplacian(image, kernel4, 11, 1600);
    Mat img5 = laplacian(image, kernel5, 11, 6);
//
    imshow("Display",img1);
    waitKey(0);
    imshow("Display",img2);
    waitKey(0);
    imshow("Display",img3);
    waitKey(0);
    imshow("Display",img4);
    waitKey(0);
    imshow("Display",img5);
    waitKey(0);

    
    return 0;
}

Mat laplacian(Mat img, Mat kernel, int size, int threshold){
    
    Mat result(img.rows, img.cols, CV_8UC1, Scalar(255));
    float first[img.rows][img.cols];
    for(int i = (size-1)/2 ; i < img.rows; i++){
        for(int j = (size-1)/2 ; j < img.cols ; j++){
            Mat temp = getKernel(img, i-(size-1)/2, j-(size-1)/2, size);
            float sum = 0 ;
            for(int x = 0 ; x < size; x++){
                for(int y = 0 ; y < size; y++){
                    sum += float(temp.at<uint8_t>(x,y)) * kernel.at<float>(x,y);
                }
            }
            first[i][j] = sum;
        }
    }
    for(int i = 0 ; i < 7; i++){
        for(int j = 0 ; j < img.cols;j++){
            first[i][j] = 0;
            first[img.rows-i-1][j] = 0;
        }
    }
    for(int i = (size-1)/2 ; i < img.rows-(size-1)/2 ; i++){
        for(int j = (size-1)/2 ; j < img.cols-(size-1)/2; j++){
            if(first[i][j] > threshold){
                for(int x = -1 ; x <= 1; x++){
                    for(int y = -1 ; y <= 1; y++){
                        if(first[i+x][j+y] < (-1*threshold)){
                            result.at<uint8_t>(i,j) = 0;
                        }
                    }
                }
            }
        }
    }
    
    return result;
}
void gernerateDOG(Mat inputMat, int kernelSize, float sigma1, float sigma2){
    float mean = 0;
    int start = -1 * ((kernelSize-1)/2);
    int end = ((kernelSize+1)/2);
    
    for(int i = start ; i < end ; i++){
        for(int j = start ; j < end; j++){
            float a = (1/sqrt((2*M_PI)*sigma1*sigma1)) * exp(-1*(i*i+j*j)/(2*sigma1*sigma1));
            float b = (1/sqrt((2*M_PI)*sigma2*sigma2)) * exp(-1*(i*i+j*j)/(2*sigma2*sigma2));
            int bias = (kernelSize-1)/2;
            inputMat.at<float>(i+bias, j+bias) = a-b;
            mean += a-b;
        }
    }
    mean /= (kernelSize*kernelSize);
    for(int i = 0; i < kernelSize ; i ++){
        for(int j = 0 ; j < kernelSize; j++){
            inputMat.at<float>(i, j)-=mean;
        }
    }
    
}

Mat getKernel(Mat inputMat, int row, int col, int size){
    
    int startRow = row > (size-3)/2 ? (size-1)/2 : row;
    int startCol = col > (size-3)/2 ? (size-1)/2 : col;
    int sizeRow = startRow == (size-1)/2 ? size : (size+1)/2 + startRow;
    int sizeCol = startCol == (size-1)/2 ? size : (size+1)/2 + startCol;
    
    sizeRow = row+(size-1)/2 > inputMat.rows-1 ? (size+1)/2 : sizeRow;
    sizeCol = col+(size-1)/2 > inputMat.cols-1 ? (size+1)/2 : sizeCol;
    Mat temp = Mat::zeros(size, size, CV_8UC1);
    
    if(size % 2 == 0)inputMat(cv::Rect(col - startCol, row - startRow - 1, sizeCol, sizeRow)).copyTo(temp);
    else if(size % 2 == 1)inputMat(cv::Rect(col - startCol, row - startRow , sizeCol, sizeRow)).copyTo(temp);
    
    
    return temp;
}
