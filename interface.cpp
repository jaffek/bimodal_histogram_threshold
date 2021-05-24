#include "interface.h"

//---------------------------------------------------------------------------------------------------------------------------
// create and display image and histogram windows
void display_windows(const Mat & source_img, Mat & hist_bckg, Mat & binary_image)
{
		namedWindow("Histogram", WINDOW_AUTOSIZE);
		namedWindow("Binary image", WINDOW_NORMAL);
		imshow("Histogram", hist_bckg);
		imshow("Binary image", binary_image);
}
//---------------------------------------------------------------------------------------------------------------------------
// create and display trackbars
void trackbars(const Mat & source_img)
{
	namedWindow("Original image", WINDOW_NORMAL);
	createTrackbar("Reso 2^2+", "Original image", &hist_degree, 6);
	createTrackbar("Method 1/2", "Original image", &algorythm_choice, 1);
	createTrackbar("START/STOP", "Original image", &start_calculation, 1, on_trackbar);
	imshow("Original image", source_img);
}
//---------------------------------------------------------------------------------------------------------------------------
// destroy histogram and binary image window
void destroy_windows()
{
	destroyWindow("Histogram");
	destroyWindow("Binary image");
}
//---------------------------------------------------------------------------------------------------------------------------
// drawing the histogram on the black background
void draw_histogram(Mat & background, vector<int> px_values, int threshold)
{
	int max = 0;
	for (int i = 0; i < px_values.size(); i++)
		if (px_values[i] > max) max = px_values[i];
	float normalize = (float)(background.rows - 40) / max;
	line(background, Point(3*(threshold+1) + 70, background.rows - 21), Point(3*(threshold+1) + 70, 5), Scalar(0, 0, 255), 1);
	for (int i = 0; i < px_values.size(); i++)
	{
		int height = px_values[i] * normalize;
		for(int j=0;j< 3*256/px_values.size();j++)
			line(background, Point(3*i*256/ px_values.size()+j + 70, background.rows - 21), Point(3*i * 256 / px_values.size() + j + 70, background.rows - 21 - height), Scalar(255, 0, 0), 1, 8);
	}
	line(background, Point(70, background.rows - 21), Point(3 * (px_values.size() -1) * 256 / px_values.size() + 3 * 256 / px_values.size() + 69, background.rows - 21), Scalar(255, 255, 255), 2);
	char threshold_symbol[50];
	snprintf(threshold_symbol,100, "Threshold = %d", threshold);
	putText(background, threshold_symbol, Point(300, background.rows - 5), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 255, 255), 1, 8);
}			