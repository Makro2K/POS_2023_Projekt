#include<opencv2/opencv.hpp>
#include<iostream>
#include"mini/ini.h"

using namespace std;
using namespace cv;

int main() {
    // create an ini file instance
    mINI::INIFile file("ini_file.ini");
    // create structure that holds data
    mINI::INIStructure ini;
    // read the ini file
    file.read(ini);
    // read the path from ini file
    string path = ini["path"]["read_path"];
    // read the image
    Mat image = imread(path);
    // convert the image to grayscale format
    Mat img_gray;
    cvtColor(image, img_gray, COLOR_BGR2GRAY);
    // apply binary thresholding
    Mat thresh;
    threshold(img_gray, thresh, 150, 255, THRESH_BINARY);
    imshow("Binary mage", thresh);
    waitKey(0);
    destroyAllWindows();

    // detect the contours on the binary image using cv2.CHAIN_APPROX_NONE
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    // draw contours on the original image
    Mat image_copy = image.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
    imshow("Image with contours", image_copy);
    waitKey(0);
    destroyAllWindows();
}