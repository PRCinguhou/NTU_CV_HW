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

int main(int argc, const char * argv[]) {
    
   
    
    // read image
    Mat image;
    image = imread("lena.bmp", IMREAD_COLOR); // Read the file
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    
    
    
    // threshold 128 image
    float hist[256]={0};
    float histPDF[256]={0};
    float histCDF[256]={0};

    Mat thresh_img ;
    image.copyTo(thresh_img);
    cv::cvtColor(thresh_img, thresh_img, CV_RGB2GRAY);
    
    for(int i = 0; i < image.rows ; i++){
        for(int j = 0 ; j < image.cols ; j++){
            hist[thresh_img.at<uint8_t>(i,j)]+=1;
        }
    }
    
    Mat histogram_graph1(255,255,CV_8UC1,Scalar(255));
    int max1=0; //找出最多的pixel個數，畫圖的時候方便計算比例
    for(int i = 0 ; i < sizeof(hist)/sizeof(*hist); i++){
        if(hist[i] > max1)max1 = int(hist[i]);
    }
    
    // 畫圖
    for(int i = 0; i <  sizeof(hist)/sizeof(*hist) ; i++){
        
        cv::line(histogram_graph1, Point(i,255), Point(i, 255-int(hist[i])*255/max1), Scalar(0),1);
    }
    
    imshow("gray", histogram_graph1);
    waitKey(0);
    
    
    
    
    
    
    int total=0;
    for(int i =0;i< sizeof(hist)/sizeof(*hist);i++){
        total+=hist[i];
    }
    
    for(int i =0;i< sizeof(histPDF)/sizeof(*histPDF);i++){
        histPDF[i]=hist[i]/total;
    }
    
    histCDF[0]=histPDF[0];
    for(int i = 1;i < sizeof(histCDF)/sizeof(*histCDF);i++){
        histCDF[i] = histCDF[i-1]+histPDF[i];
    }
    
    for(int i = 0; i < thresh_img.rows ; i++){
        for(int j = 0 ; j < thresh_img.cols ; j++){
            thresh_img.at<uint8_t>(i,j) = int(round(255*histCDF[thresh_img.at<uint8_t>(i,j)]));
        }
    }

    imwrite("./bright.jpg", thresh_img);

    
    
    // histogram
    Mat temp;
    thresh_img.copyTo(temp);
    
    // 計算各pixel數量
    float histogram[256]={0};
    for(int i =0; i < temp.rows; i++){
        for(int j =0; j<temp.cols; j++){
            histogram[int(temp.at<uint8_t>(i,j))]+=1;
        }
    }
    
    Mat histogram_graph(255,255,CV_8UC1,Scalar(255));
    int max=0; //找出最多的pixel個數，畫圖的時候方便計算比例
    for(int i = 0 ; i < sizeof(histogram)/sizeof(*histogram); i++){
        if(histogram[i] > max){
            max = int(histogram[i]);
        }
    }
    
    // 畫圖
    for(int i = 0; i <  sizeof(histogram)/sizeof(*histogram) ; i++){
        cv::line(histogram_graph, Point(i,255), Point(i, 255-int(histogram[i])*255/max), Scalar(0),1);
    }
    
    imshow("gray", histogram_graph);
    waitKey(0);

    
    
    

    
    
    return 0;
}
