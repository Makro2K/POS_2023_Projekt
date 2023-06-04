
#include <iostream>
#include "opencv2/opencv.hpp"
#include "mini/ini.h"
#include "img_proc.hpp"
#include <filesystem>



using namespace std;
using namespace cv;

namespace fs = std::filesystem;

int main() {
    // create an ini file instance
    mINI::INIFile file("ini_file.ini");
    // create structure that holds data
    mINI::INIStructure ini;
    // read the ini file
    file.read(ini);
    // read the path from ini file
    string path = ini["path"]["read_path"];
    Mat image, edge_image;

    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        image = imread(entry.path().string());
        edge_image = img_proc_edge_detection(image);
        imshow("Image with contours", edge_image);
        waitKey(0);
        destroyAllWindows();
    }

}