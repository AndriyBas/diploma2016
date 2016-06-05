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
#include "base_video_classifier.hpp"

class DetectCascade {
    
public:
    // constructor
    DetectCascade();
    
    // methods
    void addDetector(Detector detector);
    
    DetectedResults detect(cv::Mat image);
    
private:
    std::vector<Detector> detectors;
};

#endif /* detect_cascade_hpp */
