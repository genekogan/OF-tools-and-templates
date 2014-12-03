#include "MantaController.h"


//-----------
void MantaController::setup(){
    isConnected = manta.setup();    
    setMouseActive(true);
    velocityLerpRate = 0.01;
    setVisible(true);
}

//-----------
void MantaController::setMouseActive(bool mouseActive) {
    this->mouseActive = mouseActive;
    if (mouseActive) {
        ofAddListener(ofEvents().mousePressed, this, &MantaController::mousePressed);
    }
    else {
        ofRemoveListener(ofEvents().mousePressed, this, &MantaController::mousePressed);
    }
}

//-----------
void MantaController::update(){
    if (!isConnected)  return;
    
    // pad velocities
    int idx = 0;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            padVelocity[row][col] = ofLerp(padVelocity[row][col], manta.getPad(row, col) - prevPad[row][col], 0.1);
            prevPad[row][col] = manta.getPad(row, col);
            idx++;            
        }
    }
    
    // slider velocities
//    cout << "SV 1 : "<< sliderVelocity[0] << endl;
//    cout << "PV 1 : "<< prevSlider[0] << endl;
//    cout << "MV 1 : "<< manta.getSlider(0) << endl;
    sliderVelocity[0] = ofLerp(sliderVelocity[0], manta.getSlider(0) - prevSlider[0], velocityLerpRate);
    sliderVelocity[1] = ofLerp(sliderVelocity[1], manta.getSlider(1) - prevSlider[1], velocityLerpRate);
    prevSlider[0] = manta.getSlider(0);
    prevSlider[1] = manta.getSlider(1);
//    cout << "SV 2 : "<< sliderVelocity[0] << endl;
//    cout << "PV 2 : "<< prevSlider[0] << endl;
//    cout << "MV 2 : "<< manta.getSlider(0) << endl;
//    cout << "==="<<endl;
    // finger stats
    float _padSum = 0;
    float _padAverage = 0;
    numFingers = 0;
    fingers.clear();
    fingerValues.clear();
    ofPoint fingersMin = ofPoint(1, 1);
    ofPoint fingersMax = ofPoint(0, 0);
    float currentValue;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            currentValue = manta.getPad(row, col);
            if (currentValue > 0) {
                ofPoint fingerPos = getPositionAtPad(row, col);
                fingers.push_back(fingerPos);
                fingerValues.push_back(currentValue);
                numFingers+=1.0;
                _padSum += currentValue;
                if      (fingerPos.x > fingersMax.x)   fingersMax.x = fingerPos.x;
                else if (fingerPos.x < fingersMin.x)   fingersMin.x = fingerPos.x;
                if      (fingerPos.y > fingersMax.y)   fingersMax.y = fingerPos.y;
                else if (fingerPos.y < fingersMin.y)   fingersMin.y = fingerPos.y;
            }
        }
    }
    if (numFingers > 0.0) {
        _padAverage = _padSum / numFingers;
        fingersHull = convexHull.getConvexHull(fingers);
        fingersHullNormalized.resize(fingersHull.size());
        for (int i=0; i<fingersHull.size(); i++) {
            fingersHullNormalized[i].x = (fingersHull[i].x - fingersMin.x) / (fingersMax.x - fingersMin.x);
            fingersHullNormalized[i].y = (fingersHull[i].y - fingersMin.y) / (fingersMax.y - fingersMin.y);
        }
    }
    padSumVelocity = ofLerp(padSumVelocity, _padSum-padSum, velocityLerpRate);
    padAverageVelocity = ofLerp(padAverageVelocity, _padAverage-padAverage, velocityLerpRate);
    padSum = _padSum;
    padAverage = _padAverage;
    
    float _perimeter = 0;
    for (int i=0; i<fingersHull.size(); i++) {
        _perimeter += pow(fingersHull[i].x - fingersHull[(i+1)%fingersHull.size()].x, 2)+
                      pow(fingersHull[i].y - fingersHull[(i+1)%fingersHull.size()].y, 2);
    }
    perimeterVelocity = ofLerp(perimeterVelocity, _perimeter-perimeter, velocityLerpRate);
    perimeter = _perimeter;
    
    float _averageInterFingerDistance = (fingersHull.size()==0) ? 0 : perimeter / (float) fingersHull.size();
    averageInterFingerDistanceVelocity = ofLerp(averageInterFingerDistanceVelocity, _averageInterFingerDistance-averageInterFingerDistance, velocityLerpRate);
    averageInterFingerDistance = _averageInterFingerDistance;
    
    ofPoint centroid, weightedCentroid;
    for (int i=0; i<fingers.size(); i++) {
        centroid += fingers[i];
        weightedCentroid += (fingers[i] * fingerValues[i] / padSum);
    }
    centroid /= numFingers;
    
    centroidVelocityX = ofLerp(centroidVelocityX, centroidX-centroid.x, velocityLerpRate);
    centroidVelocityY = ofLerp(centroidVelocityY, centroidY-centroid.y, velocityLerpRate);
    weightedCentroidVelocityX = ofLerp(weightedCentroidVelocityX, weightedCentroidVelocityX-weightedCentroid.x, velocityLerpRate);
    weightedCentroidVelocityY = ofLerp(weightedCentroidVelocityY, weightedCentroidVelocityY-weightedCentroid.y, velocityLerpRate);
    
    centroidX = centroid.x;
    centroidY = centroid.y;
    weightedCentroidX = weightedCentroid.x;
    weightedCentroidY = weightedCentroid.y;
}

