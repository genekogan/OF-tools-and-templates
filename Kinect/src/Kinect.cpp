#include "Kinect.h"

void Kinect::setup(){
    vector<string> choices;
    choices.push_back("blobs");
    choices.push_back("segmentation");
    control.registerMenu("strategy", this, &Kinect::selectStrategy, choices);
    control.registerParameter("track blobs", &trackingBlobs);
    control.registerParameter("track keypoints", &trackingKeypoints);
    
    control.registerLabel("blobs parameters");
    control.registerParameter("farThreshold", &farThreshold, 0.0f, 255.0f);
    control.registerParameter("nearThreshold", &nearThreshold, 0.0f, 255.0f);
    
    control.registerLabel("segmentation parameters");
    control.registerParameter("fade", &fade, 0.0f, 255.0f);
    control.registerParameter("numDilate", &numDilate, 0, 10);
    control.registerParameter("numErode", &numErode, 0, 10);

    control.registerLabel("contour parameters");
    control.registerParameter("minArea", &minArea, 0.0f, 100000.0f);
    control.registerParameter("maxArea", &maxArea, 2500.0f, 150000.0f);
    control.registerParameter("threshold", &threshold, 0.0f, 255.0f);
    control.registerParameter("persistence", &persistence, 0.0f, 100.0f);
    control.registerParameter("maxDistance", &maxDistance, 0.0f, 100.0f);
    control.registerParameter("smoothingRate", &smoothingRate, 0.0f, 100.0f);
    
    control.registerLabel("output parameters");
    control.registerParameter("smoothed", &smoothness, 0, 10);
    control.registerParameter("curved", &curved);
    control.setName("kinect");
    
    blobColors[0] = ofColor(255, 0, 0);
    blobColors[1] = ofColor(0, 255, 0);
    blobColors[2] = ofColor(0, 0, 255);
    blobColors[3] = ofColor(255, 255, 0);
    blobColors[4] = ofColor(255, 0, 255);
    blobColors[5] = ofColor(0, 255, 255);
    blobColors[6] = ofColor(255, 127, 0);
    blobColors[7] = ofColor(127, 0, 255);
    blobColors[8] = ofColor(0, 255, 127);
    blobColors[9] = ofColor(127, 255, 0);
    blobColors[10]= ofColor(255, 0, 127);
    blobColors[11]= ofColor(0, 127, 255);

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
    
    kinect.setRegistration(true);
    kinect.init();
	kinect.open();
}

//---------
void Kinect::selectStrategy(string &s) {
    if      (s=="blobs")        setContourStrategy(BLOBS);
    else if (s=="segmentation") setContourStrategy(SEGMENTATION);
}

//---------
void Kinect::setContourStrategy(ContourStrategy contourStrategy) {
    this->contourStrategy = contourStrategy;
    if (contourStrategy == BLOBS) {
        grayImage.allocate(kinect.width, kinect.height);
        grayThreshNear.allocate(kinect.width, kinect.height);
        grayThreshFar.allocate(kinect.width, kinect.height);
    }
    else if (contourStrategy == SEGMENTATION) {
        colorImage.allocate(kinect.width, kinect.height);
        grayImage.allocate(kinect.width, kinect.height);
        fboKinect.allocate(kinect.width, kinect.height, GL_RGBA);
        fboEdges.allocate(kinect.width, kinect.height, GL_RGB);
        edgeShader.load("kinect/standard.vert", "kinect/edges.frag");
    }
}

//---------
void Kinect::toggleGuiVisible() {
    control.toggleVisible();
}

//---------
void Kinect::setCalibration(string path){
    kpt.loadCalibration(path);
}

//---------
void Kinect::setGuiPosition(int x, int y){
    control.setGuiPosition(x, y);
}

//---------
ContourFinder& Kinect::getContourFinder(){
    return contourFinder;
}

//---------
RectTracker& Kinect::getContourTracker(){
    return contourFinder.getTracker();
}

//---------
bool Kinect::update(){
    kinect.update();
    if(kinect.isFrameNew()) {
        if (trackingBlobs) {
            if (contourStrategy == BLOBS) {
                updateBlobs();
            }
            else if (contourStrategy == SEGMENTATION) {
                updateSegmentation();
            }
            updateContours();
        }
        if (trackingKeypoints) {
            flow.calcOpticalFlow(kinect.getDepthPixelsRef());
        }
        return true;
    }
    else {
        return false;
    }
}

//---------
void Kinect::updateBlobs() {
    grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
    grayThreshNear = grayImage;
    grayThreshFar = grayImage;
    grayThreshNear.threshold(nearThreshold, true);
    grayThreshFar.threshold(farThreshold);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    grayImage.flagImageChanged();
}

