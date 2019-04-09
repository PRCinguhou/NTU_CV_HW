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
#include <tuple>


using namespace cv;
using namespace std;

Mat getKernel(Mat inputMat, int row, int col, int padding);
Mat threshold(Mat img);
int yokoi(Mat inputMat);
Mat inter_and_border(Mat inputMat);
Mat marked(Mat inputMat);

int MatSum(Mat inputMat){
    int total = 0;
    for(int i = 0 ; i < inputMat.rows; i++ ){
        for(int j = 0 ; j < inputMat.cols; j ++){
            total += inputMat.at<uint8_t>(i,j);
        }
    }
    return total;
}

Mat threshold(Mat img){
    Mat temp;
    img.copyTo(temp);
    for(int i = 0 ; i < img.rows; i++){
        for(int j = 0 ; j < img.cols ; j++){
            if(temp.at<uint8_t>(i,j) < 128){
                temp.at<uint8_t>(i,j) = 0;
            }else{
                temp.at<uint8_t>(i,j) = 1;
            }
        }
    }
    return temp;
}


int yokoi(Mat inputMat){
    int q = 0, r = 0;
    
    int row = 1, col = 0;
    for(int i = 1; i < 5; i++){
        if(i % 3 == 0){
            col +=1;
        }else if(i % 3 != 0){
            row += ((i%4) - 1 );
        }
        
        int b = inputMat.at<uint8_t>(row, col), c = inputMat.at<uint8_t>(row, col+1),
            d = inputMat.at<uint8_t>(row-1, col+1), e = inputMat.at<uint8_t>(row-1, col);
        
        for(int j = 0; j < i%4; j++){
            int temp = b;
            b = c;
            c = d;
            d = e;
            e = temp;
        }
        if(b==c==1){
            if(d == b && e == b) r+=1;
            else if(d!=b || e!=b) q+=1;
        }
    }
    if(r == 4){
        return 5;
    }else{
        return q;
    }
    
}


Mat marked(Mat inputMat){
    Mat temp = Mat::zeros(inputMat.rows, inputMat.cols, CV_8UC1);
    for(int row = 0 ; row < inputMat.rows; row++){
        for(int col = 0 ; col < inputMat.cols; col++){
            if(inputMat.at<uint8_t>(row, col) == 1){
                Mat kernel;
                getKernel(inputMat, row, col, 0).copyTo(kernel);
                kernel.at<uint8_t>(0,0)=0;
                kernel.at<uint8_t>(2,0)=0;
                kernel.at<uint8_t>(0,2)=0;
                kernel.at<uint8_t>(2,2)=0;
                kernel.at<uint8_t>(1,1)=0;
                bool find = false;
                for(int i = 0 ; i < 3; i++){
                    for(int j = 0 ; j < 3; j++){
                        if(kernel.at<uint8_t>(i,j)==1){
                            find = true;
                            break;
                        }
                    }
                    if(find){
                        break;
                    }
                }
                if(find){
                    temp.at<uint8_t>(row, col) = 1;
                }
            }
        }
    }
    return temp;
}

Mat getKernel(Mat inputMat, int row, int col, int padding){

    int startRow = row > 0 ? 1 : 0;
    int startCol = col > 0 ? 1 : 0;
    int sizeRow = startRow == 1 ? 3 : 2;
    int sizeCol = startCol == 1 ? 3 : 2;
    
    sizeRow = row == inputMat.rows-1 ? 2 : sizeRow;
    sizeCol = col == inputMat.cols-1 ? 2 : sizeCol;
    Mat temp;
    if(padding == 1){
        temp = Mat::ones(3, 3, CV_8UC1);
    }else{
        temp = Mat::zeros(3, 3, CV_8UC1);
    }
    
    Mat gg = temp.colRange(1-startCol, 1-startCol+sizeCol).rowRange(1-startRow, 1-startRow+sizeRow);
    
    
    inputMat(cv::Rect(col - startCol, row - startRow, sizeCol, sizeRow)).copyTo(gg);
    
    
    return temp;
}

int main(int argc, const char * argv[]) {

    // read image
    Mat image;
    image = imread("lena.bmp", 1); // Read the file
    cv::cvtColor(image, image, CV_RGB2GRAY);
    // binary img
    Mat thresholdImg = threshold(image);

    // 64*64 img
    Mat shrinkImg = Mat::zeros(64, 64, CV_8UC1);
    // kernel
    Mat kernel = Mat::ones(8, 8, CV_8UC1);


    // shrink process
    for(int row = 0 ; row < 64; row++){
        for(int col = 0 ; col < 64 ; col++){
            shrinkImg.at<uint8_t>(row,col)=thresholdImg.at<uint8_t>(8*row, 8*col);
        }
    }
    
    

    Mat finalResult;
    
    while(1){
        int value = MatSum(shrinkImg);
        
        Mat yokoiImg = Mat::zeros(64, 64, CV_8UC1);
        // yokoi process
        for(int row = 0; row < 64; row++){
            for(int col = 0 ; col < 64; col++){
                if(shrinkImg.at<uint8_t>(row, col) == 1){
                    int startRow = row > 0 ? 1 : 0;
                    int startCol = col > 0 ? 1 : 0;
                    int sizeRow = startRow == 1 ? 3 : 2;
                    int sizeCol = startCol == 1 ? 3 : 2;
                    sizeRow = row == 63 ? 2 : sizeRow;
                    sizeCol = col == 63 ? 2 : sizeCol;
                    Mat temp = Mat::zeros(3, 3, CV_8UC1);
                    Mat gg = temp.colRange(0, sizeCol).rowRange(0, sizeRow);
                    shrinkImg(cv::Rect(col - startCol, row - startRow, sizeCol, sizeRow)).copyTo(gg);
                    yokoiImg.at<uint8_t>(row, col) = yokoi(temp);
                }
            }
        }
        Mat markedImg = Mat::zeros(shrinkImg.rows, shrinkImg.cols, CV_8UC1);
        marked(yokoiImg).copyTo(markedImg);
        
        shrinkImg.copyTo(finalResult);
        for(int row = 0 ; row < finalResult.rows; row++){
            for(int col = 0 ; col < finalResult.cols ; col++){
                if(markedImg.at<uint8_t>(row, col) == 1){
                    Mat kernel = getKernel(finalResult, row, col, 0);
                    if(yokoi(kernel)==1){
                        finalResult.at<uint8_t>(row, col) = 0;
                    }
                }
            }
        }
        if(MatSum(finalResult)==value){
            break;
        }
        finalResult.copyTo(shrinkImg);

    }
    for(int row = 0 ; row < finalResult.rows; row++){
        for(int col = 0 ; col < finalResult.cols ; col++){
            if(finalResult.at<uint8_t>(row, col)!=0){
                finalResult.at<uint8_t>(row, col)=255;
            }
        }
    }
    cv::namedWindow("Display",WINDOW_NORMAL);
    imshow("Display",finalResult);

    waitKey(0);
    return 0;
    
}
