#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	gui.setup();
	ofEnableAntiAliasing();

	ofParameterGroup simParams;
	simParams.setName("Simulation Parameters");

	// Setup the default parameter values
	sensorSpeed.set("Speed", 0.2f, 0.0f, 100.0f);
	sensorNum.set("Number of Sensors", 5, 0, 100);
	oldSensorNum = sensorNum.get();
	sensorRange.set("Diameter", 0.1f, 0.0f, 1.0f);
	oldSensorRange = sensorRange.get();

	// Setup event listeners for the panel
	sensorSpeed.disableEvents();
	sensorRange.addListener(this, &ofApp::sensorRangeChanged);
	sensorNum.addListener(this, &ofApp::sensorNumChanged);

	// Setup the refresh button for the simulation
	ofxButton* refreshSensorsButton = new ofxButton();
	refreshSensorsButton->setup("Refresh");
	refreshSensorsButton->addListener(this, &ofApp::refreshSensors);
	
	// Setup the reset button for the simulation
	ofxButton* resetSensorsButton = new ofxButton();
	resetSensorsButton->setup("Reset");
	resetSensorsButton->addListener(this, &ofApp::resetSensors);

	// Setup the Run button for the simulation
	ofxButton* runSim = new ofxButton();
	runSim->setup("Run Simulation");
	runSim->addListener(this, &ofApp::toggleRunSim);
	runningSim = false;

	// Setup the Run multiple sums button
	ofxButton* runSimMult = new ofxButton();
	runSimMult->setup("Run Simulation MULTIPLE");
	runSimMult->addListener(this, &ofApp::toggleRunSimMult);

	// Setup the number of sims bar
	numSims.set("Number of Simulations", 20, 1, 100);
	runningSimMult = false;
	

	// Add all GUI elements to the main GUI group
	simParams.add(sensorNum);
	simParams.add(sensorRange);
	simParams.add(sensorSpeed);
	gui.add(simParams);
	gui.add(refreshSensorsButton);
	gui.add(resetSensorsButton);
	gui.add(runSim);
	gui.add(runSimMult);
	gui.add(numSims);

	// Change the background colour
	ofBackground(ofColor::darkSlateGrey);

	// Finally, initialize the first set of random sensors
	resetSensors();
}

