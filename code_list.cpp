//
//  detector.cpp

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

//  base_detector.hpp
#ifndef base_detector_hpp
#define base_detector_hpp

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

//  base_video_classifier.hpp

#ifndef base_video_classifier_hpp
#define base_video_classifier_hpp

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
    virtual std::string getVideoClass() = 0;
};

#endif /* base_video_classifier_hpp */

//
//  video_classifier.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/31/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef video_classifier_hpp
#define video_classifier_hpp

class VideoClassifier: public BaseClassifier {
    
public:
    virtual void addFrame(DetectedResults detectedResults);
    virtual std::string getVideoClass();
    
private:
    std::vector<DetectedResults> cachedFrames;
};


#endif /* video_classifier_hpp */

//  video_classifier.cpp

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

//
//  detect_cascade.hpp

#ifndef detect_cascade_hpp
#define detect_cascade_hpp

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

//  detect_cascade.cpp

#include "detect_cascade.hpp"

DetectCascade::DetectCascade() {
}
DetectedResults DetectCascade::detect(cv::Mat image) {
    
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

//  main.cpp

#include "detector.hpp"
#include "detect_cascade.hpp"
#include "video_classifier.hpp"

//using namespace std;
//using namespace cv;

const std::string HAAR_CASCADES_PATH = "/Documents/study/OpenCV-Test/haarcascades/";
const std::string FACE_DETECT_CLASSIFIER_PATH = HAAR_CASCADES_PATH + "haarcascade_frontalface_default.xml";
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
    }
    
    cv::waitKey(0);
    return 0;
}






