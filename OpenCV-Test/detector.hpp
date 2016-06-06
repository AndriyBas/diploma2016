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
#include "base_detector.hpp"

const cv::Size DETECT_DEFAULT_MIN_SIZE = cv::Size(100, 100);

class Detector : public BaseDetector {

public:
    
    // constructor
    Detector(cv::CascadeClassifier classifier, const char* tag);
    
    // methods
    virtual std::vector<cv::Rect> detect(cv::Mat image);
    virtual const char* getTag();
    virtual void setMinSize(cv::Size size);
    virtual void setScaleFactor(int scale);
    virtual void setMinNeighbours(int n);
    

private:
    cv::CascadeClassifier cascadeClassifier;
    const char* tag;
    float scaleFactor = 1.1;
    int minNeighbours = 3;
    int flags = 0|CV_HAAR_SCALE_IMAGE;
    cv::Size minSize = DETECT_DEFAULT_MIN_SIZE;

};

#endif /* detector_hpp */
