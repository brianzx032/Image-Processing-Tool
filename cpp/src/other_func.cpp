
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