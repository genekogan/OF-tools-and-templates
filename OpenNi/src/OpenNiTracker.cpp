#include "OpenNi.h"


//-------
OpenNi::OpenNi() {
    maxUsers = 2;
    minArea = 1000;
    maxArea = 70000;
    threshold = 15;
    persistence = 15;
    maxDistance = 32;
    nearThreshold = 70;
    farThreshold = 0;
    smoothness = 1;
    curved = false;
    smoothingRate = 1;
    normalizeJoints = false;
    
    // modes
    trackingUsers = false;
    trackingContours = false;
    trackingKeypoints = false;
    pTrackingUsers = false;
    pTrackingContours = false;
    pTrackingKeypoints = false;
    
    // rendering
    contourVisuals = false;
    skeletonVisuals = false;
    calibrationLoaded = false;
    
    visible = true;
    setGuiPosition(0, 0);
}

//-------
void OpenNi::setupContourVisuals(int width, int height) {
    contourRenderer = new ContourRenderer();
    contourRenderer->setup(this, width, height);
    contourRenderer->setGuiPosition(skeletonVisuals ? 300 : 150, 0);
    if (calibrationLoaded) {
        contourRenderer->setCalibration(&kpt);
    }
    contourVisuals = true;
}

//-------
void OpenNi::setupSkeletonVisuals(int width, int height) {
    skeletonRenderer = new SkeletonRenderer();
    skeletonRenderer->setup(this, width, height);
    skeletonRenderer->setGuiPosition(contourVisuals ? 300 : 150, 0);
    if (calibrationLoaded) {
        skeletonRenderer->setCalibration(&kpt);
    }
    skeletonVisuals = true;
}

//-------
void OpenNi::setup(string oni){
    if (oni != "") {
        kinect.setupFromONI(oni);
    }
    else {
        kinect.setup();
    }
    kinect.addDepthGenerator();
    kinect.addImageGenerator();   // optional?
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.setUseDepthRawPixels(true);
    kinect.setDepthColoring(COLORING_GREY);
    
    setupControl();
    active = true;
}

//-------
void OpenNi::disableUserTracking() {
    trackingUsers = false;
    pTrackingUsers = false;
    kinect.setUseMaskPixelsAllUsers(false);
    kinect.setUseMaskTextureAllUsers(false);
    kinect.setUsePointCloudsAllUsers(false);
    kinect.removeUserGenerator();
    setupControl();
}

//-------
void OpenNi::enableUserTracking(int maxUsers) {
    this->maxUsers = maxUsers;
    trackingUsers = true;
    pTrackingUsers = true;
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(maxUsers);
    kinect.setUseMaskPixelsAllUsers(true);
    kinect.setUseMaskTextureAllUsers(true);
    kinect.setUsePointCloudsAllUsers(true);
    kinect.setPointCloudDrawSizeAllUsers(2);
    kinect.setPointCloudResolutionAllUsers(2);
    
    // setup joint structures
    jointNames.push_back("torso");
    jointNames.push_back("neck");
    jointNames.push_back("head");
    jointNames.push_back("leftShoulder");
    jointNames.push_back("leftElbow");
    jointNames.push_back("leftHand");
    jointNames.push_back("rightShoulder");
    jointNames.push_back("rightElbow");
    jointNames.push_back("rightHand");
    jointNames.push_back("leftHip");
    jointNames.push_back("leftKnee");
    jointNames.push_back("leftFoot");
    jointNames.push_back("rightHip");
    jointNames.push_back("rightKnee");
    jointNames.push_back("rightFoot");
    
    for (int i=0; i<maxUsers; i++) {
        vector<ofVec3f*> newPJoints, newRJoints, newVJoints, newAJoints, newNJoints;
        vector<float*> newDJoints, newVJointsMag, newVJointsMean, newAJointsMag, newAJointsMean, newAJointsTrajectory;
        for (int j=0; j<jointNames.size(); j++) {
            newPJoints.push_back(new ofVec3f(0, 0, 0));
            newRJoints.push_back(new ofVec3f(0, 0, 0));
            newVJoints.push_back(new ofVec3f(0, 0, 0));
            newAJoints.push_back(new ofVec3f(0, 0, 0));
            newNJoints.push_back(new ofVec3f(0, 0, 0));
            newDJoints.push_back(new float());
            newVJointsMag.push_back(new float());
            newVJointsMean.push_back(new float());
            newAJointsMag.push_back(new float());
            newAJointsMean.push_back(new float());
            newAJointsTrajectory.push_back(new float());
        }
        pJoints.push_back(newPJoints);
        rJoints.push_back(newRJoints);
        vJoints.push_back(newVJoints);
        aJoints.push_back(newAJoints);
        normalizedJoints.push_back(newNJoints);

        dJoints.push_back(newDJoints);
        vJointsMag.push_back(newVJointsMag);
        vJointsMean.push_back(newVJointsMean);
        aJointsMag.push_back(newAJointsMag);
        aJointsMean.push_back(newAJointsMean);
        aJointsTrajectory.push_back(newAJointsTrajectory);
        
        symmetry.push_back(new float());
        qom.push_back(new float());
        ci.push_back(new float());
        depth.push_back(new float());
        ymaxHands.push_back(new float());
        
        userBoundingBoxMin.push_back(ofVec3f(0,0,0));
        userBoundingBoxMax.push_back(ofVec3f(0,0,0));
    }

    setupControl();
}

