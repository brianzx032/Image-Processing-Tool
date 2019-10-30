#include "cv2ImageProcessing.h"
#include <iostream>
#include <vector>

cv2ImageProcessing::cv2ImageProcessing(void)
{
    std::cout << "Object is being created" << std::endl;
}
cv2ImageProcessing::~cv2ImageProcessing(void)
{
    std::cout << "Object is being deleted" << std::endl;
}

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
    CvImage fore, back, alph, out_img;
    Foreground.convertTo(fore, CV_32FC3);
    Background.convertTo(back, CV_32FC3);
    Alpha.convertTo(alph, CV_32FC3, 1.0/255);

    out_img=CvImage::zeros(fore.size(),fore.type());
    cv::multiply(alph, fore, fore);
    cv::multiply(cv::Scalar::all(1.0)-alph, back, back);
    cv::add(fore, back, out_img);
    return out_img;
}