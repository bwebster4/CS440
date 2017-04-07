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

#define PRINT_TIME 10

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
void myMotionEnergy(vector<Mat> mh, Mat& dst, int length = 3);

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
	namedWindow("FrameDifference", WINDOW_AUTOSIZE);
	namedWindow("MotionHistory", WINDOW_AUTOSIZE);
	namedWindow("SkinDetection", WINDOW_AUTOSIZE);

	vector<Mat> myMotionHistory;

	/*
	Mat fMH1, fMH2, fMH3;
	fMH1 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	fMH2 = fMH1.clone();
	fMH3 = fMH1.clone();
	myMotionHistory.push_back(fMH1);
	myMotionHistory.push_back(fMH2);
	myMotionHistory.push_back(fMH3);
	*/

	int historyLength = 3;
	Mat fMH1 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	myMotionHistory.push_back(fMH1);
	for (int i = 1; i < historyLength; i++) { //I don't know why it loops until i < historyLength-1 instead of i < historyLength; the array ends up being too large for the latter...
		Mat fMH2 = fMH1.clone();
		myMotionHistory.push_back(fMH2);
	}

	Mat frame0Skin = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	mySkinDetect(frame0, frame0Skin);

	int printCount = PRINT_TIME;

	while (1)
	{
		//cout << "hey" << endl;
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
		imshow("SkinDetection", frameDest);

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
		minMaxLoc(handResult, min, max, minLoc, maxLoc);
		//cout << *max << endl;

		string asciifist = ""
			"      .----.-----.-----.-----.  \n"
			"     /      \     \     \     \\  \n"
			"    |  \/    |     |   __L_____L__  \n"
			"    |   |    |     |  (           \\  \n"
			"    |    \___/    /    \______/    |  \n"
			"    |        \___/\___/\___/       |  \n"
			"     \      \     /               /  \n"
			"      |                        __/  \n"
			"       \_                   __/     \n"
			"        |        |         |          \n"
			"        |                  |        \n"
			"        |                  | ";

		// http://ascii.co.uk/art/fist

		string asciiarm = ""
			"    _______                                                           __\n"
			"           ~~~~~~~------......_______                         __..--''  \\\n"
			"                                     ~~~---...________.---'~~~           `.\n"
			"                                                                           \\\n"
			"                                                                   __/      `.\n"
			"                                                                .-' \ `-.     \\\n"
			"                                                            .  /\   /  / /    /\n"
			"                                                             `/ /  /  /.~    /\n"
			"                                                               `-.______.-----.\n"
			"    `------------------------------.......______                           |~~ `.\n"
			"                                                ~~--.._                    |___))\n"
			"                                                       ~--._____________.------'\n";

		// http://ascii.co.uk/art/fist

		string asciihandraise = ""
			"         /\"\ \n"
			"     /\"\|\./|/\"\ \n"
			"    |\./|   |\./| \n"
			"    |   |   |   | \n"
			"    |   |>~<|   |/\"\ \n"
			"    |>~<|   |>~<|\./| \n"
			"    |   |   |   |   | \n"
			"/~T\|   |   =[@]=   | \n"
			"|_/ |   |   |   |   | \n"
			"|   | ~   ~   ~ |   | \n"
			"|~< |             ~ | \n"
			"|   '               | \n"
			"\                   | \n"
			" \                 / \n"
			"  \               / \n"
			"   \.            / \n"
			"     |          | \n"
			"     |          | \n"
			"     |          | \n"
			"     |          | \n"
			"     |          | \n"
			"     |          | \n"
			"     |          | \n"
			"     |          | \n";

		// http://www.ascii-art.de/ascii/ghi/hand.txt

		if (*max > 0.834 && !printCount){ //anywhere from 0.84 to 0.89 +
			cout << asciifist << endl;
			printCount = PRINT_TIME;
		}

		minMaxLoc(armResult, min, max, minLoc, maxLoc);
		if (*max > 0.98 && !printCount){ //anywhere from 0.84 to 0.89 +
			cout << asciiarm << endl;
			printCount = PRINT_TIME;
		}
		//cout << *max << endl;

		//----------------
		//	c) Background differencing
		//----------------


		//call myFrameDifferencing function
		Mat frameDifferenced;
		frameDifferenced = Mat::zeros(frame.rows, frame.cols, CV_8UC1);

		myFrameDifferencing(frame0Skin, frameDest, frameDifferenced);
		imshow("FrameDifference", frameDifferenced);
		myMotionHistory.erase(myMotionHistory.begin());
		myMotionHistory.push_back(frameDifferenced);
		Mat myMH = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);

		//----------------
		//  d) Visualizing motion history
		//----------------

		//call myMotionEnergy function
		myMotionEnergy(myMotionHistory, myMH, historyLength);

		erode(myMH, myMH, Mat(), Point(-1, -1), 5);
		erode(myMH, myMH, Mat(), Point(-1, -1), 5);
		//dilate(myMH, myMH, Mat(), Point(-1, -1), 2);

		Rect rect = boundingRect(myMH);
		rectangle(myMH, rect, Scalar(255, 0, 0), 1, 8);

		//RotatedRect rect = fitEllipse(myMH);
		//ellipse(myMH, rect, Scalar(255, 0, 0), 1, 8);

		//bounding box analysis, detects single horizontal wave of a skin-colored object
		if (rect.area() > 50000 && rect.width * 2 < rect.height * 1) {
			cout << asciihandraise << "\n";
			printCount = PRINT_TIME;
		}

		if (printCount)
			printCount--;
		imshow("MotionHistory", myMH); //show the frame in "MyVideo" window
		frame0 = frame;
		frame0Skin = frameDest;
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
	// cvtColor(dst, gs, CV_BGR2GRAY);
	dst = gs > 50;
	Vec3b intensity = dst.at<Vec3b>(100, 100);
}

//Function that accumulates the frame differences for a certain number of pairs of frames
void myMotionEnergy(vector<Mat> mh, Mat& dst, int length) {
	for (int i = 0; i < dst.rows; i++){
		for (int j = 0; j < dst.cols; j++){
			for (int k = 0; k < length; k++) { //I don't know why it loops until k < length-1 instead of k < length; the array ends up being too large for the latter...
				if (mh[k].at<uchar>(i, j) == 255) {
					dst.at<uchar>(i, j) = 255;
					break;
				}
			}
		}
	}

	/*
	Mat mh0 = mh[0];
	Mat mh1 = mh[1];
	Mat mh2 = mh[2];

	for (int i = 0; i < dst.rows; i++){
	for (int j = 0; j < dst.cols; j++){
	if (mh0.at<uchar>(i, j) == 255 || mh1.at<uchar>(i, j) == 255 || mh2.at<uchar>(i, j) == 255){
	dst.at<uchar>(i, j) = 255;
	}
	}
	}
	*/
}