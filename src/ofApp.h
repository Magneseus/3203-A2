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

	/* Event Listeners */
	// Called when the sensor num value is changed (to alter the number of sensors)
	void sensorNumChanged(int &newSensorNum);
	// Called when the range of the sensors is changed
	void sensorRangeChanged(float &newSensorRange);

	// Refreshes the simulation
	void refreshSensors();
	// Completely resets the simulation, new set of sensors
	void resetSensors();

private:
	ofPoint mousePos;
	ofxGuiGroup gui;

	// Current window width and height
	int wWidth = 800;
	int wHeight = 600;

	// Time values
	float timeSinceStart;
	float deltaTime;


	/* Variables for the line */
	
		// The current set of sensor positions
		std::vector<float> sensors;
		// The set of sensor positions from the last time a reset was called
		std::vector<float> initialSensors;

		// Current & old number of sensors
		ofParameter<int> sensorNum;
		int oldSensorNum;
	
		// The range of each individual sensor (0.0 - 1.0)
		ofParameter<float> sensorRange;
		float oldSensorRange;

		// The speed at which the simulation will be run
		ofParameter<float> sensorSpeed;

		// Return the correct visual point on the line given a value between 0-1
		ofPoint lineInterp(float val);

};
