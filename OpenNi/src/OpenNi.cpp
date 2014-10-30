#include "OpenNi.h"


//-------
void OpenNi::setup(string oni){
    
    if (oni != "") {
        kinect.setupFromONI(oni);
    }
    kinect.addDepthGenerator();
    kinect.addImageGenerator();
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(1);
    kinect.start();
    
    
    //kinect.setUseMaskPixelsAllUsers(true);
    kinect.setUseMaskTextureAllUsers(true);
    kinect.setUsePointCloudsAllUsers(true);
    kinect.setPointCloudDrawSizeAllUsers(2);
    kinect.setPointCloudResolutionAllUsers(2);
    
    
    /*
    colorImage.allocate(640, 480);
    
    grayImage.allocate(640, 480);
    grayThreshNear.allocate(640, 480);
    grayThreshFar.allocate(640, 480);
    */
    grayImage.allocate(640, 480);
    
    
    
    // initialize joint array
    joints.push_back(&jointHead);           jointNames.push_back("head");
    joints.push_back(&jointNeck);           jointNames.push_back("neck");
    joints.push_back(&jointTorso);          jointNames.push_back("torso");
    joints.push_back(&jointLeftShoulder);   jointNames.push_back("leftShoulder");
    joints.push_back(&jointLeftElbow);      jointNames.push_back("leftElbow");
    joints.push_back(&jointLeftHand);       jointNames.push_back("leftHand");
    joints.push_back(&jointRightShoulder);  jointNames.push_back("rightShoulder");
    joints.push_back(&jointRightElbow);     jointNames.push_back("rightElbow");
    joints.push_back(&jointRightHand);      jointNames.push_back("rightHand");
    joints.push_back(&jointLeftHip);        jointNames.push_back("leftHip");
    joints.push_back(&jointLeftKnee);       jointNames.push_back("leftKnee");
    joints.push_back(&jointLeftFoot);       jointNames.push_back("leftFoot");
    joints.push_back(&jointRightHip);       jointNames.push_back("rightHip");
    joints.push_back(&jointRightKnee);      jointNames.push_back("rightKnee");
    joints.push_back(&jointRightFoot);      jointNames.push_back("rightFoot");
    
    // setup control panel
    control.setName("OpenNi");
    control.setVisible(false);
    control.addParameter("lookForJoints", &trackingJoints);
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

    // parameters
    fade = 200;
    minArea = 1000;
    maxArea = 70000;
    threshold = 15;
    persistence = 15;
    maxDistance = 32;
    numDilate = 2;
    numErode = 0;
    nearThreshold = 230;
    farThreshold = 10;
    smoothness = 1;
    curved = false;
    smoothingRate = 1;
    
    // modes
    trackingJoints = true;
    trackingBlobs = true;
    trackingKeypoints = false;
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

//-------
void OpenNi::updateJoints(){
    int numUsers = kinect.getNumTrackedUsers();
    
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = kinect.getTrackedUser(i);
        
        // head
        jointTorso = user.getJoint(JOINT_TORSO).getWorldPosition();
        jointNeck = user.getJoint(JOINT_NECK).getWorldPosition();
        jointHead = user.getJoint(JOINT_HEAD).getWorldPosition();
        
        // left arm + shoulder
        jointLeftShoulder = user.getJoint(JOINT_LEFT_SHOULDER).getWorldPosition();
        jointLeftElbow = user.getJoint(JOINT_LEFT_ELBOW).getWorldPosition();
        jointLeftHand = user.getJoint(JOINT_LEFT_HAND).getWorldPosition();
        
        // right arm + shoulder
        jointRightShoulder = user.getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition();
        jointRightElbow = user.getJoint(JOINT_RIGHT_ELBOW).getWorldPosition();
        jointRightHand = user.getJoint(JOINT_RIGHT_HAND).getWorldPosition();
        
        // left leg
        jointLeftHip = user.getJoint(JOINT_LEFT_HIP).getWorldPosition();
        jointLeftKnee = user.getJoint(JOINT_LEFT_KNEE).getWorldPosition();
        jointLeftFoot = user.getJoint(JOINT_LEFT_FOOT).getWorldPosition();
        
        // right leg
        jointRightHip = user.getJoint(JOINT_RIGHT_HIP).getWorldPosition();
        jointRightKnee = user.getJoint(JOINT_RIGHT_KNEE).getWorldPosition();
        jointRightFoot = user.getJoint(JOINT_RIGHT_FOOT).getWorldPosition();
        
        // finding bounding box
        userBoundingBoxMin = ofVec3f(1000, 1000, 1000);
        userBoundingBoxMax = ofVec3f(-1000, -1000, -1000);
        for (int i=0; i<joints.size(); i++) {
            if      (joints[i]->x < userBoundingBoxMin.x) userBoundingBoxMin.x = joints[i]->x;
            else if (joints[i]->x > userBoundingBoxMax.x) userBoundingBoxMax.x = joints[i]->x;
            if      (joints[i]->y < userBoundingBoxMin.y) userBoundingBoxMin.y = joints[i]->y;
            else if (joints[i]->y > userBoundingBoxMax.y) userBoundingBoxMax.y = joints[i]->y;
            if      (joints[i]->z < userBoundingBoxMin.z) userBoundingBoxMin.z = joints[i]->z;
            else if (joints[i]->z > userBoundingBoxMax.z) userBoundingBoxMax.z = joints[i]->z;
        }
    }
}

