#ifndef DETECT_H
#define DETECT_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

double line_support(std::vector<Vec4f>&, Vec4f&);

void detect_edgels(Mat, std::vector<Vec4f>&, int);

#endif /* DETECT_H */