//-------
void OpenNi::disableContourTracking() {
    trackingContours = false;
    pTrackingContours = false;
    setupControl();
}

//-------
void OpenNi::enableContourTracking() {
    trackingContours = true;
    pTrackingContours = true;
    grayImage.allocate(kinect.getWidth(), kinect.getHeight());
    grayThreshNear.allocate(kinect.getWidth(), kinect.getHeight());
    grayThreshFar.allocate(kinect.getWidth(), kinect.getHeight());
    setupControl();
}

//-------
void OpenNi::setupControl() {
    control.clear();
    control.setName("OpenNi");
    control.setVisible(true);
    
    control.addParameter("active", &active);
    control.addParameter("track contours", &trackingContours);
    control.addParameter("track users", &trackingUsers);
    control.addParameter("track keypoints", &trackingKeypoints);
    
    if (trackingContours) {
        control.addParameter("farThreshold", &farThreshold, 0.0f, 255.0f);
        control.addParameter("nearThreshold", &nearThreshold, 0.0f, 255.0f);
        control.addParameter("minArea", &minArea, 0.0f, 100000.0f);
        control.addParameter("maxArea", &maxArea, 2500.0f, 150000.0f);
        control.addParameter("threshold", &threshold, 0.0f, 255.0f);
        control.addParameter("persistence", &persistence, 0.0f, 100.0f);
        control.addParameter("maxDistance", &maxDistance, 0.0f, 100.0f);
        control.addParameter("smoothingRate", &smoothingRate, 0.0f, 100.0f);
        control.addParameter("smoothed", &smoothness, 0, 10);
        control.addParameter("curved", &curved);
    }
    
    if (trackingUsers) {
        control.addParameter("normalizeJoints", &normalizeJoints);
    }
    
    if (trackingKeypoints) {
        
    }
}

//-------
void OpenNi::setMaxUsers(int maxUsers) {
    this->maxUsers = maxUsers;
    kinect.setMaxNumUsers(maxUsers);
}

//-------
bool OpenNi::update(){
    if (!active)    return false;
    checkTrackingOptions();
    kinect.update();
    if (kinect.isNewFrame()) {
        depthPixels = kinect.getDepthRawPixels();
        if (trackingContours) {
            updateContours();
            if (contourVisuals) {
                contourRenderer->update();
            }
        }
        if (trackingKeypoints) {
            //updateOpticalFlow();
        }
        if (trackingUsers) {
            updateUsers();
            if (skeletonVisuals) {
                skeletonRenderer->update();
            }
        }
        return true;
    }
    else {
        return false;
    }
}