//---------
void OpenNi::updateContours() {
    grayImage.setFromPixels(kinect.getDepthPixels().getChannel(2));
    contourFinder.setMinArea(640*480*0.1);
    contourFinder.setMaxArea(640*480*0.4);
    contourFinder.setThreshold(threshold);
    contourFinder.getTracker().setPersistence(10);
    contourFinder.getTracker().setMaximumDistance(43);
    contourFinder.getTracker().setSmoothingRate(1);
    contourFinder.findContours(grayImage);
}

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
    
    //kinect.drawDepth(10, 5);    //drawDebug
    
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(w/640.0, h/480.0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    int numUsers = kinect.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = kinect.getTrackedUser(i);
        user.drawMask();
    }
    ofDisableBlendMode();
    ofPopMatrix();
}

//-------
void OpenNi::drawCalibrated(int x, int y, int width, int height){

    ofFill();
    ofSetColor(255, 255, 0);
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        for (int j=0; j<points.size(); j+=smoothness) {
            
            
            
            ofVec3f wp = kinect.projectiveToWorld(ofPoint(points[j].x, points[j].y));
            ofVec2f pp = kpt.getProjectedPoint(wp);
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


}

//---------
void OpenNi::setCalibration(string path){
    kpt.loadCalibration(path);
}

//-------
vector<ofVec3f*> & OpenNi::getJoints(bool normalized) {
    if (normalized) {
        for (int i=0; i<joints.size(); i++) {
            joints[i]->set((joints[i]->x - userBoundingBoxMin.x) / (userBoundingBoxMax.x - userBoundingBoxMin.x),
                           (joints[i]->y - userBoundingBoxMin.y) / (userBoundingBoxMax.y - userBoundingBoxMin.y),
                           (joints[i]->z - userBoundingBoxMin.z) / (userBoundingBoxMax.z - userBoundingBoxMin.z) );
        }
    }
    return joints;
}

//-------
vector<ofVec2f> & OpenNi::getProjectedJoints() {
    projectedJoints.clear();
    for (int i=0; i<joints.size(); i++) {
        joints[i]->x *= -1.0;
        joints[i]->y *= -1.0;
        projectedJoints.push_back(kpt.getProjectedPoint(*joints[i]));
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
void OpenNi::drawCalibratedSkeleton(int width, int height) {
    vector<ofVec2f> joints = getProjectedJoints();
    
    ofSetColor(0, 255, 0);
    for (int i=0; i<joints.size(); i++) {
        ofCircle(joints[i].x * width, joints[i].y * height, 15);
    }
    ofSetColor(255);
    ofSetLineWidth(8);

    // lines
    ofLine(joints[0].x * width,  joints[0].y * height,  joints[1].x * width,  joints[1].y * height);
    ofLine(joints[1].x * width,  joints[1].y * height,  joints[2].x * width,  joints[2].y * height);
    ofLine(joints[2].x * width,  joints[2].y * height,  joints[9].x * width,  joints[9].y * height);
    ofLine(joints[2].x * width,  joints[2].y * height,  joints[12].x * width, joints[12].y * height);
    ofLine(joints[1].x * width,  joints[1].y * height,  joints[3].x * width,  joints[3].y * height);
    ofLine(joints[1].x * width,  joints[1].y * height,  joints[6].x * width,  joints[6].y * height);
    ofLine(joints[3].x * width,  joints[3].y * height,  joints[4].x * width,  joints[4].y * height);
    ofLine(joints[4].x * width,  joints[4].y * height,  joints[5].x * width,  joints[5].y * height);
    ofLine(joints[6].x * width,  joints[6].y * height,  joints[7].x * width,  joints[7].y * height);
    ofLine(joints[7].x * width,  joints[7].y * height,  joints[8].x * width,  joints[8].y * height);
    ofLine(joints[9].x * width,  joints[9].y * height,  joints[10].x * width, joints[10].y * height);
    ofLine(joints[10].x * width, joints[10].y * height, joints[11].x * width, joints[11].y * height);
    ofLine(joints[12].x * width, joints[12].y * height, joints[13].x * width, joints[13].y * height);
    ofLine(joints[13].x * width, joints[13].y * height, joints[14].x * width, joints[14].y * height);
}



//---------
void OpenNi::toggleGuiVisible() {
    control.toggleVisible();
}

//---------
void OpenNi::setGuiPosition(int x, int y){
    control.setGuiPosition(x, y);
}


