#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "detect.hpp"
#include "util.hpp"


#define THR_LOW 100
#define CANNY_GRAD_KERNEL_SIZE 3

using namespace cv;

int main(int argc, char *argv[]) {
    Mat image, edges;
    image=imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    Mat scaled;
    resize(image, scaled, Size(), 0.5, 0.5);


    Canny(scaled, edges, THR_LOW, 2*THR_LOW, CANNY_GRAD_KERNEL_SIZE);
    //edges=image;
    
    namedWindow("OCV-Display Image", WINDOW_NORMAL );

    std::vector<Vec4f> edgels;

    ITIC(t);
    TIC(t);
    detect_edgels(edges, edgels, 30);
    TOC(t);

    line_support(edgels, edgels[0]);

    std::cout << std::endl << "edgels:" << std::endl;

    Mat display(image.size(), CV_8UC3);
    cvtColor(image, display, CV_GRAY2RGB);
    
    // for (auto i=edgels.begin(); i!=edgels.end(); i++) {
    //     line(display, 2*i->first, 2*i->second, Scalar(0,0,255), 2, CV_AA);
    // }

    
    imwrite("../imgs/canny.png", display);
    

    imshow("OCV-Display Image", display);


    int key;
    while(true) {
	if ((key=waitKey(15))>0) {
	    if (key==113) { // q
		break;
	    }
	}
    }
    
  
    return 0;
}