//---------
void OpenNi::checkTrackingOptions() {
    if (trackingContours != pTrackingContours) {
        trackingContours ? enableContourTracking() : disableContourTracking();
        pTrackingContours = trackingContours;
    }
    if (trackingUsers != pTrackingUsers) {
        trackingUsers ? enableUserTracking() : disableUserTracking();
        pTrackingUsers = trackingUsers;
    }
    if (trackingKeypoints != pTrackingKeypoints) {
        pTrackingKeypoints = trackingKeypoints;
    }
}

//---------
void OpenNi::updateContours() {
    grayImage.setFromPixels(kinect.getDepthPixels().getChannel(1));
    grayThreshNear = grayImage;
    grayThreshFar = grayImage;
    grayThreshNear.threshold(nearThreshold, true);
    grayThreshFar.threshold(farThreshold);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    grayImage.flagImageChanged();
    contourFinder.setMinArea(minArea);
    contourFinder.setMaxArea(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.getTracker().setMaximumDistance(maxDistance);
    contourFinder.getTracker().setSmoothingRate(smoothingRate);
    contourFinder.findContours(grayImage);
}

//-------
void OpenNi::updateUsers(){
    int numUsers = kinect.getNumTrackedUsers();

    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = kinect.getTrackedUser(i);
        
        if (!user.isSkeleton()) continue;
        if (!(user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
            (!featExtractor.skeletonExists(0) || user.getJoint((Joint)0).getWorldPosition() != featExtractor.getSkeleton(0)->getPosition(0) ))) continue;

        // update feature extractor joints
        map<int, ofPoint> theJoints;
        for (int j = 0; j < user.getNumJoints(); j++) {
            theJoints[j] = user.getJoint((Joint) j).getWorldPosition();
        }
        featExtractor.updateSkeleton(i, theJoints);
        
        // update skeleton stats
        for (int j=0; j<jointNames.size(); j++) {
            pJoints[i][j]->set(featExtractor.getSkeleton(i)->getPosition((Joint) j));
            rJoints[i][j]->set(featExtractor.getSkeleton(i)->getRelativePositionToTorso((Joint) j));
            vJoints[i][j]->set(featExtractor.getSkeleton(i)->getVelocity((Joint) j));
            aJoints[i][j]->set(featExtractor.getSkeleton(i)->getAcceleration((Joint) j));
            
            *dJoints[i][j] = featExtractor.getSkeleton(i)->getDistanceToTorso((Joint) j);
            *vJointsMag[i][j] = featExtractor.getSkeleton(i)->getVelocityMagnitude((Joint) j);
            *vJointsMean[i][j] = featExtractor.getSkeleton(i)->getVelocityMean((Joint) j);
            *aJointsMag[i][j] = featExtractor.getSkeleton(i)->getAccelerationMagnitude((Joint) j);
            *aJointsMean[i][j] = featExtractor.getSkeleton(i)->getAccelerationMean((Joint) j);
            *aJointsTrajectory[i][j] = featExtractor.getSkeleton(i)->getAccelerationTrajectory((Joint) j);
        }
        *symmetry[i] = featExtractor.getSkeleton(i)->getSymmetry();
        *qom[i] = featExtractor.getSkeleton(i)->getQom();
        *ci[i] = featExtractor.getSkeleton(i)->getCI();
        *depth[i] = featExtractor.getSkeleton(i)->getDepth();
        *ymaxHands[i] = featExtractor.getSkeleton(i)->getYMaxHands();
        
        // finding bounding box
        userBoundingBoxMin[i].set(10000, 10000, 10000);
        userBoundingBoxMax[i].set(-10000, -10000, -10000);
        for (int j=0; j<pJoints[i].size(); j++) {
            if      (pJoints[i][j]->x < userBoundingBoxMin[i].x) userBoundingBoxMin[i].x = pJoints[i][j]->x;
            else if (pJoints[i][j]->x > userBoundingBoxMax[i].x) userBoundingBoxMax[i].x = pJoints[i][j]->x;
            if      (pJoints[i][j]->y < userBoundingBoxMin[i].y) userBoundingBoxMin[i].y = pJoints[i][j]->y;
            else if (pJoints[i][j]->y > userBoundingBoxMax[i].y) userBoundingBoxMax[i].y = pJoints[i][j]->y;
            if      (pJoints[i][j]->z < userBoundingBoxMin[i].z) userBoundingBoxMin[i].z = pJoints[i][j]->z;
            else if (pJoints[i][j]->z > userBoundingBoxMax[i].z) userBoundingBoxMax[i].z = pJoints[i][j]->z;
        }
        
        // normalize if requested
        if (normalizeJoints) {
            for (int j=0; j<pJoints[i].size(); j++) {
                normalizedJoints[i][j]->set((pJoints[i][j]->x - userBoundingBoxMin[i].x) / (userBoundingBoxMax[i].x - userBoundingBoxMin[i].x),
                                            (pJoints[i][j]->y - userBoundingBoxMin[i].y) / (userBoundingBoxMax[i].y - userBoundingBoxMin[i].y),
                                            (pJoints[i][j]->z - userBoundingBoxMin[i].z) / (userBoundingBoxMax[i].z - userBoundingBoxMin[i].z) );
            }
        }
    }
}

