#include <iostream>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
#include "src/cv2ImageProcessing.h"
using namespace cv;
using namespace std;
// my cv2imageprocessing
cv2ImageProcessing cv2ip;
void add_blend_img(const std::string& filename, CvImage& cvImg, int _x, int _y, float _wdth_f, float _hght_f);
CvImage show_equalize_hist(CvImage& cvImg);
int main()
{ 

    CvImage background,pac_man;
    pac_man=cv2ip.ImRead("pac_man.png");
    

// =================================================
    CvImage joker_orig=cv2ip.ImRead("joker.jpeg");
    
    CvImage joker_eq=show_equalize_hist(joker_orig);
    
    // cv2ip.ImShow("pac_man",pac_man);
    // waitKey(0); 
    add_blend_img("pac_man.png",joker_eq,525,150,0.65,0.65);

    CvImage face_roi=joker_eq(cv::Rect(525,150,pac_man.cols*0.65,pac_man.rows*0.65));
    CvImage face_hsv;

    namedWindow("joker_eq",CV_WINDOW_AUTOSIZE);
    cv2ip.ImShow("joker_eq", joker_eq); 
    waitKey(0); 

    // cvtColor(face_roi,face_hsv,COLOR_BGR2HSV);
    face_hsv=show_equalize_hist(face_roi);
    face_hsv.copyTo(face_roi);

    cv2ip.ImShow("joker_eq", joker_eq); 
    waitKey(0); 

    cvtColor(joker_eq,joker_eq,COLOR_BGR2HSV);
    cv2ip.ImShow("joker_eq", joker_eq); 
    waitKey(0); 


    return 0;
} 

void add_blend_img(const std::string& filename, CvImage& bgImg, int _x, int _y, float _wdth_f, float _hght_f)
{
    CvImage srcImg, s_img, img_fg, img_bg, img_al;
    srcImg=cv2ip.ImRead(filename);
    
    s_img.create(srcImg.cols*_wdth_f,srcImg.rows*_hght_f,CV_8U);
// cout<<"pre"<<endl;
    cv::resize(srcImg,s_img,s_img.size());
// cout<<"post"<<endl;
    cv2ip.SplitAlpha(img_fg,img_al,s_img);

// cout<<"pre"<<endl;
    CvImage img_roi = bgImg(cv::Rect(_x,_y,s_img.cols,s_img.rows));
    img_bg=img_roi.clone();

// cout<<"post"<<endl;
    CvImage fg, bg, al;
    cv2ip.Resize(fg,bg,img_fg,img_bg);
    cv2ip.Resize(fg,al,img_fg,img_al);

    cv2ip.ImShow("fg", fg); 
    waitKey(0);
    cv2ip.ImShow("bg", bg); 
    waitKey(0);
    // cv2ip.ImShow("al", al); 
    // waitKey(0);

    CvImage img_blend=cv2ip.AlphaBlend(fg,bg,al);
    cv2ip.ImShow("blend", img_blend); 
    waitKey(0);
    img_blend.copyTo(img_roi);

}
CvImage show_equalize_hist(CvImage& cvImg)
{
    namedWindow("Orig",CV_WINDOW_AUTOSIZE);
    cv2ip.ImShow("Orig", cvImg); 
    waitKey(0);
// ========================================
// gray
// ========================================
    CvImage grayImg=cv2ip.ImBGR2Gray(cvImg);
    // cv2ip.ImShow("gray", grayImg); 
    // waitKey(0);

// gray hist
    CvImage result,outputImg,gray_eq;
    cv2ip.CalcGrayHist(result,grayImg);
    // cv2ip.ShowGrayHist("Gray Hist", result);

// gray equalize
    grayImg.convertTo(gray_eq,CV_8U);
    equalizeHist(gray_eq,gray_eq);
    // cv2ip.ImShow("new gray", gray_eq); 
    // waitKey(0); 
// new gray hist
    cv2ip.CalcGrayHist(result,gray_eq);
    // cv2ip.ShowGrayHist("new Gray Hist", result);

// ========================================
// color
// ========================================
// hist
    vector<Mat> colorresult,splitch;
    cv2ip.CalcColorHist(colorresult,cvImg);
    cv2ip.ShowColorHist("Color Hist", colorresult);
// equalize
    outputImg=cv2ip.EqualizeColorHist(cvImg);
    namedWindow("Equalize",CV_WINDOW_AUTOSIZE);
    cv2ip.ImShow("Equalize", outputImg); 
    waitKey(0); 
// new hist
    vector<Mat> newhist;
    cv2ip.CalcColorHist(newhist,outputImg);
    cv2ip.ShowColorHist("Equalize Hist", newhist);

    return outputImg;
}