//--------------------------------------------------------------
void ofApp::update() {
	// Update the time variables
	deltaTime = ofGetElapsedTimef() - timeSinceStart;
	timeSinceStart = ofGetElapsedTimef();

	// Update the simulation if it's currently running
	if (runningSim)
	{
		checkIfComplete();

		int numSensors = sensors.size();
		if (numSensors * sensorRange.get() >= 1.0f && curScanInd < numSensors)
		{
			/* Rigid coverage */

			// Move the current scan position forwards until it reaches a node
			curScan += sensorSpeed * deltaTime;
			auto iter = sensors.begin();
			std::advance(iter, curScanInd);

			// If we've moved past the sensor, go to that sensor
			if (curScan > *iter)
			{
				// Set the value to the sensor
				curScan = *iter;

				// Check if we have a gap behind us
				float prevSensorRangeExtent = curScanInd > 0 ?
					sensors.at(curScanInd - 1) : 0.0f - sensorRange.get()/2.0f;
				// Or in front of us
				float nextSensorRangeExtent = curScanInd + 1 < sensors.size() ?
					sensors.at(curScanInd + 1) : 1.0f + sensorRange.get()/2.0f;


				// If gap behind, move the sensor backwards
				if (curScan - prevSensorRangeExtent > sensorRange.get())
				{
					(*iter) -= sensorSpeed * deltaTime;

					// If we've moved far enough, stop moving and set the
					// finishedScan marker to here
					if ((*iter) < prevSensorRangeExtent + sensorRange.get())
					{
						(*iter) = prevSensorRangeExtent + sensorRange.get();

						finishedScan = (*iter);
						finishedScanInd = curScanInd;
						curScanInd++;
					}

					sumOfMovements += abs(curScan - *iter);
					curScan = (*iter);
				}
				// If gap ahead, move the sensor forwards
				else if (nextSensorRangeExtent - curScan > sensorRange.get())
				{
					(*iter) += sensorSpeed * deltaTime;

					// If we've moved too far, then switch to the next node
					// and set the finishedScan marker to here
					if ((*iter) > prevSensorRangeExtent + sensorRange.get())
					{
						(*iter) = prevSensorRangeExtent + sensorRange.get();
						finishedScan = (*iter);
						finishedScanInd = curScanInd;
						curScanInd++;
					}

					// If we've moved far enough, stop moving and set the
					// finishedScan marker to here
					if (!scannedOnce && 
						(*iter) > nextSensorRangeExtent - sensorRange.get())
					{
						(*iter) = nextSensorRangeExtent - sensorRange.get();
						finishedScan = (*iter);
						finishedScanInd = curScanInd;
						curScanInd++;
					}

					sumOfMovements += abs(curScan - *iter);
					curScan = (*iter);
				}
				// If we've scanned once through already we need to start caring
				// about overlaps
				else if (scannedOnce)
				{
					(*iter) += sensorSpeed * deltaTime;

					// If we've surpassed a sensor, then switch places
					if (curScanInd < numSensors - 1)
					{
						if ((*iter) > sensors.at(curScanInd + 1))
						{
							std::iter_swap(iter, iter + 1);
							iter = iter + 1;
						}
					}

					// If we've moved too far, then switch to the next node
					// and set the finishedScan marker to here
					if ((*iter) > prevSensorRangeExtent + sensorRange.get())
					{
						(*iter) = prevSensorRangeExtent + sensorRange.get();
						finishedScan = (*iter);
						finishedScanInd = curScanInd;
						curScanInd++;
					}

					sumOfMovements += abs(curScan - *iter);
					curScan = (*iter);
				}
				// Otherwise everything is fine and we can continue onwards
				else
				{
					finishedScan = curScan;
					finishedScanInd = curScanInd;
					curScanInd++;
				}
			}
		}
		// If we've finished one scan, start again
		else if (curScanInd >= numSensors)
		{
			curScanInd = 0;
			scannedOnce = !scannedOnce;

			checkIfComplete();
		}
	} // End of if (runningSim)
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Calculate the size of the range, according to the line
	float _range = (lineInterp(1.0f)).x - (lineInterp(0.0f)).x;
	_range *= sensorRange.get();
	
	//Draw the scan sections
	float y = (wHeight / 2.0f) - _range - (_range / 10.0f);
	float h = (wHeight / 2.0f) + _range + (_range / 10.0f);

	ofSetColor(ofColor::forestGreen, 50);
	drawBoxCorners(0.0f, y, lineInterp(finishedScan).x, h);
	ofSetColor(ofColor::darkSlateBlue, 50);
	drawBoxCorners(lineInterp(finishedScan).x, y, lineInterp(curScan).x, h);
	//ofSetColor(ofColor::darkRed, 50);
	//drawBoxCorners(lineInterp(curScan).x, y, wWidth, h);

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

/* Custom functions */


bool ofApp::checkIfComplete()
{
	if (sensors.size() * sensorRange.get() < 1.0f)
		return false;

	// check distance from beginning of line to first sensor
	if (sensors.at(0) - sensorRange.get() / 2.0f > 0.00001f)
		return false;

	

	// check all sensors in between
	for (int i = 0; i < sensors.size() - 1; ++i)
	{
		if ((sensors[i + 1] - sensors[i]) - sensorRange.get() > 0.00001f)
			return false;
	}

	// Check last sensor
	if ((1.0f - (sensorRange.get() / 2.0f)) - sensors[sensors.size() - 1] > 0.00001f)
		return false;


	// Stop the sim and print out the total movement
	runningSim = false;
	std::cout << "\nTotal Movement: " << sumOfMovements << "\n\n";
		
	// If we're running multiple sims
	if (runningSimMult)
	{
		// Add it to the list of movements
		sumOfMovementsList.push_back(sumOfMovements);

		// If we haven't done the required number of sims, keep going
		if (sumOfMovementsList.size() < numSims.get())
		{
			runningSim = true;
			resetSensors();
		}
		else
		{
			// Otherwise stop the simulations
			runningSim = false;
			runningSimMult = false;

			// Average all movements
			float avgMovement = 0.0f;
			for (int i = 0; i < sumOfMovementsList.size(); ++i)
			{
				avgMovement += sumOfMovementsList[i];
			}
			avgMovement /= sumOfMovementsList.size();

			// Print that to the console
			std::cout << "\n    Average Movement over "
				<< sumOfMovementsList.size()
				<< " sims: "
				<< avgMovement
				<< "\n\n";

			// Clear the list for future sims
			sumOfMovementsList.clear();
		}
	}

	return true;
}

// Returns a point on the visual line, given a value upon that line (0-1)
ofPoint ofApp::lineInterp(float val)
{
	ofPoint retPoint;
	float lineVal = 1.0f / 6.0f;

	retPoint.x = (wWidth * lineVal) + (val * (wWidth - (wWidth * 2.0f * lineVal)));
	retPoint.y = wHeight / 2.0f;

	return retPoint;
}

// Refreshes the simulation by returning all sensors to their original positions
// and starting the algorithm for placement anew
void ofApp::refreshSensors()
{
	// Go through the current number of sensors, copying the old values to the
	// new value
	int i = 0;
	for (auto it = sensors.begin(), it1 = initialSensors.begin(); 
		it != sensors.end() && it1 != initialSensors.end() && i < sensorNum;
		++it, ++it1, ++i)
	{
		(*it) = (*it1);
	}

	// Sort our set of sensors, so that we can use it more efficiently in our
	// algorithm
	std::sort(sensors.begin(), sensors.end());

	curScan = 0.0f;
	curScanInd = 0;
	finishedScan = 0.0f;
	finishedScanInd = 0;
	sumOfMovements = 0.0f;
	scannedOnce = false;
}

// Creates a new set of sensors, and refreshes the simulation
void ofApp::resetSensors()
{
	// Clear all existing sensors
	sensors.clear();

	// Initialize a new set of random sensors
	for (int i = 0; i < sensorNum; i++)
	{
		sensors.push_back(ofRandom(0.0f, 1.0f));
	}

	// New set of initial sensors
	initialSensors = sensors;

	// Finally refresh the simulation
	refreshSensors();
}

// Draws a box to the screen based on corner coordinates
void ofApp::drawBoxCorners(float x1, float y1, float x2, float y2)
{
	float w = (x2 - x1);
	float h = (y2 - y1);
	float x = x1 + (w / 2.0f);
	float y = y1 + (h / 2.0f);

	ofDrawBox(x, y, 0.0f, w, h, 0.0f);
}



/* EVENT LISTENERS */

// Alter the sensor num based on the change in the sensorNum bar
void ofApp::sensorNumChanged(int &newSensorNum)
{
	// Add a new sensor
	if (oldSensorNum < newSensorNum)
	{
		// Add as many sensors as required
		for (int i = 0; i < newSensorNum - oldSensorNum; ++i)
		{
			// New sensor value
			float newSensorValue = ofRandom(0.0f, 1.0f);

			// Add a new sensor to the current sensor list
			sensors.push_back(newSensorValue);
			// Add a new sensor to the initial sensor list
			initialSensors.push_back(newSensorValue);
		}

		// Refresh the simulation as the parameters have changed
		refreshSensors();
	}
	// Remove the latest sensor
	else if (oldSensorNum > newSensorNum)
	{
		// Remove as many sensors as required
		for (int i = 0; i < oldSensorNum - newSensorNum; ++i)
		{
			// Remove the sensor from the current sensor list
			if (sensors.size() > 0)
				sensors.pop_back();
			// Remove the sensor from the initial sensor list
			if (initialSensors.size() > 0)
				initialSensors.pop_back();
		}

		// Refresh the simulation as the parameters have changed
		refreshSensors();
	}

	oldSensorNum = newSensorNum;
}

// Listener for the sensor range bar
void ofApp::sensorRangeChanged(float &newSensorRange)
{
	if (oldSensorRange != newSensorRange)
		refreshSensors();

	oldSensorRange = newSensorRange;
}

// Called when the button for runSim is changed
void ofApp::toggleRunSim()
{
	runningSim = true;
	
	refreshSensors();
}

void ofApp::toggleRunSimMult()
{
	runningSimMult = true;
	runningSim = true;

	refreshSensors();
}



/* More generated functions */
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
