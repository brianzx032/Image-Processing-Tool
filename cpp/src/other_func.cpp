
void cv2ImageProcessing::ShowGrayHist(const std::string& winname, const CvImage& GrayHist)
{
    double maxVal=0;
    double minVal=0;

    //找到直方图中的最大值和最小值
    cv::minMaxLoc(GrayHist,&minVal,&maxVal,0,0);
    int histSize=GrayHist.rows;
    CvImage histImg(histSize,histSize,CV_8U,cv::Scalar(255));
    // 设置最大峰值为图像高度的90%
    int hpt=static_cast<int>(0.9*histSize);

    for(int h=0;h<histSize;h++)
    {
        float binVal=GrayHist.at<float>(h);
        int intensity=static_cast<int>(binVal*hpt/maxVal);
        line(histImg,cv::Point(h,histSize),cv::Point(h,histSize-intensity),cv::Scalar::all(0));
    }

    cv::namedWindow(winname, cv::WINDOW_AUTOSIZE); 
    ImShow(winname, histImg); 
    cv::waitKey(0);  
}



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


    Mat foreground, alpha, src_img, background,blend,result,grayblend,eout,jpegimg;

    src_img=cv2ip.ImRead("pac_man.png");
    background=cv2ip.ImRead("dark.bmp");
    jpegimg=cv2ip.ImRead("pepper.bmp");
    cv2ip.SplitAlpha(foreground,alpha,src_img);
    // cv2ip.ImWrite("fore.jpg",foreground);
    cv2ip.ImWrite("pac.jpg",src_img);
    // cv2ip.ImWrite("alpha.jpg",alpha);
    
    Mat dst1, dst2, alph,dstones,mask;
    cv2ip.Resize(dst1,dst2,foreground,background);
    cv2ip.Resize(dst1,alph,foreground,alpha);
    blend=cv2ip.AlphaBlend(dst1,dst2,alph);
    cv2ip.ImShow("blend", blend); 
    waitKey(0); 

    // cv2ip.showInfo(dst1,"dst1");
    // cv2ip.showInfo(dst2,"dst2");
    // cv2ip.showInfo(blend,"blend");
// gray
    grayblend=cv2ip.ImBGR2Gray(blend);
    cv2ip.ImShow("gray", grayblend); 
    waitKey(0);

// gray hist
    cv2ip.CalcGrayHist(result,grayblend);
    cv2ip.ShowGrayHist("Gray Hist", result);


// gray equalize
    grayblend.convertTo(eout,CV_8U);
    // cv2ip.showInfo(eout,"grayblend");
    equalizeHist(eout,eout);
    cv2ip.ImShow("new gray", eout); 
    waitKey(0); 
// new gray hist
    cv2ip.CalcGrayHist(result,eout);
    cv2ip.ShowGrayHist("new Gray Hist", result);

// dark orig
    // cv2ip.ImShow("dark", jpegimg); 
    waitKey(0); 
    vector<Mat> colorresult,splitch;
    cv2ip.CalcColorHist(colorresult,blend);
    // cv2ip.showInfo(blend,"first");
    cv2ip.ShowColorHist("Color Hist", colorresult);

    result=cv2ip.EqualizeColorHist(blend);
    cv2ip.ImShow("equalize ele", result); 
    waitKey(0); 

// equalize dark
    result=cv2ip.EqualizeColorHist(jpegimg);
    // cvtColor(result,result,COLOR_BGR2RGB);
    cv2ip.ImWrite("equalize.jpg",result);
    cv2ip.ImShow("equalize", result); 
    waitKey(0); 

    vector<Mat> newhist;
    cv2ip.CalcColorHist(newhist,result);
    // cv2ip.showInfo(jpegimg,"second");
    // cv2ip.showInfo(result,"second");
    cv2ip.ShowColorHist("equalize Hist", newhist);

    CvImage pinksky,skymask,newimg;
    newimg=cv2ip.ImRead("sky.jpeg");

    
    
    CvImage sky=cv2ip.ImRead("flower.jpg");

    // pinksky.create(pinksky.rows*3, pinksky.cols*3, CV_8U);
    cv::resize(newimg,pinksky,newimg.size()*3);
    
    // CvImage myroi2=cv2ip.ImRead("mcdonald.png");
    CvImage myroi = pinksky(cv::Rect(25,30,sky.cols,sky.rows));
    CvImage myroi2 = pinksky(cv::Rect(20,50,sky.cols,sky.rows));
    // CvImage myroi2 = (cv::Rect(20,50,sky.cols,sky.rows));
    // cout<<&pinksky<<endl<<&myroi<<endl;
    skymask=sky(cv::Rect(0,0,sky.cols,sky.rows));
    cv2ip.ImShow("ROI", myroi); 
    waitKey(0); 
    myroi2=cv2ip.EqualizeColorHist(myroi);

    // myroi = cv::Mat::zeros(myroi.size(), myroi.type());
    // sky.convertTo(myroi,myroi.type());
    // myroi2.copyTo(myroi);
    // myroi = sky.clone();
    addWeighted(myroi, 0.2, sky, 0.8, 0.0, myroi);//dst = src1[I]*alpha+ src2[I]*beta + gamma；第一第四个参数就是各自权重，第5个参数就是公式中的偏执因子gamma。
    cv2ip.ImShow("ROI", myroi); 
    waitKey(0); 
    cv2ip.ImWrite("pinksky_1.jpg",pinksky);
    cv2ip.ImShow("pinksky", pinksky); 
    waitKey(0); 


    return 0;
} 