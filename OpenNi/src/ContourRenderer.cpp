#include "ContourRenderer.h"


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
}

//-------
void ContourRenderer::checkChanges() {
    if ((pDrawOutwardLines != drawOutwardLines) ||
        (pDrawRibbons != drawRibbons)) {
        setupControl();
        pDrawOutwardLines = drawOutwardLines;
        pDrawRibbons = drawRibbons;
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
    if (drawRibbons) {
        renderRibbons();
    }
    if (drawOutwardLines) {
        renderOutwardLines();
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
void ContourRenderer::setupControl() {
    control.clear();
    control.setName("contour render");

    control.addParameter("drawRibbons", &drawRibbons);
    control.addParameter("drawOutwardLines", &drawOutwardLines);

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
    
    threshold = 240;
    frameSkip = 3;
    numNew = 1;
    
    smooth = 1;
    offset = 0;
    lineWidth = 1;
    length = 600;
    centered = false;
    color = ofColor(255);
    
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
