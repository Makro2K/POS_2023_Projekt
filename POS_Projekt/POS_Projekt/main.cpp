
#include <iostream> 
#include "opencv2/opencv.hpp"
#include "mini/ini.h"
#include "img_proc.hpp"
#include <filesystem>
//#include

using namespace std;
using namespace cv;

namespace fs = std::filesystem;

#define MAX_MATRIX_WIDTH (1000)

typedef struct _image_matrix_t {
    Mat matrix;
    int width = 0;
    int last_row_height = 0;
    bool first_row_complete = false;

    Mat _buffer;
    void add_image(Mat image);
    void complete_mtrx(void);
    int max_width = MAX_MATRIX_WIDTH;
    float default_reduction_factor = 0.2;
} image_matrix_t;

image_matrix_t original_images, edge_images;
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
    Mat image, edge_image;

    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        image = imread(entry.path().string());
        edge_image = img_proc_edge_detection(image);
        original_images.add_image(image);
            iteration++;
        if (iteration == 15) break;
    }
    original_images.complete_mtrx();
    imshow("Current matrix", original_images.matrix);
    waitKey(0);

}

void _image_matrix_t::add_image(Mat image)
{
    Mat _buf;
    if (this->width == 0) {

        resize(image, _buf, Size(), this->default_reduction_factor, this->default_reduction_factor);
        this->_buffer = _buf;
        this->width = _buf.cols;
        this->last_row_height = _buf.rows;
    }
    else {
        if (this->first_row_complete) {
            resize(image, _buf, Size(), (double)this->last_row_height / (double)image.rows, (double)this->last_row_height / (double)image.rows);
            if (this->_buffer.cols + (double)image.cols * ((double)this->last_row_height / (double)image.rows) <= this->width) {
                hconcat(this->_buffer, _buf, this->_buffer);
            }
            else {
                //fill buffer to this->width
                Mat3b black_box(1, 1, Vec3b(255, 255, 255));
                resize(black_box, black_box, Size(this->width - this->_buffer.cols, this->last_row_height));
                hconcat(this->_buffer, black_box, this->_buffer);
                //concatenate
                vconcat(this->matrix, this->_buffer, this->matrix);

                this->_buffer = _buf;
                this->last_row_height = _buf.rows;

            }
        }
        else { // complete the first row
            if (this->last_row_height != 0 && (this->width + (double)image.cols * ((double)this->last_row_height / (double)image.rows) <= this->max_width)) {
                resize(image, _buf, Size(), (double)this->last_row_height / image.rows, (double)this->last_row_height / image.rows);
                hconcat(this->_buffer, _buf, this->_buffer);
                this->width += _buf.cols;
            }
            else {
                this->matrix = this->_buffer;
                this->first_row_complete = true;
                resize(image, _buf, Size(), this->default_reduction_factor, this->default_reduction_factor);
                this->_buffer = _buf;
                this->last_row_height = _buf.rows;
            }
        }
    }
}

void _image_matrix_t::complete_mtrx(void)
{
    Mat3b black_box(1, 1, Vec3b(255, 255, 255));
    resize(black_box, black_box, Size(this->width - this->_buffer.cols, this->last_row_height));
    hconcat(this->_buffer, black_box, this->_buffer);
    //concatenate
    vconcat(this->matrix, this->_buffer, this->matrix);
}