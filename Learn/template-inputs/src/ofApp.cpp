#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    kinect.setup();
    kinect.addImageGenerator();
    kinect.addDepthGenerator();
    kinect.setRegister(true);
    kinect.setMirror(true);
    kinect.addUserGenerator();
    kinect.setMaxNumUsers(1);
    kinect.start();
    
    // set properties for all user masks and point clouds
    //kinect.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    kinect.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    kinect.setUsePointCloudsAllUsers(true);
    kinect.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    kinect.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    // initialize joint array
    joints.push_back(&jointTorso);
    joints.push_back(&jointNeck);
    joints.push_back(&jointHead);
    joints.push_back(&jointLeftShoulder);
    joints.push_back(&jointLeftElbow);
    joints.push_back(&jointLeftHand);
    joints.push_back(&jointRightShoulder);
    joints.push_back(&jointRightElbow);
    joints.push_back(&jointRightHand);
    joints.push_back(&jointLeftHip);
    joints.push_back(&jointLeftKnee);
    joints.push_back(&jointLeftFoot);
    joints.push_back(&jointRightHip);
    joints.push_back(&jointRightKnee);
    joints.push_back(&jointRightFoot);
    
    // names for osc
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
    
    
    // setup OSC
    //osc.setup(HOST, PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    kinect.drawDebug();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    int numUsers = kinect.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = kinect.getTrackedUser(i);
        user.drawMask();
        
        ofPushMatrix();
        ofTranslate(320, 240);
        user.drawPointCloud();
        ofPopMatrix();
        
        // get joint positions:
        
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
        ofPoint pointMax = ofPoint(-1000, -1000, -1000);
        ofPoint pointMin = ofPoint(1000, 1000, 1000);
        for (int i=0; i<joints.size(); i++) {
            if      (joints[i]->x < pointMin.x) pointMin.x = joints[i]->x;
            else if (joints[i]->x > pointMax.x) pointMax.x = joints[i]->x;
            if      (joints[i]->y < pointMin.y) pointMin.y = joints[i]->y;
            else if (joints[i]->y > pointMax.y) pointMax.y = joints[i]->y;
            if      (joints[i]->z < pointMin.z) pointMin.z = joints[i]->z;
            else if (joints[i]->z > pointMax.z) pointMax.z = joints[i]->z;
        }
        
        // normalize
        for (int i=0; i<joints.size(); i++) {
            joints[i]->set((joints[i]->x - pointMin.x) / (pointMax.x - pointMin.x),
                           (joints[i]->y - pointMin.y) / (pointMax.y - pointMin.y),
                           (joints[i]->z - pointMin.z) / (pointMax.z - pointMin.z) );
            cout << joints[i]->x << ", " << joints[i]->y << ", " << joints[i]->z << endl;
        }
        cout << " ========== "<<endl;
        
        /*
        // send osc
        for (int i=0; i<joints.size(); i++) {
            ofxOscMessage mx, my, mz;
            mx.setAddress("/openni/"+jointNames[i]+"X");
            my.setAddress("/openni/"+jointNames[i]+"Y");
            mz.setAddress("/openni/"+jointNames[i]+"Z");
            mx.addFloatArg(joints[i]->x);
            my.addFloatArg(joints[i]->y);
            mz.addFloatArg(joints[i]->z);
            osc.sendMessage(mx);
            osc.sendMessage(my);
            osc.sendMessage(mz);
        }
        */
    }
    ofDisableBlendMode();
}


//--------------------------------------------------------------
void ofApp::userEvent(ofxOpenNIUserEvent & event){
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}