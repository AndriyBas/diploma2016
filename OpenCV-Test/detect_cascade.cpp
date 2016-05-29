//
//  detect_cascade.cpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/29/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#include "detect_cascade.hpp"

// constructor
DetectCascade::DetectCascade() {
    // empty
}


// methods
std::vector<DetectResult> DetectCascade::detect(cv::Mat image) {
    
    std::vector<DetectResult> result;
    
    for (int i = 0; i < detectors.size(); i++) {
        
        // detect all objects
        std::vector<cv::Rect> rects = detectors[i].detect(image);
        
        // save result
        for (int j = 0; j < rects.size(); j++) {
            result.push_back(DetectResult(detectors[i].getTag(), rects[j]));
        }
    }
    
    return result;
}

void DetectCascade::addDetector(Detector detector) {
    detectors.push_back(detector);
}
