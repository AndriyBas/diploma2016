//
//  base_detector.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 6/6/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef base_detector_hpp
#define base_detector_hpp

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <stdio.h>


class BaseDetector {
    
public:
    
    // methods
    virtual std::vector<cv::Rect> detect(cv::Mat image) = 0;
    virtual const char* getTag() = 0;
    virtual void setMinSize(cv::Size size) = 0;
    virtual void setScaleFactor(int scale) = 0;
    virtual void setMinNeighbours(int n) = 0;
    
};

#endif /* base_detector_hpp */