//-----------
void MantaController::markPad(int row, int col, bool mark) {
    manta.setLedManual(true);
    manta.setPadLedState(row, col, mark ? Manta::Red : Manta::Off);
    manta.setLedManual(false);
}

//-----------
void MantaController::markSlider(int index, int column) {
    manta.setLedManual(true);
    manta.setSliderLedState(index, Manta::Amber, column);
    manta.setLedManual(false);
}

//-----------
void MantaController::markButton(int index, bool mark) {
    manta.setLedManual(true);
    manta.setButtonLedState(index, mark ? Manta::Red : Manta::Off);
    manta.setLedManual(false);
}

//-----------
void MantaController::draw(int x, int y, int width){
    if (!visible)   return;
    this->x = x;
    this->y = y;
    this->width = width;
    manta.draw(x, y, width);
    if (px != x || py != y || pwidth != width) {
        px = x;
        py = y;
        pwidth = width;
        if (mouseActive) {
            setMouseResponders();
        }
    }
}

//-----------
void MantaController::drawStats(int x, int y, int w){
    int h = w * 310.0 / 400.0;
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(x, y);
    
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, w, h);
    
    // draw convex hull
    ofNoFill();
    ofSetColor(0, 255, 0);
    ofSetLineWidth(1);
    ofBeginShape();
    for (int i=0; i<fingersHull.size(); i++) {
        float x = ofMap(fingersHull[i].x, 0, 1, 0, w);
        float y = ofMap(fingersHull[i].y, 0, 1, 0, h);
        ofVertex(x, y);
    }
    ofEndShape();

    // draw fingers
    ofFill();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(0);
    for (int i=0; i<fingers.size(); i++) {
        float x = ofMap(fingers[i].x, 0, 1, 0, w);
        float y = ofMap(fingers[i].y, 0, 1, 0, h);
        float r = ofMap(fingerValues[i], 0, 196, 0, 10);
        ofCircle(x, y, r);
    }

    // draw centroids
    float cx = ofMap(centroidX, 0, 1, 0, w);
    float cy = ofMap(centroidY, 0, 1, 0, h);
    float wcx = ofMap(weightedCentroidX, 0, 1, 0, w);
    float wcy = ofMap(weightedCentroidY, 0, 1, 0, h);
    ofNoFill();
    ofSetColor(150);
    ofSetLineWidth(2);
    ofLine(cx-4, cy-4, cx+4, cy+4);
    ofLine(cx+4, cy-4, cx-4, cy+4);
    ofSetColor(255);
    ofLine(wcx-4, wcy-4, wcx+4, wcy+4);
    ofLine(wcx+4, wcy-4, wcx-4, wcy+4);

    // draw stats
    ofSetColor(255);
    ofDrawBitmapString("pad sum", 3, 12);
    ofDrawBitmapString("pad avg", 3, 28);
    ofDrawBitmapString("perimeter", 3, 44);
    ofDrawBitmapString("bw fingers", 3, 60);
    ofSetColor(0, 255, 0);
    ofSetLineWidth(0);
    ofFill();
    ofRect(75,  1, ofClamp(ofMap(padSum, 0, 1024, 0, w-80), 0, w-80), 14);
    ofRect(75, 17, ofClamp(ofMap(padAverage, 0, 196, 0, w-80), 0, w-80), 14);
    ofRect(75, 33, ofClamp(ofMap(perimeter, 0, 2, 0, w-80), 0, w-80), 14);
    ofRect(75, 49, ofClamp(ofMap(averageInterFingerDistance, 0, 1, 0, w-80), 0, w-80), 14);
    
    ofPopMatrix();
    ofPopStyle();
}

