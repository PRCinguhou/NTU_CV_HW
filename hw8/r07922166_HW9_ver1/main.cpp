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



using namespace cv;
using namespace std;
Mat getKernel(Mat inputMat, int row, int col, int size);
Mat convolved(Mat inputMat, Mat kernel);
float matrixMultiplySum(Mat inputMat, Mat kernel);
Mat robert(Mat inputMat, int threshold);
Mat prewitt(Mat inputMat, int threshold);
Mat sobel(Mat inputMat, int threshold);
Mat frei_chen(Mat inputMat, int threshold);
Mat kirsch(Mat inputMat, int threshold);
Mat robinson(Mat inputMat, int threshold);
Mat babu(Mat inputMat, int threshold);

int main(int argc, const char * argv[]) {

    Mat image;
    image = imread("lena.bmp", 1); // Read the file
    cv::cvtColor(image, image, CV_RGB2GRAY);

    Mat robertImg = robert(image, 22);
    imwrite( "./robert.jpg", robertImg);
    
    Mat prewittImg = prewitt(image, 42);
    imwrite( "./prewitt.jpg", prewittImg);
    
    Mat sobelImg = sobel(image, 38);
    imwrite( "./sobel.jpg", sobelImg);
    
    Mat fre = frei_chen(image, 40);
    imwrite( "./frei_chen.jpg", fre);
    
    Mat kirschImg = kirsch(image, 125);
    imwrite( "./kirsch.jpg", kirschImg);
    
    Mat robinsonImg = robinson(image, 36);
    imwrite( "./robinson.jpg", robinsonImg);
    
    Mat babuImg = babu(image, 15400);
    imwrite( "./babu.jpg", babuImg);

    
    return 0;
}

Mat babu(Mat inputMat, int threshold){
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    Mat mask[6];
    mask[0] = (Mat_<float>(5,5)<<
               100, 100, 100, 100, 100,
               100, 100, 100, 100, 100,
                 0,   0,   0,   0,   0,
              -100,-100,-100,-100,-100,
              -100,-100,-100,-100,-100
               );
    mask[1] = (Mat_<float>(5,5)<<
               100, 100, 100, 100, 100,
               100, 100, 100,  78, -32,
               100,  92,   0, -92,-100,
                32, -78,-100,-100,-100,
               -100,-100,-100,-100,-100
               );
    mask[2] = (Mat_<float>(5,5)<<
               100, 100, 100, 32, 100,
               100, 100,  92,-78,-100,
               100, 100,   0,-100,-100,
               100,  78, -92,-100,-100,
               100, -32,-100,-100,-100
               );
    mask[3] = (Mat_<float>(5,5)<<
               -100,-100,   0, 100, 100,
               -100,-100,   0, 100, 100,
               -100,-100,   0, 100, 100,
               -100,-100,   0, 100, 100,
               -100,-100,   0, 100, 100
               );
    mask[4] = (Mat_<float>(5,5)<<
               -100,  32, 100, 100, 100,
               -100, -78,  92, 100, 100,
               -100,-100,   0, 100, 100,
               -100,-100, -92,  78, 100,
               -100,-100,-100, -32, 100
               );
    mask[5] = (Mat_<float>(5,5)<<
                100, 100, 100, 100, 100,
                -32,  78, 100, 100, 100,
               -100, -92,   0,  92, 100,
               -100,-100,-100, -78,  32,
               -100,-100,-100,-100,-100
               );
    for(int i = 5; i < inputMat.rows-5; i++){
        for(int j = 5 ; j < inputMat.cols-5; j++){
            Mat kernel = getKernel(inputMat, i, j, 5);
            float value = threshold-1;
            for(int k = 0 ; k < 6; k++){
                float temp = matrixMultiplySum(kernel, mask[k]);
                int num = 0;
                for(int row = 0 ; row < mask[k].rows; row++){
                    for(int col = 0 ; col < mask[k].cols; col++){
                        if(mask[k].at<uint8_t>(row, col) > 0)num+=1;
                    }
                }
                temp /= sqrt(num);
                if(temp > value)value = temp;
            }
            if(value > threshold)result.at<uint8_t>(i,j) = 0;
        }
    }
    return result;
}


Mat robinson(Mat inputMat, int threshold){
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    
    Mat mask[4];
    mask[0] = (Mat_<float>(3,3)<<
               1,  0, -1,
               2,  0, -2,
               1,  0, -1);
    mask[1] = (Mat_<float>(3,3)<<
               0,  1, 2,
              -1,  0, 1,
              -2, -1, 0);
    mask[2] = (Mat_<float>(3,3)<<
               1,  2,  1,
               0,  0,  0,
               -1, -2, -1);
    mask[3] = (Mat_<float>(3,3)<<
               2,  1,  0,
               1,  0, -1,
               0, -1, -2);
    for(int i = 2; i < inputMat.rows-2; i++){
        for(int j = 2 ; j < inputMat.cols-2; j++){
            
            Mat kernel = getKernel(inputMat, i, j, 3);
            float value = 0;
            
            for(int k = 0 ; k < 4; k++){
                float temp = matrixMultiplySum(kernel, mask[k]);
                temp /= (sqrt(6));
                if(temp > value)value = temp;
                    
                    }
            if(value > threshold)result.at<uint8_t>(i,j) = 0;
            
        }
    }
    
    return result;
}


