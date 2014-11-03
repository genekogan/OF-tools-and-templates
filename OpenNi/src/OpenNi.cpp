#include "OpenNi.h"


//-------
void OpenNi::setup(string oni){
    maxUsers = 2;    
    fade = 200;
    minArea = 1000;
    maxArea = 70000;
    threshold = 15;
    persistence = 15;
    maxDistance = 32;
    numDilate = 2;
    numErode = 0;
    nearThreshold = 254;
    farThreshold = 220;
    smoothness = 1;
    curved = false;
    smoothingRate = 1;
    normalizeJoints = false;
    
    // modes
    trackingJoints = true;
    trackingBlobs = true;
    trackingKeypoints = false;

    if (oni != "") {
        kinect.setupFromONI(oni);
    }
    else {
        kinect.setup();
    }
    kinect.addDepthGenerator();

    kinect.addImageGenerator();
    kinect.setRegister(true);
    kinect.setMirror(true);
    
    
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(maxUsers);
    
    kinect.setUseMaskPixelsAllUsers(true);
    kinect.setUseMaskTextureAllUsers(true);
    kinect.setUsePointCloudsAllUsers(true);
    kinect.setPointCloudDrawSizeAllUsers(2);
    kinect.setPointCloudResolutionAllUsers(2);
    
    
    
    kinect.setUseDepthRawPixels(true);
    
    kinect.start();

    
    // setup resources
    grayImage.allocate(kinect.getWidth(), kinect.getHeight());
    grayThreshNear.allocate(kinect.getWidth(), kinect.getHeight());
    grayThreshFar.allocate(kinect.getWidth(), kinect.getHeight());
    
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
        vector<ofVec3f*> newJoints, newProjectedJoints;
        for (int j=0; j<jointNames.size(); j++) {
            newJoints.push_back(new ofVec3f(0, 0, 0));
            newProjectedJoints.push_back(new ofVec3f(0, 0, 0));
        }
        joints.push_back(newJoints);
        normalizedJoints.push_back(newProjectedJoints);
    }
    
    for (int i=0; i<maxUsers; i++) {
        cout << "JOINTS " << joints[i].size() << endl;
    }
    
    // setup control panel
    control.setName("OpenNi");
    control.setVisible(true);
    control.addParameter("lookForJoints", &trackingJoints);
    control.addParameter("normalizeJoints", &normalizeJoints);
    control.addParameter("track blobs", &trackingBlobs);
    control.addParameter("track keypoints", &trackingKeypoints);
    control.addParameter("farThreshold", &farThreshold, 0.0f, 255.0f);
    control.addParameter("nearThreshold", &nearThreshold, 0.0f, 255.0f);
    control.addParameter("fade", &fade, 0.0f, 255.0f);
    control.addParameter("numDilate", &numDilate, 0, 10);
    control.addParameter("numErode", &numErode, 0, 10);
    control.addParameter("minArea", &minArea, 0.0f, 100000.0f);
    control.addParameter("maxArea", &maxArea, 2500.0f, 150000.0f);
    control.addParameter("threshold", &threshold, 0.0f, 255.0f);
    control.addParameter("persistence", &persistence, 0.0f, 100.0f);
    control.addParameter("maxDistance", &maxDistance, 0.0f, 100.0f);
    control.addParameter("smoothingRate", &smoothingRate, 0.0f, 100.0f);
    control.addParameter("smoothed", &smoothness, 0, 10);
    control.addParameter("curved", &curved);
}

//-------
void OpenNi::setMaxUsers(int maxUsers) {
    this->maxUsers = maxUsers;
    kinect.setMaxNumUsers(maxUsers);
}

//-------
bool OpenNi::update(){
    kinect.update();
    if (kinect.isNewFrame()) {
        if (trackingBlobs) {
            updateContours();
        }
        if (trackingKeypoints) {
            //flow.calcOpticalFlow(kinect.getDepthPixelsRef());
        }
        if (trackingJoints) {
            updateJoints();
        }
        return true;
    }
    else {
        return false;
    }
}

