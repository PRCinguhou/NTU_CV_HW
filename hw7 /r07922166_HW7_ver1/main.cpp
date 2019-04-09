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
#include <random>

using namespace cv;
using namespace std;
default_random_engine gernerator;
int findMax(Mat inputMat);
int findMin(Mat inputMat);
Mat grayScaleDilation(Mat inputMat, Mat kernel);
Mat grayScaleErosion(Mat inputMat, Mat kernel);
int MatMean(Mat inputMatrix);
Mat getKernel(Mat inputMat, int row, int col, int size);
Mat gaussianNoise(Mat inputImg, int amplitude, int mean, int sigma);
Mat salt_and_pepper(Mat inputImg, float prob_theshold);
Mat box_filter(Mat inputImg, int boxSize);
int find_median(Mat inputMatrix);
Mat median_filter(Mat inputImg, int size);
Mat openning_closing(Mat inputMat);
Mat closing_openning(Mat inputMat);
int MatSum(Mat inputMatrix);
float SNR(Mat original, Mat noise);

Mat kernel = (Mat_<uint8_t>(5,5)<<
              0, 0, 0, 0, 0,
              0, 0, 0, 0, 0,
              0, 0, 0, 0, 0,
              0, 0, 0, 0, 0,
              0, 0, 0, 0, 0);
int main(int argc, const char * argv[]) {
    
    // read image
    Mat image;
    image = imread("lena.bmp", 1); // Read the file
    cv::cvtColor(image, image, CV_RGB2GRAY);
    // noise image
    
    Mat gaussian10 = gaussianNoise(image, 10, 0, 1);
    cout << "gaussian10 SNR: " << SNR(image, gaussian10) << endl;
    imwrite( "./gaussian10.jpg", gaussian10 );
    
    Mat gaussian30 = gaussianNoise(image, 30, 0, 1);
    cout << "gaussian30 SNR: " << SNR(image, gaussian30) << endl;
    imwrite( "./gaussian30.jpg", gaussian30 );
    
    Mat saltPepper_5 = salt_and_pepper(image, 0.05);
    cout << "saltPepper_5 SNR: " << SNR(image, saltPepper_5) << endl;
    imwrite( "./saltPepper5.jpg", saltPepper_5);
    
    Mat saltPepper_10 = salt_and_pepper(image, 0.1);
    cout << "saltPepper_10 SNR: " << SNR(image, saltPepper_10) << endl;
    imwrite( "./saltPepper10.jpg", saltPepper_10);
    
    // box filter remove noise
    Mat gaussian10_Box3 = box_filter(gaussian10, 3);
    imwrite( "./gaussian10_Box3.jpg", gaussian10_Box3);
    
    Mat gaussian10_Box5 = box_filter(gaussian10, 5);
    imwrite( "./gaussian10_Box5.jpg", gaussian10_Box5);
    
    Mat gaussian30_Box3 = box_filter(gaussian30, 3);
    imwrite( "./gaussian30_Box3.jpg", gaussian30_Box3);
    
    Mat gaussian30_Box5 = box_filter(gaussian30, 5);
    imwrite( "./gaussian30_Box5.jpg", gaussian30_Box5);
    
    Mat saltPepper5_Box3 = box_filter(saltPepper_5, 3);
    imwrite( "./saltPepper5_Box3.jpg", saltPepper5_Box3);
    
    Mat saltPepper5_Box5 = box_filter(saltPepper_5, 5);
    imwrite( "./saltPepper5_Box5.jpg", saltPepper5_Box5);
    
    Mat saltPepper10_Box3 = box_filter(saltPepper_10, 3);
    imwrite( "./saltPepper10_Box3.jpg", saltPepper10_Box3);
    
    Mat saltPepper10_Box5 = box_filter(saltPepper_10, 5);
    imwrite( "./saltPepper10_Box5.jpg", saltPepper10_Box5);
    
    // median filter remove noise
    Mat gaussian10_median3 = median_filter(gaussian10, 3);
    imwrite( "./gaussian10_median3.jpg", gaussian10_median3);
    
    Mat gaussian10_median5 = median_filter(gaussian10, 5);
    imwrite( "./gaussian10_median5.jpg", gaussian10_median5);
    
    Mat gaussian30_median3 = median_filter(gaussian30, 3);
    imwrite( "./gaussian30_median3.jpg", gaussian30_median3);
    
    Mat gaussian30_median5 = median_filter(gaussian30, 5);
    imwrite( "./gaussian30_median5.jpg", gaussian10_median5);
    
    Mat saltPepper5_median3 = median_filter(saltPepper_5, 3);
    imwrite( "./saltPepper5_median3.jpg", saltPepper5_median3);
    
    Mat saltPepper5_median5 = median_filter(saltPepper_5, 5);
    imwrite( "./saltPepper5_median5.jpg", saltPepper5_median5);
    
    Mat saltPepper10_median3 = median_filter(saltPepper_10, 3);
    imwrite( "./saltPepper10_median3.jpg", saltPepper10_median3);
    
    Mat saltPepper10_median5 = median_filter(saltPepper_10, 5);
    imwrite( "./saltPepper10_median5.jpg", saltPepper10_median5);
    
    // opening closing
    Mat gaussian10_op_cl = openning_closing(gaussian10);
    imwrite( "./gaussian10_op_cl.jpg", gaussian10_op_cl);
    
    Mat gaussian10_cl_op = closing_openning(gaussian10);
    imwrite( "./gaussian10_cl_op.jpg", gaussian10_cl_op);
    
    Mat gaussian30_op_cl = openning_closing(gaussian30);
    imwrite( "./gaussian30_op_cl.jpg", gaussian30_op_cl);
    
    Mat gaussian30_cl_op = closing_openning(gaussian30);
    imwrite( "./gaussian30_cl_op.jpg", gaussian30_cl_op);
    
    Mat saltPepper5_op_cl = openning_closing(saltPepper_5);
    imwrite( "./saltPepper5_op_cl.jpg", saltPepper5_op_cl);
    
    Mat saltPepper5_cl_op = closing_openning(saltPepper_5);
    imwrite( "./saltPepper5_cl_op.jpg", saltPepper5_cl_op);
    
    Mat saltPepper10_op_cl = openning_closing(saltPepper_10);
    imwrite( "./saltPepper10_op_cl.jpg", saltPepper10_op_cl);
    
    Mat saltPepper10_cl_op = closing_openning(saltPepper_10);
    imwrite( "./saltPepper10_cl_op.jpg", saltPepper10_cl_op);
    
    
    

    
    
    return 0;
}

