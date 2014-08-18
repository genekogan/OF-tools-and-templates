#include "Bubbles.h"

void Bubbles::setup() {
    setName("Bubbles");
    
    control.registerParameter("speed", &speed, 0.0f, 0.1f);
    control.registerParameter("numBubbles", &numBubbles, 1, 2048);
    control.registerParameter("maxSizeInitial", &maxSizeInitial, 100, 1000);
    control.registerParameter("maxSize", &maxSize, 10, 800);
    control.registerParameter("baseColor", &baseColor, ofColor(0), ofColor(255));
    control.registerParameter("varColor", &varColor, ofVec3f(0,0,0), ofVec3f(255,255,255));
    control.registerParameter("maxPasses", &maxPasses, 3, 50);
    control.registerParameter("blurAmt", &blurAmt, 0, 20);
    
    speed = 0.091;
    numBubbles = 700;
    maxSizeInitial = 500;
    maxSize = 100;
    baseColor = ofColor(200, 40, 220);
    varColor = ofVec3f(20, 20, 20);
    maxPasses = 30;
    blurAmt = 5;
    
    maxPasses.addListener(this, &Bubbles::fboParametersChanged);
    blurAmt.addListener(this, &Bubbles::fboParametersChanged);
    maxSizeInitial.addListener(this, &Bubbles::fboParametersChanged);
    
    setupBubblesFbo();
    time = 0;
}

void Bubbles::fboParametersChanged(int & newMaxPasses) {
    setupBubblesFbo();
}

void Bubbles::setupBubblesFbo() {
    ofPushMatrix();
    ofPushStyle();

    ofSetCircleResolution(1024);
    blur.setup(maxSizeInitial*1.25, maxSizeInitial*1.25);

    for (int passes=1; passes <= maxPasses; passes++) {
        ofFbo fbo;
        fbo.allocate(blur.getWidth(), blur.getHeight(), GL_RGBA);
        
        fbo.begin();
        ofClear(255, 0);
        fbo.end();
        
        fbo.begin();
        
        blur.begin(blurAmt, passes+1);
        
        ofClear(255, 0);
        ofSetColor(255);
        ofCircle(fbo.getWidth()/2, fbo.getHeight()/2, maxSizeInitial/2);
        
        blur.end();
        
        fbo.end();
        
        bubbleFbo.push_back(fbo);
    }
    
    ofPopStyle();
    ofPopMatrix();
}

void Bubbles::addNewBubble() {
    TimeFunction<ofPoint> *newPosition = new TimeFunction<ofPoint>();
    newPosition->setConstant(ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
    newPosition->setFunctionNoise(ofPoint(-100, -100), ofPoint(100, 100), ofPoint(0.001, 0.001));

    position.push_back(newPosition);
    blurLevel.push_back(ofRandom(1));
    alpha.push_back(0);
    size.push_back(0);
    colorMargin.push_back(ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1)));
}

void Bubbles::update() {
    while (position.size() < numBubbles) {
        addNewBubble();
    }
    
    if (position.size() > numBubbles) {
        position.resize(numBubbles);
        colorMargin.resize(numBubbles);
        blurLevel.resize(numBubbles);
        alpha.resize(numBubbles);
        size.resize(numBubbles);
    }
    
    for (int i=0; i<numBubbles; i++) {
        alpha[i] = ofLerp(alpha[i], 100 + 100*sin(time + i), 0.03f);
        blurLevel[i] = ofLerp(blurLevel[i], 0.5 + 0.5*sin(1.2*time - i), 0.03f);
        size[i] = ofLerp(size[i], 0.2 + 0.4*sin(1.1*time-5+1.8*i), 0.03f);
    }
    
    time += speed;
}

void Bubbles::draw() {
    ofSetRectMode(OF_RECTMODE_CENTER);
    for (int i=0; i<numBubbles; i++) {
        ofSetColor(baseColor->r + colorMargin[i].x * varColor->x,
                   baseColor->g + colorMargin[i].y * varColor->y,
                   baseColor->b + colorMargin[i].z * varColor->z,
                   alpha[i]);
        int idx = (int) (blurAmt * blurLevel[i]);
            bubbleFbo[idx].draw(position[i]->get().x, position[i]->get().y,
                            maxSize * size[i], maxSize * size[i]);

    }
}