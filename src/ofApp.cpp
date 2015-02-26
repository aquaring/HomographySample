#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	
    // Camera
    camera.setDeviceID(0); //0:iSight 1:WebCamera
    camera.initGrabber(ofGetWidth()/2, ofGetHeight());
	
	movingPoint = false;
	homographyReady = false;
    
    warpedPoints.push_back(Point2f(0, 0));
    warpedPoints.push_back(Point2f(ofGetWidth()/2, 0));
    warpedPoints.push_back(Point2f(ofGetWidth()/2, ofGetHeight()));
    warpedPoints.push_back(Point2f(0, ofGetHeight()));
    
    originalPoints.push_back(ofVec2f(ofGetWidth()/2+100, 100));
    originalPoints.push_back(ofVec2f(ofGetWidth()-100, 100));
    originalPoints.push_back(ofVec2f(ofGetWidth()-100, ofGetHeight()-100));
    originalPoints.push_back(ofVec2f(ofGetWidth()/2+100, ofGetHeight()-100));
}

void ofApp::update() {
    camera.update();
    if(camera.isFrameNew()) {
        originalImage.setFromPixels(camera.getPixelsRef());
        imitate(warpedImage, originalImage);
        
        vector<Point2f> srcPoints;
        for(int i = 0; i < 4; i++) {
            srcPoints.push_back(Point2f(originalPoints[i].x - originalImage.getWidth(), originalPoints[i].y));
        }
        
        // generate a homography from the two sets of points
        homography = findHomography(Mat(srcPoints), Mat(warpedPoints));
        homographyReady = true;
        
        if(homographyReady) {
            warpPerspective(originalImage, warpedImage, homography, CV_INTER_LINEAR);
            warpedImage.update();
        }
    }
}

void ofApp::draw() {
	ofSetColor(255);
	originalImage.draw(ofGetWidth()/2, 0);
	if(homographyReady) {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255, 128);
		warpedImage.draw(0, 0);
		ofDisableBlendMode();
	}
    
    ofSetColor(ofColor::red);
    ofNoFill();
    for(int i = 0; i < 4; i++) {
        ofCircle(originalPoints[i], 10);
        ofCircle(originalPoints[i], 1);
    }
    
    ofSetColor(ofColor::blue);
    ofLine(originalPoints[0], originalPoints[1]);
    ofLine(originalPoints[1], originalPoints[2]);
    ofLine(originalPoints[2], originalPoints[3]);
    ofLine(originalPoints[3], originalPoints[0]);
}

void ofApp::movePoint(vector<ofVec2f>& points, ofVec2f point) {
	for(int i = 0; i < 4; i++) {
		if(points[i].distance(point) < 20) {
			movingPoint = true;
			curPoint = &points[i];
		}
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	ofVec2f cur(x, y);
	ofVec2f originalImageOffset(ofGetWidth()/2, 0);
    movePoint(originalPoints, cur);
}

void ofApp::mouseDragged(int x, int y, int button) {
	if(movingPoint) {
		curPoint->set(x, y);
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	movingPoint = false;
}