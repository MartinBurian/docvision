#include <opencv2/opencv.hpp>
#include "util.hpp"

using namespace cv;

void center_pts(Mat ps, Vec2f &mean) {
    assert(ps.isContinuous() && "Noncontinuous point matrix!");

    int npts=ps.rows*ps.cols/2;
    float *px=(float*)ps.data;
    for (int a = 0; a < npts; a++) {
	mean[0]+=*(px++);
	mean[1]+=*(px++);
    }
		
    mean=mean/npts;
	     	
    px=(float*)ps.data;
    for (int a = 0; a < npts; a++) {
	*(px++)-=mean[0];
	*(px++)-=mean[1];
    }
}
