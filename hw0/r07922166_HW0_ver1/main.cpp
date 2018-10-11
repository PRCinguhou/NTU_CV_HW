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


using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    
   
    
    // read image
    Mat image;
    image = imread("lena.bmp", IMREAD_COLOR); // Read the file
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    
    
    // 負片
    Mat neg_img ;
    image.copyTo(neg_img);
    for(int i = 0; i < image.rows ; i++){
        for(int j = 0 ; j < image.cols*3 ; j++){
            neg_img.at<uint8_t>(i,j)=255-image.at<uint8_t>(i,j);
        }
    }
    imwrite("./neg.jpg", neg_img);
    
    // 全部變128
    Mat const_img ;
    image.copyTo(const_img);
    for(int i = 0; i < image.rows ; i++){
        // *3 是因為r,g,b
        for(int j = 0 ; j < image.cols*3 ; j++){
            const_img.at<uint8_t>(i,j)=128;
        }
    }
    imwrite("./const.jpg", const_img);
    
    
    // threshold 128 image
    Mat thresh_img ;
    image.copyTo(thresh_img);
    for(int i = 0; i < image.rows ; i++){
        for(int j = 0 ; j < image.cols*3 ; j++){
            if(thresh_img.at<uint8_t>(i,j) <= 128){
                thresh_img.at<uint8_t>(i,j)=0;
            }else if(thresh_img.at<uint8_t>(i,j) > 128){
                thresh_img.at<uint8_t>(i,j)=255;
            }
        }
    }
    imwrite("./thresh.jpg", thresh_img);
    
    

//  rotate image / 或使用software 轉45度
//    Mat rot ;
//    Point2f pc(image.cols/2., image.rows/2.);
//    Mat rotate_img = getRotationMatrix2D(pc, -45., 1);
//    warpAffine(image, rot, rotate_img, image.size());
//    imwrite("rot.jpg", rot);

    
    
    return 0;
}