float SNR(Mat original, Mat noise){
    float Src=0;
    float Des=0;
    float totalPixel = original.rows * original.cols;
    for(int i = 0 ; i < original.rows; i++){
        for(int j = 0 ; j < original.cols; j++){
            Src += float(original.at<uint8_t>(i,j));
            Des += float(original.at<uint8_t>(i,j)-noise.at<uint8_t>(i,j));
        }
    }
    float SrcMean = Src/totalPixel;
    float DesMean = Des/totalPixel;
    
    float VsSum = 0;
    float VnSum = 0;
    
    for(int i = 0 ; i < original.rows; i++){
        for(int j = 0 ; j < original.cols; j++){
            VsSum += float(original.at<uint8_t>(i,j)-SrcMean);
            VnSum += float(original.at<uint8_t>(i,j)-noise.at<uint8_t>(i,j)-DesMean);
        }
    }
    
    float VsToVn =0;
    VsToVn = sqrt((VsSum/totalPixel)/(VnSum/totalPixel));
    
    
    return 20 * log10(VsToVn);
}

int MatSum(Mat inputMatrix){
    int total = 0;
    
    for(int i = 0 ; i < inputMatrix.rows; i++){
        for(int j = 0 ; j < inputMatrix.cols; j++){
            total+=inputMatrix.at<uint8_t>(i,j);
        }
    }
    return total;
}

Mat openning_closing(Mat inputMat){
    Mat result;
    inputMat.copyTo(result);
    grayScaleErosion(result, kernel).copyTo(result);
    grayScaleDilation(result, kernel).copyTo(result);
    grayScaleDilation(result, kernel).copyTo(result);
    grayScaleErosion(result, kernel).copyTo(result);
    
    return result;
}


