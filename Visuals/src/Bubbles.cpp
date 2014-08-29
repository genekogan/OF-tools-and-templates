#include "Bubbles.h"

//------------
void Bubbles::setup() {
    setName("Bubbles");
    
    control.registerParameter("speed", &speed, 0.0f, 0.1f);
    control.registerParameter("numBubbles", &numBubbles, 1, 2048);
    control.registerParameter("maxSizeInitial", &maxSizeInitial, 100, 1000);
    control.registerParameter("maxSize", &maxSize, 10, 800);
    control.registerParameter("baseColor", &baseColor, ofColor(0), ofColor(255));
    control.registerParameter("varColor", &varColor, ofVec3f(0,0,0), ofVec3f(255,255,255));
    control.registerParameter("position_NoiseMargin", &positionNoiseMargin, ofVec2f(0, 0), ofVec2f(300, 300));
    control.registerParameter("position_NoiseFactor", &positionNoiseFactor, ofVec2f(0, 0), ofVec2f(0.1, 0.1));
    control.registerParameter("position_SineMargin", &positionSineMargin, ofVec2f(0, 0), ofVec2f(300, 300));
    control.registerParameter("position_SineFreq", &positionSineFreq, ofVec2f(0, 0), ofVec2f(0.1, 0.1));
    control.registerParameter("position_TimeConstant", &positionTimeConstant, ofVec2f(-10, -10), ofVec2f(10, 10));
    control.registerParameter("maxPasses", &maxPasses, 3, 50);
    control.registerParameter("blurAmt", &blurAmt, 0, 20);
    
    speed = 0.091;
    numBubbles = 600;
    maxSizeInitial = 500;
    maxSize = 140;
    baseColor = ofColor(200, 40, 220);
    varColor = ofVec3f(20, 20, 20);
    positionNoiseMargin = ofVec2f(30, 30);
    positionNoiseFactor = ofVec2f(0.01, 0.01);
    positionSineMargin = ofVec2f(0, 0);
    positionSineFreq = ofVec2f(0.05, 0.05);
    positionTimeConstant = ofVec2f(0, 0);
    maxPasses = 30;
    blurAmt = 7;
    
    maxPasses.addListener(this, &Bubbles::fboParametersChanged);
    blurAmt.addListener(this, &Bubbles::fboParametersChanged);
    maxSizeInitial.addListener(this, &Bubbles::fboParametersChanged);
    
    setupBubblesFbo();
    time = 0;
    
    bubbleCreator.setup(position, colorMargin, blurLevel, alpha, size, numBubbles);
}

//------------
Bubbles::~Bubbles() {
    for (int i=0; i<position.size(); i++) {
        delete position[i];
    }
}

//------------
void Bubbles::fboParametersChanged(int & newMaxPasses) {
    setupBubblesFbo();
}

//------------
void Bubbles::setupBubblesFbo() {
    ofPushStyle();
    ofSetCircleResolution(1024);
    blur.setup(maxSizeInitial*1.25, maxSizeInitial*1.25);
    for (int passes=1; passes <= maxPasses; passes++) {
        ofFbo fbo;
        fbo.allocate(blur.getWidth(), blur.getHeight(), GL_RGBA);
        fbo.begin();
        ofClear(0, 0);
        ofSetColor(255);
        blur.begin(blurAmt, passes+1);
        ofCircle(fbo.getWidth()/2, fbo.getHeight()/2, maxSizeInitial/2);
        blur.end();
        fbo.end();
        bubbleFbo.push_back(fbo);
    }
    ofPopStyle();
}

//------------
void Bubbles::update() {
    bubbleCreator.setRunning(position.size()<numBubbles);
    
    if (position.size() > numBubbles) {
        int newSize = position.size()-1;
        for (int i=newSize; i<position.size(); i++) {
            delete position[i];
        }
        position.erase(position.begin()+newSize, position.end());
        blurLevel.erase(blurLevel.begin()+newSize, blurLevel.end());
        alpha.erase(alpha.begin()+newSize, alpha.end());
        size.erase(size.begin()+newSize, size.end());
        colorMargin.erase(colorMargin.begin()+newSize, colorMargin.end());
    }
    
    for (int i=0; i<position.size(); i++) {
        alpha[i] = ofLerp(alpha[i], 100 + 100*sin(time + i), 0.03f);
        blurLevel[i] = ofLerp(blurLevel[i], 0.5 + 0.5*sin(1.2*time - i), 0.03f);
        size[i] = ofLerp(size[i], 0.2 + 0.4*sin(1.1*time-5+1.8*i), 0.03f);
        position[i]->setNoiseMin(-1*positionNoiseMargin);
        position[i]->setNoiseMax(positionNoiseMargin);
        position[i]->setNoiseSpeed(positionNoiseFactor);
        position[i]->setSineMin(-1*positionSineMargin);
        position[i]->setSineMax(positionSineMargin);
        position[i]->setSineFreq(positionSineFreq);
        position[i]->setTimeCoefficient(ofVec2f(
            positionTimeConstant->x * (0.5 + 0.5*ofNoise(i, 10)),
            positionTimeConstant->y * (0.5 + 0.5*ofNoise(i, 20))));
        position[i]->setDelTime(speed*10.0);
        position[i]->update();
    }
    
    time += speed;
}

//------------
void Bubbles::draw() {
    ofSetRectMode(OF_RECTMODE_CENTER);
    for (int i=0; i<position.size(); i++) {
        ofSetColor(baseColor->r + colorMargin[i].x * varColor->x,
                   baseColor->g + colorMargin[i].y * varColor->y,
                   baseColor->b + colorMargin[i].z * varColor->z,
                   alpha[i]);
        int idx = (int) (blurAmt * blurLevel[i]);
        
        // draw + wrap
        ofVec2f pos = position[i]->get();
        bubbleFbo[idx].draw((int)(pos.x + width  * (1 + abs(ceil(pos.x / width )))) % width,
                            (int)(pos.y + height * (1 + abs(ceil(pos.y / height)))) % height,
                            maxSize * size[i], maxSize * size[i]);
    }
}