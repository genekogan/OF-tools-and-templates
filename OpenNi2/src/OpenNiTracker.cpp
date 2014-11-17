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
    
    setGuiPosition(0, 0, 1280, 960);
}

//-------
void OpenNi::setupContourVisuals(int width, int height) {
    contourRenderer = new ContourRenderer();
    contourRenderer->setup(this, width, height);
    contourRenderer->setGuiPosition(skeletonVisuals ? 400 : 200, 0);
    if (calibrationLoaded) {
        contourRenderer->setCalibration(&kpt);
    }
    contourVisuals = true;
}

//-------
void OpenNi::setupSkeletonVisuals(int width, int height) {
    skeletonRenderer = new SkeletonRenderer();
    skeletonRenderer->setup(this, width, height);
    skeletonRenderer->setGuiPosition(contourVisuals ? 400 : 200, 0);
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
    jointNames.push_back("head");
    jointNames.push_back("neck");
    jointNames.push_back("torso");
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
        vector<ofVec3f*> newJoints, newVJoints, newProjectedJoints;
        for (int j=0; j<jointNames.size(); j++) {
            newJoints.push_back(new ofVec3f(0, 0, 0));
            newVJoints.push_back(new ofVec3f(0, 0, 0));
            newProjectedJoints.push_back(new ofVec3f(0, 0, 0));
        }
        joints.push_back(newJoints);
        vjoints.push_back(newVJoints);
        normalizedJoints.push_back(newProjectedJoints);
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
            //flow.calcOpticalFlow(kinect.getDepthPixelsRef());
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
        
        /* VELOCITY */
        float vLerpRate = 0.03;
        vjoints[i][ 0]->set(*vjoints[i][ 0] * (1.0 - vLerpRate) + (user.getJoint(JOINT_HEAD).getWorldPosition() - *joints[i][ 0]) * vLerpRate);
        vjoints[i][ 1]->set(*vjoints[i][ 1] * (1.0 - vLerpRate) + (user.getJoint(JOINT_NECK).getWorldPosition() - *joints[i][ 1]) * vLerpRate);
        vjoints[i][ 2]->set(*vjoints[i][ 2] * (1.0 - vLerpRate) + (user.getJoint(JOINT_TORSO).getWorldPosition() - *joints[i][ 2]) * vLerpRate);
        vjoints[i][ 3]->set(*vjoints[i][ 3] * (1.0 - vLerpRate) + (user.getJoint(JOINT_LEFT_SHOULDER).getWorldPosition() - *joints[i][ 3]) * vLerpRate);
        vjoints[i][ 4]->set(*vjoints[i][ 4] * (1.0 - vLerpRate) + (user.getJoint(JOINT_LEFT_ELBOW).getWorldPosition() - *joints[i][ 4]) * vLerpRate);
        vjoints[i][ 5]->set(*vjoints[i][ 5] * (1.0 - vLerpRate) + (user.getJoint(JOINT_LEFT_HAND).getWorldPosition() - *joints[i][ 5]) * vLerpRate);
        vjoints[i][ 6]->set(*vjoints[i][ 6] * (1.0 - vLerpRate) + (user.getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition() - *joints[i][ 6]) * vLerpRate);
        vjoints[i][ 7]->set(*vjoints[i][ 7] * (1.0 - vLerpRate) + (user.getJoint(JOINT_RIGHT_ELBOW).getWorldPosition() - *joints[i][ 7]) * vLerpRate);
        vjoints[i][ 8]->set(*vjoints[i][ 8] * (1.0 - vLerpRate) + (user.getJoint(JOINT_RIGHT_HAND).getWorldPosition() - *joints[i][ 8]) * vLerpRate);
        vjoints[i][ 9]->set(*vjoints[i][ 9] * (1.0 - vLerpRate) + (user.getJoint(JOINT_LEFT_HIP).getWorldPosition() - *joints[i][ 9]) * vLerpRate);
        vjoints[i][10]->set(*vjoints[i][10] * (1.0 - vLerpRate) + (user.getJoint(JOINT_LEFT_KNEE).getWorldPosition() - *joints[i][10]) * vLerpRate);
        vjoints[i][11]->set(*vjoints[i][11] * (1.0 - vLerpRate) + (user.getJoint(JOINT_LEFT_FOOT).getWorldPosition() - *joints[i][11]) * vLerpRate);
        vjoints[i][12]->set(*vjoints[i][12] * (1.0 - vLerpRate) + (user.getJoint(JOINT_RIGHT_HIP).getWorldPosition() - *joints[i][12]) * vLerpRate);
        vjoints[i][13]->set(*vjoints[i][13] * (1.0 - vLerpRate) + (user.getJoint(JOINT_RIGHT_KNEE).getWorldPosition() - *joints[i][13]) * vLerpRate);
        vjoints[i][14]->set(*vjoints[i][14] * (1.0 - vLerpRate) + (user.getJoint(JOINT_RIGHT_FOOT).getWorldPosition() - *joints[i][14]) * vLerpRate);
        
        /* JOINTS */
        
        
        
        
        joints[i][ 0]->set(user.getJoint(JOINT_HEAD).getWorldPosition());
        joints[i][ 1]->set(user.getJoint(JOINT_NECK).getWorldPosition());
        joints[i][ 2]->set(user.getJoint(JOINT_TORSO).getWorldPosition());
        
        // left arm + shoulder
        joints[i][ 3]->set(user.getJoint(JOINT_LEFT_SHOULDER).getWorldPosition());
        joints[i][ 4]->set(user.getJoint(JOINT_LEFT_ELBOW).getWorldPosition());
        joints[i][ 5]->set(user.getJoint(JOINT_LEFT_HAND).getWorldPosition());
        
        // right arm + shoulder
        joints[i][ 6]->set(user.getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition());
        joints[i][ 7]->set(user.getJoint(JOINT_RIGHT_ELBOW).getWorldPosition());
        joints[i][ 8]->set(user.getJoint(JOINT_RIGHT_HAND).getWorldPosition());
        
        // left leg
        joints[i][ 9]->set(user.getJoint(JOINT_LEFT_HIP).getWorldPosition());
        joints[i][10]->set(user.getJoint(JOINT_LEFT_KNEE).getWorldPosition());
        joints[i][11]->set(user.getJoint(JOINT_LEFT_FOOT).getWorldPosition());
        
        // right leg
        joints[i][12]->set(user.getJoint(JOINT_RIGHT_HIP).getWorldPosition());
        joints[i][13]->set(user.getJoint(JOINT_RIGHT_KNEE).getWorldPosition());
        joints[i][14]->set(user.getJoint(JOINT_RIGHT_FOOT).getWorldPosition());
        
        // finding bounding box
        userBoundingBoxMin[i].set(10000, 10000, 10000);
        userBoundingBoxMax[i].set(-10000, -10000, -10000);
        for (int j=0; j<joints[i].size(); j++) {
            if      (joints[i][j]->x < userBoundingBoxMin[i].x) userBoundingBoxMin[i].x = joints[i][j]->x;
            else if (joints[i][j]->x > userBoundingBoxMax[i].x) userBoundingBoxMax[i].x = joints[i][j]->x;
            if      (joints[i][j]->y < userBoundingBoxMin[i].y) userBoundingBoxMin[i].y = joints[i][j]->y;
            else if (joints[i][j]->y > userBoundingBoxMax[i].y) userBoundingBoxMax[i].y = joints[i][j]->y;
            if      (joints[i][j]->z < userBoundingBoxMin[i].z) userBoundingBoxMin[i].z = joints[i][j]->z;
            else if (joints[i][j]->z > userBoundingBoxMax[i].z) userBoundingBoxMax[i].z = joints[i][j]->z;
        }
        
        // normalize if requested
        if (normalizeJoints) {
            for (int j=0; j<joints[i].size(); j++) {
                normalizedJoints[i][j]->set((joints[i][j]->x - userBoundingBoxMin[i].x) / (userBoundingBoxMax[i].x - userBoundingBoxMin[i].x),
                                            (joints[i][j]->y - userBoundingBoxMin[i].y) / (userBoundingBoxMax[i].y - userBoundingBoxMin[i].y),
                                            (joints[i][j]->z - userBoundingBoxMin[i].z) / (userBoundingBoxMax[i].z - userBoundingBoxMin[i].z) );
            }
        }
    }
}

