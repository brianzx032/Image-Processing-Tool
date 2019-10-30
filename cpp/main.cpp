#include <iostream>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
#include "src/cv2ImageProcessing.h"
using namespace cv;
using namespace std;
int main()
{ 
    cv2ImageProcessing cv2ip;
    // // Mat img = imread("logo.png", CV_LOAD_IMAGE_UNCHANGED); 
    // Mat img = cv2ip.ImRead("logo.png");
    // // imwrite("output.jpg",img); 
    // cv2ip.ImWrite("output.jpg",img);
    // namedWindow("Display window", WINDOW_AUTOSIZE); 
    // // imshow("Display window", img);  
    // cv2ip.ImShow("Display window", img); 

    Mat foreground, alpha, src_img, background,blend;
    src_img=cv2ip.ImRead("logo.png");
    background=cv2ip.ImRead("pepper.bmp");
    cv2ip.SplitAlpha(foreground,alpha,src_img);
    cv2ip.ImWrite("fore.jpg",foreground);
    cv2ip.ImWrite("alpha.jpg",alpha);
    blend=cv2ip.AlphaBlend(foreground,background,alpha);
    cv2ip.ImWrite("blend.jpg", blend);

    namedWindow("Display window", WINDOW_AUTOSIZE); 
    cv2ip.ImShow("Display window", foreground); 
    waitKey(0); 
    cv2ip.ImShow("Display window", alpha); 
    waitKey(0); 
    cv2ip.ImShow("Display window", blend); 
    waitKey(0); 
    return 0;
} 