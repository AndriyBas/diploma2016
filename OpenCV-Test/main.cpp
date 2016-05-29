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
    cv::namedWindow("window1", 1 );
    
    
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
    
    Detector bananaDetector(banana_classifier, "banana");
    bananaDetector.setScaleFactor(2);
    bananaDetector.setMinNeighbours(6);
    
    // init cascade
    DetectCascade detectCascade;
//    detectCascade.addDetector(faceDetector);
//    detectCascade.addDetector(faceProfileDetector);
//    detectCascade.addDetector(elephantDetector);
    detectCascade.addDetector(bananaDetector);
    
    std::vector<DetectResult> detectedObjects;
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
}


//
//#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//
//#include <iostream>
//#include <stdio.h>
//
//using namespace std;
//using namespace cv;
//
///** Function Headers */
//void detectAndDisplay( Mat frame );
//
///** Global variables */
////String face_cascade_name = "/Users/andriybas/Dev/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt2.xml";
//String face_cascade_name = "/Users/andriybas/Dev/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt2.xml";
//String eyes_cascade_name = "/Users/andriybas/Documents/study/OpenCV-Test/haarcascade_eye_tree_eyeglasses.xml";
//
//CascadeClassifier face_cascade;
//CascadeClassifier eyes_cascade;
//String window_name = "Capture - Face detection";
//
///** @function main */
//int main( void )
//{
//    VideoCapture capture;
//    Mat frame;
//    
//    //-- 1. Load the cascades
//    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
//    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };
//    
//    //-- 2. Read the video stream
//    capture.open( -1 );
//    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
//    
//    while ( capture.read(frame) )
//    {
//        if( frame.empty() )
//        {
//            printf(" --(!) No captured frame -- Break!");
//            break;
//        }
//        
//        //-- 3. Apply the classifier to the frame
//        detectAndDisplay( frame );
//        
//        int c = waitKey(10);
//        if( (char)c == 27 ) { break; } // escape
//    }
//    return 0;
//}
//
///** @function detectAndDisplay */
//void detectAndDisplay( Mat frame )
//{
//    std::vector<Rect> faces;
//    Mat frame_gray;
//    
//    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
//    equalizeHist( frame_gray, frame_gray );
//    
//    //-- Detect faces
//    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
//    
//    for ( size_t i = 0; i < faces.size(); i++ )
//    {
//        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
//        
//        Mat faceROI = frame_gray( faces[i] );
//        std::vector<Rect> eyes;
//        
//        //-- In each face, detect eyes
//        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
//        
//        for ( size_t j = 0; j < eyes.size(); j++ )
//        {
//            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
//            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
//        }
//    }
//    //-- Show what you got
//    imshow( window_name, frame );
//}
//




//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//
//using namespace cv;
//
//int main(int argc, const char * argv[]) {
//    
//    
//    VideoCapture cap(0);
//    
//    while (true) {
//        Mat webcam;
//        cap.read(webcam);
//        imshow("Webcam", webcam);
//    }
//    
//    
//    // insert code here...
//    std::cout << "Hello, World!\n" << CV_VERSION << std::endl;
//    return 0;
//}
