#include "project2.h"
#include "interface.h"

int hist_degree = 6, start_calculation = 0, algorythm_choice = 0;
bool gray_or_rgb;
// hist_degree					define histogram resolution
// start_calculation			starts and stops the program
// gray_or_rgb					information about loaded image
// algorythm_choice				define which algorythm is used

Mat loaded_img, gray_img;
//loaded_img						the original photo loaded from the file
//gray_img							original image converted to grayscale

//declared as global variables due to the gui environment requirements in opencv (trackbars)

//***************************************************************************************************************************
// calculate histogram of the image: intensity - the number of the vector element, number of pixels as a value in each element
void calculate_histogram(vector<int> & hist, const Mat & source_img)
{
	for (int i = 0; i < source_img.cols; i++)
		for (int j = 0; j < source_img.rows; j++)
			hist[source_img.at<uchar>(j, i)] += 1;
}
//---------------------------------------------------------------------------------------------------------------------------
// dividing histogram values into blocks and saving as new simpler histogram
void divide_histogram(vector<int> source_histogram, vector<int> & out_histogram, int divisor)
{
	int sum = 0;
	for (int i = 0; i < divisor; i++)
	{
		for (int j = i * source_histogram.size() / divisor; j < (i + 1) * source_histogram.size() / divisor; j++)
			sum += source_histogram[j];
		out_histogram[i] = (int)(sum / (source_histogram.size() / divisor));
		sum = 0;
	}
}
//---------------------------------------------------------------------------------------------------------------------------
// algorithm: finding the index of the histogram subregion with the smallest average number of pixels between 2 subregions with the biggest average number of pixels 
int find_min_subregion(vector<int> hist, int divisor)
{
	int minimum_subregion = -1, max_left_subregion = -1, max_right_subregion = -1, max_height_difference = -1;
	for (int i = 1; i < divisor - 1; i++)
		if (hist[i] < hist[i - 1] && hist[i] < hist[i + 1])
		{
			max_right_subregion = i + 1;
			max_left_subregion = i - 1;
			for (int j = i + 1; j < divisor - 1; j++)
				if (hist[j + 1] > hist[max_right_subregion])
					max_right_subregion = j + 1;
			for (int j = i - 1; j > 0; j--)
				if (hist[j - 1] > hist[max_left_subregion])
					max_left_subregion = j - 1;
			if ((abs(hist[max_left_subregion] - hist[i]) + abs(hist[max_right_subregion] - hist[i])) > max_height_difference)
			{
				max_height_difference = abs(hist[max_left_subregion] - hist[i]) + abs(hist[max_right_subregion] - hist[i]);
				minimum_subregion = i;
			}
		}
	return minimum_subregion;
}
//---------------------------------------------------------------------------------------------------------------------------
// loop that looks for the best match of the threshold to the histogram - implemented algorithm
int threshold_calculation(vector<int> source_histogram)
{
	int hist_divisor = 2, threshold_previous = -1, threshold = -1, min_subregion = -1;
	do
	{
		threshold_previous = threshold;
		hist_divisor *= 2;
		if (hist_divisor > source_histogram.size()) break;
		vector<int> div_histogram(hist_divisor);
		divide_histogram(source_histogram, div_histogram, hist_divisor);
		min_subregion = find_min_subregion(div_histogram, hist_divisor);
		threshold = 256 / (2 * hist_divisor) + min_subregion * (256 / hist_divisor) - 1;
	} while (hist_divisor <= source_histogram.size());
	if (threshold < 0) threshold = 0;
	return threshold;
}
//---------------------------------------------------------------------------------------------------------------------------
//an alternative algorithm for determining the threshold: Balanced histogram thresholding
int balanced_thresholding(vector<int> source_histogram)
{
	int right_end = source_histogram.size() - 1, left_end = 0;
	int center_weight = (int)((right_end + left_end) / 2);
	int sum_left = 0, sum_right = 0;
	for (int i = 0; i < center_weight + 1; i++)
		sum_left += source_histogram[i];
	for (int i = center_weight + 1; i < right_end + 1; i++)
		sum_right += source_histogram[i];
	while (left_end <= right_end)
	{
		if (sum_right > sum_left)
		{
			sum_right -= source_histogram[right_end--];
			if (((right_end + left_end) / 2) < center_weight)
			{
				sum_right += source_histogram[center_weight];
				sum_left -= source_histogram[center_weight--];
			}
		}
		else
			if (sum_left >= sum_right)
			{
				sum_left -= source_histogram[left_end++];
				if (((left_end + right_end) / 2) > center_weight)
				{
					sum_left += source_histogram[center_weight + 1];
					sum_right -= source_histogram[center_weight + 1];
					center_weight++;
				}
			}
	}
	return (256 / (2 * source_histogram.size()) + center_weight * (256 / source_histogram.size()) - 1);
}
//----------------------------------------------------------------------------------------------------------------------------
// creating a binarized image
void thresholding_image(int threshold, const Mat & source_img, Mat & out_img)
{
	out_img = Mat(source_img.rows, source_img.cols, CV_8UC1);
	for (int i = 0; i < source_img.cols; i++)
		for (int j = 0; j < source_img.rows; j++)
		{
			if (source_img.at<uchar>(j, i) > threshold)
				out_img.at<uchar>(j, i) = 255;
			else
				out_img.at<uchar>(j, i) = 0;
		}
}
//----------------------------------------------------------------------------------------------------------------------------
// conversion rgb to grayscale image
void convert_to_grayscale(const Mat & source_img, Mat & out_img)
{
	out_img = Mat(source_img.rows, source_img.cols, CV_8UC1);
	for (int i = 0; i < source_img.cols; i++)
		for (int j = 0; j < source_img.rows; j++)
			out_img.at<uchar>(j, i) = (source_img.at<Vec3b>(j, i)[0] + source_img.at<Vec3b>(j, i)[1] + source_img.at<Vec3b>(j, i)[2]) / 3;
}
//----------------------------------------------------------------------------------------------------------------------------
// calling program functions one by one
void call(const Mat & source_img)
{
	vector<int> histogram(256, 0), final_histogram(pow(2,hist_degree+2), 0);
	Mat histogram_background = imread("black.jpg");
	Mat binary_img;
	int calculated_threshold;
	calculate_histogram(histogram, source_img);
	divide_histogram(histogram, final_histogram, pow(2, hist_degree + 2));
	if(algorythm_choice == 0)
		calculated_threshold = threshold_calculation(final_histogram);
	else
		 calculated_threshold = balanced_thresholding(final_histogram);
	draw_histogram(histogram_background, final_histogram, calculated_threshold);
	thresholding_image(calculated_threshold, source_img, binary_img);
	display_windows(source_img, histogram_background, binary_img);
}
//------------------------------------------------------------------------------------------------------------------------------------
// function called when user move the START/STOP trackbar
void on_trackbar(int, void*)
{
	if (start_calculation == 1)
		if (gray_or_rgb == true)
			call(loaded_img);
		else
			call(gray_img);
	else
		destroy_windows();
}
//----------------------------------------------------------------------------------------------------------------------------
// load image and start program
void main_function()
{
	loaded_img = imread("cameraman.jpg");
	if (loaded_img.channels() == 3)
	{
		convert_to_grayscale(loaded_img, gray_img);
		trackbars(gray_img);
		gray_or_rgb = false;
	}
	else
	{
		trackbars(loaded_img);
		gray_or_rgb = true;
	}
	waitKey(0);
}
//***************************************************************************************************************************