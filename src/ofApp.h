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
    
    ofVideoGrabber camera;
	
    ofImage originalImage, warpedImage;
    
    vector<Point2f> warpedPoints;
	vector<ofVec2f> originalPoints;
    
	bool movingPoint;
	ofVec2f* curPoint;
	bool homographyReady;
	
	cv::Mat homography;
};
