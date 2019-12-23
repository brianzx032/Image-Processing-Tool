#include "cv2ImageProcessing.h"

// Constructor & Deconstructor
cv2ImageProcessing::cv2ImageProcessing(void)
{
    InitRobertsKernel();
    InitPrewittKernel();
    InitKirschKernel();
    std::cout << "cv2ImageProcessing is being created" << std::endl;
}
cv2ImageProcessing::~cv2ImageProcessing(void)
{
    std::cout << "cv2ImageProcessing is being deleted" << std::endl;
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

CvImage cv2ImageProcessing::AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha)
{
    CvImage fore, back, alph, out_img,al;
    std::vector<CvImage> alph_mask;
    
    Foreground.convertTo(fore, CV_8U);
    Background.convertTo(back, CV_8U);
    Alpha.convertTo(alph, CV_8U,1.f/255);

    alph_mask.push_back(alph);
    alph_mask.push_back(alph);
    alph_mask.push_back(alph);
    cv::merge(alph_mask, alph);

    out_img=CvImage::zeros(fore.size(),fore.type());
    
    cv::multiply(alph, fore, fore);
    cv::multiply(cv::Scalar::all(1.0)-alph, back, back);
    cv::add(fore, back, out_img);
    return out_img;
}

void cv2ImageProcessing::BlendImage(CvImage& DstImg, const CvImage& FgImg, const CvImage& BgImg, int width, int height, int x, int y, double transparent)
{
    CvImage Fg_taget, ForeImg, AlphaImg, BackImg;
    
    /* resize foreground image */
    Fg_taget.create(height,width,CV_8U);
    cv::resize(FgImg,Fg_taget,Fg_taget.size());

    /* split image and alpha */
    SplitAlpha(ForeImg,AlphaImg,Fg_taget);

    /* set roi */
    BgImg.copyTo(DstImg);
    CvImage roiImg = DstImg(cv::Rect(x,y,Fg_taget.cols,Fg_taget.rows));
    BackImg=roiImg.clone();

    /* resize */
    CvImage Fg_resized, Bg_resized, Al_resized;
    Resize(Fg_resized,Bg_resized,ForeImg,BackImg,true);
    Resize(Fg_resized,Al_resized,ForeImg,AlphaImg,true);

    /* transparency */
    Fg_resized=Fg_resized*(1-transparent)+Bg_resized*transparent;

    /* blend */
    CvImage blendImg=AlphaBlend(Fg_resized,Bg_resized,Al_resized);
    blendImg.copyTo(roiImg);
}

// convert BGR => Gray
void cv2ImageProcessing::ImBGR2Gray(CvImage& DstImg, const CvImage& SrcImg)
{
    cv::cvtColor(SrcImg,DstImg,CV_BGR2GRAY);
}
CvImage cv2ImageProcessing::ImBGR2Gray(const CvImage& SrcImg)
{
    CvImage DstImg;    
    cv::cvtColor(SrcImg,DstImg,CV_BGR2GRAY);
    return DstImg;
}

// Gray Hist
void cv2ImageProcessing::CalcGrayHist(CvImage& GrayHist, const CvImage& SrcGray)
{
    const int *histChannel = 0, histSize = 256;
    float ranges[]={0,256};
    const float *hstranges={ranges};
    // rearrange
    CvImage src;
    SrcGray.convertTo(src,CV_8U);
    cv::calcHist(&src,1,histChannel,CvImage(),GrayHist,1,&histSize,&hstranges,true,false);
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


        cv::LUT(SrcCh.at(ch),refLut.at(ch),DstCh.at(ch));
    }
    cv::merge(DstCh,DstImg);
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

void cv2ImageProcessing::ShowDiff(CvImage& Img1,CvImage& Img2,double Factor)
{
    CvImage ImgDiff;
    cv::subtract(Img1,Img2,ImgDiff);
    ImgDiff=abs(ImgDiff)*Factor;
    ImShow("diff",ImgDiff);
    cv::waitKey(0);
}
void cv2ImageProcessing::ShowDiff(CvImage& Img1,CvImage& Img2,double Factor,const std::string& filename)
{
    CvImage ImgDiff;
    cv::subtract(Img1,Img2,ImgDiff);
    ImgDiff=abs(ImgDiff)*Factor;
    // std::cout<<sum(ImgDiff)<<std::endl;    
    ImWrite(filename,ImgDiff);
}

