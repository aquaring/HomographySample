#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	right.loadImage("right.jpg");
	imitate(warpedColor, right);
	
	movingPoint = false;
	homographyReady = false;
    
    leftPoints.push_back(Point2f(0, 0));
    leftPoints.push_back(Point2f(ofGetWidth()/2, 0));
    leftPoints.push_back(Point2f(ofGetWidth()/2, ofGetHeight()));
    leftPoints.push_back(Point2f(0, ofGetHeight()));
    
    rightPoints.push_back(ofVec2f(ofGetWidth()/2+100, 100));
    rightPoints.push_back(ofVec2f(ofGetWidth()-100, 100));
    rightPoints.push_back(ofVec2f(ofGetWidth()-100, ofGetHeight()-100));
    rightPoints.push_back(ofVec2f(ofGetWidth()/2+100, ofGetHeight()-100));
}

void ofApp::update() {
    vector<Point2f> srcPoints;
    for(int i = 0; i < 4; i++) {
        srcPoints.push_back(Point2f(rightPoints[i].x - right.getWidth(), rightPoints[i].y));
    }
    
    // generate a homography from the two sets of points
    homography = findHomography(Mat(srcPoints), Mat(leftPoints));
    homographyReady = true;
	
	if(homographyReady) {
		warpPerspective(right, warpedColor, homography, CV_INTER_LINEAR);
		warpedColor.update();
	}
}

void ofApp::draw() {
	ofSetColor(255);
	right.draw(ofGetWidth()/2, 0);
	if(homographyReady) {
		ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		ofSetColor(255, 128);
		warpedColor.draw(0, 0);
		ofDisableBlendMode();
	}
    
    ofSetColor(ofColor::red);
    ofNoFill();
    for(int i = 0; i < 4; i++) {
        ofCircle(rightPoints[i], 10);
        ofCircle(rightPoints[i], 1);
    }
    
    ofSetColor(ofColor::blue);
    ofLine(rightPoints[0], rightPoints[1]);
    ofLine(rightPoints[1], rightPoints[2]);
    ofLine(rightPoints[2], rightPoints[3]);
    ofLine(rightPoints[3], rightPoints[0]);
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
	ofVec2f rightOffset(ofGetWidth()/2, 0);
    movePoint(rightPoints, cur);
}

void ofApp::mouseDragged(int x, int y, int button) {
	if(movingPoint) {
		curPoint->set(x, y);
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	movingPoint = false;
}