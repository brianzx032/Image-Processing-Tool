#ifndef CV2IMAGEPROCESSING_H
#define CV2IMAGEPROCESSING_H
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define NoCvt 200
typedef cv::Mat CvImage;
class cv2ImageProcessing
{
    public:

    // constructor, deconstructor
    cv2ImageProcessing(void);
    ~cv2ImageProcessing(void);

    // show info
    void showInfo(CvImage& cvImg, const std::string& filename);

    // image read, write, show
    CvImage ImRead(const std::string& filename);
    void ImWrite(const std::string& filename, CvImage& cvImg);
    void ImShow(const std::string& winname, CvImage& cvImg);
    
    // resize, split alpha, alpha blend
    void Resize(CvImage& newImg1, CvImage& newImg2, const CvImage& SrcImg1, const CvImage& SrcImg2, bool MaxSize);
    void SplitAlpha(CvImage& Foreground, CvImage& Alpha, const CvImage& SrcImg);

    CvImage AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha);
    void BlendImage(CvImage& DstImg, const CvImage& FgImg, const CvImage& BgImg, int width, int height, int x, int y, double transparent);

    // BGR ==> Gray
    void ImBGR2Gray(CvImage& DstImg, const CvImage& SrcImg);
    CvImage ImBGR2Gray(const CvImage& SrcImg);

    // Gray Hist
    void CalcGrayHist(CvImage& GrayHist, const CvImage& SrcGray);
    void ShowGrayHist(const std::string& winname, const CvImage& GrayHist);

    // color hist
    void CalcColorHist(std::vector<CvImage>& ColorHist, const CvImage& SrcColor);
    void ShowColorHist(CvImage& HistImg, const std::vector<CvImage>& ColorHist);
    void ShowColorHist(const std::string& winname, const std::vector<CvImage>& ColorHist);

    void MonoEqualize(CvImage& DstGray, const CvImage& SrcGray);

    enum CV2_COLOREQUALIZE_TYPE {USE_RGB=0, USE_HSV, USE_YUV};
    void ColorEqualize(CvImage& DstColor, const CvImage& SrcColor, const CV2_COLOREQUALIZE_TYPE Type=USE_RGB);
    void AllChEqualize(CvImage& DstImg, const CvImage& SrcImg);

    void HistMatching(CvImage& DstImg, const CvImage& SrcImg, const CvImage& RefImg, const CV2_COLOREQUALIZE_TYPE Type=USE_RGB);
    void HistMatchAll(CvImage& DstImg, const CvImage& SrcImg, const CvImage& RefImg);
    void ShowCDF(CvImage& Img);
    void ShowCDF(CvImage& Img, CvImage& Pdf_img, CvImage& Cdf_img);
    void ShowDiff(CvImage& Img1,CvImage& Img2,double Factor);
    void ShowDiff(CvImage& Img1,CvImage& Img2,double Factor, const std::string& filename);

    enum CV2_IMSMOOTH_TYPE{BLUR=0,BOX,GAUSSIAN,MEDIAN,BILATERAL};
    void Smooth2D(CvImage& DstImg, const CvImage& SrcImg, int ksize=15, const CV2_IMSMOOTH_TYPE Type=BLUR);

    enum CV2_EDGEDETECT_TYPE{SOBEL=0,CANNY,SCHARR,LAPLACE,ROBERTS,PREWITT,KIRSCH};
    void EdgeDetect(CvImage& DstImg, const CvImage& SrcImg, const CV2_EDGEDETECT_TYPE Type=SOBEL);

    void Conv2D(CvImage& DstImg, const CvImage& SrcImg, const CvImage& Kernel);
    std::vector<CvImage> GetRobertsKernel() const;
    std::vector<CvImage> GetPrewittKernel() const;
    std::vector<CvImage> GetKirschKernel() const;

    enum CV2_SHARPENING_TYPE {LAPLACE_TYPE1 = 0, LAPLACE_TYPE2, SECOND_ORDER_LOG, UNSHARP_MASK};
    void ImSharpening(CvImage& DstImg, const CvImage& SrcImg, const CV2_SHARPENING_TYPE Type1 = LAPLACE_TYPE1, const CV2_IMSMOOTH_TYPE Type2 = BILATERAL);

    int kernel_size=3;
    double landa=1.5;
    double canny_threshold1=50, canny_threshold2=50;

    private:
    std::vector<CvImage> m_RobertsKernel;
    std::vector<CvImage> m_PrewittKernel;
    std::vector<CvImage> m_KirschKernel;
    void InitRobertsKernel();
    void InitPrewittKernel();
    void InitKirschKernel();

};
#endif