#include<opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    // read the image
    Mat image = imread("../../input/crop001242.png");
    // convert the image to grayscale format
    Mat img_gray, img_gray_from_highcontrast, image_higcontrast;
    convertScaleAbs(image, image_higcontrast, 4,  0);
    imshow("Highcontrast image", image_higcontrast);
    waitKey(0); 
    // apply binary thresholding
    cvtColor(image_higcontrast, img_gray_from_highcontrast, COLOR_BGR2GRAY);
    cvtColor(image, img_gray, COLOR_BGR2GRAY);
    imshow("Highcontrast image", img_gray_from_highcontrast);
    imshow("Gray image", img_gray);
    Mat thresh, thresh_highcontrast;
    threshold(img_gray_from_highcontrast, thresh_highcontrast, 150, 250, THRESH_TRIANGLE);
    threshold(img_gray, thresh, 150, 250, THRESH_TRIANGLE);
    imshow("Binary from high contrast image", thresh_highcontrast);
    imshow("Binary image", thresh);
    
    imwrite("image_thres1.jpg", thresh);
    waitKey(0);
    // detect the contours on the binary image using cv2.CHAIN_APPROX_NONE
    vector<vector<Point>> contours, contours_high;
    vector<Vec4i> hierarchy, hierarchy_high;
    findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    findContours(thresh_highcontrast, contours_high, hierarchy_high, RETR_TREE, CHAIN_APPROX_NONE);
    // draw contours on the original image
    Mat image_copy = image.clone();
    drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
    drawContours(image_copy, contours_high, -1, Scalar(0, 255, 0), 2);
    imshow("None approximation", image_copy);
    waitKey(0);
    imwrite("contours_none_image1.jpg", image_copy);
    destroyAllWindows();
}
