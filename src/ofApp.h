#pragma once

#include "ofMain.h"
#include "ofxCv.h"

using namespace ofxCv;
using namespace cv;

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    
	void movePoint(vector<ofVec2f>& points, ofVec2f point);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
    ofImage right, warpedColor;
    
    vector<Point2f> leftPoints;
	vector<ofVec2f> rightPoints;
    
	bool movingPoint;
	ofVec2f* curPoint;
	bool homographyReady;
	
	cv::Mat homography;
};