Mat closing_openning(Mat inputMat){
    Mat result;
    inputMat.copyTo(result);
    
    grayScaleDilation(result, kernel).copyTo(result);
    grayScaleErosion(result, kernel).copyTo(result);
    grayScaleErosion(result, kernel).copyTo(result);
    grayScaleDilation(result, kernel).copyTo(result);
    
    return result;
}

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
int MatMean(Mat inputMatrix){
    int total = 0;
    int numbers = inputMatrix.rows * inputMatrix.cols;
    for(int i = 0 ; i < inputMatrix.rows; i++){
        for(int j = 0 ; j < inputMatrix.cols; j++){
            total+=inputMatrix.at<uint8_t>(i,j);
        }
    }
    return total/numbers;
}
Mat getKernel(Mat inputMat, int row, int col, int size){

    int startRow = row > (size-3)/2 ? (size-1)/2 : row;
    int startCol = col > (size-3)/2 ? (size-1)/2 : col;
    int sizeRow = startRow == (size-1)/2 ? size : (size+1)/2 + startRow;
    int sizeCol = startCol == (size-1)/2 ? size : (size+1)/2 + startCol;
    
    sizeRow = row+(size-1)/2 > inputMat.rows-1 ? (size+1)/2 : sizeRow;
    sizeCol = col+(size-1)/2 > inputMat.cols-1 ? (size+1)/2 : sizeCol;
    Mat temp;
    
    inputMat(cv::Rect(col - startCol, row - startRow, sizeCol, sizeRow)).copyTo(temp);
    
    
    return temp;
}

Mat gaussianNoise(Mat inputImg, int amplitude, int mean, int sigma){
    Mat noiseImg;
    inputImg.copyTo(noiseImg);
    
    normal_distribution<double> distribution(mean, sigma);
    for(int i = 0 ; i < noiseImg.rows ; i++){
        for(int j = 0 ; j < noiseImg.cols ; j++){
            int value = distribution(gernerator);
            noiseImg.at<uint8_t>(i,j) += value * amplitude;
        }
    }
    return noiseImg;
}

Mat salt_and_pepper(Mat inputImg, float prob_theshold){
    Mat noiseImg;
    inputImg.copyTo(noiseImg);
    uniform_real_distribution<double>distribution(0.0, 1.0);
    for(int i=0; i < noiseImg.rows; i++){
        for(int j=0 ; j < noiseImg.cols; j++){
            float prob = distribution(gernerator);
            if(prob <= prob_theshold){
                noiseImg.at<uint8_t>(i,j)=0;
            }else if(prob >= (1 - prob_theshold)){
                noiseImg.at<uint8_t>(i,j)=255;
            }
        }
    }
    
    return noiseImg;
}

Mat box_filter(Mat inputImg, int boxSize){
    Mat result;
    inputImg.copyTo(result);
    for(int i = 0; i < inputImg.rows; i++){
        for(int j = 0 ; j < inputImg.cols; j++){
            Mat box = getKernel(result, i, j, boxSize);
            int mean = MatMean(box);
            result.at<uint8_t>(i,j) = mean;
        }
    }
    
    return result;
}

int find_median(Mat inputMatrix){
    int median = 0;
    
    int temp[inputMatrix.rows*inputMatrix.cols];
    
    for(int i = 0 ; i < inputMatrix.rows;i++){
        for(int j = 0 ; j < inputMatrix.cols; j++){
            temp[i*inputMatrix.cols+j] = inputMatrix.at<uint8_t>(i,j);
        }
    }
    sort(temp, temp + inputMatrix.rows*inputMatrix.cols);
    
    median = temp[int((inputMatrix.rows*inputMatrix.cols-1)/2)];
    
    return median;
}

Mat median_filter(Mat inputImg, int size){
    Mat result;
    inputImg.copyTo(result);
    for(int i = 0 ; i < result.rows; i++){
        for(int j = 0 ; j < result.cols; j++){
            Mat kernel = getKernel(inputImg, i, j, size);
            int median = find_median(kernel);
            
            result.at<uint8_t>(i,j) = median;
        }
    }
    return result;
}
