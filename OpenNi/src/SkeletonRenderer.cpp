#include "SkeletonRenderer.h"

//--------
void SkeletonRenderer::renderJointLines(){
    ofSetColor(color);
    ofSetLineWidth(lineWidth);
    int numUsers = openNi->getNumUsers();
    for (int user=0; user<numUsers; user++) {
        vector<ofVec2f> joints = openNi->getProjectedJoints(user);
        for (int i=0; i<joints.size(); i++) {
            for (int j=i+1; j<joints.size(); j++) {
                ofPoint p1(joints[i].x * width, joints[i].y * height);
                ofPoint p2(joints[j].x * width, joints[j].y * height);
                for (int k=0; k<iterations; k++) {
                    ofLine(p1.x + ofRandom(-randomOffset, randomOffset),
                           p1.y + ofRandom(-randomOffset, randomOffset),
                           p2.x + ofRandom(-randomOffset, randomOffset),
                           p2.y + ofRandom(-randomOffset, randomOffset));
                }
            }
        }
    }
}

//--------
void SkeletonRenderer::renderOutwardLines(){
    ofSetColor(color);
    ofSetLineWidth(lineWidth);
    vector<ofVec2f> joints = openNi->getProjectedJoints(0);
    for (int i=0; i<numLines; i++) {
        float ang = ofMap(i, 0, numLines, 0, TWO_PI);
        float x = width * 0.5 + radius * cos(ang);
        float y = height * 0.5 + radius * sin(ang);
        ofLine(width * joints[3].x, height * joints[3].y, x, y);
    }
}

//--------
void SkeletonRenderer::setupFluids(){
    // hold previous joints
    for (int i=0; i<maxUsers; i++) {
        vector<ofVec2f> newVec;
        newVec.resize(openNi->getJointNames().size());
        pJoints.push_back(newVec);
    }

    // Initial Allocation
    fluid.allocate(width, height, 0.5);
    
    // Seting the gravity set up & injecting the background image
    fluid.dissipation = 0.99;
    fluid.velocityDissipation = 0.99;
    
    fluid.setGravity(ofVec2f(0.0,0.0));
    //    fluid.setGravity(ofVec2f(0.0,0.0098));
    
    //  Set obstacle
    fluid.begin();
    ofSetColor(0,0);
    ofSetColor(255);
    ofCircle(width*0.5, height*0.35, 40);
    fluid.end();
    fluid.setUseObstacles(false);
    
    // Adding constant forces
    /*
     fluid.addConstantForce(ofPoint(gfx.getWidth()*0.5,
     gfx.getHeight()*0.85),
     ofPoint(0,-2),
     ofFloatColor(0.5,0.1,0.0),
     10.f);
     */
    
    for (int i=0; i<12; i++) {
        float x = ofMap(i+0.5, 0, 12, 0, width);
        //fluid.addConstantForce(ofPoint(x, gfx.getHeight()*0.85),
        //                       ofPoint(0,-6),
        //                       ofFloatColor(0.5,0.1,0.0),
        //                       10.f);
    }
    
    
    
    dissipation = 0.99;
    velDissipation = 0.99;
    gravityX = 0.0;
    gravityY = 0.0;
    displacement = 10;
    strength = 4.8;

}

//--------
void SkeletonRenderer::updateFluids(){
    // Adding temporal Force
    for (int i=0; i<min(maxUsers, openNi->getNumUsers()); i++) {
        vector<ofVec2f> joints = openNi->getProjectedJoints(i);
        for (int j=0; j<joints.size(); j++) {
            joints[j].x *= width;
            joints[j].y *= height;
            /*
             ofVec2f c = ofPoint(640*0.5, 480*0.5) - joints[j];
             c.normalize();
             fluid.addTemporalForce(joints[j],
             joints[j] - pJoints[i][j]*displacement,
             ofFloatColor(c.x, c.y, 0.5)*sin(ofGetElapsedTimef()),
             strength);*/
            
            
            ofPoint m = ofPoint(joints[j].x, joints[j].y);
            ofPoint d = (m - pJoints[i][j])*10.0;
            ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
            c.normalize();
            fluid.addTemporalForce(m,
                                   d,
                                   ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),
                                   strength);
            
        }
        pJoints[i] = joints;
    }
    
    //fluid.setGravity(ofVec2f(gravityX, gravityY));
    fluid.dissipation = dissipation;
    fluid.velocityDissipation = velDissipation;
    
    fluid.update();
}

//--------
void SkeletonRenderer::renderFluids(){
    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_LINEAR);
    fluid.draw();
}

//--------
void SkeletonRenderer::setupControl() {
    control.setName("skeleton render");
    
    control.addParameter("joint lines", &drawJointLines);
    control.addParameter("outward lines", &drawOutwardLines);
    control.addParameter("fluids", &drawFluids);
    
    // joint lines
    control.addParameter("iterations", &iterations, 1, 10);
    control.addParameter("randomOffset", &randomOffset, 0.0f, 100.0f);
    control.addColor("color", &color);
    control.addParameter("lineWidth", &lineWidth, 0.5f, 4.0f);
    control.addParameter("numLines", &numLines, 5, 500);
    control.addParameter("radius", &radius, 0, 800);
    
    // fluids
    control.addParameter("dissipation", &dissipation, 0.0f, 1.0f);
    control.addParameter("vel dissipation", &velDissipation, 0.0f, 1.0f);
    control.addParameter("displacement", &displacement, 0.0f, 100.0f);
    control.addParameter("strength", &strength, 0.0f, 100.0f);
    control.addParameter("gravityX",  &gravityX, -0.02f, 0.02f);
    control.addParameter("gravityY", &gravityY, -0.02f, 0.02f);
}

//-------
void SkeletonRenderer::checkChanges() {
    if (!pDrawFluids && drawFluids) {
        setupFluids();
    }
    if ((pDrawJointLines != drawJointLines) ||
        (pDrawOutwardLines != drawOutwardLines) ||
        (pDrawFluids != drawFluids)) {
        setupControl();
        pDrawOutwardLines = drawOutwardLines;
        pDrawJointLines = drawJointLines;
        pDrawFluids = drawFluids;
    }
}

//-------
void SkeletonRenderer::setGuiVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
}
