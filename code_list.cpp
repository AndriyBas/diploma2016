//
//  detector.hpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/29/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#ifndef detector_hpp
#define detector_hpp

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

const cv::Size DETECT_DEFAULT_MIN_SIZE = cv::Size(30, 30);

class Detector {

public:
    
    // constructor
    Detector(cv::CascadeClassifier classifier, const char* tag);
    
    // methods
    std::vector<cv::Rect> detect(cv::Mat image);
    const char* getTag();
    void setMinSize(cv::Size size);
    void setScaleFactor(int scale);
    void setMinNeighbours(int n);

private:
    cv::CascadeClassifier cascadeClassifier;
    const char* tag;
    float scaleFactor = 1.1;
    int minNeighbours = 3;
    int flags = 0|CV_HAAR_SCALE_IMAGE;
    cv::Size minSize = DETECT_DEFAULT_MIN_SIZE;

};

#endif /* detector_hpp */




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

typedef std::vector<DetectResult> DetectedResults;

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

class VideoClassifier {
    
    
public:
    void addFrame(DetectedResults detectedResults);
    void getVideoClass();
    
private:
    std::vector<DetectedResults> cachedFrames;
};


#endif /* video_classifier_hpp */


//
//  video_classifier.cpp
//  OpenCV-Test
//
//  Created by Andriy Bas on 5/31/16.
//  Copyright © 2016 Andriy Bas. All rights reserved.
//

#include "video_classifier.hpp"


void VideoClassifier::addFrame(std::vector<DetectResult> detectedResults) {
    
}

void VideoClassifier::getVideoClass() {
    
}


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

#include "detector.hpp"
#include "detect_cascade.hpp"

//using namespace std;
//using namespace cv;

const std::string HAAR_CASCADES_PATH = "/Users/andriybas/Documents/study/OpenCV-Test/haarcascades/";

const std::string FACE_DETECT_CLASSIFIER_PATH = HAAR_CASCADES_PATH + "haarcascade_frontalface_alt_tree.xml";
const std::string PROFILE_FACE_DETECT_CLASSIFIER_PATH = HAAR_CASCADES_PATH + "haarcascade_profileface.xml";
const std::string ELEPHANT_DETECT_CLASSIFIER_PATH =  HAAR_CASCADES_PATH + "elephant_classifier.xml";

const std::string BANANA_DETECT_CLASSIFIER_PATH =  HAAR_CASCADES_PATH + "banana_classifier.xml";




const int SKIP_COUNT = 2;


const cv::Scalar DETECT_FRAME_COLOR = cv::Scalar( 255, 0, 255 );

const cv::Scalar TEXT_COLOR = cv::Scalar( 0, 255, 0 );


const int DETECT_FRAME_THICKNESS = 2;
const cv::LineTypes DETECH_FRAME_LINE_TYPE = cv::LINE_AA;


std::vector<cv::Rect> detectObjects(cv::Mat src, cv::CascadeClassifier classifier);
void drawDetectedFrames(cv::Mat image, std::vector<DetectResult> objects);
void drawTags(cv::Mat image, std::vector<DetectResult> objects);


int main( )
{
    // init input video source
    
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
    Detector faceProfileDetector(profile_face_classifier, "face_profile");
    
    Detector elephantDetector(elephant_classifier, "elephant");
    elephantDetector.setScaleFactor(3);
    elephantDetector.setMinNeighbours(4);
    
    
    Detector bananaDetector(banana_classifier, "banana");
    bananaDetector.setScaleFactor(2);
    bananaDetector.setMinNeighbours(6);
    
    // init cascade
    DetectCascade detectCascade;
//    detectCascade.addDetector(faceDetector);
//    detectCascade.addDetector(faceProfileDetector);
//    detectCascade.addDetector(elephantDetector);
    detectCascade.addDetector(bananaDetector);
    
    DetectedResults detectedObjects;
    cv::Mat frame;
    
    while(true)
    {
        captureInput >> frame;
        
        if (frameCount < SKIP_COUNT) {
            frameCount++;
        } else {
            frameCount = 0;
            detectedObjects = detectCascade.detect(frame);
        }
        
        drawDetectedFrames(frame, detectedObjects);
        
        drawTags(frame, detectedObjects);
        
        imshow("Video classifier", frame );
        // Press 'c' to escape
//        if(waitKey(1) == 'c') break;
    }
    
    cv::waitKey(0);
    return 0;
}

void drawDetectedFrames(cv::Mat image, std::vector<DetectResult> detectedObjects) {
    // Draw circles on the detected faces
    for( int i = 0; i < detectedObjects.size(); i++ )
    {
        
        cv::rectangle(image, detectedObjects[i].rect, DETECT_FRAME_COLOR, DETECT_FRAME_THICKNESS, DETECH_FRAME_LINE_TYPE);
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
        tags.append(detectedObjects[0].tag);
    }
    cv::putText(image, tags.c_str(), cv::Point(10, 20), CV_FONT_HERSHEY_PLAIN, 1.3, TEXT_COLOR, 1);
    if (tags.length() > 0) {
        std::cout << "tags : " << tags << std::endl;
    }
}





