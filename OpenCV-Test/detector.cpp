//
//  detector.cpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/29/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#include "opencv2/imgproc.hpp"
#include "detector.hpp"

// constructor
Detector::Detector(cv::CascadeClassifier classifier, const char* tagArg) {
    cascadeClassifier = classifier;
    tag = tagArg;
}

// methods
std::vector<cv::Rect> Detector::detect(cv::Mat image) {
    
    // gray image
    cv::Mat frame_gray;
    
    // convert to gray image
    cv::cvtColor( image, frame_gray, cv::COLOR_BGR2GRAY );
    // Equalizes the histogram of a grayscale image.
    cv::equalizeHist( frame_gray, frame_gray );
    
    // Detect objects
    std::vector<cv::Rect> objects;
    // run cascade
    cascadeClassifier.detectMultiScale(
                                       frame_gray, // input image
                                       objects, // result
                                       scaleFactor, // scale factor
                                       minNeighbours, // min neighbours
                                       flags, // flags
                                       minSize);
    
    return objects;
}

const char* Detector::getTag() {
    return tag;
}

void Detector::setMinSize(cv::Size size) {
    minSize = size;
}

void Detector::setScaleFactor(int scale) {
    scaleFactor = scale;
}

void Detector::setMinNeighbours(int n) {
    minNeighbours = n;
}