//-------
void OpenNi::updateOpticalFlow() {
    //flow.calcOpticalFlow(kinect.getDepthPixelsRef());
}

//-------
void OpenNi::draw() {
    if (!visible)   return;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(guiX+120, guiY);
    ofScale(0.5, 0.5);

    kinect.drawImage();
    if (trackingUsers) {
        kinect.drawSkeletons();
    }
    if (trackingContours) {
        ofTranslate(640, 0);
        grayImage.draw(0, 0);
        ofSetColor(255, 0, 0);
        ofSetLineWidth(4);
        contourFinder.draw();
    }

    ofPopStyle();
    ofPopMatrix();
}

//---------
void OpenNi::drawVisuals() {
    if (contourVisuals) {
        contourRenderer->draw();
    }
    if (skeletonVisuals) {
        skeletonRenderer->draw();
    }
}


//---------
void OpenNi::setCalibration(string path){
    kpt.loadCalibration(path);
    if (contourVisuals) {
        contourRenderer->setCalibration(&kpt);
    }
    if (contourVisuals) {
        contourRenderer->setCalibration(&kpt);
    }
    calibrationLoaded = true;
}

//-------
ofVec3f * OpenNi::getJoint(int idxUser, int joint) {
    return pJoints[idxUser][joint];
}
ofVec3f * OpenNi::getNormalizedJoint(int idxUser, int joint) {
    return normalizedJoints[idxUser][joint];
}
ofVec3f * OpenNi::getRelativeJoint(int idxUser, int joint) {
    return rJoints[idxUser][joint];
}
ofVec3f * OpenNi::getVelocityJoint(int idxUser, int joint) {
    return vJoints[idxUser][joint];
}
ofVec3f * OpenNi::getAccelerationJoint(int idxUser, int joint) {
    return aJoints[idxUser][joint];
}
float * OpenNi::getRelativeDistanceJoint(int idxUser, int joint) {
    return dJoints[idxUser][joint];
}
float * OpenNi::getVelocityMagnitudeJoint(int idxUser, int joint) {
    return vJointsMag[idxUser][joint];
}
float * OpenNi::getVelocityMeanJoint(int idxUser, int joint) {
    return vJointsMean[idxUser][joint];
}
float * OpenNi::getAccelerationMagnitudeJoint(int idxUser, int joint) {
    return aJointsMag[idxUser][joint];
}
float * OpenNi::getAccelerationMeanJoint(int idxUser, int joint) {
    return aJointsMean[idxUser][joint];
}
float * OpenNi::getAccelerationTrajectoryJoint(int idxUser, int joint) {
    return aJointsTrajectory[idxUser][joint];
}
//-------
vector<ofVec3f*> & OpenNi::getJoints(int idxUser) {
    return pJoints[idxUser];
}
vector<ofVec3f*> & OpenNi::getNormalizedJoints(int idxUser) {
    return normalizedJoints[idxUser];
}
vector<ofVec3f*> & OpenNi::getRelativeJoints(int idxUser) {
    return rJoints[idxUser];
}
vector<ofVec3f*> & OpenNi::getVelocityJoints(int idxUser) {
    return vJoints[idxUser];
}
vector<ofVec3f*> & OpenNi::getAccelerationJoints(int idxUser) {
    return aJoints[idxUser];
}
vector<float*> & OpenNi::getRelativeDistanceJoints(int idxUser) {
    return dJoints[idxUser];
}
vector<float*> & OpenNi::getVelocityMagnitudeJoints(int idxUser) {
    return vJointsMag[idxUser];
}
vector<float*> & OpenNi::getVelocityMeanJoints(int idxUser) {
    return vJointsMean[idxUser];
}
vector<float*> & OpenNi::getAccelerationMagnitudeJoints(int idxUser) {
    return aJointsMag[idxUser];
}
vector<float*> & OpenNi::getAccelerationMeanJoints(int idxUser) {
    return aJointsMean[idxUser];
}
vector<float*> & OpenNi::getAccelerationTrajectoryJoints(int idxUser) {
    return aJointsTrajectory[idxUser];
}
//-------
float * OpenNi::getSymmetry(int idxUser) {
    return symmetry[idxUser];
}
float * OpenNi::getQom(int idxUser) {
    return qom[idxUser];
}
float * OpenNi::getCi(int idxUser) {
    return ci[idxUser];
}
float * OpenNi::getDepth(int idxUser) {
    return depth[idxUser];
}
float * OpenNi::getYMaxHands(int idxUser) {
    return ymaxHands[idxUser];
}

