/*	CS585_Lab2.cpp
*	CS585 Image and Video Computing Fall 2014
*	
*	used for:
*	CS440 Lab: Introduction to OpenCV
*	--------------
*	This program introduces the following concepts:
*		a) Reading a stream of images from a webcamera, and displaying the video
*		b) Skin color detection
*		c) Background differencing
*		d) Visualizing motion history
*	--------------
*/

//opencv libraries
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//C++ standard libraries
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//function declarations

/**
Function that returns the maximum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMax(int a, int b, int c);

/**
Function that returns the minimum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMin(int a, int b, int c);

/**
Function that detects whether a pixel belongs to the skin based on RGB values
@param src The source color image
@param dst The destination grayscale image where skin pixels are colored white and the rest are colored black
*/
void mySkinDetect(Mat& src, Mat& dst);

/**
Function that does frame differencing between the current frame and the previous frame
@param src The current color image
@param prev The previous color image
@param dst The destination grayscale image where pixels are colored white if the corresponding pixel intensities in the current
and previous image are not the same
*/
void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst);

/**
Function that accumulates the frame differences for a certain number of pairs of frames
@param mh Vector of frame difference images
@param dst The destination grayscale image to store the accumulation of the frame difference images
*/
void myMotionEnergy(vector<Mat> mh, Mat& dst);

int main()
{

	//----------------
	//a) Reading a stream of images from a webcamera, and displaying the video
	//----------------
	// For more information on reading and writing video: http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
	// open the video camera no. 0
	VideoCapture cap(0);

	// if not successful, exit program
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	//create a window called "MyVideoFrame0"
	namedWindow("MyVideo0", WINDOW_AUTOSIZE);
	Mat frame0;

	// read a new frame from video
	bool bSuccess0 = cap.read(frame0);

	//if not successful, break loop
	if (!bSuccess0)
	{
		cout << "Cannot read a frame from video stream" << endl;
	}

	//show the frame in "MyVideo" window
	imshow("MyVideo0", frame0);

	//create a window called "MyVideo"
	namedWindow("MyVideo", WINDOW_AUTOSIZE);
	namedWindow("MyVideoMH", WINDOW_AUTOSIZE);
	namedWindow("Skin", WINDOW_AUTOSIZE);

	vector<Mat> myMotionHistory;
	Mat fMH1, fMH2, fMH3;
	fMH1 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	fMH2 = fMH1.clone();
	fMH3 = fMH1.clone();
	myMotionHistory.push_back(fMH1);
	myMotionHistory.push_back(fMH2);
	myMotionHistory.push_back(fMH3);

	while (1)
	{
		// read a new frame from video
		Mat frame;
		bool bSuccess = cap.read(frame);

		//if not successful, break loop
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		// destination frame
		Mat frameDest;
		frameDest = Mat::zeros(frame.rows, frame.cols, CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
		//----------------
		//	b) Skin color detection
		//----------------
		mySkinDetect(frame, frameDest);
		imshow("Skin", frameDest);


		//------------
		//  STATIC GESTURE DETECTION
		//------------
		//dont edit frame directly
		Mat handTemplate = cv::imread("Hand.jpg");
		Mat armTemplate = cv::imread("Arm.jpg");
		//cv::imshow(armTemplate);

		Mat handResult = Mat::zeros(frame.rows - handTemplate.rows + 1, frame.cols - handTemplate.cols + 1, CV_8UC1);
		//handResult = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		matchTemplate(frame, handTemplate, handResult, CV_TM_CCORR_NORMED);

		Mat armResult = Mat::zeros(frame.rows - armTemplate.rows + 1, frame.cols - armTemplate.cols + 1, CV_8UC1);
		//armResult = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		matchTemplate(frame, armTemplate, armResult, CV_TM_CCORR_NORMED);


		double* min = new double();
		double* max = new double();
		Point* minLoc = new Point();
		Point* maxLoc = new Point();
		minMaxLoc(armResult, min, max, minLoc, maxLoc);
		cout << min << " " << max << endl;


		//----------------
		//	c) Background differencing
		//----------------


		//call myFrameDifferencing function
		myFrameDifferencing(frame0, frame, frameDest);
		imshow("MyVideo", frameDest);
		myMotionHistory.erase(myMotionHistory.begin());
		myMotionHistory.push_back(frameDest);
		Mat myMH = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);

		//----------------
		//  d) Visualizing motion history
		//----------------

		//call myMotionEnergy function
		myMotionEnergy(myMotionHistory, myMH);


		imshow("MyVideoMH", myMH); //show the frame in "MyVideo" window
		frame0 = frame;
		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	cap.release();
	return 0;
}

//Function that returns the maximum of 3 integers
int myMax(int a, int b, int c) {
	int m = a;
	(void)((m < b) && (m = b));
	(void)((m < c) && (m = c));
	return m;
}

//Function that returns the minimum of 3 integers
int myMin(int a, int b, int c) {
	int m = a;
	(void)((m > b) && (m = b));
	(void)((m > c) && (m = c));
	return m;
}

//Function that detects whether a pixel belongs to the skin based on RGB values
void mySkinDetect(Mat& src, Mat& dst) {
	//Surveys of skin color modeling and detection techniques:
	//Vezhnevets, Vladimir, Vassili Sazonov, and Alla Andreeva. "A survey on pixel-based skin color detection techniques." Proc. Graphicon. Vol. 3. 2003.
	//Kakumanu, Praveen, Sokratis Makrogiannis, and Nikolaos Bourbakis. "A survey of skin-color modeling and detection methods." Pattern recognition 40.3 (2007): 1106-1122.
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			//For each pixel, compute the average intensity of the 3 color channels
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar (unsigned character)
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20) && (myMax(R, G, B) - myMin(R, G, B) > 15) && (abs(R - G) > 15) && (R > G) && (R > B)){
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}

