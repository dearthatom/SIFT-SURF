#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/opencv_lib.hpp>

int main(int argc, char *argv[])
{


	std::string filename1 = argv[1];
	if (filename1.empty()) return -1;
	cv::Mat colorImg1 = cv::imread(filename1, 1); //�摜�P
	if (colorImg1.empty()) return -1;

	std::string filename2 = argv[2];
	if (filename2.empty()) return -1;
	cv::Mat colorImg2 = cv::imread(filename2, 1); //�摜�Q
	if (colorImg2.empty()) return -1;
    
    cv::Mat grayImg1, grayImg2;
    cv::cvtColor(colorImg1, grayImg1, CV_BGR2GRAY);
    cv::normalize(grayImg1, grayImg1, 0, 255, cv::NORM_MINMAX);
    cv::cvtColor(colorImg2, grayImg2, CV_BGR2GRAY);
    cv::normalize(grayImg2, grayImg2, 0, 255, cv::NORM_MINMAX);
    
    //SIFT
    cv::SiftFeatureDetector detector;
    cv::SiftDescriptorExtractor extractor;    
    //SURF
    //cv::SurfFeatureDetector detector(1000);
    //cv::SurfDescriptorExtractor extractor;
    
    //�摜��������_�����o
    std::vector<cv::KeyPoint> keypoints1;
    detector.detect(grayImg1, keypoints1);
    std::vector<cv::KeyPoint> keypoints2;
    detector.detect(grayImg2, keypoints2);
    
    //�摜�̓����_�ɂ���������ʂ𒊏o
    cv::Mat descriptors1;    
    extractor.compute(grayImg1, keypoints1, descriptors1);    
    cv::Mat descriptors2;    
    extractor.compute(grayImg2, keypoints2, descriptors2);  
    
    //�����_�̑Ή��t��
    std::vector<cv::DMatch> matches;
    cv::BruteForceMatcher<cv::L2<float> > matcher;
    matcher.match(descriptors1, descriptors2, matches);
    
    //�\�[�g����n�Ԗڂ܂ł̑Ή�����\��
    int N=50;
    nth_element(matches.begin(), matches.begin()+N-1, matches.end());
    matches.erase(matches.begin()+N, matches.end());
    
    //�Ή��Â����ꂽ�摜
    cv::Mat matchedImg;
    cv::drawMatches(colorImg1, keypoints1, colorImg2, keypoints2, matches, matchedImg);
    
    cv::namedWindow("image", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::imshow("image", matchedImg);
    
    cv::waitKey(0);
    return 0;
}