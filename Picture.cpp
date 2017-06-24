//
// Created by lazycal on 2017/6/15.
//

#include "Picture.h"

Picture::Picture() {data = NULL;}

Picture::Picture(const Mat &mat)
{
    operator=(mat);
}

Picture::~Picture()
{
    if (data != NULL)
        delete[] data;
}

Picture &Picture::operator=(const Mat &mat)
{
    if (data != NULL)
        delete[] data;
    data = new Vec3b[mat.rows * mat.cols];
    rows = mat.rows; cols = mat.cols;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Vec3b tmp = mat.at<Vec3b>(i, j);
            std::swap(tmp[0], tmp[2]);
            data[i * cols + j] = tmp;
        }
    }
    return *this;
}

Color Picture::getColor(double x, double y) const
{
    x = min(max(x, 0.), 1.);
    y = min(max(y, 0.), 1.);
    y = (1 - y) * (rows - 1);
    x = x * (cols - 1);
    Color res;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            int ny = std::min(int(y) + i, this->rows);
            int nx = std::min(int(x) + j, this->cols);
            res += (1 - fabs(y - ny)) * (1 - fabs(x - nx)) * (*this)[ny][nx];
        }
    return res / 255;
}
