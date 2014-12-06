#include "ContourRenderer.h"


//----------
static bool shouldRemove(ofPtr<ofxBox2dBaseShape>shape) {
    //return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition());
    return !ofRectangle(0, -400, 1280, 800+400).inside(shape.get()->getPosition());
}

//-------
Contour::Contour(vector<ofVec2f> & points, ofPoint center, int label) {
    this->points = points;
    this->center = center;
    this->label = label;
    age = 0;
    color = ofColor(ofRandom(60,255), ofRandom(60,255), ofRandom(60,255));
}

//-------
void Contour::setPoints(vector<ofVec2f> & points, ofPoint center) {
    this->points = points;
    this->center = center;
}

//-------
void Contour::draw() {
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(color);
    ofBeginShape();
    for (int j=0; j<points.size(); j++) {
        ofVertex(points[j].x, points[j].y);
    }
    ofEndShape();
    ofPopStyle();
}

//-------
Ribbon::Ribbon(Contour *contour,
       int maxAge, int speed, int length, int skip,
       int margin, float noiseFactor, float ageFactor,
       float lineWidth, int maxAlpha, int updateRate,
       float lerpRate, float dilate, bool curved, bool match) {
    this->contour = contour;
    this->maxAge = maxAge;
    this->speed = speed;
    this->length = length;
    this->skip = skip;
    this->margin = margin;
    this->noiseFactor = noiseFactor;
    this->ageFactor = ageFactor;
    this->lineWidth = lineWidth;
    this->maxAlpha = maxAlpha;
    this->updateRate = updateRate;
    this->lerpRate = lerpRate;
    this->dilate = dilate;
    this->curved = curved;
    this->match = match;
    idx = ofRandom(contour->points.size());
    age = 0;
    active = true;
    
    for (int i=0; i<length; i++) {
        int j = (idx + i*skip) % contour->points.size();
        addPoint(j);
    }
    idx = (idx + length * skip) % contour->points.size();
}

//-------
void Ribbon::update() {
    if (ofGetFrameNum() % updateRate == 0) {
        idx = (idx + speed) % contour->points.size();
        addPoint(idx);
        points.erase(points.begin());
        lookup.erase(lookup.begin());
        lookupMatched.erase(lookupMatched.begin());
    }
    age++;
    if (age >= maxAge) {
        active = false;
    }
}

//-------
void Ribbon::addPoint(int p) {
    if (dilate != 1.0) {
        points.push_back(ofVec2f(contour->center.x + dilate * (contour->points[p].x - contour->center.x) + margin * ofSignedNoise(p * noiseFactor + 25, ageFactor * age - 9, -22),
                                 contour->center.y + dilate * (contour->points[p].y - contour->center.y) + margin * ofSignedNoise(p * noiseFactor + 17, ageFactor * age + 6, -50)));
    }
    else {
        points.push_back(ofVec2f(contour->points[p].x + margin * ofSignedNoise(p * noiseFactor + 25, ageFactor * age - 9, -22),
                                 contour->points[p].y + margin * ofSignedNoise(p * noiseFactor + 17, ageFactor * age + 6, -50)));
    }
    lookup.push_back(p);
    lookupMatched.push_back((float) p / contour->points.size());
}

//-------
void Ribbon::draw() {
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(contour->color, ofMap(abs(age - maxAge*0.5), 0, maxAge*0.5, 255, 0));
    ofBeginShape();
    for (int i=0; i<points.size(); i++) {
        if (match) {
            idxMatched = floor(contour->points.size() * lookupMatched[i]);
            points[i].x = ofLerp(points[i].x, contour->points[idxMatched].x +
                                 margin * ofSignedNoise(i * noiseFactor, ageFactor * age, 5), lerpRate);
            points[i].y = ofLerp(points[i].y, contour->points[idxMatched].y +
                                 margin * ofSignedNoise(i * noiseFactor, ageFactor * age, 10), lerpRate);
        }
        else {
            points[i].x = ofLerp(points[i].x, contour->points[lookup[i]].x +
                                 margin * ofSignedNoise(i * noiseFactor, ageFactor * age, 5), lerpRate);
            points[i].y = ofLerp(points[i].y, contour->points[lookup[i]].y +
                                 margin * ofSignedNoise(i * noiseFactor, ageFactor * age, 10), lerpRate);
        }
        curved ? ofCurveVertex(points[i].x, points[i].y) : ofVertex(points[i].x, points[i].y);
    }
    ofEndShape();
    ofPopStyle();
}

