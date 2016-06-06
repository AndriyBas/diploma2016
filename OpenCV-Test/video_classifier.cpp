//
//  video_classifier.cpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/31/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#include "video_classifier.hpp"

#include <set>

const int CACHE_SIZE = 7;

void VideoClassifier::addFrame(std::vector<DetectResult> detectedResults) {
    cachedFrames.push_back(detectedResults);
    if (cachedFrames.size() > CACHE_SIZE) {
        cachedFrames.erase(cachedFrames.begin());
    }
}

std::string VideoClassifier::getVideoClass() {

    std::set<std::string> foundSet;
    
    for (int i = 0; i < cachedFrames.size(); i++) {
        
        DetectedResults res = cachedFrames[i];
        
        for (int j = 0; j < res.size(); j++) {
            DetectResult dr = res[j];
            
            foundSet.insert(std::string(dr.tag));
        }
    }
    
    bool b = foundSet.find("banana") != foundSet.end();
    bool e = foundSet.find("elephant") != foundSet.end();
    bool f = foundSet.find("face") != foundSet.end();
    
    if (b && e && f) {
        return "zoo";
    } else if (b && e) {
        return "wild life";
    }
    
    return "-";
}
