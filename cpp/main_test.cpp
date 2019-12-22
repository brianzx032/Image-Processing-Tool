#include <iostream>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
#include "src/cv2ImageProcessing.h"
using namespace cv;
using namespace std;
// my cv2imageprocessing
cv2ImageProcessing cv2ip;
// void add_blend_img(const std::string& filename, CvImage& cvImg, int _x, int _y, float _wdth_f, float _hght_f,int code);
CvImage add_blend_img(CvImage& fgImg, CvImage& bgImg, int _x, int _y, float _wdth_f, float _hght_f);
CvImage show_equalize_hist(CvImage& cvImg);
void assignment1();
void assignment2();
void compareRst2();
void assignment3();
int main()
{ 
    // assignment1();
    // assignment2();
    // assignment3();
    
    CvImage pac_man=cv2ip.ImRead("./img/src/pac_man.png");
    CvImage joker=cv2ip.ImRead("./img/src/joker.jpg");
    CvImage output;
    // output= add_blend_img(pac_man,joker,525,200,0.65,0.65);
    cv2ip.BlendImage(output,pac_man,joker,300,300,525,200);

    cv2ip.ImShow("output", output); 
    waitKey(0);

    return 0;
} 

CvImage add_blend_img(CvImage& fgImg, CvImage& bgImg, int _x, int _y, float _wdth_f, float _hght_f)
{
    CvImage srcImg, s_img, img_fg, img_bg, img_al;

    srcImg=fgImg;
    s_img.create(srcImg.rows*_hght_f,srcImg.cols*_wdth_f,CV_8U);
    cv::resize(srcImg,s_img,s_img.size());
    cv2ip.SplitAlpha(img_fg,img_al,s_img);

    // set roi
    CvImage img_roi = bgImg(cv::Rect(_x,_y,s_img.cols,s_img.rows));
    img_bg=img_roi.clone();

    // resize srcs
    CvImage fg, bg, al;
    cv2ip.Resize(fg,bg,img_fg,img_bg,true);
    cv2ip.Resize(fg,al,img_fg,img_al,true);

    cv2ip.ImShow("processing", fg); 
    waitKey(0);
    cv2ip.ImShow("processing", bg); 
    waitKey(0);
    
    // blend
    CvImage img_blend=cv2ip.AlphaBlend(fg,bg,al);
    cv2ip.ImShow("processing", img_blend); 
    waitKey(0);
    img_blend.copyTo(img_roi);
    return bgImg;
}
// void add_blend_img(const std::string& filename, CvImage& bgImg, int _x, int _y, float _wdth_f, float _hght_f, int code)
// {
//     CvImage srcImg=cv2ip.ImRead(filename);
//     add_blend_img(srcImg,bgImg,_x,_y,_wdth_f,_hght_f,code);
// }
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
    // outputImg=cv2ip.EqualizeColorHist(cvImg);
    namedWindow("Equalize",CV_WINDOW_AUTOSIZE);
    cv2ip.ImShow("Equalize", outputImg); 
    waitKey(0);

// new hist
    vector<Mat> newhist;
    cv2ip.CalcColorHist(newhist,outputImg);
    cv2ip.ShowColorHist("Equalize Hist", newhist);

    return outputImg;
}