//-------
void ContourRenderer::setup(OpenNi *openNi, int width, int height) {
    this->openNi = openNi;
    this->width = width;
    this->height = height;
    setupControl();
}

//-------
void ContourRenderer::setCalibration(ofxKinectProjectorToolkit *kpt) {
    this->kpt = kpt;
    calibrated = true;
}

//-------
void ContourRenderer::update() {
    checkChanges();
    
    recordContours();

    if (drawRibbons) {
        manageContours();
        manageRibbons();
    }
    if (drawOutwardLines) {
        
    }
    if (drawPhysics) {
        updatePhysics();
    }
    if (drawFluids) {
        updateFluids();
    }
}

//-------
void ContourRenderer::checkChanges() {
    if (!pDrawPhysics && drawPhysics) {
        setupPhysics();
    }
    if (!pDrawFluids && drawFluids) {
        setupFluids();
    }
    
    if ((pDrawOutwardLines != drawOutwardLines) ||
        (pDrawRibbons != drawRibbons) ||
        (pDrawContours != drawContours) ||
        
        (pDrawFluids != drawFluids) ||
        
        
        (pDrawPhysics != drawPhysics)) {
        
        setupControl();
        pDrawOutwardLines = drawOutwardLines;
        pDrawRibbons = drawRibbons;
        pDrawPhysics = drawPhysics;
        pDrawContours = drawContours;
        
        
        pDrawFluids = drawFluids;
    }
}

//-------
void ContourRenderer::manageContours() {
    if (ofGetFrameNum() % frameSkip != 0)   return;
    
    // add new contours
    if (contours.size() > 0 && labels.size() > 0) {
        int labelToAdd = labels[ofRandom(labels.size())];
        for (int c=0; c<contours.size(); c++) {
            if (contours[c]->label == labelToAdd) {
                int maxAge = ofRandom(maxAgeMin, maxAgeMax);
                int speed = ofRandom(speedMin, speedMax);
                int length = ofRandom(lengthMin, lengthMax);
                int skip = ofRandom(skipMin, skipMax);
                int margin = ofRandom(marginMin, marginMax);
                float noiseFactor = ofRandom(noiseFactorMin, noiseFactorMax);
                float ageFactor = ofRandom(ageFactorMin, ageFactorMax);
                float lineWidth = ofRandom(lineWidthMin, lineWidthMax);
                int maxAlpha = ofRandom(maxAlphaMin, maxAlphaMax);
                int updateRate = ofRandom(updateRateMin, updateRateMax);
                float lerpRate = ofRandom(lerpRateMin, lerpRateMax);
                ribbons.push_back(new Ribbon(contours[c], maxAge, speed, length, skip,
                                             margin, noiseFactor, ageFactor,
                                             lineWidth, maxAlpha, updateRate,
                                             lerpRate, dilate, curved, match));
                break;
            }
        }
    }
    
    // erase old contours
    vector<Contour *>::iterator cit = contours.begin();
    while (cit != contours.end()) {
        bool foundRibbon = false;
        for (int r=0; r<ribbons.size(); r++) {
            if ((*cit) == ribbons[r]->getContour()) {
                foundRibbon = true;
            }
        }
        if (foundRibbon) {
            ++cit;
        }
        else {
            contours.erase(cit);
        }
    }
}

//-------
void ContourRenderer::manageRibbons() {
    vector<Ribbon *>::iterator it = ribbons.begin();
    while (it != ribbons.end()) {
        if ((*it)->getActive()) {
            ++it;
        }
        else {
            ribbons.erase(it);
        }
    }
}

//-------
void ContourRenderer::recordContours() {
    ContourFinder & contourFinder = openNi->getContourFinder();
    RectTracker & tracker = openNi->getContourTracker();

    currentContours.clear();
    labels.clear();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        bool contourExists = false;
        for (int c=0; c<contours.size(); c++) {
            if (label == contours[c]->label) {
                if (calibrated) {
                    vector<ofVec2f> calibratedContour;
                    openNi->getCalibratedContour(contourFinder.getContour(i), calibratedContour, width, height);
                    currentContours.push_back(calibratedContour);
                    contours[c]->setPoints(calibratedContour, center);
                }
                else {
                    contours[c]->setPoints((vector<ofVec2f> &) contourFinder.getContour(i), center);
                }
                contourExists = true;
                break;
            }
        }
        if (!contourExists) {
            if (calibrated) {
                vector<ofVec2f> calibratedContour;
                openNi->getCalibratedContour(contourFinder.getContour(i), calibratedContour, width, height);
                contours.push_back(new Contour(calibratedContour, center, label));
            }
            else {
                contours.push_back(new Contour((vector<ofVec2f> &) contourFinder.getContour(i), center, label));
            }
        }
        labels.push_back(label);
    }
}

