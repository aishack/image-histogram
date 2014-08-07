// ImageHistogram.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// The standard headers
#include <cv.h>
#include <highgui.h>

// A function to draw the histogram
IplImage* DrawHistogram(CvHistogram *hist, float scaleX=1, float scaleY=1)
{
	// Find the maximum value of the histogram to scale
	// other values accordingly
	float histMax = 0;
	cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0);

	// Create a new blank image based on scaleX and scaleY
	IplImage* imgHist = cvCreateImage(cvSize(256*scaleX, 64*scaleY), 8 ,1);
	cvZero(imgHist);

	// Go through each bin
	for(int i=0;i<255;i++)
	{
		float histValue = cvQueryHistValue_1D(hist, i);		// Get value for the current bin...
		float nextValue = cvQueryHistValue_1D(hist, i+1);	// ... and the next bin

		// Calculate the four points of the polygon that these two 
		// bins enclose
		CvPoint pt1 = cvPoint(i*scaleX, 64*scaleY);
		CvPoint pt2 = cvPoint(i*scaleX+scaleX, 64*scaleY);
		CvPoint pt3 = cvPoint(i*scaleX+scaleX, (64-nextValue*64/histMax)*scaleY);
		CvPoint pt4 = cvPoint(i*scaleX, (64-histValue*64/histMax)*scaleY);
		
		// A close convex polygon
		int numPts = 5;
		CvPoint pts[] = {pt1, pt2, pt3, pt4, pt1};

		// Draw it to the image
		cvFillConvexPoly(imgHist, pts, numPts, cvScalar(255));
	}

	// Return it... make sure you delete it once you're done!
	return imgHist;
}

// The main() function
void main()
{
	// Load an image. Try replacing this with a camera loop somehow!
	IplImage* img = cvLoadImage("C:\\orangeman.jpg");

	// Variables to be used for the histgoram
	int numBins = 256;
	float range[] = {0, 255};
	float *ranges[] = { range };
	
	// Initialize a histogram
	CvHistogram *hist = cvCreateHist(1, &numBins, CV_HIST_ARRAY, ranges, 1);
	cvClearHist(hist);

	// Create three images, each for one channel of the original color
	// image
	IplImage* imgRed = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* imgGreen = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* imgBlue = cvCreateImage(cvGetSize(img), 8, 1);

	// Split it into constituent channels
	cvSplit(img, imgBlue, imgGreen, imgRed, NULL);

	cvCalcHist(&imgRed, hist, 0, 0);			// Calculate the histogram for red
	IplImage* imgHistRed = DrawHistogram(hist);	// Draw it
	cvClearHist(hist);							// Clear the histgoram

	cvCalcHist(&imgGreen, hist, 0, 0);			// Reuse it to calculate histogram for green
	IplImage* imgHistGreen = DrawHistogram(hist);
	cvClearHist(hist);

	cvCalcHist(&imgBlue, hist, 0, 0);			// And again for blue
	IplImage* imgHistBlue = DrawHistogram(hist);
	cvClearHist(hist);

	// Create three windows for the three histograms
	cvNamedWindow("Red");
	cvNamedWindow("Green");
	cvNamedWindow("Blue");

	// Display them
	cvShowImage("Red", imgHistRed);
	cvShowImage("Green", imgHistGreen);
	cvShowImage("Blue", imgHistBlue);

	// Wait until a key is pressed.
	cvWaitKey(0);
}