void cv2ImageProcessing::Smooth2D(CvImage& DstImg, const CvImage& SrcImg, int ksize, const CV2_IMSMOOTH_TYPE Type)
{
    switch (Type)
    {
    case BLUR:
        cv::blur(SrcImg,DstImg,cv::Size(ksize,ksize));
        break;

    case BOX:
        // int ddepth: 输出图像的深度，-1代表使用原图深度，即src.depth()
        cv::boxFilter(SrcImg,DstImg,-1,cv::Size(ksize,ksize));
        break;

    case GAUSSIAN:
        // double sigmaX: 表示高斯核函数在X方向的标准偏差。
        // double sigmaY: 表示高斯核函数在Y方向的标准偏差。
        // 当sigmaY为0时，就将其设为sigmaX；如果两者均为0，则由ksize.with和ksize.height计算出来，
        // 因此在高斯滤波函数中，ksize的w和h均必须是正数和奇数，或0，两者可以不同。
        cv::GaussianBlur(SrcImg,DstImg,cv::Size(ksize,ksize),0,0);
        break;

    case MEDIAN:
        cv::medianBlur(SrcImg,DstImg,ksize);
        break;

    case BILATERAL:
        // double sigmaColor:
        // 颜色空间滤波器的sigma值，这个参数的值越大，就表明该像素邻域内有越宽广的颜色会被混合到一起，产生较大的半相等颜色区域。
        // double sigmaSpace:
        // 坐标空间中滤波器的sigma值，坐标空间的标注方差。它的数值越大，意味着越远的像素会相互影响，从而使更大的区域中足够相似的颜色获取相同的颜色。
        // 当d>0时，d制定了邻域大小与sigmaSpace无关。否则，d正比于sigmaSpace。
        cv::bilateralFilter(SrcImg,DstImg,ksize,ksize*2,ksize/2);
        break;
    
    default:
        break;
    }
}

void cv2ImageProcessing::EdgeDetect(CvImage& DstImg, const CvImage& SrcImg, const CV2_EDGEDETECT_TYPE Type)
{
    CvImage DstX,DstY;
    std::vector<CvImage> kernel,cannyDst;
    switch (Type)
    {
    case SOBEL:
        cv::Sobel(SrcImg,DstX,-1,1,0);
        cv::Sobel(SrcImg,DstY,-1,0,1);
        DstImg=(abs(DstX)+abs(DstY))/2;
        break;
    
    case CANNY:
        cv::Canny(SrcImg,DstImg,canny_threshold1,canny_threshold2);//threshold1 threshold2
        for(int i =0; i<3; i++)
        {
            cannyDst.push_back(DstImg);
        }
        cv::merge(cannyDst, DstImg);
        break;
    
    case SCHARR:
        cv::Scharr(SrcImg,DstX,-1,1,0);
        cv::Scharr(SrcImg,DstY,-1,0,1);
        DstImg=(abs(DstX)+abs(DstY))/2;
        break;
    
    case LAPLACE:
        cv::Laplacian(SrcImg,DstImg,-1,std::min(kernel_size,31));
        break;

    case ROBERTS:
        kernel=GetRobertsKernel();
        Conv2D(DstX,SrcImg,kernel[0]);
        Conv2D(DstY,SrcImg,kernel[1]);
        DstImg=(abs(DstX)+abs(DstY))/2;
        // DstImg=cv::max(DstX,DstY);
        break;
    
    case PREWITT:
        kernel=GetPrewittKernel();
        Conv2D(DstX,SrcImg,kernel[0]);
        Conv2D(DstY,SrcImg,kernel[1]);
        DstImg=(abs(DstX)+abs(DstY))/2;
        break;
    
    case KIRSCH:
        kernel=GetKirschKernel();
        Conv2D(DstImg,SrcImg,kernel[0]);
        for(int i=1;i<8;i++)
        {
            Conv2D(DstX,SrcImg,kernel[i]);
            DstImg=cv::max(DstImg,DstX);
        }
        break;
    
    default:
        break;
    }
}

void cv2ImageProcessing::Conv2D(CvImage& DstImg, const CvImage& SrcImg, const CvImage& Kernel)
{
    cv::filter2D(SrcImg,DstImg,-1,Kernel);
}

