#include "MantaController.h"

//-----------
void MantaController::setup(){
    isConnected = manta.setup();
}

//-----------
void MantaController::update(){
    if (!isConnected) return;
    
    fingers.clear();
    fingerValues.clear();
    
    padSum = 0;
    padAverage = 0;
    numFingers = 0;
    
    float currentValue;
    for (int row=0; row<6; row++) {
        for (int col=0; col<8; col++) {
            currentValue = manta.getPad(row, col);
            if (currentValue > 0) {
                fingers.push_back(getPositionAtPad(row, col));
                fingerValues.push_back(currentValue);
                numFingers++;
                padSum += currentValue;
            }
        }
    }
    
    if (numFingers > 0) {
        padAverage = padSum / numFingers;
        fingersHull = convexHull.getConvexHull(fingers);
    }
    

    perimeter = 0;
    float currentDist;
    for (int i=0; i<fingersHull.size(); i++) {
        currentDist = pow(fingersHull[i].x - fingersHull[(i+1)%fingersHull.size()].x, 2)+
                      pow(fingersHull[i].y - fingersHull[(i+1)%fingersHull.size()].y, 2);
        perimeter += currentDist;
    }
    if (fingersHull.size() > 0) {
        averageInterFingerDistance = perimeter / (float) fingersHull.size();
    }
    else {
        averageInterFingerDistance = 0;
    }
    
    ofPoint centroid, weightedCentroid;
    for (int i=0; i<fingers.size(); i++) {
        centroid += fingers[i];
        weightedCentroid += (fingers[i] * fingerValues[i] / padSum);
    }
    centroid /= numFingers;
    
    centroidX = centroid.x;
    centroidY = centroid.y;
    weightedCentroidX = weightedCentroid.x;
    weightedCentroidY = weightedCentroid.y;
}

//-----------
float * MantaController::getPadRef(int x, int y){
    return manta.getPadRef(x, y);
}

//-----------
void MantaController::draw(int x, int y, int w){
    manta.draw(x, y, w);
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
        float y = ofMap(fingersHull[i].y, 1, 0, 0, h);
        ofVertex(x, y);
    }
    ofEndShape();

    // draw fingers
    ofFill();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(0);
    for (int i=0; i<fingers.size(); i++) {
        float x = ofMap(fingers[i].x, 0, 1, 0, w);
        float y = ofMap(fingers[i].y, 1, 0, 0, h);
        float r = ofMap(fingerValues[i], 0, 196, 0, 10);
        ofCircle(x, y, r);
    }

    // draw centroids
    float cx = ofMap(centroidX, 0, 1, 0, w);
    float cy = ofMap(centroidY, 1, 0, 0, h);
    float wcx = ofMap(weightedCentroidX, 0, 1, 0, w);
    float wcy = ofMap(weightedCentroidY, 1, 0, 0, h);
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
                       ofMap(row + 0.5, 0, 7, 0, 1));
    }
    else {
        return ofPoint(ofMap(col, 0, 9, 0, 1),
                       ofMap(row + 0.5, 0, 7, 0, 1));
    }
}

//----------
void MantaController::close() {
    if (isConnected)
        manta.close();
}

//-----------
MantaController::~MantaController() {
    close();
}