//-------
void ContourRenderer::draw() {
    if (drawContours) {
        renderContours();
    }
    if (drawRibbons) {
        renderRibbons();
    }
    if (drawOutwardLines) {
        renderOutwardLines();
    }
    if (drawPhysics) {
        renderPhysics();
    }
    if (drawFluids) {
        renderFluids();
    }
}

//-------
void ContourRenderer::renderRibbons() {
    for (int i=0; i<ribbons.size(); i++) {
        ribbons[i]->update();
        ribbons[i]->draw();
    }
}

//-------
void ContourRenderer::renderOutwardLines() {
    ContourFinder & contourFinder = openNi->getContourFinder();
    RectTracker & tracker = openNi->getContourTracker();

    ofPushMatrix();
    ofPushStyle();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        ofSetColor(color);
        ofSetLineWidth(lineWidth);
        for (int k=0; k<points.size(); k+=smooth) {
            float ang = offset + atan2(points[k].y - center.y, points[k].x - center.x);
            float x, y;
            if (centered) {
                x = width * 0.5 + length * cos(ang);
                y = height * 0.5 + length * sin(ang);
            }
            else {
                x = ((float) width  / 640.0) * center.x * 0.5 + length * cos(ang);
                y = ((float) height / 480.0) * center.y * 0.5 + length * sin(ang);
            }
            ofLine(x, y, points[k].x, points[k].y);
        }
    }
    ofPopStyle();
    ofPopMatrix();
}

//-------
void ContourRenderer::setupPhysics() {
    img.loadImage("/Users/Gene/Desktop/star.png");
    img.resize(40, 40);
    
	box2d.init();
	box2d.setGravity(0, 16);
	//box2d.createGround(ofPoint(0, secondWindow.getHeight()), ofPoint(secondWindow.getWidth(), secondWindow.getHeight()));
    
    rate = 5;
    tolerance = 0.3f;
    circleDensity = 0.3;
    circleBounce = 0.5;
    circleFriction = 0.1;
}

//-------
void ContourRenderer::updatePhysics() {
    lines.clear();
    edges.clear();
    
    // create box2d edges
    for(int i = 0; i < currentContours.size(); i++) {
        lines.push_back(ofPolyline());
        for (int j=0; j<currentContours[i].size(); j++) {
            lines.back().addVertex(currentContours[i][j].x, currentContours[i][j].y);
        }
        ofPtr <ofxBox2dEdge> edge = ofPtr<ofxBox2dEdge>(new ofxBox2dEdge);
        lines.back().simplify(tolerance);
        for (int i=0; i<lines.back().size(); i++) {
            edge.get()->addVertex(lines.back()[i]);
        }
        edge.get()->create(box2d.getWorld());
        edges.push_back(edge);
    }
    
    // add some falling circles every so often
	if((int)ofRandom(0, rate) == 0) {
        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(circleDensity, circleBounce, circleFriction);
        circle.get()->setup(box2d.getWorld(), ofRandom(width), -20, ofRandom(15, 40));
		circles.push_back(circle);
	}
	
    // update box2d
    ofRemove(circles, shouldRemove);
    box2d.update();
}

//-------
void ContourRenderer::renderPhysics() {
    for (int i=0; i<circles.size(); i++) {
        ofPushMatrix();
        ofPushStyle();

		ofFill();
		ofSetHexColor(0xc0dd3b);
		
        ofVec2f pos = circles[i].get()->getPosition();
        float rad = circles[i].get()->getRadius();
        float ang = circles[i].get()->getRotation();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(pos.x, pos.y);
        ofRotate(ang);
        img.draw(0, 0, 2*rad, 2*rad);
        
        ofPopStyle();
        ofPopMatrix();
	}
}

//-------
void ContourRenderer::renderContours() {
    ofPushMatrix();
    ofPushStyle();
    for (int i=0; i<currentContours.size(); i++) {
        ofBeginShape();
        ofSetColor(contourColor);
        for (int j=0; j<currentContours[i].size(); j+=contourSmoothness) {
            ofCurveVertex(currentContours[i][j].x, currentContours[i][j].y);
        }
        ofEndShape();
    }
    ofPopStyle();
    ofPopMatrix();
}

