//
//  video_classifier.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/31/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef video_classifier_hpp
#define video_classifier_hpp

#include <stdio.h>
#include "detect_cascade.hpp"
#include "base_video_classifier.hpp"

class VideoClassifier: public BaseClassifier {
    
    
public:
    virtual void addFrame(DetectedResults detectedResults);
    virtual void getVideoClass();
    
private:
    std::vector<DetectedResults> cachedFrames;
};


#endif /* video_classifier_hpp */
