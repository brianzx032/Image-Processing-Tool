#ifndef CV2IMAGEPROCESSING_H
#define CV2IMAGEPROCESSING_H
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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
    void Resize(CvImage& newImg1, CvImage& newImg2, const CvImage& SrcImg1, const CvImage& SrcImg2);
    void SplitAlpha(CvImage& Foreground, CvImage& Alpha, const CvImage& SrcImg);
    CvImage AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha);

    // BGR ==> Gray
    void ImBGR2Gray(CvImage& DstImg, const CvImage& SrcImg);
    CvImage ImBGR2Gray(const CvImage& SrcImg);

    // Gray Hist
    void CalcGrayHist(CvImage& GrayHist, const CvImage& SrcGray);
    void ShowGrayHist(const std::string& winname, const CvImage& GrayHist);

    private:

};
#endif