//Function that does frame differencing between the current frame and the previous frame
void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst) {
	//For more information on operation with arrays: http://docs.opencv.org/modules/core/doc/operations_on_arrays.html
	//For more information on how to use background subtraction methods: http://docs.opencv.org/trunk/doc/tutorials/video/background_subtraction/background_subtraction.html
	absdiff(prev, curr, dst);
	Mat gs = dst.clone();
	cvtColor(dst, gs, CV_BGR2GRAY);
	dst = gs > 50;
	Vec3b intensity = dst.at<Vec3b>(100, 100);
}

//Function that accumulates the frame differences for a certain number of pairs of frames
void myMotionEnergy(vector<Mat> mh, Mat& dst) {
	Mat mh0 = mh[0];
	Mat mh1 = mh[1];
	Mat mh2 = mh[2];

	int count = 0;
	for (int i = 0; i < dst.rows; i++){
		for (int j = 0; j < dst.cols; j++){
			if (mh0.at<uchar>(i, j) == 255 || mh1.at<uchar>(i, j) == 255 || mh2.at<uchar>(i, j) == 255){
				dst.at<uchar>(i, j) = 255;
				count++;
			}
		}
	}
	cout << count << endl;
}


////opencv libraries
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//#include <stdio.h>
//////C++ standard libraries
//#include <vector>
//
//using namespace cv;
//using namespace std;
//
//
///// Global Variables
//Mat img; Mat templ; Mat result;
//char* image_window = "Source Image";
//char* result_window = "Result window";
//
//int match_method;
//int max_Trackbar = 5;
//
///// Function Headers
//void MatchingMethod(int, void*);
//
///** @function main */
//int main(int argc, char** argv)
//{
//	/// Load image and template
//	/*img = imread(argv[1], 1);
//	templ = imread(argv[2], 1);*/
//
//
//	std::cout << "hey" << std::endl;
//
//	img = imread("dog.jpg", 1);
//	templ = imread("red_ball.png", 1);
//	imshow("image", img);
//
//	////from stack overflow:
//	//if (img.empty() || templ.empty())
//	//	std::cout << "failed to open img.jpg" << std::endl;
//	//else
//	//	std::cout << "img.jpg loaded OK" << std::endl;
//
//	/*/// Create windows
//	namedWindow(image_window, CV_WINDOW_AUTOSIZE);
//	namedWindow(result_window, CV_WINDOW_AUTOSIZE);
//
//	/// Create Trackbar
//	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
//	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
//
//	MatchingMethod(0, 0);
//	*/
//	waitKey();
//	return 0;
//}
//
///**
//* @function MatchingMethod
//* @brief Trackbar callback
//*/
//void MatchingMethod(int, void*)
//{
//	/// Source image to display
//	Mat img_display;
//	img.copyTo(img_display);
//
//	/// Create the result matrix
//	int result_cols = img.cols - templ.cols + 1;
//	int result_rows = img.rows - templ.rows + 1;
//
//	result.create(result_rows, result_cols, CV_32FC1);
//
//	/// Do the Matching and Normalize
//	matchTemplate(img, templ, result, match_method);
//	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
//
//	/// Localizing the best match with minMaxLoc
//	double minVal; double maxVal; Point minLoc; Point maxLoc;
//	Point matchLoc;
//
//	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//
//	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
//	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
//	{
//		matchLoc = minLoc;
//	}
//	else
//	{
//		matchLoc = maxLoc;
//	}
//
//	/// Show me what you got
//	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//
//	imshow(image_window, img_display);
//	imshow(result_window, result);
//
//	return;
//}
//
//
//
//
//
//
//

