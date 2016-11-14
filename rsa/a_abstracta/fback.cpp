#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <NTL/ZZ.h>
#include <stack>
#include <iostream>

using namespace NTL;
using namespace cv;
using namespace std;

static ZZ drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, double, const Scalar& color){
    ZZ temp = ZZ(0);
    for(int y = 0; y < cflowmap.rows; y += step){
        for(int x = 0; x < cflowmap.cols; x += step){
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            temp += cvRound(x+fxy.x);
            //cout << Point(cvRound(x+fxy.x), cvRound(y+fxy.y));
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
        //cout << endl;
    }
    return temp;
}

stack<ZZ> camera(){
    int t;
    cout << "    Generacion de numero primos\n    Presione '1':  "; cin >> t;
    cout << endl << endl;
    VideoCapture cap(0);
    stack<ZZ> v;
    if( !cap.isOpened() ) cout << "can't open" << endl;
    else{
        Mat prevgray, gray, flow, cflow, frame;
        namedWindow("camera", 1);

        for(;;){
            cap >> frame;
            cvtColor(frame, gray, COLOR_BGR2GRAY);

            if( prevgray.data){
                calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
                cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
                v.push(drawOptFlowMap(flow, cflow, 10, 1.5, Scalar(0, 255, 0)));
                imshow("camera", cflow);
            }
            if(waitKey(30)>=0)  break;
            swap(prevgray, gray);
        }
        destroyWindow("camera");
    }
    return v;
}
/*
int main(int argc, char const *argv[]){
    stack<ZZ> v = camera();
    for(int i=0; i<v.size(); i++){
        cout << v.top() << "  ";
        v.pop();
    }
    cout << endl;
    return 0;
}*/