//---------
void OpenNi::updateContours() {
    grayImage.setFromPixels(kinect.getDepthPixels().getChannel(2));
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
void OpenNi::updateJoints(){
    int numUsers = kinect.getNumTrackedUsers();
    
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = kinect.getTrackedUser(i);
        if (!user.isSkeleton()) continue;
        
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
        userBoundingBoxMin = ofVec3f(10000, 10000, 10000);
        userBoundingBoxMax = ofVec3f(-10000, -10000, -10000);
        for (int j=0; j<joints[i].size(); j++) {
            if      (joints[i][j]->x < userBoundingBoxMin.x) userBoundingBoxMin.x = joints[i][j]->x;
            else if (joints[i][j]->x > userBoundingBoxMax.x) userBoundingBoxMax.x = joints[i][j]->x;
            if      (joints[i][j]->y < userBoundingBoxMin.y) userBoundingBoxMin.y = joints[i][j]->y;
            else if (joints[i][j]->y > userBoundingBoxMax.y) userBoundingBoxMax.y = joints[i][j]->y;
            if      (joints[i][j]->z < userBoundingBoxMin.z) userBoundingBoxMin.z = joints[i][j]->z;
            else if (joints[i][j]->z > userBoundingBoxMax.z) userBoundingBoxMax.z = joints[i][j]->z;
        }
        
        // normalize if requested
        if (normalizeJoints) {
            for (int j=0; j<joints[i].size(); j++) {
                normalizedJoints[i][j]->set((joints[i][j]->x - userBoundingBoxMin.x) / (userBoundingBoxMax.x - userBoundingBoxMin.x),
                                            (joints[i][j]->y - userBoundingBoxMin.y) / (userBoundingBoxMax.y - userBoundingBoxMin.y),
                                            (joints[i][j]->z - userBoundingBoxMin.z) / (userBoundingBoxMax.z - userBoundingBoxMin.z) );
            }
        }
    }
}

//-------
void OpenNi::recordContours() {
    RectTracker& tracker = contourFinder.getTracker();
    vector<int> labels;
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        //int label = contourFinder.getLabel(i);
        bool contourExists = false;
        for (int c=0; c<contours.size(); c++) {
            if (label == contours[c]->label) {
                contours[c]->setPoints(contourFinder.getContour(i));
                contourExists = true;
                break;
            }
        }
        if (!contourExists) {
            contours.push_back(new Contour(contourFinder.getContour(i), label));
        }
        labels.push_back(label);
    }
}

//-------
void OpenNi::draw(int x, int y, int w, int h){
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(w/1280.0, h/960.0);

    if (trackingBlobs) {
        grayImage.draw(0, 0);
        ofSetColor(255, 0, 0);
        ofSetLineWidth(4);
        contourFinder.draw();
        ofSetColor(255);
        ofTranslate(640, 0);
    }    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    int numUsers = kinect.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = kinect.getTrackedUser(i);
        user.drawMask();
        user.drawSkeleton();
    }
    ofDisableBlendMode();
    ofPopMatrix();
}

//---------
void OpenNi::setCalibration(string path){
    kpt.loadCalibration(path);
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
void OpenNi::drawCalibratedContours(int width, int height){
    ofPushStyle();
    ofFill();
    ofSetColor(255, 255, 0);
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        for (int j=0; j<points.size(); j+=smoothness) {
            ofPoint dp = ofPoint(points[j].x, points[j].y, kinect.getDepthPixels()[(int)(points[j].x + points[j].y*640.0)]);
            ofVec3f wp = kinect.projectiveToWorld(dp);


            
            ofVec2f pp = kpt.getProjectedPoint(wp);
            
            
            ofPoint mp(ofMap(pp.x, 0, 1, 0, width),
                       ofMap(pp.y, 0, 1, 0, height));
            if (curved) {
                ofCurveVertex(mp.x, mp.y);
            } else {
                ofVertex(mp.x, mp.y);
            }
            if (abs(wp.x) > 0) {
                cout << ofToString(ofGetFrameNum()) << " " << ofToString(j) << " : " <<ofToString(mp) << endl;
            }
        }
        ofEndShape();
    }
    ofPopStyle();
}

