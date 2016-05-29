//
//  detector.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/29/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef detector_hpp
#define detector_hpp

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

const cv::Size DETECT_DEFAULT_MIN_SIZE = cv::Size(30, 30);

class Detector {

public:
    
    // constructor
    Detector(cv::CascadeClassifier classifier, const char* tag);
    
    // methods
    std::vector<cv::Rect> detect(cv::Mat image);
    const char* getTag();
    void setMinSize(cv::Size size);

private:
    cv::CascadeClassifier cascadeClassifier;
    const char* tag;
    float scaleFactor = 1.1;
    int minNeighbours = 3;
    int flags = 0|CV_HAAR_SCALE_IMAGE;
    cv::Size minSize = DETECT_DEFAULT_MIN_SIZE;

};

#endif /* detector_hpp */
