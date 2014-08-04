#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    debug = new DebugScreen();
    rivers = new Rivers();
    agents = new Agents();
    movie = new MoviePlayer();
    shader = new Shader();
    
    debug->setup(ofGetWidth()/2.0, ofGetHeight()/2.0);
    rivers->setup(ofGetWidth()/2.0, ofGetHeight()/2.0);
    agents->setup(ofGetWidth()/2.0, ofGetHeight()/2.0);
    movie->setup(ofGetWidth()/2.0, ofGetHeight()/2.0);
    
    shader->setup(ofGetWidth()/2.0, ofGetHeight()/2.0);
    ((Shader *) shader)->setupBlobby();
    
    rivers->setGuiPosition(200, 10);
    agents->setGuiPosition(400, 10);
    movie->setGuiPosition(600, 10);
    shader->setGuiPosition(800, 10);
}

//-----------
void ofApp::update(){
    
}

//-----------
void ofApp::draw(){
    //debug->draw(0, 0);
    shader->draw(0, 0);
    rivers->draw(ofGetWidth()/2.0, 0);
    agents->draw(0, ofGetHeight()/2.0);
    movie->draw(ofGetWidth()/2.0, ofGetHeight()/2.0);
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='g') {
        rivers->toggeVisible();
        agents->toggeVisible();
        debug->toggeVisible();
        movie->toggeVisible();
        shader->toggeVisible();
    }
    
    else if (key=='1') ((Shader *) shader)->setupBits();
    else if (key=='2') ((Shader *) shader)->setupBlobby();
    else if (key=='3') ((Shader *) shader)->setupElectro();
    else if (key=='4') ((Shader *) shader)->setupEye();
    else if (key=='5') ((Shader *) shader)->setupLandscape();
    else if (key=='6') ((Shader *) shader)->setupMonjori();
    else if (key=='7') ((Shader *) shader)->setupRain();
    else if (key=='8') ((Shader *) shader)->setupWave();
    else if (key=='9') ((Shader *) shader)->setupSinewave();
    else if (key=='0') ((Shader *) shader)->setupNoisy();
    else if (key=='-') ((Shader *) shader)->setupHerokuBubbles();


}