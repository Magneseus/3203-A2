#pragma once

#include "ofxGui.h"
#include "ofMain.h"
#include <vector>

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// Called when the sensor num value is changed (to alter the number of sensors)
	void sensorNumChanged(int &newSensorNum);

private:
	ofPoint mousePos;
	ofxPanel gui;

	int wWidth = 800;
	int wHeight = 600;

	float timeSinceStart;
	float deltaTime;

	// Variables for the line
	std::vector<float> sensors;
	ofParameter<int> sensorNum;
	int oldSensorNum;
	ofParameter<float> sensorRange;
	ofParameter<float> sensorSpeed;

	// Return the correct visual point on the line given a value between 0-1
	ofPoint lineInterp(float val);

};
