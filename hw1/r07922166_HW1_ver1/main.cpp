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

std::tuple<Mat, int> topdown_labeling(Mat image, int i, int j, int label){
    int min = label;
    if(i < image.rows-1 && j < image.cols-1){
        if(image.at<uint8_t>(i,j+1)>1 && image.at<uint8_t>(i,j+1)<min) min = image.at<uint8_t>(i,j+1);
        if(image.at<uint8_t>(i,j-1)>1 && image.at<uint8_t>(i,j-1)<min) min = image.at<uint8_t>(i,j-1);
        if(image.at<uint8_t>(i+1,j)>1 && image.at<uint8_t>(i+1,j)<min) min = image.at<uint8_t>(i+1,j);
        if(image.at<uint8_t>(i-1,j)>1 && image.at<uint8_t>(i-1,j)<min) min = image.at<uint8_t>(i-1,j);
//        if(image.at<uint8_t>(i+1,j+1)>1 && image.at<uint8_t>(i+1,j+1)<min) min = image.at<uint8_t>(i+1,j+1);
//        if(image.at<uint8_t>(i-1,j-1)>1 && image.at<uint8_t>(i-1,j-1)<min) min = image.at<uint8_t>(i-1,j-1);
//        if(image.at<uint8_t>(i+1,j-1)>1 && image.at<uint8_t>(i+1,j-1)<min) min = image.at<uint8_t>(i+1,j-1);
//        if(image.at<uint8_t>(i-1,j+1)>1 && image.at<uint8_t>(i-1,j+1)<min) min = image.at<uint8_t>(i-1,j+1);
        image.at<uint8_t>(i,j)=min;
    }
    if(min == label){
        label = label % 256 + 1;
    }
    return {image, label};
}

Mat bottomup_labeling(Mat image, int i, int j){
    int min = 1000;
    if(i < image.rows-1 && j < image.cols-1){
        if(image.at<uint8_t>(i,j+1)>1 && image.at<uint8_t>(i,j+1)<min) min = image.at<uint8_t>(i,j+1);
        if(image.at<uint8_t>(i,j-1)>1 && image.at<uint8_t>(i,j-1)<min) min = image.at<uint8_t>(i,j-1);
        if(image.at<uint8_t>(i+1,j)>1 && image.at<uint8_t>(i+1,j)<min) min = image.at<uint8_t>(i+1,j);
        if(image.at<uint8_t>(i-1,j)>1 && image.at<uint8_t>(i-1,j)<min) min = image.at<uint8_t>(i-1,j);
//        if(image.at<uint8_t>(i+1,j+1)>1 && image.at<uint8_t>(i+1,j+1)<min) min = image.at<uint8_t>(i+1,j+1);
//        if(image.at<uint8_t>(i-1,j-1)>1 && image.at<uint8_t>(i-1,j-1)<min) min = image.at<uint8_t>(i-1,j-1);
//        if(image.at<uint8_t>(i+1,j-1)>1 && image.at<uint8_t>(i+1,j-1)<min) min = image.at<uint8_t>(i+1,j-1);
//        if(image.at<uint8_t>(i-1,j+1)>1 && image.at<uint8_t>(i-1,j+1)<min) min = image.at<uint8_t>(i-1,j+1);
        image.at<uint8_t>(i,j)=min;
    }
    
    return image;
}



