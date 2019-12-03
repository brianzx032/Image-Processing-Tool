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
void cv2ImageProcessing::Resize(CvImage& newImg1, CvImage& newImg2, const CvImage& SrcImg1, const CvImage& SrcImg2, bool MaxSize)
{
    int rows, cols;
    if(MaxSize)
    {
        rows = std::max(SrcImg1.rows,SrcImg2.rows);
        cols = std::max(SrcImg1.cols,SrcImg2.cols);
    }
    else
    {
        rows = std::min(SrcImg1.rows,SrcImg2.rows);
        cols = std::min(SrcImg1.cols,SrcImg2.cols);
    }
    
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

CvImage cv2ImageProcessing::AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha, int code)
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
    if (code!=NoCvt)
    {
        cv::cvtColor(fore,fore,code);
        ImShow("processing", fore); 
        cv::waitKey(0); 
    }
    
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

CvImage cv2ImageProcessing::AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha)
{
    return AlphaBlend(Foreground,Background,Alpha,NoCvt);
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
    // std::cout<<src<<std::endl;
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
void cv2ImageProcessing::ShowColorHist(CvImage& HistImg, const std::vector<CvImage>& ColorHist)
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
    HistImg=histImage.clone();
}
void cv2ImageProcessing::ShowColorHist(const std::string& winname, const std::vector<CvImage>& ColorHist)
{
    CvImage histImage;
    ShowColorHist(histImage,ColorHist);
    cv::namedWindow(winname,CV_WINDOW_AUTOSIZE);
	ImShow(winname, histImage);
	cv::waitKey(0);
}


void cv2ImageProcessing::MonoEqualize(CvImage& DstGray, const CvImage& SrcGray)
{
    cv::equalizeHist(SrcGray,DstGray);
}

void cv2ImageProcessing::ColorEqualize(CvImage& DstColor, const CvImage& SrcColor, const CV2_COLOREQUALIZE_TYPE Type)
{
    std::vector<CvImage> ImgCh;
    cv::split(SrcColor,ImgCh);
    CvImage Ch[3];
    Ch[0]=ImgCh.at(0);
    Ch[1]=ImgCh.at(1);
    Ch[2]=ImgCh.at(2);

    Ch[0].convertTo(Ch[0],CV_8U);
    Ch[1].convertTo(Ch[1],CV_8U);
    Ch[2].convertTo(Ch[2],CV_8U);
    
    switch (Type)
    {
    case USE_YUV:
        cv::equalizeHist(Ch[0],Ch[0]);//Y
        break;
    case USE_HSV:
        cv::equalizeHist(Ch[2],Ch[2]);//V
        break;
    case USE_RGB:
        cv::equalizeHist(Ch[0],Ch[0]);//R
        cv::equalizeHist(Ch[1],Ch[1]);//G
        cv::equalizeHist(Ch[2],Ch[2]);//B
        break;
    
    default:
        break;
    }
    cv::merge(ImgCh,DstColor);
}

