#include "MantaController.h"


//-----------
void MantaController::setup(){
    isConnected = manta.setup();    
    setMouseActive(true);
    velocityLerpRate = 0.1;
    setVisible(true);
}

//-----------
void MantaController::setMouseActive(bool mouseActive) {
    this->mouseActive = mouseActive;
    if (mouseActive) {
        ofAddListener(ofEvents().mousePressed, this, &MantaController::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &MantaController::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &MantaController::mouseReleased);

        ofAddListener(ofEvents().keyPressed, this, &MantaController::keyPressed);
        ofAddListener(ofEvents().keyReleased, this, &MantaController::keyReleased);
    }
    else {
        ofRemoveListener(ofEvents().mousePressed, this, &MantaController::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &MantaController::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &MantaController::mouseReleased);

        ofRemoveListener(ofEvents().keyPressed, this, &MantaController::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &MantaController::keyReleased);
    }
}

//-----------
void MantaController::update(){
    if (!isConnected) {
        isConnected = manta.getConnected();
        return;
    }
    
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
    for (int i=0; i<2; i++) {
        sliderVelocity[i] = ofLerp(sliderVelocity[i], manta.getSlider(i) - prevSlider[i], velocityLerpRate);
        prevSlider[i] = manta.getSlider(i);
    }
    // button velocities
    for (int i=0; i<4; i++) {
        buttonVelocity[i] = ofLerp(buttonVelocity[i], manta.getButton(i) - prevButton[i], velocityLerpRate);
        prevButton[i] = manta.getButton(i);
    }
    
    // finger stats
    float _padSum = 0;
    float _padAverage = 0;
    float _width = 0;
    float _height = 0;
    float _whRatio = 0;
    float _numPads = 0;
    
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
                _numPads+=1.0;
                _padSum += currentValue;
                if (fingerPos.x > fingersMax.x)   fingersMax.x = fingerPos.x;
                if (fingerPos.x < fingersMin.x)   fingersMin.x = fingerPos.x;
                if (fingerPos.y > fingersMax.y)   fingersMax.y = fingerPos.y;
                if (fingerPos.y < fingersMin.y)   fingersMin.y = fingerPos.y;
            }
        }
    }
    
    _padAverage = fingers.size() > 0 ? _padSum / _numPads : 0.0;
    
    float _perimeter = 0.0;
    float _averageInterFingerDistance = 0.0;

    // stats on finger groups
    if (fingers.size() < 2) {
        _width = 0;
        _height = 0;
        _whRatio = 0;
        _perimeter = 0;
        _averageInterFingerDistance = 0;
        fingersHull.resize(0);
        fingersHullNormalized.resize(0);
    }
    else if (fingers.size() == 2) {
        _width = fingersMax.x - fingersMin.x;
        _height = fingersMax.y - fingersMin.y;
        _whRatio = _width / (1.0 + _height);
        
        _perimeter = (pow(fingers[0].x - fingers[1].x, 2)+
                      pow(fingers[0].y - fingers[1].y, 2));

        _averageInterFingerDistance = _perimeter;
        fingersHull.resize(0);
        fingersHullNormalized.resize(0);
    }
    else {
        _width = fingersMax.x - fingersMin.x;
        _height = fingersMax.y - fingersMin.y;
        _whRatio = _width / _height;
        
        fingersHull = convexHull.getConvexHull(fingers);
        fingersHullNormalized.resize(fingersHull.size());
        for (int i=0; i<fingersHull.size(); i++) {
            fingersHullNormalized[i].x = (fingersHull[i].x - fingersMin.x) / (fingersMax.x - fingersMin.x);
            fingersHullNormalized[i].y = (fingersHull[i].y - fingersMin.y) / (fingersMax.y - fingersMin.y);
        }
        for (int i=0; i<fingersHull.size()-1; i++) {
            _perimeter += (pow(fingersHull[i].x - fingersHull[(i+1)].x, 2)+
                           pow(fingersHull[i].y - fingersHull[(i+1)].y, 2));
        }
        _averageInterFingerDistance = _perimeter / (float) (fingersHull.size()-1);
    }
    
    numPadsVelocity = ofLerp(numPadsVelocity, _numPads-numPads, velocityLerpRate);
    perimeterVelocity = ofLerp(perimeterVelocity, _perimeter-perimeter, velocityLerpRate);
    averageInterFingerDistanceVelocity = ofLerp(averageInterFingerDistanceVelocity, _averageInterFingerDistance-averageInterFingerDistance, velocityLerpRate);

    padSumVelocity = ofLerp(padSumVelocity, _padSum-padSum, velocityLerpRate);
    padAverageVelocity = ofLerp(padAverageVelocity, _padAverage-padAverage, velocityLerpRate);

    widthVelocity = ofLerp(widthVelocity, _width-padWidth, velocityLerpRate);
    heightVelocity = ofLerp(heightVelocity, _height-padHeight, velocityLerpRate);
    whRatioVelocity = ofLerp(whRatioVelocity, _whRatio-whRatio, velocityLerpRate);
    
    padWidth = _width;
    padHeight = _height;
    whRatio = _whRatio;
    perimeter = _perimeter;
    averageInterFingerDistance = _averageInterFingerDistance;
    padSum = _padSum;
    padAverage = _padAverage;
    numPads = _numPads;
    
    // centroid and weighted centroid
    ofPoint _centroid, _weightedCentroid;
    for (int i=0; i<fingers.size(); i++) {
        _centroid += fingers[i];
        _weightedCentroid += (fingers[i] * fingerValues[i] / padSum);
    }
    _centroid /= _numPads;
    
    centroidVelocityX = ofLerp(centroidVelocityX, _centroid.x-centroidX, velocityLerpRate);
    centroidVelocityY = ofLerp(centroidVelocityY, _centroid.y-centroidY, velocityLerpRate);
    weightedCentroidVelocityX = ofLerp(weightedCentroidVelocityX, _weightedCentroid.x-weightedCentroidX, velocityLerpRate);
    weightedCentroidVelocityY = ofLerp(weightedCentroidVelocityY, _weightedCentroid.y-weightedCentroidY, velocityLerpRate);
    
    centroidX = _centroid.x;
    centroidY = _centroid.y;
    weightedCentroidX = _weightedCentroid.x;
    weightedCentroidY = _weightedCentroid.y;    
}

