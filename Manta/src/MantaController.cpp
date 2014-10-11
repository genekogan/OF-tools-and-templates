#include "MantaController.h"

//-----------
void MantaController::setup(){
    
    // connect to manta
    manta.setup();
    
    
    //control.setVisible(false);
    
    // set up listeners
    /*
     */
    
    /*
     - all pads
     - all velocities
     - some pads
     - some velocities
     - inter finger distances
     - perimeter
     - avg pad (among those > on)
     - total pad
     - num fingers
     - centroid
     - weighted centroid
     
     */
}

//-----------
void MantaController::update(){
    
    vector<ofPoint> fingers;
    float padSum = 0;
    float padAverage = 0;
    int numFingers = 0;
    
    
    float currentValue;
    for (int y=0; y<6; y++) {
        for (int x=0; x<8; x++) {
            currentValue = manta.getPad(y, x);
            padSum += currentValue;
            if (currentValue > 0) {
                fingers.push_back(ofPoint(x, y, currentValue));
                numFingers++;
            }
        }
    }
    
    padAverage = padSum / numFingers;
    
    
    vector<ofPoint> fingerHull = convexHull.getConvexHull(fingers);
    float perimeter = 0;
    float currentDist;
    for (int i=0; i<fingerHull.size(); i++) {
        currentDist = pow(fingerHull[i].x - fingerHull[(i+1)%fingerHull.size()].x, 2)+
                      pow(fingerHull[i].y - fingerHull[(i+1)%fingerHull.size()].y, 2);
        perimeter += currentDist;
    }
    float averageDist = perimeter / (float) fingerHull.size();
    
    
    
    ofPoint centroid, weightedCentroid;
    for (int i=0; i<fingers.size(); i++) {
        centroid += ofPoint(fingers[i].x, fingers[i].y);
        weightedCentroid += (ofPoint(fingers[i].x, fingers[i].y) * fingers[i].z / padSum);
    }
    centroid /= numFingers;
    
    
    cout << "CENTROID " << ofToString(centroid) << endl;
    cout << "CENTROID W " << ofToString(weightedCentroid) << endl;
    cout << "NUM FINGERS " << numFingers << "  SUM " << padSum << endl;
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
void MantaController::setGuiPosition(int x, int y) {
    //control.setGuiPosition(x, y);
}

//----------
void MantaController::close() {
    manta.close();
}

//-----------
MantaController::~MantaController() {
    close();
}