void assignment1(){

// =================================================
// imread
// =================================================
    CvImage background,pac_man;
    pac_man=cv2ip.ImRead("pac_man.png");
    CvImage ref5_orig=cv2ip.ImRead("ref5.jpeg");
    

// =================================================
// eualize
// =================================================
    CvImage ref5_eq=show_equalize_hist(ref5_orig);
    cv2ip.ImWrite("ref5_eq.jpg",ref5_eq);

// =================================================
// Add pac man
// =================================================
    // cv2ip.ImShow("pac_man",pac_man);
    // waitKey(0); 
    add_blend_img(pac_man,ref5_eq,525,250,0.65,0.65);
    cv2ip.ImWrite("ref5_pac.jpg",ref5_eq);

    // set roi
    CvImage face_roi=ref5_eq(cv::Rect(525,250,pac_man.cols*0.65,pac_man.rows*0.65));
    CvImage face_hsv;

    namedWindow("ref5_eq",CV_WINDOW_AUTOSIZE);
    cv2ip.ImShow("ref5_eq", ref5_eq); 
    waitKey(0); 

    cvtColor(face_roi,face_hsv,COLOR_BGR2RGB);
    face_hsv=show_equalize_hist(face_roi);
    face_hsv.copyTo(face_roi);

    cv2ip.ImShow("ref5_eq", ref5_eq); 
    cv2ip.ImWrite("ref5_face.jpg",ref5_eq);
    waitKey(0); 


// =================================================
// Add opencv logo
// =================================================

    // add_blend_img("logo.png",ref5_eq,50,50,0.2,0.2);
    cv2ip.ImShow("ref5_eq", ref5_eq); 
    cv2ip.ImWrite("ref5_logo.jpg",ref5_eq);
    waitKey(0); 

    // cvtColor(ref5_eq,ref5_eq,COLOR_BGR2HSV);

    // add_blend_img("logo.png",ref5_eq,50,50,0.2,0.2,NoCvt);
    // cv2ip.ImShow("ref5_eq", ref5_eq); 
    // cv2ip.ImWrite("ref5_logo_hsv.jpg",ref5_eq);
    // waitKey(0); 

}
void assignment2(){

// =================================================
// imread
// =================================================
    CvImage img_orig=cv2ip.ImRead("ref10.jpg");
    CvImage img_ref=cv2ip.ImRead("ref5.JPG");
    cv2ip.ImShow("orig",img_orig);
    cv2ip.ImShow("ref",img_ref);
    
// =================================================
// equalize(BGR, HSV, YUV)
// =================================================
    CvImage img_match,img_eq,img_hsv,img_yuv,img_rgb;

// YUV
    cvtColor(img_orig,img_yuv,COLOR_BGR2YUV);
    cv2ip.ColorEqualize(img_eq,img_yuv,cv2ip.USE_YUV);
    cvtColor(img_eq,img_yuv,COLOR_YUV2BGR);

// HSV
    cvtColor(img_orig,img_hsv,COLOR_BGR2HSV);
    cv2ip.ColorEqualize(img_eq,img_hsv,cv2ip.USE_HSV);
    cvtColor(img_eq,img_hsv,COLOR_HSV2BGR);

// RGB
    cv2ip.ColorEqualize(img_rgb,img_orig,cv2ip.USE_RGB);

    cv2ip.AllChEqualize(img_eq,img_orig);
    cv2ip.ShowCDF(img_eq);
    cv2ip.ImShow("eq",img_eq);
    cv2ip.ImShow("hsv",img_hsv);
    cv2ip.ImShow("yuv",img_yuv);
    cv2ip.ImShow("rgb",img_rgb);

// hist
    // vector<Mat> srchist;
    // cv2ip.CalcColorHist(srchist,img_orig);
    // cv2ip.ShowColorHist("src Hist", srchist);
    // cv2ip.ShowDiff(img_orig,img_hsv,10);

    cv::waitKey(0);

// =================================================
// rgb match
// =================================================
    cv2ip.HistMatching(img_match,img_orig,img_ref);
    cv2ip.ImShow("rgb match",img_match);
    // cv2ip.ShowCDF(img_match);
// =================================================
// hsv match
// =================================================
    CvImage img_ref_hsv;
    cvtColor(img_orig,img_hsv,COLOR_BGR2HSV);
    cvtColor(img_ref,img_ref_hsv,COLOR_BGR2HSV);
    cv2ip.HistMatching(img_match,img_hsv,img_ref_hsv,cv2ip.USE_HSV);
    cvtColor(img_match,img_match,COLOR_HSV2BGR);
    cv2ip.ImShow("hsv_v match",img_match);
    // cv2ip.ShowCDF(img_match);

    cv2ip.HistMatching(img_match,img_hsv,img_ref_hsv,cv2ip.USE_RGB);
    cvtColor(img_match,img_match,COLOR_HSV2BGR);
    cv2ip.ImShow("hsv match",img_match);
    // cv2ip.ShowCDF(img_match);
// =================================================
// yuv match
// =================================================
    CvImage img_ref_yuv;
    cvtColor(img_orig,img_yuv,COLOR_BGR2YUV);
    cvtColor(img_ref,img_ref_yuv,COLOR_BGR2YUV);
    cv2ip.HistMatching(img_match,img_yuv,img_ref_yuv,cv2ip.USE_YUV);
    cvtColor(img_match,img_match,COLOR_YUV2BGR);
    cv2ip.ImShow("yuv_y match",img_match);
    // cv2ip.ShowCDF(img_match);

    cv2ip.HistMatching(img_match,img_yuv,img_ref_yuv,cv2ip.USE_RGB);
    cvtColor(img_match,img_match,COLOR_YUV2BGR);
    cv2ip.ImShow("yuv match",img_match);
    // cv2ip.ShowCDF(img_match);

    cv::waitKey(0);
}

