#ifndef UTIL_H
#define UTIL_H

#include <vector>

using namespace cv;
using namespace std;

#define ITIC(xx) double xx
#define TIC(xx) xx=(double)getTickCount()
#define TOC(xx) cout<<"Elapsed time: "<<((double)getTickCount()-(xx))/getTickFrequency()*1000<<"ms"<<endl

void center_pts(Mat pts, Vec2f &mean);


#endif /* UTIL_H */