void cv2ImageProcessing::AllChEqualize(CvImage& DstImg, const CvImage& SrcImg)
{
    std::vector<CvImage> SrcHist,SrcCh(3),DstCh(3);
    CvImage SrcPdf,SrcCdf,Lut;
    CalcColorHist(SrcHist,SrcImg);
    cv::split(SrcImg,SrcCh);

    SrcHist.at(0)=SrcHist.at(0)+SrcHist.at(1)+SrcHist.at(2);
    int Sum = SrcImg.rows*SrcImg.cols*3;
    SrcPdf=SrcHist.at(0)/Sum;
    SrcCdf=SrcPdf.clone();

    for(int i=1; i<256; i++)
    {
        SrcCdf.at<float>(i)=SrcCdf.at<float>(i-1)+SrcPdf.at<float>(i);
    }
    SrcCdf.convertTo(Lut,CV_8U,255); 
    for(int ch=0; ch<3; ch++)
    {
        cv::LUT(SrcCh.at(ch),Lut,DstCh.at(ch));
    }
    cv::merge(DstCh,DstImg);

}
void cv2ImageProcessing::HistMatching(CvImage& DstImg, const CvImage& SrcImg, const CvImage& RefImg, const CV2_COLOREQUALIZE_TYPE Type)
{
    std::vector<CvImage> SrcHist,RefHist,SrcPdf(3),RefPdf(3),SrcCdf(3),RefCdf(3),SrcCh(3),DstCh(3),refLut(3);
    CalcColorHist(SrcHist,SrcImg);
    CalcColorHist(RefHist,RefImg);
    cv::split(SrcImg,SrcCh);
    // std::cout<<SrcCh.at(0)<<std::endl;
    int initCh=0, endCh=3;
    int SrcSum = SrcImg.rows*SrcImg.cols;
    int RefSum = RefImg.rows*RefImg.cols;

    switch (Type)
    {
    case USE_YUV:
        initCh=0;
        endCh=0;//Y
        DstCh.at(1)=SrcCh.at(1).clone();
        DstCh.at(2)=SrcCh.at(2).clone();
        break;
    case USE_HSV:
        DstCh.at(0)=SrcCh.at(0).clone();
        DstCh.at(1)=SrcCh.at(1).clone();
        initCh=2;
        endCh=2;//V
        break;
    case USE_RGB:
        initCh=0;
        endCh=2;//BGR
        break;
    default:
        break;
    }
    for(int ch=initCh; ch<=endCh; ch++)
    {
        SrcPdf.at(ch)=SrcHist.at(ch)/SrcSum;
        RefPdf.at(ch)=RefHist.at(ch)/RefSum;
        
        SrcCdf.at(ch)=SrcPdf.at(ch).clone();
        RefCdf.at(ch)=RefPdf.at(ch).clone();
        
        for(int i=1; i<256; i++)
        {
            SrcCdf.at(ch).at<float>(i)=SrcCdf.at(ch).at<float>(i-1)+SrcPdf.at(ch).at<float>(i);
            RefCdf.at(ch).at<float>(i)=RefCdf.at(ch).at<float>(i-1)+RefPdf.at(ch).at<float>(i);
        }

        // RefCdf.at(ch).convertTo(refLut.at(ch),CV_8U,255); 
        

        // 累积概率的差值
        float diff_cdf[256][256];
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                diff_cdf[i][j] = fabs(SrcCdf.at(ch).at<float>(i) - RefCdf.at(ch).at<float>(j));

        // 构建灰度级映射表
        RefCdf.at(ch).convertTo(refLut.at(ch),CV_8U); 
        for (int i = 0; i < 256; i++)
        {
            // 查找源灰度级为ｉ的映射灰度
            //　和ｉ的累积概率差值最小的规定化灰度
            float min = diff_cdf[i][0];
            int index = 0;
            for (int j = 1; j < 256; j++)
            {
                if (min > diff_cdf[i][j])
                {
                    min = diff_cdf[i][j];
                    index = j;
                }
            }
            refLut.at(ch).at<uchar>(i) = static_cast<uchar>(index);
        }

        
        
        // SrcCdf.at(ch).convertTo(SrcCdf.at(ch),CV_8U,255); 
        // RefCdf.at(ch).convertTo(RefCdf.at(ch),CV_8U,255); 
        // int Sval,Rval,Ridx=0;
        // // refLut.at(ch).at<uchar>(0)=0;
        // for(int i=0; i<256; i++)
        // {
        //     Sval=SrcCdf.at(ch).at<uchar>(i);
        //     for(int j=0; j<256; j++)
        //     {
        //         Rval=RefCdf.at(ch).at<uchar>(j);
        //         if(Sval==Rval)
        //         {
        //             Ridx=j;
        //             break;
        //         }
        //     }
        //     std::cout<<Ridx<<std::endl;
        //     refLut.at(ch).at<uchar>(i)==static_cast<uchar>(Ridx);
        // }


        // std::cout<<SrcCdf.at(ch)<<std::endl;
        // std::cout<<RefCdf.at(ch)<<std::endl;
        // std::cout<<refLut.at(ch)<<std::endl;

        cv::LUT(SrcCh.at(ch),refLut.at(ch),DstCh.at(ch));
    }

    // ShowColorHist("hist",SrcHist);

    // ShowColorHist("SrcPdf",SrcPdf);
    // ShowColorHist("SrcCdf",SrcCdf);
    // ShowColorHist("RefPdf",RefPdf);
    // ShowColorHist("RefCdf",RefCdf);

    cv::merge(DstCh,DstImg);
    // showInfo(DstImg,"dst");
    // std::cout<<DstImg<<std::endl;
}

