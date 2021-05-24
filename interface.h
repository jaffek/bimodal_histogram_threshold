#ifndef INTERFACE_H 
#define INTERFACE_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include <stdio.h>
#include<vector>
#include "project2.h"

using namespace cv;
using namespace std;


//***************************************************************************************************************************
// create and display image and histogram windows
void display_windows(const Mat & source_img, Mat & hist_bckg, Mat & binary_image);
//---------------------------------------------------------------------------------------------------------------------------
// create and display trackbars
void trackbars(const Mat & source_img);
//---------------------------------------------------------------------------------------------------------------------------
// destroy histogram and binary image window
void destroy_windows();
//---------------------------------------------------------------------------------------------------------------------------
// drawing histogram on black background
void draw_histogram(Mat & background, vector<int> px_values, int threshold);

#endif