//-------
void ContourRenderer::setupControl() {
    control.clear();
    control.setName("contour render");

    control.addParameter("drawContours", &drawContours);
    control.addParameter("drawRibbons", &drawRibbons);
    control.addParameter("drawOutwardLines", &drawOutwardLines);
    control.addParameter("drawPhysics", &drawPhysics);
    control.addParameter("drawFluids", &drawFluids);

    if (drawOutwardLines) {
        control.addParameter("offset", &offset, 0.0f, float(TWO_PI));
        control.addParameter("smooth", &smooth, 1, 10);
        control.addParameter("lineWidth", &lineWidth, 0.0f, 5.0f);
        control.addParameter("length", &length, 0, 1000);
        control.addColor("color", &color);
        control.addParameter("centered", &centered);
    }
    
    if (drawRibbons) {
        control.addParameter("threshold", &threshold, 100, 255);
        control.addParameter("numNew", &numNew, 1, 10);
        control.addParameter("frameSkip", &frameSkip, 1, 10);
        
        control.addParameter("maxAgeMin", &maxAgeMin, 5, 50);
        control.addParameter("maxAgeMax", &maxAgeMax, 20, 100);
        
        control.addParameter("speedMin", &speedMin, 1, 5);
        control.addParameter("speedMax", &speedMax, 3, 20);
        
        control.addParameter("lengthMin", &lengthMin, 5, 30);
        control.addParameter("lengthMax", &lengthMax, 20, 120);
        
        control.addParameter("skipMin", &skipMin, 1, 10);
        control.addParameter("skipMax", &skipMax, 5, 30);
        
        control.addParameter("marginMin", &marginMin, 0, 40);
        control.addParameter("marginMax", &marginMax, 0, 100);
        
        control.addParameter("noiseFactorMin", &noiseFactorMin, 0.001f, 0.02f);
        control.addParameter("noiseFactorMax", &noiseFactorMax, 0.02f, 0.1f);
        
        control.addParameter("ageFactorMin", &ageFactorMin, 0.0f, 0.01f);
        control.addParameter("ageFactorMax", &ageFactorMax, 0.0f, 0.02f);
        
        control.addParameter("lineWidthMin", &lineWidthMin, 0.0f, 3.0f);
        control.addParameter("lineWidthMax", &lineWidthMax, 1.0f, 8.0f);
        
        control.addParameter("maxAlphaMin", &maxAlphaMin, 0, 200);
        control.addParameter("maxAlphaMax", &maxAlphaMax, 100, 255);
        
        control.addParameter("updateRateMin", &updateRateMin, 1, 5);
        control.addParameter("updateRateMax", &updateRateMax, 2, 10);
        
        control.addParameter("lerpRateMin", &lerpRateMin, 0.0f, 1.0f);
        control.addParameter("lerpRateMax", &lerpRateMax, 0.0f, 1.0f);
        
        control.addParameter("dilate", &dilate, 0.0f, 2.0f);
        
        control.addParameter("curved", &curved);
        control.addParameter("match", &match);
    }
    
    if (drawPhysics) {
        control.addParameter("rate", &rate, 1, 20);
        control.addParameter("tolerance", &tolerance, 0.0f, 100.0f);
        control.addParameter("circleDensity", &circleDensity, 0.0f, 1.0f);
        control.addParameter("circleBounce", &circleBounce, 0.0f, 1.0f);
        control.addParameter("circleFriction", &circleFriction, 0.0f, 1.0f);
        control.addEvent("clear", this, &ContourRenderer::clearCircles);
    }
    
    if (drawContours) {
        control.addParameter("contourSmoothness", &contourSmoothness, 1, 10);
        control.addColor("contourColor", &contourColor);
    }
    
    
    
    if (drawFluids) {
        control.addParameter("simplify", &simplify, 0.0f, 100.0f);
        control.addParameter("numContourPts", &numContourPts, 3, 100);
        control.addParameter("skip", &skip, 1, 100);
        control.addParameter("displaceLerp", &displaceLerp, 0.0f, 1.0f);
        control.addParameter("dissipation", &dissipation, 0.0f, 1.0f);
        control.addParameter("vel dissipation", &velDissipation, 0.0f, 1.0f);
        control.addParameter("displacement", &displacement, 0.0f, 100.0f);
        control.addParameter("strength", &strength, 0.0f, 10.0f);
        control.addParameter("gravityX",  &gravityX, -0.02f, 0.02f);
        control.addParameter("gravityY", &gravityY, -0.02f, 0.02f);

        simplify = 40.0f;
        numContourPts = 10;
        displaceLerp = 0.1;
        skip = 5;
    }
    
    
    
    
    
    
    
    threshold = 240;
    frameSkip = 3;
    numNew = 1;
    
    smooth = 1;
    offset = 0;
    lineWidth = 1;
    length = 600;
    centered = false;
    color = ofColor(255);
    
    contourColor = ofColor(255);
    contourSmoothness = 3;
    
    maxAgeMin = 50;         maxAgeMax = 100;
    speedMin = 1;           speedMax = 4;
    lengthMin = 30;         lengthMax = 120;
    skipMin = 5;            skipMax = 10;
    marginMin = 8;          marginMax = 24;
    noiseFactorMin = 0.01;  noiseFactorMax = 0.03;
    ageFactorMin = 0.01;    ageFactorMax = 0.03;
    lineWidthMin = 3;       lineWidthMax = 5;
    maxAlphaMin = 200;      maxAlphaMax = 255;
    lerpRateMin = 0.4;      lerpRateMax = 0.6;
    updateRateMin = 1;      updateRateMax = 1;
    dilate = 1.0;
    curved = true;
    match = true;
}