//-------
void OpenNi::draw() {
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(w/1280.0, h/960.0);
    
    kinect.drawDepth();
    if (trackingUsers) {
        int numUsers = kinect.getNumTrackedUsers();
        for (int i = 0; i < numUsers; i++){
            ofxOpenNIUser & user = kinect.getTrackedUser(i);
            user.drawSkeleton();
        }
    }
    if (trackingContours) {
        ofTranslate(640, 0);
        grayImage.draw(0, 0);
        ofSetColor(255, 0, 0);
        ofSetLineWidth(4);
        contourFinder.draw();
        ofSetColor(255);
    }
    
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
vector<ofVec3f*> & OpenNi::getJoints(int idxUser) {
    return joints[idxUser];
}

//-------
vector<ofVec3f*> & OpenNi::getNormalizedJoints(int idxUser) {
    return normalizedJoints[idxUser];
}

//-------
vector<ofVec2f> & OpenNi::getProjectedJoints(int idxUser) {
    projectedJoints.clear();
    for (int i=0; i<joints[idxUser].size(); i++) {
        joints[idxUser][i]->x *= -1.0;
        joints[idxUser][i]->y *= -1.0;
        projectedJoints.push_back(kpt.getProjectedPoint(*joints[idxUser][i]));
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
void OpenNi::setGuiPosition(int x, int y, int w, int h){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    control.setGuiPosition(x+5, y+5);
}

//---------
void OpenNi::toggleGuiVisible() {
    control.toggleVisible();
}




void OpenNi::skeletonStats() {

    // size bounding box
    // skeleton velocity (magnitude / direction)
    // wingspan
    // symmetry
    
    
    for (int i=0; i<14; i++) {
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(500, 40 + 18*i);
        ofSetColor(0, 255, 0);
        float w = ofMap(abs(vjoints[0][i]->x) + abs(vjoints[0][i]->y) + abs(vjoints[0][i]->z), 0, 20, 0, 400);
        ofRect(0, 0, w, 16);
        ofPopMatrix();
        ofPopStyle();
    }
    
    
    
}