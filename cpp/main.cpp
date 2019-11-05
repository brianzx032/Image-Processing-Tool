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
    // background=cv2ip.ImRead("blackhole.jpg");
    // CvImage pig=cv2ip.ImRead("pig.jpg");
    // // cvtColor(pig,pig,COLOR_BGR2BGRA);
    // // cvtColor(pig,pig,COLOR_BGR2HSV);
    // CvImage s_pig;
    // s_pig.create(pig.cols*0.5,pig.rows*0.5,CV_8U);
    // resize(pig,s_pig,s_pig.size());
    // // cv2ip.ImShow("s_pig", s_pig); 
    // // waitKey(0); 
    // CvImage pig_roi=background(cv::Rect(500,100,s_pig.cols,s_pig.rows));
    // addWeighted(pig_roi, 0.3, s_pig, 0.7, 0.0, s_pig);//dst = src1[I]*pac_alph+ src2[I]*beta + gamma；第一第四个参数就是各自权重，第5个参数就是公式中的偏执因子gamma。
    // s_pig.copyTo(pig_roi);

    // add_blend_img("pac_man.png",background,100,100,1,1);
    
    // CvImage centre_roi = background(cv::Rect(100,100,pac_man.cols,pac_man.rows));
    // CvImage centre_eq;
    // // centre_eq=cv2ip.EqualizeColorHist(centre_roi);
    // cvtColor(centre_roi,centre_eq,COLOR_BGR2HSV);
    // centre_eq.copyTo(centre_roi);

    // cv2ip.ImShow("background", background); 
    // waitKey(0); 

    // show_equalize_hist(background);

// =================================================
    CvImage joker_orig=cv2ip.ImRead("joker.jpeg");
    
    CvImage joker_eq=show_equalize_hist(joker_orig);
    
    add_blend_img("pac_man.png",joker_eq,525,150,0.65,0.65);

    CvImage face_roi=joker_eq(cv::Rect(525,150,pac_man.cols*0.65,pac_man.rows*0.65));
    CvImage face_hsv;

    // cvtColor(face_roi,face_hsv,COLOR_BGR2HSV);
    // face_hsv=show_equalize_hist(face_roi);
    // face_hsv.copyTo(face_roi);

    namedWindow("joker_eq",CV_WINDOW_AUTOSIZE);
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

    // cv2ip.ImShow("fg", fg); 
    // waitKey(0);
    // cv2ip.ImShow("bg", bg); 
    // waitKey(0);
    // cv2ip.ImShow("al", al); 
    // waitKey(0);

    CvImage img_blend=cv2ip.AlphaBlend(fg,bg,al);
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