void compareRst2()
{
    CvImage ref5=cv2ip.ImRead("./img/src/ref5.jpg");
    CvImage ref25=cv2ip.ImRead("./img/src/ref25.jpg");

    CvImage ref5_rgb_all_eq=cv2ip.ImRead("./img/output/ref5_rgb_all_eq.jpg");
    CvImage ref5_rgb_ind_eq=cv2ip.ImRead("./img/output/ref5_rgb_ind_eq.jpg");
    CvImage ref5_v_eq=cv2ip.ImRead("./img/output/ref5_v_eq.jpg");
    CvImage ref5_y_eq=cv2ip.ImRead("./img/output/ref5_y_eq.jpg");
    
    CvImage ref5_25_rgb_all_match=cv2ip.ImRead("./img/output/ref5_25_rgb_all_match.jpg");
    CvImage ref5_25_rgb_ind_match=cv2ip.ImRead("./img/output/ref5_25_rgb_ind_match.jpg");
    CvImage ref5_25_hsv_match=cv2ip.ImRead("./img/output/ref5_25_hsv_match.jpg");
    CvImage ref5_25_v_match=cv2ip.ImRead("./img/output/ref5_25_v_match.jpg");
    CvImage ref5_25_yuv_match=cv2ip.ImRead("./img/output/ref5_25_yuv_match.jpg");
    CvImage ref5_25_y_match=cv2ip.ImRead("./img/output/ref5_25_y_match.jpg");

    int mul=5;

    cv2ip.ShowDiff(ref5,ref5_rgb_all_eq,mul,"./img/diff/diff_ref5_rgb_all_eq.jpg");
    cv2ip.ShowDiff(ref5,ref5_rgb_ind_eq,mul,"./img/diff/diff_ref5_rgb_ind_eq.jpg");
    cv2ip.ShowDiff(ref5,ref5_v_eq,mul,"./img/diff/diff_ref5_v_eq.jpg");
    cv2ip.ShowDiff(ref5,ref5_y_eq,mul,"./img/diff/diff_ref5_y_eq.jpg");

    cv2ip.ShowDiff(ref5_rgb_all_eq,ref5_v_eq,mul,"./img/diff/diff_all_v_eq.jpg");
    cv2ip.ShowDiff(ref5_rgb_ind_eq,ref5_v_eq,mul,"./img/diff/diff_ind_v_eq.jpg");
    cv2ip.ShowDiff(ref5_rgb_all_eq,ref5_y_eq,mul,"./img/diff/diff_all_y_eq.jpg");
    cv2ip.ShowDiff(ref5_rgb_ind_eq,ref5_y_eq,mul,"./img/diff/diff_ind_y_eq.jpg");


    cv2ip.ShowDiff(ref5,ref5_25_rgb_all_match,mul,"./img/diff/diff_ref5_25_rgb_all_match.jpg");
    cv2ip.ShowDiff(ref5,ref5_25_rgb_ind_match,mul,"./img/diff/diff_ref5_25_rgb_ind_match.jpg");
    cv2ip.ShowDiff(ref5,ref5_25_hsv_match,mul,"./img/diff/diff_ref5_25_hsv_match.jpg");
    cv2ip.ShowDiff(ref5,ref5_25_v_match,mul,"./img/diff/diff_ref5_25_v_match.jpg");
    cv2ip.ShowDiff(ref5,ref5_25_yuv_match,mul,"./img/diff/diff_ref5_25_yuv_match.jpg");
    cv2ip.ShowDiff(ref5,ref5_25_y_match,mul,"./img/diff/diff_ref5_25_y_match.jpg");

    cv2ip.ShowDiff(ref5_25_rgb_all_match,ref5_25_v_match,mul,"./img/diff/diff_all_v_match.jpg");
    cv2ip.ShowDiff(ref5_25_rgb_ind_match,ref5_25_v_match,mul,"./img/diff/diff_ind_v_match.jpg");
    cv2ip.ShowDiff(ref5_25_rgb_all_match,ref5_25_y_match,mul,"./img/diff/diff_all_y_match.jpg");
    cv2ip.ShowDiff(ref5_25_rgb_ind_match,ref5_25_y_match,mul,"./img/diff/diff_ind_y_match.jpg");

}

void assignment3()
{
    CvImage ref5=cv2ip.ImRead("./img/src/ref5.jpg");
    CvImage output,imgx,imgy;
    // cv2ip.Smooth2D(output,ref5,10,cv2ip.BILATERAL);
    // cv2ip.EdgeDetect(output,ref5,cv2ip.KIRSCH);
    cv2ip.ImSharpening(output,ref5,cv2ip.UNSHARP_MASK,cv2ip.BLUR);

    // vector<CvImage> kernel=cv2ip.GetKirschKernel();

    // cv::filter2D(ref5,output,-1,kernel[0]);
    // cout<<"error"<<endl;
    // cv2ip.Conv2D(output,ref5,kernel[0]);
    // output=(abs(imgx)+abs(imgy))/2;

    // output=cv::max(imgx,imgy);
    // for(int i=1;i<8;i++)
    // {
    //     cv2ip.Conv2D(imgy,ref5,kernel[i]);
    //     output=cv::max(output,imgy);
    // }

    cv2ip.ImShow("ref5", ref5); 
    cv2ip.ImShow("output", output); 
    CvImage diff=abs(output-ref5)*10;
    cv2ip.ImShow("diff", diff); 
    waitKey(0);

}