//---------
void Kinect::updateSegmentation() {
    // depth image fade
    fboKinect.begin();
    ofEnableAlphaBlending();
    ofSetColor(255, fade);
    kinect.drawDepth(0, 0);
    ofDisableAlphaBlending();
    fboKinect.end();
    
    // get Edges
    fboEdges.begin();
    edgeShader.begin();
    fboKinect.draw(0, 0);
    edgeShader.end();
    fboEdges.end();
    
    fboEdges.readToPixels(pixels);
    colorImage.setFromPixels(pixels);
    grayImage.setFromColorImage(colorImage);
    
    // post-processing
    for (int i=0; i<numDilate; i++) grayImage.dilate_3x3();
    for (int i=0; i<numErode; i++)  grayImage.erode_3x3();
    grayImage.threshold(threshold);
}

//---------
void Kinect::updateContours() {
    contourFinder.setMinArea(minArea);
    contourFinder.setMaxArea(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.getTracker().setMaximumDistance(maxDistance);
    contourFinder.getTracker().setSmoothingRate(smoothingRate);
    contourFinder.findContours(grayImage);
}

//---------
void Kinect::drawDebug(int x, int y, int w, int h){
    ofPushMatrix();

    ofTranslate(x, y);
    ofScale((float) w / 1280.0f, (float) h / 960.0f);
    
    ofSetColor(255, 255);
    kinect.draw(0, 0);
    if (trackingKeypoints) {
        flow.draw();
    }
    ofTranslate(640, 0);
    grayImage.draw(0, 0);
    ofTranslate(-640, 480);
    contourFinder.draw();
    ofTranslate(640, 0);
    
    RectTracker& tracker = contourFinder.getTracker();
    
    for(int i = 0; i < contourFinder.size(); i++)
    {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        // draw contours
        ofFill();
        ofSetColor(blobColors[label % 12]);
        ofBeginShape();
        for (int j=0; j<points.size(); j++) {
            ofVertex(points[j].x, points[j].y);
        }
        ofEndShape();
        
        // draw approximate contour
        ofNoFill();
        ofSetLineWidth(3);
        ofBeginShape();
        for (int j=0; j<fitPoints.size(); j++) {
            ofVertex(fitPoints[j].x, fitPoints[j].y);
        }
        ofEndShape();
        
        // draw rotated bounding rect
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255);
        ofTranslate(fitQuad.center.x, fitQuad.center.y);
        ofRotate(fitQuad.angle);
        ofRect(0, 0, fitQuad.size.width, fitQuad.size.height);
        ofPopStyle();
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

//---------
void Kinect::drawMask(ofBaseDraws &tex, bool useCalibration) {
    beginMask(tex.getWidth(), tex.getHeight(), useCalibration);
    tex.draw(0, 0);
    endMask();
}

//---------
void Kinect::beginMask(int w, int h, bool useCalibration) {
    if (w != kmask.getWidth() || h != kmask.getHeight()) {
        kmask.setup(w, h, KMask::LUMINANCE);
    }
    
    // draw mask
    kmask.beginMask();
    if (useCalibration) {
        drawCalibratedContours(kmask.getWidth(), kmask.getHeight());
    }
    else {
        grayImage.draw(0, 0, kmask.getWidth(), kmask.getHeight());
    }
    kmask.endMask();
    
    // begin drawing texture
    kmask.begin();
}

//---------
void Kinect::endMask() {
    kmask.end();
    kmask.draw();
}

//---------
void Kinect::setKeypointROI(ofPoint topLeft, ofPoint bottomRight) {
    rect.set(topLeft, bottomRight.x-topLeft.x, bottomRight.y-topLeft.y);
	vector<KeyPoint> keypoints;
	vector<KeyPoint> keypointsInside;
	vector<cv::Point2f> featuresToTrack;
    copyGray(kinect, kinectGray);
	FAST(kinectGray, keypoints,2);
	for(int i=0;i<keypoints.size();i++){
		if(rect.inside(toOf(keypoints[i].pt))){
			keypointsInside.push_back(keypoints[i]);
		}
	}
#if CV_MAJOR_VERSION>=2 && (CV_MINOR_VERSION>4 || (CV_MINOR_VERSION==4 && CV_SUBMINOR_VERSION>=1))
	KeyPointsFilter::retainBest(keypointsInside,30);
#endif
	KeyPoint::convert(keypointsInside,featuresToTrack);
	flow.setFeaturesToTrack(featuresToTrack);
}

//---------
void Kinect::drawCalibratedContours(int width, int height) {
    ofFill();
    ofSetColor(255);
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        for (int j=0; j<points.size(); j+=smoothness) {
            ofVec3f wp = kinect.getWorldCoordinateAt(points[j].x, points[j].y);
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
ofVec2f Kinect::getCalibratedPoint(ofVec2f depthPoint) {
    ofVec3f worldPoint = kinect.getWorldCoordinateAt(depthPoint.x, depthPoint.y);
    ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
    return projectedPoint;
}

//---------
void Kinect::close() {
    kinect.close();
}

//---------
Kinect::~Kinect() {
    close();
}