#include <iostream>
#include "img_proc.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat img_proc_edge_detection(Mat image){

    Mat img_gray;
    cvtColor(image, img_gray, COLOR_BGR2GRAY);
    // apply binary thresholding
    Mat thresh;
    threshold(img_gray, thresh, 150, 255, THRESH_BINARY);
    // detect the contours on the binary image using cv2.CHAIN_APPROX_NONE
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    // draw contours on the original image
    Mat image_copy = image.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
	return image_copy;
}