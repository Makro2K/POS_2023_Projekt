
#include <iostream>
#include "opencv2/opencv.hpp"
#include "mini/ini.h"
#include "img_proc.hpp"
#include <filesystem>



using namespace std;
using namespace cv;

namespace fs = std::filesystem;

//Mat combine_image(Mat current_matrix, Mat input_image)
//{

//}
int main() {
    // create an ini file instance
    mINI::INIFile file("ini_file.ini");
    // create structure that holds data
    mINI::INIStructure ini;
    // read the ini file
    file.read(ini);
    // read the path from ini file
    string path = ini["path"]["read_path"];
    Mat3b image, edge_image, resized_image, combined_image, previous_image, previous_matrix;
    Mat3b black_box(1, 1, Vec3b(0, 0, 0));
    int iteration = 0;
    int row_iteration = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        image = imread(entry.path().string());
        edge_image = img_proc_edge_detection(image);
        resize(edge_image, resized_image, Size(), 0.1, 0.1);
        imshow("Image with contours", edge_image);
        waitKey(0);
        destroyAllWindows();
        imshow("Resized image", resized_image);
        waitKey(0);
        destroyAllWindows();
        if (iteration == 0)
        {
            combined_image = resized_image;
        }
        if (iteration > 0)
        {
            previous_image = combined_image;
            if (resized_image.rows != combined_image.rows)
            {
                resize(black_box, black_box, Size(resized_image.cols, combined_image.rows - resized_image.rows));
                imshow("black box", black_box);
                waitKey(0);
                destroyAllWindows();
                vconcat(resized_image, black_box, resized_image);
            }
            hconcat(previous_image, resized_image, combined_image);
            imshow("Current matrix", combined_image);
            waitKey(0);
            destroyAllWindows();
        }
        /*
        if (iteration == 10)
        {
            previous_matrix = combined_image;
            combined_image = resized_image;
            iteration = 0;
            if (row_iteration > 0 && previous_matrix.cols != combined_image.cols)
            {
                resize(black_box, black_box, Size(previous_image.cols - combined_image.cols, combined_image.rows));
                imshow("black box", black_box);
                waitKey(0);
                destroyAllWindows();
                hconcat(combined_image, black_box, combined_image);
            }
            hconcat(previous_image, resized_image, combined_image);
            imshow("Current matrix", combined_image);
            waitKey(0);
            destroyAllWindows();
            row_iteration++;
        }
        */
        iteration++;
    }

}