/*
//-------
void OpenNi::drawCalibratedContours(int width, int height){
    ofPushStyle();
    ofFill();
    ofSetColor(255, 255, 0);
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        for (int j=0; j<points.size(); j+=smoothness) {
            //ofVec3f wp = kinect.projectiveToWorld(ofPoint((float)points[j].x, (float)points[j].y));
            //ofVec3f wp = kinect.cameraToWorld(ofVec2f((float)points[j].x, (float)points[j].y/480.0));
            
            ofVec3f wp;

            
            int nPoints = 1;
            
            vector<XnPoint3D> projective(nPoints);
            XnPoint3D *out = &projective[0];
            
            //	if(threaded) lock();
//            const XnDepthPixel* d = depthPixels.getPixels();
            const XnDepthPixel* d = (XnDepthPixel *) &kinect.getDepthPixels();
            
            unsigned int pixel;
            pixel  = (int)points[i].x + (int)points[i].y * 640;
            if (pixel >= 640*480)
                continue;
            
            projective[i].X = points[i].x;
            projective[i].Y = points[i].y;
            projective[i].Z = float(d[pixel]) / 1000.0f;

            //	if(threaded) unlock();
            
            kinect.getDepthGenerator().ConvertProjectiveToRealWorld(1, &projective[0], (XnPoint3D *) &wp);
            //recordDepth.getXnDepthGenerator().ConvertProjectiveToRealWorld(nPoints, &projective[0], (XnPoint3D*)&w[0]);
            
            
            cout << ofToString(wp) << endl;
            ofVec2f pp = kpt.getProjectedPoint(wp);
            cout << "pp " << ofToString(pp) << endl;
            
            
            
            ofPoint mp(ofMap(pp.x, 0, 1, 0, width),
                       ofMap(pp.y, 0, 1, 0, height));
            if (curved) {
                ofCurveVertex(mp.x, mp.y);
            } else {
                ofVertex(mp.x, mp.y);
            }
        }
        ofEndShape();
    }
    ofPopStyle();
}
*/


/*
//-------
void OpenNi::drawCalibratedContours(int width, int height){
    ofPushStyle();
    ofFill();
    ofSetColor(255, 255, 0);
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        
            
        int nPoints = points.size();
        
        if (nPoints < 3) continue;
        
        
//        vector<ofVec3f> wp(nPoints);
 //       wp.resize(nPoints);
        vector<ofVec3f> wp;
        for (int i=0; i<nPoints; i++) {
            wp.push_back(ofVec3f(0, 0, 0));
        }
        
        vector<XnPoint3D> projective(nPoints);
        XnPoint3D *out = &projective[0];
        
        //	if(threaded) lock();
        //            const XnDepthPixel* d = depthPixels.getPixels();
        const XnDepthPixel* d = (XnDepthPixel *) &kinect.getDepthPixels();
        
        unsigned int pixel;
        for (int j=0; j<nPoints; j++) {
            pixel  = (int)points[j].x + (int)points[j].y * 640;
            if (pixel >= 640*480)
                continue;
            
            projective[j].X = points[j].x;
            projective[j].Y = points[j].y;
            projective[j].Z = float(d[pixel]) / 1000.0f;
        }
        
        //	if(threaded) unlock();
        
//        kinect.getDepthGenerator().ConvertProjectiveToRealWorld(1, &projective, (XnPoint3D *) &wp);
        kinect.getDepthGenerator().ConvertProjectiveToRealWorld(nPoints, (XnPoint3D *) &projective, (XnPoint3D *) &wp);
//        kinect.getDepthGenerator().ConvertProjectiveToRealWorld(<#XnUInt32 nCount#>, <#const XnPoint3D *aProjective#>, <#XnPoint3D *aRealWorld#>)
        //recordDepth.getXnDepthGenerator().ConvertProjectiveToRealWorld(nPoints, &projective[0], (XnPoint3D*)&w[0]);
        
    }
    ofPopStyle();
}
*/


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
void OpenNi::setGuiPosition(int x, int y){
    control.setGuiPosition(x, y);
}

//---------
void OpenNi::toggleGuiVisible() {
    control.toggleVisible();
}

