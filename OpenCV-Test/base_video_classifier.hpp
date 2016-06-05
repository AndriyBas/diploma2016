//
//  base_video_classifier.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 6/6/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef base_video_classifier_hpp
#define base_video_classifier_hpp

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

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

typedef std::vector<DetectResult> DetectedResults;

class BaseClassifier {
    
public:
    virtual void addFrame(DetectedResults detectedResults) = 0;
    virtual void getVideoClass() = 0;
};

#endif /* base_video_classifier_hpp */