int main(int argc, const char * argv[]) {
    
   
    
    // read image
    Mat image;
    image = imread("lena.bmp", 1); // Read the file
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    
    
    
    
    
    // histogram
    Mat temp;
    cvtColor(image, temp, CV_RGB2GRAY );
    
    // 計算各pixel數量
    int histogram[255]={0};
    for(int i =0; i < temp.rows; i++){
        for(int j =0; j<temp.cols; j++){
            histogram[temp.at<uint8_t>(i,j)]+=1;
        }
    }
    
    Mat histogram_graph(256,256,CV_8UC1,Scalar(255));
    int max=0; //找出最多的pixel個數，畫圖的時候方便計算比例
    for(int i = 0 ; i < sizeof(histogram)/sizeof(*histogram); i++){
        if(histogram[i] > max)max = histogram[i];
    }
    
    // 畫圖
    for(int i = 0; i <  sizeof(histogram)/sizeof(*histogram) ; i++){
        cv::line(histogram_graph, Point(i,255), Point(i, 255-histogram[i]*255/max), Scalar(0),1);
    }
    
    imshow("gray", histogram_graph);
    waitKey(0);

    
    
    
    
    

    // connect component

    // threshold 128 image
    Mat thresh_img ;
    cvtColor(image, thresh_img, CV_RGB2GRAY );
    Mat result ;
    
    for(int i = 0; i < thresh_img.rows ; i++){
        for(int j = 0 ; j < thresh_img.cols ; j++){
            if(thresh_img.at<uint8_t>(i,j) < 128){
                thresh_img.at<uint8_t>(i,j)=0;
            }else{
                thresh_img.at<uint8_t>(i,j)=255;
            }
        }
    }
    thresh_img.copyTo(result);
    
    imshow("gray", thresh_img);
    waitKey(0);
    
    

    int label = 1;
    int count[700]={0};
    int threshold = 400;
    int epoch = 10;

    for(int z = 0 ; z < epoch ; z++){
        
        // Step 1
        
        for(int x = 0 ; x < 50 ; x++){
            label=1;
            // top-down
            for(int i = 0; i < thresh_img.rows-1 ; i++){
                for(int j = 0 ; j < thresh_img.cols-1 ; j++){
                    if(thresh_img.at<uint8_t>(i,j)!=0){
                       tie(thresh_img,label) = topdown_labeling(thresh_img, i, j, label);
                    }
                }
            }
            // bottom-up
            for(int i = thresh_img.rows-1; 0 < i ; i--){
                for(int j = thresh_img.cols-1 ; 0 < j ; j--){
                    if(thresh_img.at<uint8_t>(i,j)!=0){
                        thresh_img = bottomup_labeling(thresh_img, i, j);
                    }
                }
            }
        }
        
        for(int i=0; i<sizeof(count)/sizeof(*count); i++){
            count[i]=0;
        }
        for(int i = 0; i < thresh_img.rows ; i++){
            for(int j = 0 ; j < thresh_img.cols ; j++){
                count[thresh_img.at<uint8_t>(i,j)]+=1;
            }
        }
        for(int i=0; i<sizeof(count)/sizeof(*count); i++){
            printf("%d:%d\n", i, count[i]);
        }
        // step 2
        if(z!=epoch-1){
            for(int x = 1 ; x < sizeof(count)/sizeof(*count); x++){
                if(count[x] < threshold){
                    for(int i = 0; i < thresh_img.rows ; i++){
                        for(int j = 0 ; j < thresh_img.cols ; j++){
                            if(thresh_img.at<uint8_t>(i,j)==x){
                                thresh_img.at<uint8_t>(i,j)=0;
                            }
                        }
                    }
                }else{
                    for(int i = 0; i < thresh_img.rows ; i++){
                        for(int j = 0 ; j < thresh_img.cols ; j++){
                            if(thresh_img.at<uint8_t>(i,j)==x){
                                thresh_img.at<uint8_t>(i,j)=255;
                            }
                        }
                    }

                }
            }
        }
    }


    for(int i = 0; i < thresh_img.rows ; i++){
        for(int j = 0 ; j < thresh_img.cols ; j++){
            if(thresh_img.at<uint8_t>(i,j)==255){
                thresh_img.at<uint8_t>(i,j)=0;
            }
        }
    }
    //draw rect and center
    Mat test_img;
    thresh_img.copyTo(test_img);

    for(int x = 1 ; x < sizeof(count)/sizeof(*count); x++){
        if(count[x] < threshold){
            for(int i = 0; i < thresh_img.rows ; i++){
                for(int j = 0 ; j < thresh_img.cols ; j++){
                    if(thresh_img.at<uint8_t>(i,j)==x){
                        thresh_img.at<uint8_t>(i,j)=0;
                    }
                }
            }
        }else{
            for(int i = 0; i < thresh_img.rows ; i++){
                for(int j = 0 ; j < thresh_img.cols ; j++){
                    if(thresh_img.at<uint8_t>(i,j)==x){
                        thresh_img.at<uint8_t>(i,j)=255;
                    }
                }
            }

        }
    }
    cvtColor(result, result, CV_GRAY2BGR );
    cvtColor(thresh_img, thresh_img, CV_GRAY2BGR );
    for(int x=1; x<sizeof(count)/sizeof(*count); x++){

        if(count[x]>=500 && x!=255){
            int left=512,top=512,right=0,bottom=0;
            for(int i = 0; i < test_img.rows ; i++){
                for(int j = 0 ; j < test_img.cols ; j++){
                    if(test_img.at<uint8_t>(i,j) == x){
                        if(i>bottom)bottom=i+1;
                        if(i<top)top=i;
                        if(j>right)right=j+1;
                        if(j<left)left=j;

                    }
                }
            }
            cv::Rect rect(left,top,(right-left),bottom-top);
            cv::rectangle(result, rect, cv::Scalar(255,0,0));
            cv::Point center((right+left)/2,(top+bottom)/2);
            cv::circle(result, center, 4, cv::Scalar(0,0,255), CV_FILLED);
            imshow("gray", result);
            waitKey(0);

        }
    }

    return 0;
}
