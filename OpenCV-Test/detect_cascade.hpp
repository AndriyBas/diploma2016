//
//  detect_cascade.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/29/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef detect_cascade_hpp
#define detect_cascade_hpp

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

#include "detector.hpp"


struct DetectResult {
    const char* tag;
    cv::Rect rect;
    
    DetectResult(const char* tagArg, const cv::Rect objectRect) {
        tag = tagArg;
        rect.width = objectRect.width;
        rect.height = objectRect.height;
        rect.x = objectRect.x;
        rect.y = objectRect.y;
    }
};


class DetectCascade {
    
public:
    // constructor
    DetectCascade();
    
    // methods
    void addDetector(Detector detector);
    
    std::vector<DetectResult> detect(cv::Mat image);
    
private:
    std::vector<Detector> detectors;
};

#endif /* detect_cascade_hpp */