//-----------
void MantaController::setPadSelection(vector<int> idx, int selection) {
    manta.clearPadSelection();
    for (int i=0; i<idx.size(); i++) {
        manta.addPadToSelection(idx[i], selection);
    }
}

//-----------
void MantaController::setSliderSelection(vector<int> idx, int selection) {
    manta.clearSliderSelection();
    for (int i=0; i<idx.size(); i++) {
        manta.addSliderToSelection(idx[i], selection);
    }
}

//-----------
void MantaController::setButtonSelection(vector<int> idx, int selection) {
    manta.clearButtonSelection();
    for (int i=0; i<idx.size(); i++) {
        manta.addButtonToSelection(idx[i], selection);
    }
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
    mainDrawRect = ofRectangle(x, y, width, width * 310.0 / 400.0);
    manta.draw(x, y, width);
    if (mainDrawRect.inside(ofGetMouseX(), ofGetMouseY())) {
        ofPushStyle();
        selection == 0 ? ofSetColor(255, 125, 0) : ofSetColor(0, 255, 125);
        ofDrawBitmapString("Click to select. SHIFT for multiple. 'v' to toggle velocity",
                           x+3, y+mainDrawRect.getHeight() * 0.22);
        ofDrawBitmapString(selection == 0 ? "values" : "velocities", x+width-80, y+13);
        ofPopStyle();
    }
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
int MantaController::getSizeSelection() {
    return manta.getPadSelection(0).size()+manta.getPadSelection(1).size()+
        manta.getSliderSelection(0).size()+manta.getSliderSelection(1).size()+
        manta.getButtonSelection(0).size()+manta.getButtonSelection(1).size();
}

//-----------
void MantaController::drawStats(int x, int y, int w){
    int h = w * 310.0 / 400.0;
    statsDrawRect = ofRectangle(x, y, w, h);
    
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

/*
//----------
void MantaController::mousePressed(ofMouseEventArgs &evt) {
    if (!mouseActive ||
        !mainDrawRect.inside(evt.x, evt.y)) {
        return;
    }
    for (int i=0; i<2; i++) {
        if (sliderPositions[i].inside(evt.x, evt.y)) {
            if (!shift) manta.clearSelection();
            manta.addSliderToSelection(i, selection);
            MantaElement mantaEvt(SLIDER, i, selection);
            ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    for (int i=0; i<4; i++) {
        if (buttonPositions[i].inside(evt.x, evt.y)) {
            if (!shift) manta.clearSelection();
            manta.addButtonToSelection(i, selection);
            MantaElement mantaEvt(BUTTON, i, selection);
            ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    for (int i=0; i<48; i++) {
        if (padPositions[i].inside(evt.x, evt.y)) {
            if (!shift) manta.clearSelection();
            manta.addPadToSelection(i, selection);
            MantaElement mantaEvt(PAD, i, selection);
            ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    manta.clearSelection();
}
*/


//----------
void MantaController::mousePressed(ofMouseEventArgs &evt) {
    dragging = true;
    dragPoint1 = ofPoint(evt.x, evt.y);
    dragPoint2 = dragPoint1;
    manta.clearSelection();
}
//----------
void MantaController::mouseDragged(ofMouseEventArgs &evt) {
    if (!mouseActive || !dragging ||
        !mainDrawRect.inside(evt.x, evt.y))  return;
    
    manta.clearSelection();
    dragPoint2 = ofPoint(evt.x, evt.y);
    ofRectangle rect = ofRectangle(min(dragPoint1.x, dragPoint2.x), min(dragPoint1.y, dragPoint2.y),
                                   abs(dragPoint1.x-dragPoint2.x), abs(dragPoint1.y-dragPoint2.y));
    for (int i=0; i<48; i++) {
        if (rect.inside(padPositions[i].x, padPositions[i].y) ||
            rect.inside(padPositions[i].x+ padPositions[i].width, padPositions[i].y) ||
            rect.inside(padPositions[i].x+ padPositions[i].width, padPositions[i].y+ padPositions[i].height) ||
            rect.inside(padPositions[i].x, padPositions[i].y+ padPositions[i].height)) {
            manta.addPadToSelection(i, selection);
        }
    }
    for (int i=0; i<2; i++) {
        if (rect.inside(sliderPositions[i].x, sliderPositions[i].y) ||
            rect.inside(sliderPositions[i].x+ sliderPositions[i].width, sliderPositions[i].y) ||
            rect.inside(sliderPositions[i].x+ sliderPositions[i].width, sliderPositions[i].y+sliderPositions[i].height) ||
            rect.inside(sliderPositions[i].x, sliderPositions[i].y+ sliderPositions[i].height)) {
            manta.addSliderToSelection(i, selection);
        }
    }
    for (int i=0; i<2; i++) {
        if (rect.inside(buttonPositions[i].x, buttonPositions[i].y) ||
            rect.inside(buttonPositions[i].x+ buttonPositions[i].width, buttonPositions[i].y) ||
            rect.inside(buttonPositions[i].x+ buttonPositions[i].width, buttonPositions[i].y+ buttonPositions[i].height) ||
            rect.inside(buttonPositions[i].x, buttonPositions[i].y+ buttonPositions[i].height)) {
            manta.addButtonToSelection(i, selection);
        }
    }
}

//----------
void MantaController::mouseReleased(ofMouseEventArgs &evt) {
    dragging = false;

    if (!mouseActive ||
        !mainDrawRect.inside(evt.x, evt.y) ||
        ofDist(dragPoint1.x, dragPoint1.y, dragPoint2.x, dragPoint2.y) > 1 ) {
        return;
    }
    
    for (int i=0; i<2; i++) {
        if (sliderPositions[i].inside(evt.x, evt.y)) {
            if (!shift) manta.clearSelection();
            manta.addSliderToSelection(i, selection);
            MantaElement mantaEvt(SLIDER, i, selection);
            ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    for (int i=0; i<4; i++) {
        if (buttonPositions[i].inside(evt.x, evt.y)) {
            if (!shift) manta.clearSelection();
            manta.addButtonToSelection(i, selection);
            MantaElement mantaEvt(BUTTON, i, selection);
            ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    for (int i=0; i<48; i++) {
        if (padPositions[i].inside(evt.x, evt.y)) {
            if (!shift) manta.clearSelection();
            manta.addPadToSelection(i, selection);
            MantaElement mantaEvt(PAD, i, selection);
            ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
}

//----------
void MantaController::keyPressed(ofKeyEventArgs &e) {
    if (e.key == OF_KEY_SHIFT) {
        shift = true;
    }
    else if (e.key=='v') {
        selection = 1 - selection;
        manta.setSelectionView(selection);
    }
}

//----------
void MantaController::keyReleased(ofKeyEventArgs &e) {
    if (e.key == OF_KEY_SHIFT) {
        shift = false;
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
