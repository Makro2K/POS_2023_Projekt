#include <iostream> 
#include "opencv2/opencv.hpp"
#include "mini/ini.h"
#include "img_proc.hpp"
#include <filesystem>

using namespace std;
using namespace cv;

namespace fs = std::filesystem;

#define MAX_MATRIX_WIDTH (1000)
/*! \struct _image_matrix_t
* A structure for convenient filling of the miniatures image matrix.
*/
typedef struct _image_matrix_t {
        /*! The image (matrix) */
        Mat matrix;
        /*! Function for adding new image to the matrix */
        void add_image(Mat image);
        /*! The function of adding a buffer after finishing work with the matrix */
        void complete_mtrx(void);
    
    private:
        /*! A buffer that contains a row that is not completely filled */
        Mat _buffer;
        /* A maximum width of matrix */
        int max_width = MAX_MATRIX_WIDTH;
        /* Default reduction factor */
        float default_reduction_factor = 0.2;

        int width = 0;
        int last_row_height = 0;
        bool first_row_complete = false;

} image_matrix_t;

image_matrix_t original_images, edge_images;

int main() {
    // create an ini file instance
    mINI::INIFile file("ini_file.ini");
    // create structure that holds data
    mINI::INIStructure ini;
    // read the ini file
    file.read(ini);
    // read the path from ini file
    string path = ini["path"]["read_path"];
    string write_path_original = ini["path"]["write_path_original"];
    string write_path_edge = ini["path"]["write_path_edge"];
    Mat image, edge_image;
    int iteration = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        image = imread(entry.path().string());
        edge_image = img_proc_edge_detection(image);
        original_images.add_image(image);
        edge_images.add_image(edge_image);
            iteration++;
        //if (iteration == 20) break;
    }
    original_images.complete_mtrx();
    imshow("Original matrix", original_images.matrix);
    waitKey(0);
    imwrite(write_path_original, original_images.matrix);
    edge_images.complete_mtrx();
    imshow("Contours matrix", edge_images.matrix);
    waitKey(0);
    imwrite(write_path_edge, edge_images.matrix);
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