//-------
void ContourRenderer::setGuiVisible(bool visible) {
    this->visible = visible;
    control.setVisible(visible);
}





//-------
void ContourRenderer::setupFluids() {
    
    
    // hold previous joints
    for (int i=0; i<maxUsers; i++) {
        vector<ofVec2f> newVec;
        //newVec.resize(openNi->getJointNames().size());
        pContourPoints.push_back(newVec);
        displace.push_back(newVec);
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
//-------
void ContourRenderer::updateFluids() {
    
    for (int i=0; i<pContourPoints.size(); i++) {
        while (pContourPoints[i].size() < numContourPts) {
            pContourPoints[i].push_back(ofVec2f(0,0));
        }
        if (pContourPoints[i].size() > numContourPts) {
            pContourPoints[i].resize(numContourPts);
        }
    }

    for (int i=0; i<displace.size(); i++) {
        while (displace[i].size() < numContourPts) {
            displace[i].push_back(ofVec2f(0,0));
        }
        if (displace[i].size() > numContourPts) {
            displace[i].resize(numContourPts);
        }
    }
    
    
    
    for (int i=0; i < min(maxUsers,(int) currentContours.size()); i++) {
        ofPolyline pl;
        for (int j=0; j<currentContours[i].size(); j++) {
            pl.addVertex(currentContours[i][j]);
        }
        //pl.addVertices((vector<ofPoint>&) currentContours[i]);
        pl.simplify(simplify);
        
        
        vector<ofPoint> verts = pl.getVertices();
        ofVec2f newDisplace;
        for (int j=0; j<numContourPts; j++) {
            int idx = ofMap(j, 0, numContourPts, 0, verts.size());
            ofVec2f pt = verts[idx];
            
            
            newDisplace = pt - pContourPoints[i][j];
            displace[i][j].set(ofLerp(displace[i][j].x, newDisplace.x, displaceLerp),
                               ofLerp(displace[i][j].y, newDisplace.y, displaceLerp));
            
            pContourPoints[i][j] = pt;
            
            
            
            if (j%skip==0) {
            ofPoint m = pt;
            ofPoint d = displace[i][j]*displacement;
            ofPoint c = ofPoint(320, 240) - m;
            c.normalize();
            fluid.addTemporalForce(m,
                                   d,
                                   ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),
                                   strength);
            }
            
            
        }
    }
    
    
    //fluid.setGravity(ofVec2f(gravityX, gravityY));
    fluid.dissipation = dissipation;
    fluid.velocityDissipation = velDissipation;
    
    fluid.update();

}
//-------
void ContourRenderer::renderFluids() {
    

//    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_LINEAR);
    fluid.draw();

    
    renderContours();
    
    ofSetColor(255);    
    for (int i=0; i<pContourPoints.size(); i++) {
        for (int j=0; j<pContourPoints[i].size(); j++) {
            ofLine(pContourPoints[i][j].x, pContourPoints[i][j].y, pContourPoints[i][j].x + displace[i][j].x, pContourPoints[i][j].y + displace[i][j].y);
        }
    }

}



