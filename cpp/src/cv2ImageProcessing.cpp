#include "cv2ImageProcessing.h"
#include <iostream>
#include <vector>

// Constructor & Deconstructor
cv2ImageProcessing::cv2ImageProcessing(void)
{
    std::cout << "Object is being created" << std::endl;
}
cv2ImageProcessing::~cv2ImageProcessing(void)
{
    std::cout << "Object is being deleted" << std::endl;
}

// show image infomation
void cv2ImageProcessing::showInfo(CvImage& cvImg, const std::string& filename)
{
    std::cout << filename << " Info" << std::endl;
    std::cout << "Size:" << cvImg.size() << std::endl;
    std::cout << "Type:" << cvImg.type() << std::endl;
    std::cout << "Depth:" << cvImg.depth() << std::endl;
    std::cout << "Channels:" << cvImg.channels() << std::endl;
}

// read, write, show
CvImage cv2ImageProcessing::ImRead(const std::string& filename)
{
    return cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
}

void cv2ImageProcessing::ImWrite(const std::string& filename, CvImage& cvImg)
{
    cv::imwrite(filename, cvImg);
}

void cv2ImageProcessing::ImShow(const std::string& winname, CvImage& cvImg)
{
    cv::imshow(winname, cvImg);
}

// resize
void cv2ImageProcessing::Resize(CvImage& newImg1, CvImage& newImg2, const CvImage& SrcImg1, const CvImage& SrcImg2)
{
    int rows, cols;
    rows = std::max(SrcImg1.rows,SrcImg2.rows);
    cols = std::max(SrcImg1.cols,SrcImg2.cols);

    newImg1.create(rows, cols, CV_8U);
    newImg2.create(rows, cols, CV_8U);
    
    cv::resize(SrcImg1,newImg1,newImg1.size());
    cv::resize(SrcImg2,newImg2,newImg2.size());
    // showInfo(newImg1,"1");
    // showInfo(newImg2,"2");
}

// alpha blend
void cv2ImageProcessing::SplitAlpha(CvImage& Foreground, CvImage& Alpha, const CvImage& SrcImg)
{
    std::vector<CvImage> channels_src, rgb_img;
    cv::split(SrcImg, channels_src);

    rgb_img.push_back(channels_src.at(0));
    rgb_img.push_back(channels_src.at(1));
    rgb_img.push_back(channels_src.at(2));

    cv::merge(rgb_img, Foreground);
    channels_src.at(3).copyTo(Alpha);
}

CvImage cv2ImageProcessing::AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha)
{
    CvImage fore, back, alph, out_img,al;
    std::vector<CvImage> alph_mask;

    // std::cout<<Foreground<<std::endl;
    
    Foreground.convertTo(fore, CV_8U);
    Background.convertTo(back, CV_8U);
    Alpha.convertTo(alph, CV_8U,1.f/255);

    alph_mask.push_back(alph);
    alph_mask.push_back(alph);
    alph_mask.push_back(alph);
    cv::merge(alph_mask, alph);


    out_img=CvImage::zeros(fore.size(),fore.type());
    
    // alph.convertTo(al,CV_32FC3,255);
    // ImShow("alpha", al); 
    // cv::waitKey(0); 
    // ImShow("back", back); 
    // cv::waitKey(0); 

    cv::cvtColor(fore,fore,cv::COLOR_BGR2HSV);
    ImShow("fore", fore); 
    cv::waitKey(0); 

    cv::multiply(alph, fore, fore);
    cv::multiply(cv::Scalar::all(1.0)-alph, back, back);
    cv::add(fore, back, out_img);

    // ImShow("alpha", alph); 
    // cv::waitKey(0); 
    // ImShow("fore", fore); 
    // cv::waitKey(0); 
    // ImShow("back", back); 
    // cv::waitKey(0); 
    // std::cout<<out_img<<std::endl;
    return out_img;
}

// convert BGR => Gray
void cv2ImageProcessing::ImBGR2Gray(CvImage& DstImg, const CvImage& SrcImg)
{
    cv::cvtColor(SrcImg,DstImg,CV_BGR2GRAY);
}
CvImage cv2ImageProcessing::ImBGR2Gray(const CvImage& SrcImg)
{
    CvImage DstImg;
    
    // std::cout<<SrcImg<<std::endl;
    cv::cvtColor(SrcImg,DstImg,CV_BGR2GRAY);
    // std::cout<<DstImg<<std::endl;
    return DstImg;
}

