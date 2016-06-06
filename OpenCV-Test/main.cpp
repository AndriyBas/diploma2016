//
//  main.cpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/9/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//



#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>

#include <chrono>

#include "detector.hpp"
#include "detect_cascade.hpp"
#include "video_classifier.hpp"

//using namespace std;
//using namespace cv;

const std::string HAAR_CASCADES_PATH = "/Users/andriybas/Documents/study/OpenCV-Test/haarcascades/";

const std::string FACE_DETECT_CLASSIFIER_PATH = HAAR_CASCADES_PATH + "haarcascade_frontalface_default.xml";
const std::string PROFILE_FACE_DETECT_CLASSIFIER_PATH = HAAR_CASCADES_PATH + "haarcascade_profileface.xml";
const std::string ELEPHANT_DETECT_CLASSIFIER_PATH =  HAAR_CASCADES_PATH + "elephant_classifier.xml";

const std::string BANANA_DETECT_CLASSIFIER_PATH =  HAAR_CASCADES_PATH + "banana_classifier.xml";




const int SKIP_COUNT = 2;


const cv::Scalar DETECT_FRAME_COLOR_MAGENTA = cv::Scalar( 255, 0, 255 );
const cv::Scalar DETECT_FRAME_COLOR_YELLOW = cv::Scalar( 0, 255, 255 );
const cv::Scalar DETECT_FRAME_COLOR_AQUA = cv::Scalar(255, 255, 0  );





const cv::Scalar TEXT_COLOR = cv::Scalar( 0, 255, 0 );
const cv::Scalar TEXT_COLOR_RED = cv::Scalar( 255, 0, 0 );


const int DETECT_FRAME_THICKNESS = 2;
const cv::LineTypes DETECH_FRAME_LINE_TYPE = cv::LINE_AA;


std::vector<cv::Rect> detectObjects(cv::Mat src, cv::CascadeClassifier classifier);
void drawDetectedFrames(cv::Mat image, std::vector<DetectResult> objects);
void drawTags(cv::Mat image, std::vector<DetectResult> objects);
void drawClass(cv::Mat image, std::string videoClass);


int main( )
{
    // init input video source
//    cvCaptureFromFile
    
//    cv::VideoCapture captureInput("/Users/andriybas/Downloads/elephant_wild_life.m4v");
//    cv::VideoCapture captureInput("/Users/andriybas/Documents/test.mov");
    cv::VideoCapture captureInput(0);
    
    if (!captureInput.isOpened()) {
        std::cout << "Could not open input source" << std::endl;
        return -1;
    }
    
    double fps = captureInput.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
    
    std::cout << "Frame per seconds : " << fps << std::endl;
    
    cv::namedWindow("window1", CV_WINDOW_AUTOSIZE);
    
    
    int frameCount = 0;

    // loading classifiers
    cv::CascadeClassifier face_classifier(FACE_DETECT_CLASSIFIER_PATH);
    cv::CascadeClassifier profile_face_classifier(PROFILE_FACE_DETECT_CLASSIFIER_PATH);
    cv::CascadeClassifier elephant_classifier(ELEPHANT_DETECT_CLASSIFIER_PATH);
    cv::CascadeClassifier banana_classifier(BANANA_DETECT_CLASSIFIER_PATH);


    // creating detectors
    Detector faceDetector(face_classifier, "face");
    faceDetector.setScaleFactor(2);
    
//    Detector faceProfileDetector(profile_face_classifier, "face_profile");
    
    Detector elephantDetector(elephant_classifier, "elephant");
    elephantDetector.setScaleFactor(3);
    elephantDetector.setMinNeighbours(4);
    
    
    Detector bananaDetector(banana_classifier, "banana");
    bananaDetector.setScaleFactor(2);
    bananaDetector.setMinNeighbours(6);
    
    // init cascade
    DetectCascade detectCascade;
    detectCascade.addDetector(faceDetector);
//    detectCascade.addDetector(faceProfileDetector);
    detectCascade.addDetector(elephantDetector);
    detectCascade.addDetector(bananaDetector);
    
    VideoClassifier videoClassifier;
    
    DetectedResults detectedObjects;
    cv::Mat frame;
    
    long totalTime = 0;
    int detectedFrames = 0;
    
    while(true)
    {
        captureInput >> frame;
        
        if (frameCount < SKIP_COUNT) {
            frameCount++;
        } else {
            frameCount = 0;
            
            
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            
            
            detectedObjects = detectCascade.detect(frame);
            
            
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
            
//            std::cout << duration << std::endl;
            
            totalTime += duration;
            detectedFrames++;
            
            videoClassifier.addFrame(detectedObjects);
        }
        
        drawDetectedFrames(frame, detectedObjects);
        
        drawTags(frame, detectedObjects);
        
        std::string videoClass = videoClassifier.getVideoClass();
        
        drawClass(frame, videoClass);
        
        imshow("Video classifier", frame );
    
        if (detectedFrames > 100) {
            std::cout << "Average frame detect: " << 1.0 * totalTime / detectedFrames << "\n";
            
            detectedFrames = 0;
            totalTime = 0;
        }
        
        // Press 'c' to escape
//        if(waitKey(1) == 'c') break;
    }
    
    cv::waitKey(0);
    return 0;
}

cv::Scalar selectColor(const char* tag) {
    if (strcmp(tag, "elephant") == 0) {
//        printf("selected AQUA\n");
        return DETECT_FRAME_COLOR_AQUA;
    } else if (strcmp(tag, "banana") == 0) {
//        printf("selected YELLOW\n");
        return DETECT_FRAME_COLOR_YELLOW;
    } else {
//        printf("selected MAGENTA\n");
        return DETECT_FRAME_COLOR_MAGENTA;
    }
}

void drawDetectedFrames(cv::Mat image, std::vector<DetectResult> detectedObjects) {
    // Draw circles on the detected faces
    for( int i = 0; i < detectedObjects.size(); i++ )
    {
        const char* tag = detectedObjects[i].tag;
        cv::Rect rect = detectedObjects[i].rect;
//        printf("%s, %d, %d\n", tag, rect.width, rect.height);
        cv::rectangle(image, detectedObjects[i].rect, selectColor(tag), DETECT_FRAME_THICKNESS, DETECH_FRAME_LINE_TYPE);
//        cv::Point center( faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5 );
//        cv::ellipse(image, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, DETECT_COLOR, 4, 8, 0 );
    }
}


void drawTags(cv::Mat image, std::vector<DetectResult> detectedObjects) {
    
    std::string tags = "";
    for (int i = 0; i < detectedObjects.size(); i++) {
        if (tags.length() > 0) {
            tags.append(", ");
        }
        tags.append(detectedObjects[i].tag);
    }
    tags.insert(0, "tags: ");
    cv::putText(image, tags.c_str(), cv::Point(10, 20), CV_FONT_HERSHEY_PLAIN, 1.3, TEXT_COLOR, 2);
//    if (tags.length() > 0) {
//        std::cout << "tags : " << tags << std::endl;
//    }
}

void drawClass(cv::Mat image, std::string videoClass) {
    
    std::string txt = "class: " + videoClass;
    cv::putText(image, txt.c_str(), cv::Point(10, 45), CV_FONT_HERSHEY_PLAIN, 1.5, TEXT_COLOR_RED, 2);
    
}
