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

int findMax(Mat inputMat){
    int max = 0;
    for(int i = 0; i < inputMat.rows; i++){
        for(int j = 0 ; j < inputMat.cols; j++){
            if(inputMat.at<uint8_t>(i,j) > max){
                max = inputMat.at<uint8_t>(i,j);
            }
        }
    }
    return max;
}

int findMin(Mat inputMat){
    int min = 255;
    for(int i = 0; i < inputMat.rows; i++){
        for(int j = 0 ; j < inputMat.cols; j++){
            if(inputMat.at<uint8_t>(i,j) < min){
                min = inputMat.at<uint8_t>(i,j);
            }
        }
    }
    return min;
}

Mat grayScaleDilation(Mat inputMat, Mat kernel){
    Mat temp;
    temp = Mat::zeros(inputMat.rows, inputMat.cols, CV_8UC1);
    int kernelCount = (kernel.cols-1)/2;
    
    for(int row = 0; row < inputMat.rows; row++){
        for(int col = 0 ; col < inputMat.cols; col++){
            int startRow = row > kernelCount ? kernelCount : row;
            int startCol = col > kernelCount ? kernelCount : col;
            int sizeRow = (inputMat.rows-1) - row < kernelCount ? inputMat.rows - row : kernelCount + 1;
            int sizeCol = (inputMat.cols-1) - col < kernelCount ? inputMat.cols - col : kernelCount + 1;
            
            Mat dilation_temp = inputMat(cv::Rect(col - startCol, row - startRow, startCol + sizeCol,  startRow + sizeRow));
            int max = findMax(dilation_temp);
            temp.at<uint8_t>(row, col)=max;
        }
    }
    return temp;
}

Mat grayScaleErosion(Mat inputMat, Mat kernel){
    Mat temp;
    temp = Mat::zeros(inputMat.rows, inputMat.cols, CV_8UC1);
    int kernelCount = (kernel.cols-1)/2;
    
    for(int row = 0; row < inputMat.rows; row++){
        for(int col = 0 ; col < inputMat.cols; col++){
            int startRow = row > kernelCount ? kernelCount : row;
            int startCol = col > kernelCount ? kernelCount : col;
            int sizeRow = (inputMat.rows-1) - row < kernelCount ? inputMat.rows - row : kernelCount + 1;
            int sizeCol = (inputMat.cols-1) - col < kernelCount ? inputMat.cols - col : kernelCount + 1;
            
            Mat erosion_temp = inputMat(cv::Rect(col - startCol, row - startRow, startCol + sizeCol,  startRow + sizeRow));
            int min = findMin(erosion_temp);
            temp.at<uint8_t>(row, col)=min;
        }
    }
    return temp;
}

int main(int argc, const char * argv[]) {
    // read image
    Mat image;
    image = imread("lena.bmp", IMREAD_COLOR); // Read the file
    // threshold 128 image
    Mat thresh_img ;
    image.copyTo(thresh_img);
    cv::cvtColor(thresh_img, thresh_img, CV_RGB2GRAY);
    imshow("graydilation", thresh_img );
    waitKey(0);
    
    
    
    Mat kernel = (Mat_<uint8_t>(5,5)<<
                  0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0);
    
    Mat DilationResult = grayScaleDilation(thresh_img, kernel);
    imshow("graydilation", DilationResult);
    waitKey(0);
    imwrite("./dilation.jpg", DilationResult);
    
    Mat ErosionResult = grayScaleErosion(thresh_img, kernel);
    imshow("graydilation", ErosionResult);
    waitKey(0);
    imwrite("./erosion.jpg", ErosionResult);
    
    Mat OpeningResult = grayScaleDilation(ErosionResult, kernel);
    imshow("graydilation", OpeningResult);
    waitKey(0);
    imwrite("opening.jpg", OpeningResult);
    
    Mat ClosingResult = grayScaleErosion(DilationResult, kernel);
    imshow("graydilation", ClosingResult);
    waitKey(0);
    imwrite("./closing.jpg", ClosingResult);
  
}
