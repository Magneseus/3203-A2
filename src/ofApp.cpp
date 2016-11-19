#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	ofEnableAntiAliasing();

	ofParameterGroup simParams;
	simParams.setName("Simulation Parameters");

	// Setup the default parameter values
	sensorSpeed.set("Speed", 0.2f, 0.0f, 1.0f);
	sensorNum.set("Number of Sensors", 5, 0, 100);
	oldSensorNum = 5;
	sensorRange.set("Range", 0.1f, 0.0f, 1.0f);

	// Setup event listeners for the panel
	sensorSpeed.disableEvents();
	sensorRange.disableEvents();
	sensorNum.addListener(this, &ofApp::sensorNumChanged);

	// Setup the refresh button for the sim
	ofxButton* refreshSensors = new ofxButton();
	refreshSensors->setup("Refresh");
	refreshSensors->addListener(this, &ofApp::refreshSensorsClicked);
	
	// Add all GUI elements to the main GUI group
	simParams.add(sensorNum);
	simParams.add(sensorRange);
	simParams.add(sensorSpeed);
	gui.add(simParams);
	gui.add(refreshSensors);

	for (int i = 0; i < 10; i++)
	{
		sensors.push_back(ofRandom(0.0f, 1.0f));
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	// Update the time variables
	deltaTime = ofGetLastFrameTime() - timeSinceStart;
	timeSinceStart = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Calculate the size of the range, according to the line
	float _range = (lineInterp(1.0f)).x - (lineInterp(0.0f)).x;
	_range *= sensorRange;

	// Draw all sensors
	for (auto it = sensors.begin(); it != sensors.end(); ++it)
	{
		// Draw the range of the sensors
		ofSetColor(ofColor::indianRed, 100);
		ofDrawEllipse(lineInterp((*it)), _range, _range);
	}
	for (auto it = sensors.begin(); it != sensors.end(); ++it)
	{
		// Draw the point on the line for the sensor
		ofSetColor(ofColor::antiqueWhite, 255);
		ofDrawEllipse(lineInterp((*it)), 10.f, 10.f);
	}

	// Draw the sensor line
	ofSetColor(ofColor::antiqueWhite, 255);
	ofSetLineWidth(3);
	ofDrawLine(lineInterp(0.0f), lineInterp(1.0f));

	gui.draw();
}

// Custom functions

// Returns a point on the visual line, given a value upon that line (0-1)
ofPoint ofApp::lineInterp(float val)
{
	ofPoint retPoint;
	float lineVal = 1.0f / 6.0f;

	retPoint.x = (wWidth * lineVal) + (val * (wWidth - (wWidth * 2.0f * lineVal)));
	retPoint.y = wHeight / 2.0f;

	return retPoint;
}

// Alter the sensors based on the change
void ofApp::sensorNumChanged(int &newSensorNum)
{
	// Add a new sensor
	if (oldSensorNum < newSensorNum)
	{
		sensors.push_back(ofRandom(0.0f, 1.0f));
	}
	// Remove the latest sensor
	else if (oldSensorNum > newSensorNum)
	{
		if (sensors.size() > 0)
			sensors.pop_back();
	}

	oldSensorNum = newSensorNum;
}

// Listener for the Refresh button
void ofApp::refreshSensorsClicked()
{
	std::cout << "test\n";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	mousePos.x = x;
	mousePos.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	wWidth = w;
	wHeight = h;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