void cv2ImageProcessing::HistMatchAll(CvImage& DstImg, const CvImage& SrcImg, const CvImage& RefImg)
{
    std::vector<CvImage> SrcHist,RefHist,SrcCh(3),DstCh(3);
    CvImage SrcPdf, SrcCdf, RefPdf, RefCdf, refLut;
    CalcColorHist(SrcHist,SrcImg);
    CalcColorHist(RefHist,RefImg);
    cv::split(SrcImg,SrcCh);
    SrcHist.at(0)=SrcHist.at(0)+SrcHist.at(1)+SrcHist.at(2);
    RefHist.at(0)=RefHist.at(0)+RefHist.at(1)+RefHist.at(2);
    // std::cout<<SrcCh.at(0)<<std::endl;
    int SrcSum = SrcImg.rows*SrcImg.cols*3;
    int RefSum = RefImg.rows*RefImg.cols*3;

    SrcPdf=SrcHist.at(0)/SrcSum;
    RefPdf=RefHist.at(0)/RefSum;
        
    SrcCdf=SrcPdf.clone();
    RefCdf=RefPdf.clone();
        
    for(int i=1; i<256; i++)
    {
        SrcCdf.at<float>(i)=SrcCdf.at<float>(i-1)+SrcPdf.at<float>(i);
        RefCdf.at<float>(i)=RefCdf.at<float>(i-1)+RefPdf.at<float>(i);
    }
    // 累积概率的差值
    float diff_cdf[256][256];
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            diff_cdf[i][j] = fabs(SrcCdf.at<float>(i) - RefCdf.at<float>(j));

    // 构建灰度级映射表
    RefCdf.convertTo(refLut,CV_8U); 
    for (int i = 0; i < 256; i++)
    {
        // 查找源灰度级为ｉ的映射灰度
        //　和ｉ的累积概率差值最小的规定化灰度
        float min = diff_cdf[i][0];
        int index = 0;
        for (int j = 1; j < 256; j++)
        {
            if (min > diff_cdf[i][j])
            {
                min = diff_cdf[i][j];
                index = j;
            }
        }
        refLut.at<uchar>(i) = static_cast<uchar>(index);
    }

    for(int ch=0; ch<3; ch++)
    {
        cv::LUT(SrcCh.at(ch),refLut,DstCh.at(ch));
    }
    cv::merge(DstCh,DstImg);
    
}

void cv2ImageProcessing::ShowCDF(CvImage& Img, CvImage& Pdf_img, CvImage& Cdf_img)
{
    std::vector<CvImage> Hist,Pdf(3),Cdf(3);
    CalcColorHist(Hist,Img);
    
    int pixelSum = Img.rows*Img.cols;

    for(int ch=0; ch<3; ch++)
    {
        Pdf.at(ch)=Hist.at(ch)/pixelSum;
        Cdf.at(ch)=Pdf.at(ch).clone();
        
        for(int i=1; i<256; i++)
            Cdf.at(ch).at<float>(i)=Cdf.at(ch).at<float>(i-1)+Pdf.at(ch).at<float>(i);
    }
    ShowColorHist(Pdf_img,Pdf);
    ShowColorHist(Cdf_img,Cdf);

}
void cv2ImageProcessing::ShowCDF(CvImage& Img)
{
    std::vector<CvImage> Hist,Pdf(3),Cdf(3);
    CalcColorHist(Hist,Img);
    
    int pixelSum = Img.rows*Img.cols;

    for(int ch=0; ch<3; ch++)
    {
        Pdf.at(ch)=Hist.at(ch)/pixelSum;
        Cdf.at(ch)=Pdf.at(ch).clone();
        
        for(int i=1; i<256; i++)
            Cdf.at(ch).at<float>(i)=Cdf.at(ch).at<float>(i-1)+Pdf.at(ch).at<float>(i);
    }
    ShowColorHist("Pdf",Pdf);
    ShowColorHist("Cdf",Cdf);
}

void cv2ImageProcessing::ShowDiff(CvImage& Img1,CvImage& Img2,int Factor)
{
    CvImage ImgDiff;
    cv::subtract(Img1,Img2,ImgDiff);
    ImgDiff=ImgDiff*Factor;
    // std::cout<<ImgDiff<<std::endl;
    ImShow("diff",ImgDiff);
    cv::waitKey(0);
}