// Gray Hist
void cv2ImageProcessing::CalcGrayHist(CvImage& GrayHist, const CvImage& SrcGray)
{
    // CvImage shwimg=SrcGray.clone();
    // ImShow("x", shwimg); 
    // cv::waitKey(0); 
    // std::cout<<SrcGray<<std::endl;
    const int *histChannel = 0, histSize = 256;
    float ranges[]={0,256};
    const float *hstranges={ranges};
    // rearrange
    CvImage src;
    // SrcGray.convertTo(src,CV_32FC3, 255);
    SrcGray.convertTo(src,CV_8U);
    cv::calcHist(&src,1,histChannel,CvImage(),GrayHist,1,&histSize,&hstranges,true,false);
    // std::cout<<GrayHist<<std::endl<<std::endl;
}
void cv2ImageProcessing::ShowGrayHist(const std::string& winname, const CvImage& GrayHist)
{

    int histSize=256;
	//归一化
	int hist_h = 400;//直方图的图像的高
	int hist_w = 512;//直方图的图像的宽
	int bin_w = hist_w / histSize;//直方图的等级
	CvImage histImage(hist_w, hist_h, CV_8U, cv::Scalar(0, 0, 0));//绘制直方图显示的图像
	normalize(GrayHist, GrayHist, 0, hist_h, cv::NORM_MINMAX, -1, CvImage());//归一化
	//步骤三：绘制直方图（render histogram chart）
	for (int i = 1; i < histSize; i++)
	{
		//绘制直方图
		line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(GrayHist.at<float>(i - 1))),
			cv::Point((i)*bin_w, hist_h - cvRound(GrayHist.at<float>(i))), cv::Scalar(255, 255, 255), 2, CV_AA);
	}
    cv::namedWindow(winname,CV_WINDOW_AUTOSIZE);
	ImShow(winname, histImage);
	cv::waitKey(0);

}
void cv2ImageProcessing::CalcColorHist(std::vector<CvImage>& ColorHist, const CvImage& SrcColor)
{
//步骤一：分通道显示
	std::vector<CvImage>bgr_planes;
    CvImage b_hist, g_hist, r_hist, srcImg;
    SrcColor.convertTo(srcImg,CV_8U);
    // SrcColor.convertTo(srcImg,CV_8U);
	cv::split(SrcColor, bgr_planes);
    CalcGrayHist(b_hist,bgr_planes.at(0));
    CalcGrayHist(g_hist,bgr_planes.at(1));
    CalcGrayHist(r_hist,bgr_planes.at(2));
    ColorHist.push_back(b_hist);
    ColorHist.push_back(g_hist);
    ColorHist.push_back(r_hist);
    // std::cout<<b_hist<<std::endl<<std::endl;
	

}
void cv2ImageProcessing::ShowColorHist(const std::string& winname, const std::vector<CvImage>& ColorHist)
{

	CvImage b_hist, g_hist, r_hist;
    b_hist=ColorHist.at(0).clone();
    g_hist=ColorHist.at(1).clone();
    r_hist=ColorHist.at(2).clone();
    int histSize=256;
	//归一化
	int hist_h = 400;//直方图的图像的高
	int hist_w = 512;////直方图的图像的宽
	int bin_w = hist_w / histSize;//直方图的等级
	CvImage histImage(hist_w, hist_h, CV_32FC3, cv::Scalar(0, 0, 0));//绘制直方图显示的图像
	normalize(b_hist, b_hist, 0, hist_h, cv::NORM_MINMAX, -1, CvImage());//归一化
	normalize(g_hist, g_hist, 0, hist_h, cv::NORM_MINMAX, -1, CvImage());
	normalize(r_hist, r_hist, 0, hist_h, cv::NORM_MINMAX, -1, CvImage());
	//步骤三：绘制直方图（render histogram chart）
	for (int i = 1; i < histSize; i++)
	{
		//绘制蓝色分量直方图
		line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, CV_AA);
		//绘制绿色分量直方图
		line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			cv::Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, CV_AA);
		//绘制红色分量直方图
		line(histImage, cv::Point((i - 1)*bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			cv::Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, CV_AA);
	}
    cv::namedWindow(winname,CV_WINDOW_AUTOSIZE);
	ImShow(winname, histImage);
	cv::waitKey(0);
}

CvImage cv2ImageProcessing::EqualizeColorHist(const CvImage& SrcImg)
{
    std::vector<CvImage> channels;
    cv::split(SrcImg,channels);
    CvImage blue, green, red, result;
    blue=channels.at(0);
    green=channels.at(1);
    red=channels.at(2);

    blue.convertTo(blue,CV_8U);
    green.convertTo(green,CV_8U);
    red.convertTo(red,CV_8U);
    
    cv::equalizeHist(blue,blue);
    cv::equalizeHist(green,green);
    cv::equalizeHist(red,red);
    cv::merge(channels,result);
    return result;
}