//-------
vector<ofVec2f> & OpenNi::getProjectedJoints(int idxUser) {
    projectedJoints.clear();
    for (int i=0; i<pJoints[idxUser].size(); i++) {
        ofVec3f raw = ofVec3f(-pJoints[idxUser][i]->x,
                              -pJoints[idxUser][i]->y,
                              +pJoints[idxUser][i]->z);
        projectedJoints.push_back(kpt.getProjectedPoint(raw));
    }
    return projectedJoints;
}

//-------
void OpenNi::close() {
    kinect.stop();
}

//-------
void OpenNi::userEvent(ofxOpenNIUserEvent & event) {
    ofLog(OF_LOG_VERBOSE, getUserStatusAsString(event.userStatus)+"for user"+ofToString(event.id)+"from device"+ofToString(event.deviceID));
}


//-------
void OpenNi::getCalibratedContour(vector<cv::Point> & points, vector<ofVec2f> & calibratedPoints, int width, int height) {
    for (int j=0; j<points.size(); j+=smoothness) {
        ofPoint depthPoint = ofPoint(points[j].x, points[j].y,
                                     depthPixels[(int)(points[j].x + points[j].y*640)]);
        ofVec3f worldPoint = kinect.projectiveToWorld(depthPoint);
        
        // discrepancy between ofxKinect (used for calibration) and ofxOpenNi
        worldPoint.x *= -1.0;
        worldPoint.y *= -1.0;
        
        ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
        ofPoint mappedPoint(ofMap(projectedPoint.x, 0, 1, 0, width),
                            ofMap(projectedPoint.y, 0, 1, 0, height));

        calibratedPoints.push_back(mappedPoint);
    }
}

