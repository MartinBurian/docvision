#include "detect.hpp"
#include "util.hpp"
#include <vector>
#include <algorithm>

using namespace cv;

void page_from_edgels(std::vector<Vec4f> &edgels) {
    
}

double line_support(std::vector<Vec4f> &edgels,
		    Vec4f &line) {
    Vec2f line_dir(line[2]-line[0], line[3]-line[1]);
    Vec2f line_norm(-line_dir[1], line_dir[0]);

    Mat edgel_ends(edgels);

    std::cout << edgel_ends  << "\n";
    

    return 0;
}

void detect_edgels(Mat edge_im, std::vector<Vec4f> &outedgels, int length) {
    Mat tmp=edge_im.clone();
    int rows=tmp.rows, cols=tmp.cols;

    Mat display(edge_im.size(), CV_8UC3);
    cvtColor(edge_im, display, CV_GRAY2RGB);
    
    
    int neigh[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    int nneigh=8;
    
    std::vector<Point2f> segment;
    Point2f pt;

    for (int i = 0; i < rows; i++) {
        for (int j=0; j < cols; j++) {
	    if (!tmp.at<unsigned char>(i,j)) continue;
	    
	    int starti=i, startj=j, ii=i, jj=j;
	    
	    int N=length;
	    int d=3;
	    while (--d) {
		int l;
		for (l=0; l<N; l++) {
		    tmp.at<unsigned char>(ii,jj)=128;
		    pt.x=jj;  pt.y=ii;
		    segment.push_back(pt);

		    int r, nii, njj;
		    for (r=0; r<nneigh; r++) {
			nii=ii+neigh[r][0];
			njj=jj+neigh[r][1];
			if (nii>0 && nii<tmp.rows && njj>0 && njj<tmp.cols
			    && tmp.at<unsigned char>(nii, njj)>128) {
			    break;
			    // you have found another edge
			}
		    }

		    if (r==nneigh) {
			break;
		    }

		    ii=ii+neigh[r][0];
		    jj=jj+neigh[r][1];
		}

		if (l==N) {
		    break;
		}
		else {
		    N=N-l;
		    ii=starti;
		    jj=startj;
		    std::reverse(segment.begin(), segment.end());
		}
	    }
	    
	    if (d) { // we have found a continuous edge streak
		Mat u,s,vt;
		Mat ps=Mat(segment).reshape(1).clone();

		// center points
		Vec2f mean; // col matrix
		center_pts(ps, mean);

		// fit line
		Vec4f fitted_line;
		fitLine(ps, fitted_line, CV_DIST_L2, 0, 0.01, 0.01);
		Vec2f normal(-fitted_line[1], fitted_line[0]);
		Vec2f dir(fitted_line[0], fitted_line[1]);
		
		Mat dists=abs(ps*Mat(normal));

		double minD, maxD;
		minMaxLoc(dists, &minD, &maxD);

		if (maxD<1) {
		    for (auto s=segment.begin(); s!=segment.end(); s++) {
			tmp.at<unsigned char>(*s)=0;
		    }

		    Vec4f edgel;
		    float *psp=ps.ptr<float>(0);
		    float dp=(psp[0]*dir[0]+psp[1]*dir[1]);
		    edgel[0]=dp*dir[0]+mean[0];
		    edgel[1]=dp*dir[1]+mean[1];
		    
		    psp=ps.ptr<float>(ps.rows-1);
		    dp=(psp[0]*dir[0]+psp[1]*dir[1]);
		    edgel[2]=dp*dir[0]+mean[0];
		    edgel[3]=dp*dir[1]+mean[1];
		    
		    outedgels.push_back(edgel);

		    // line(display, outedgels.back().first, outedgels.back().second,
		    //      Scalar(0,0,255), 2, CV_AA);

		    // imshow("OCV-Display Image", display);
		    // waitKey(0);
		}
	    }
	    // else { // fail, but cleanup
	    // 	for (auto s=segment.begin(); s!=segment.end(); s++) {
	    // 	    tmp.at<unsigned char>(*s)=255;
	    // 	}
	    // }

	    segment.erase(segment.begin(), segment.end());
	}
    }
}