void cv2ImageProcessing::InitRobertsKernel()
{
    CvImage kernel_x = (cv::Mat_<char>(2,2) <<  1, 0,
                                                0, -1);
    CvImage kernel_y = (cv::Mat_<char>(2,2) <<  0, 1,
                                                -1, 0);
    m_RobertsKernel.push_back(kernel_x);
    m_RobertsKernel.push_back(kernel_y);
}
void cv2ImageProcessing::InitPrewittKernel()
{
    CvImage kernel_x = (cv::Mat_<char>(3,3) <<  -1, 0, 1,
                                                -1, 0, 1,
                                                -1, 0, 1);
    CvImage kernel_y = (cv::Mat_<char>(3,3) <<  -1, -1, -1,
                                                 0,  0,  0,
                                                 1,  1,  1);
    m_PrewittKernel.push_back(kernel_x);
    m_PrewittKernel.push_back(kernel_y);
}
void cv2ImageProcessing::InitKirschKernel()
{
    CvImage kernel_w = (cv::Mat_<char>(3,3) <<  5, -3, -3,
                                                5,  0, -3,
                                                5, -3, -3);
    CvImage kernel_sw = (cv::Mat_<char>(3,3) << -3, -3, -3,
                                                5,  0, -3,
                                                5,  5, -3);
    CvImage kernel_s = (cv::Mat_<char>(3,3) <<  -3, -3, -3,
                                                -3,  0, -3,
                                                5, 5, 5);
    CvImage kernel_se = (cv::Mat_<char>(3,3) << -3, -3, -3,
                                                -3,  0, 5,
                                                -3,  5, 5);
    CvImage kernel_e = (cv::Mat_<char>(3,3) <<  -3, -3, 5,
                                                -3,  0, 5,
                                                -3, -3, 5);
    CvImage kernel_ne = (cv::Mat_<char>(3,3) << -3, 5, 5,
                                                -3, 0, 5,
                                                -3, -3, -3);
    CvImage kernel_n = (cv::Mat_<char>(3,3) <<  5, 5, 5,
                                                -3,  0, -3,
                                                -3, -3, -3);
    CvImage kernel_nw = (cv::Mat_<char>(3,3) << 5, 5, -3,
                                                5, 0, -3,
                                                -3, -3, -3);
    m_KirschKernel.push_back(kernel_w);
    m_KirschKernel.push_back(kernel_sw);
    m_KirschKernel.push_back(kernel_s);
    m_KirschKernel.push_back(kernel_se);
    m_KirschKernel.push_back(kernel_e);
    m_KirschKernel.push_back(kernel_ne);
    m_KirschKernel.push_back(kernel_n);
    m_KirschKernel.push_back(kernel_nw);
}


void cv2ImageProcessing::ImSharpening(CvImage& DstImg, const CvImage& SrcImg, const CV2_SHARPENING_TYPE Type1, const CV2_IMSMOOTH_TYPE Type2)
{
    CvImage LapKern_type1 = (cv::Mat_<char>(3,3) <<  0,-1, 0,
                                                    -1, 4,-1,
                                                     0,-1, 0);
    CvImage LapKern_type2 = (cv::Mat_<char>(3,3) << -1,-1,-1,
                                                    -1, 8,-1,
                                                    -1,-1,-1);
    CvImage LoG_Kern = (cv::Mat_<char>(5,5) <<  0, 0, 1, 0, 0,
                                                0, 1, 2, 1, 0,
                                                1, 2,-16,2, 1,
                                                0, 1, 2, 1, 0,
                                                0, 0, 1, 0, 0);
    CvImage coarse, fine;
    switch (Type1)
    {
    case LAPLACE_TYPE1:
        Conv2D(fine,SrcImg,LapKern_type1);
        DstImg=SrcImg+fine*landa;
        break;

    case LAPLACE_TYPE2:
        Conv2D(fine,SrcImg,LapKern_type2);
        DstImg=SrcImg+fine*landa;
        break;

    case SECOND_ORDER_LOG:
        Conv2D(fine,SrcImg,LoG_Kern);
        DstImg=SrcImg+fine*landa;
        break;
    
    case UNSHARP_MASK:
        Smooth2D(coarse,SrcImg,kernel_size,Type2);
        fine=SrcImg-coarse;
        // ImShow("fine",fine);
        DstImg=coarse+fine*landa;
        break;
    
    default:
        break;
    }
    
}

std::vector<CvImage> cv2ImageProcessing::GetRobertsKernel() const
{
    return m_RobertsKernel;
}
std::vector<CvImage> cv2ImageProcessing::GetPrewittKernel() const
{
    return m_PrewittKernel;
}
std::vector<CvImage> cv2ImageProcessing::GetKirschKernel() const
{
    return m_KirschKernel;
}