Mat kirsch(Mat inputMat, int threshold){
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    
    Mat mask[8];
    mask[0] = (Mat_<float>(3,3)<<
               5,  5,  5,
              -3,  0, -3,
              -3, -3, -3);
    mask[1] = (Mat_<float>(3,3)<<
               5,  5, -3,
               5,  0, -3,
              -3, -3, -3);
    mask[2] = (Mat_<float>(3,3)<<
               5, -3, -3,
               5,  0, -3,
               5, -3, -3);
    mask[3] = (Mat_<float>(3,3)<<
              -3, -3, -3,
               5,  0, -3,
               5,  5, -3);
    mask[4] = (Mat_<float>(3,3)<<
              -3, -3, -3,
              -3,  0, -3,
               5,  5,  5);
    mask[5] = (Mat_<float>(3,3)<<
               -3, -3, -3,
               -3,  0,  5,
               -3,  5,  5);
    mask[6] = (Mat_<float>(3,3)<<
               -3, -3,  5,
               -3,  0,  5,
               -3, -3,  5);
    mask[7] = (Mat_<float>(3,3)<<
               -3,  5,  5,
               -3,  0,  5,
               -3, -3, -3);
    
    for(int i = 2; i < inputMat.rows-2; i++){
        for(int j = 2 ; j < inputMat.cols-2; j++){
            
            Mat kernel = getKernel(inputMat, i, j, 3);
            float value = 0;
            
            for(int k = 0 ; k < 8; k++){
                float temp = matrixMultiplySum(kernel, mask[k]);
                temp /= sqrt(15);
                if(temp > value)value = temp;
                
            }
            if(value > threshold)result.at<uint8_t>(i,j) = 0;
        }
    }
    return result;
}


Mat frei_chen(Mat inputMat, int threshold){
    
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    
    Mat mask[2];
    mask[0] = (Mat_<float>(3,3)<<
               1,  sqrt(2),  1,
               0,  0,  0,
               -1,  -sqrt(2), -1);
    mask[1] = (Mat_<float>(3,3)<<
               1,  0, -1,
            sqrt(2),  0,  -sqrt(2),
               1,  0, -1);
    
    for(int i = 2; i < inputMat.rows-2; i++){
        for(int j = 2 ; j < inputMat.cols-2; j++){
            
            Mat kernel = getKernel(inputMat, i, j, 3);
            float value = 0;
            
            for(int k = 0 ; k < 2; k++){
                float temp = matrixMultiplySum(kernel, mask[k]);
                
                value += ((temp * temp)/8);
            }
            if(sqrt(value) > threshold)result.at<uint8_t>(i,j) = 0;
        }
    }

    return result;
}

Mat sobel(Mat inputMat, int threshold){
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    
    Mat mask[2];
    mask[0] = (Mat_<float>(3,3)<<
               1,  0, -1,
               2,  0, -2,
               1,  0, -1);
    mask[1] = (Mat_<float>(3,3)<<
                1,  2,  1,
                0,  0,  0,
               -1, -2, -1);
    for(int i = 2; i < inputMat.rows-2; i++){
        for(int j = 2 ; j < inputMat.cols-2; j++){
            
            Mat kernel = getKernel(inputMat, i, j, 3);
            float value = 0;
            for(int k = 0 ; k < sizeof(mask)/sizeof(*mask); k++){
                
                float temp = matrixMultiplySum(kernel, mask[k]);
                
                value += (temp * temp)/8;
            }
            value = sqrt(value);
            
            if(value > threshold)result.at<uint8_t>(i,j) = 0;
        }
    }
    return result;
}
Mat prewitt(Mat inputMat, int threshold){
    
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    
    Mat mask[2];
    mask[0] = (Mat_<float>(3,3)<<
               -1, -1, -1,
                0,  0,  0,
                1,  1,  1);
    mask[1] = (Mat_<float>(3,3)<<
               -1,  0,  1,
               -1,  0,  1,
               -1,  0,  1);
    
    for(int i = 2; i < inputMat.rows-2; i++){
        for(int j = 2 ; j < inputMat.cols-2; j++){
            
            Mat kernel = getKernel(inputMat, i, j, 3);
            float value = 0;
            for(int k = 0 ; k < sizeof(mask)/sizeof(*mask); k++){
                
                float temp = matrixMultiplySum(kernel, mask[k]);
                
                value += (temp * temp)/6;
            }
            value = sqrt(value);
            
            if(value > threshold)result.at<uint8_t>(i,j) = 0;
        }
    }
    
    return result;
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

Mat robert(Mat inputMat, int threshold){
    
    Mat result(inputMat.rows, inputMat.cols, CV_8UC1, Scalar(255));
    Mat mask[2];
    
    mask[0] = (Mat_<float>(2,2)<<
               1, 0,
               0,-1);
    mask[1] = (Mat_<float>(2,2)<<
               0, 1,
               -1, 0);
    
    for(int i = 2; i < inputMat.rows-2; i++){
        for(int j = 2 ; j < inputMat.cols-2; j++){
            
            Mat kernel = getKernel(inputMat, i, j, 2);
            float value = 0;
            for(int k = 0 ; k < sizeof(mask)/sizeof(*mask); k++){
                
                float temp = matrixMultiplySum(kernel, mask[k]);
                
                value += (temp * temp)/2;
            }
            value = sqrt(value);
            
            if(value > threshold)result.at<uint8_t>(i,j) = 0;
        }
    }
    
    return result;
}

float matrixMultiplySum(Mat inputMat, Mat kernel){
    float total = 0;
    
    
    for(int i = 0 ; i < inputMat.rows; i++){
        for(int j= 0; j < inputMat.cols; j++){
            
            total += signed(inputMat.at<uint8_t>(i,j)) * kernel.at<float>(i,j);
        }
    }
    return total;
}
