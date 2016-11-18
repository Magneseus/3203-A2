#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	ofEnableAntiAliasing();

	// Setup sensor values
	sensorSpeed.set("Speed", 0.2f, 0.0f, 1.0f);
	sensorNum.set("Number of Sensors", 5, 0, 100);
	sensorRange.set("Range", 0.1f, 0.0f, 1.0f);

	// Add all the mutable values to the gui
	gui.add(sensorNum);
	gui.add(sensorRange);
	gui.add(sensorSpeed);
}

//--------------------------------------------------------------
void ofApp::update() {
	// Update the time variables
	deltaTime = ofGetLastFrameTime() - timeSinceStart;
	timeSinceStart = ofGetElapsedTimef();


}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(ofColor::indianRed, 255);

	gui.draw();
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