//-------
void OpenNi::drawCalibratedContours(int width, int height){
    ofPushStyle();
    ofFill();
    ofSetColor(255, 255, 100, 50);
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        for (int j=0; j<points.size(); j+=smoothness) {
            ofPoint depthPoint = ofPoint(points[j].x, points[j].y,
                                         depthPixels[(int)(points[j].x + points[j].y*640)]);
            ofVec3f worldPoint = kinect.projectiveToWorld(depthPoint);
            
            // discrepancy between ofxKinect (used for calibration) and ofxOpenNi
            worldPoint.x *= -1.0;
            worldPoint.y *= -1.0;
            
            ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
            ofPoint mappedPoint(ofMap(projectedPoint.x, 0, 1, 0, width),
                                ofMap(projectedPoint.y, 0, 1, 0, height));
            if (curved) {
                ofCurveVertex(mappedPoint.x, mappedPoint.y);
            } else {
                ofVertex(mappedPoint.x, mappedPoint.y);
            }
        }
        ofEndShape();
    }
    ofPopStyle();
}

//-------
void OpenNi::drawCalibratedSkeleton(int idx, int width, int height) {
    ofPushStyle();
    vector<ofVec2f> pjoints = getProjectedJoints(idx);
    
    ofSetColor(0, 255, 0);
    for (int i=0; i<pjoints.size(); i++) {
        ofCircle(pjoints[i].x * width, pjoints[i].y * height, 15);
    }
    ofSetColor(255);
    ofSetLineWidth(8);
    
    // lines
    ofLine(pjoints[0].x * width,  pjoints[0].y * height,  pjoints[1].x * width,  pjoints[1].y * height);
    ofLine(pjoints[1].x * width,  pjoints[1].y * height,  pjoints[2].x * width,  pjoints[2].y * height);
    ofLine(pjoints[2].x * width,  pjoints[2].y * height,  pjoints[9].x * width,  pjoints[9].y * height);
    ofLine(pjoints[2].x * width,  pjoints[2].y * height,  pjoints[12].x * width, pjoints[12].y * height);
    ofLine(pjoints[1].x * width,  pjoints[1].y * height,  pjoints[3].x * width,  pjoints[3].y * height);
    ofLine(pjoints[1].x * width,  pjoints[1].y * height,  pjoints[6].x * width,  pjoints[6].y * height);
    ofLine(pjoints[3].x * width,  pjoints[3].y * height,  pjoints[4].x * width,  pjoints[4].y * height);
    ofLine(pjoints[4].x * width,  pjoints[4].y * height,  pjoints[5].x * width,  pjoints[5].y * height);
    ofLine(pjoints[6].x * width,  pjoints[6].y * height,  pjoints[7].x * width,  pjoints[7].y * height);
    ofLine(pjoints[7].x * width,  pjoints[7].y * height,  pjoints[8].x * width,  pjoints[8].y * height);
    ofLine(pjoints[9].x * width,  pjoints[9].y * height,  pjoints[10].x * width, pjoints[10].y * height);
    ofLine(pjoints[10].x * width, pjoints[10].y * height, pjoints[11].x * width, pjoints[11].y * height);
    ofLine(pjoints[12].x * width, pjoints[12].y * height, pjoints[13].x * width, pjoints[13].y * height);
    ofLine(pjoints[13].x * width, pjoints[13].y * height, pjoints[14].x * width, pjoints[14].y * height);
    
    ofPopStyle();
}

//---------
void OpenNi::setGuiPosition(int guiX, int guiY) {
    this->guiX = guiX;
    this->guiY = guiY;
    control.setGuiPosition(guiX, guiY);
    if (contourVisuals) {
        contourRenderer->setGuiPosition(guiX+150, guiY);
    }
    if (skeletonVisuals) {
        skeletonRenderer->setGuiPosition(guiX+150, guiY);
    }
}

//---------
void OpenNi::setVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
    if (contourVisuals) {
        contourRenderer->setGuiVisible(visible);
    }
    if (skeletonVisuals) {
        skeletonRenderer->setGuiVisible(visible);
    }
}

//---------
void OpenNi::toggleVisible() {
    setVisible(!visible);
}
