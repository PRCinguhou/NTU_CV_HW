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

Mat threshold(Mat img){
    Mat temp;
    img.copyTo(temp);
    for(int i = 0 ; i < img.rows; i++){
        for(int j = 0 ; j < img.cols ; j++){
            if(temp.at<uint8_t>(i,j) < 128){
                temp.at<uint8_t>(i,j) = 0;
            }else{
                temp.at<uint8_t>(i,j) = 255;
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
        if(b==c != 0){
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
    int kernelCount = kernel.rows;
    
    // shrink process
    for(int row = 0 ; row < 64; row++){
        for(int col = 0 ; col < 64 ; col++){
            shrinkImg.at<uint8_t>(row,col)=thresholdImg.at<uint8_t>(8*row, 8*col);
        }
    }
    cv::namedWindow("Display",WINDOW_NORMAL);
    imshow("Display",shrinkImg);
    
    waitKey(0);
    
    Mat finalResult = Mat::zeros(64, 64, CV_8UC1);
    
    // yokoi process
    for(int row = 0; row < 64; row++){
        for(int col = 0 ; col < 64; col++){
            if(shrinkImg.at<uint8_t>(row, col) == 255){
                int startRow = row > 0 ? 1 : 0;
                int startCol = col > 0 ? 1 : 0;
                int sizeRow = startRow == 1 ? 3 : 2;
                int sizeCol = startCol == 1 ? 3 : 2;
                sizeRow = row == 63 ? 2 : sizeRow;
                sizeCol = col == 63 ? 2 : sizeCol;
                Mat temp = Mat::zeros(3, 3, CV_8UC1);
                Mat gg = temp.colRange(0, sizeCol).rowRange(0, sizeRow);
                shrinkImg(cv::Rect(col - startCol, row - startRow, sizeCol, sizeRow)).copyTo(gg);
                
                finalResult.at<uint8_t>(row, col) = yokoi(temp);
            }
            
        }
        
    }
    for(int row = 0; row < 64; row++){
        for(int col = 0 ; col < 64; col++){
            if(finalResult.at<uint8_t>(row, col)!=0){
                cout << unsigned(finalResult.at<uint8_t>(row, col));
            }else{
                cout << ' ';
            }
        }
        cout << endl;
    }
    return 0;
}