//----------
ofPoint MantaController::getPositionAtPad(int row, int col) {
    if (row % 2 != 0) {
        return ofPoint(ofMap(col+0.5, 0, 9, 0, 1),
                       ofMap(row + 0.5, 0, 7, 1, 0));
    }
    else {
        return ofPoint(ofMap(col, 0, 9, 0, 1),
                       ofMap(row + 0.5, 0, 7, 1, 0));
    }
}

//----------
void MantaController::mousePressed(ofMouseEventArgs &evt) {
    if (!mouseActive)   return;
    for (int i=0; i<2; i++) {
        if (sliderPositions[i].inside(evt.x, evt.y)) {
            ofNotifyEvent(sliderClickEvent, i, this);
            return;
        }
    }
    for (int i=0; i<4; i++) {
        if (buttonPositions[i].inside(evt.x, evt.y)) {
            ofNotifyEvent(buttonClickEvent, i, this);
            return;
        }
    }
    for (int i=0; i<48; i++) {
        if (padPositions[i].inside(evt.x, evt.y)) {
            ofNotifyEvent(padClickEvent, i, this);
            return;
        }
    }
}

//----------
void MantaController::setMouseResponders() {
    float sliderSizeX = 0.65*manta.getDrawWidth();
    float sliderSizeY = 0.05*manta.getDrawHeight();
    float buttonSize = 1.5*0.02*manta.getDrawWidth();

    ofRectangle sliderPosition0(x + 0.08*manta.getDrawWidth(), y + 0.05*manta.getDrawHeight(), sliderSizeX, sliderSizeY);
    ofRectangle sliderPosition1(x + 0.05*manta.getDrawWidth(), y + 0.13*manta.getDrawHeight(), sliderSizeX, sliderSizeY);
    ofRectangle buttonPosition0(x + 0.8*manta.getDrawWidth() - 0.5*buttonSize, y + 0.075*manta.getDrawHeight() - 0.5*buttonSize, buttonSize, buttonSize);
    ofRectangle buttonPosition1(x + 0.9*manta.getDrawWidth() - 0.5*buttonSize, y + 0.075*manta.getDrawHeight() - 0.5*buttonSize, buttonSize, buttonSize);
    ofRectangle buttonPosition2(x + 0.85*manta.getDrawWidth() - 0.5*buttonSize, y + 0.155*manta.getDrawHeight() - 0.5*buttonSize, buttonSize, buttonSize);
    ofRectangle buttonPosition3(x + 0.95*manta.getDrawWidth() - 0.5*buttonSize, y + 0.155*manta.getDrawHeight() - 0.5*buttonSize, buttonSize, buttonSize);
    
    sliderPositions[0] = sliderPosition0;
    sliderPositions[1] = sliderPosition1;
    buttonPositions[0] = buttonPosition0;
    buttonPositions[1] = buttonPosition1;
    buttonPositions[2] = buttonPosition2;
    buttonPositions[3] = buttonPosition3;
    
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            int rx = x + ofMap(col+0.5, 0, 8, 0.01*manta.getDrawWidth(), 0.94*manta.getDrawWidth());
            int ry = y + ofMap(row+0.5, 6, 0, 0.24*manta.getDrawHeight(), 0.97*manta.getDrawHeight());
            int size = manta.getDrawWidth() / 11.0;
            if (row %2 != 0)  rx += 0.93*manta.getDrawWidth()/16.0;
            ofRectangle padPosition(rx - 0.5*size, ry - 0.5*size, size, size);
            padPositions[row * 8 + col] = padPosition;
        }
    }
}

//----------
void MantaController::close() {
    if (isConnected) {
        manta.close();
        isConnected = false;
    }
}

//----------
void MantaController::setVisible(bool visible) {
    this->visible = visible;
    setMouseActive(visible);
}

//-----------
MantaController::~MantaController() {
    close();
}
