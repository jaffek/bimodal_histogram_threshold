#ifndef PROJECT2_H
#define PROJECT2_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>

using namespace cv;
using namespace std;

extern int hist_degree, start_calculation, algorythm_choice;
extern bool gray_or_rgb;
// hist_degree					define histogram resolution
// start_calculation			starts and stops the program
// gray_or_rgb					information about loaded image
// algorythm_choice				define which algorythm is used

extern Mat loaded_img, gray_img;
//loaded_img						the original photo loaded from the file
// gray_img							original image converted to grayscale

//declared as global variables due to the gui environment requirements in opencv (trackbars)

//***************************************************************************************************************************
// calculate histogram of the image: intensive - number of vector element, number of pixels as a value in each element
void calculate_histogram(vector<int> & hist, const Mat & source_img);
//---------------------------------------------------------------------------------------------------------------------------
// dividing histogram values into blocks and saving as new simpler histogram
void divide_histogram(vector<int> source_histogram, vector<int> & out_histogram, int divisor);
//---------------------------------------------------------------------------------------------------------------------------
// algorithm to find the index of the histogram subregion with the smallest number of pixels between 2 subregions with the biggest number of pixels 
int find_min_subregion(vector<int> hist, int divisor);
//---------------------------------------------------------------------------------------------------------------------------
// a loop that looks for the best match of the threshold to the histogram
int threshold_calculation(vector<int> source_histogram, int thres_parameter);
//----------------------------------------------------------------------------------------------------------------------------
// creating a binarized image
void thresholding_image(int threshold, const Mat & source_img, Mat & out_img);
//----------------------------------------------------------------------------------------------------------------------------
// conversion rgb to grayscale
void convert_to_grayscale(const Mat & source_img, Mat & out_img);
//----------------------------------------------------------------------------------------------------------------------------
// calling program functions one by one
void call(const Mat & source_img, int threshold_parameter);
//------------------------------------------------------------------------------------------------------------------------------------
// function called when user move the START/STOP trackbar
void on_trackbar(int, void*);
//----------------------------------------------------------------------------------------------------------------------------
// load image and start program
void main_function();
//***************************************************************************************************************************

#endif