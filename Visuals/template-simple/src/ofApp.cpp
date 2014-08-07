#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    debug = new DebugScreen();
    rivers = new Rivers();
    agents = new Agents();
    movie = new MoviePlayer();
    shader = new Shader();
    syphon = new Syphon();
    letters = new Letters();
    amoeba = new Amoeba();
    bubbles = new Bubbles();
    subdivide = new Subdivide();
    
    debug->setup(1024, 768);
    rivers->setup(1024, 768);
    agents->setup(1024, 768);
    movie->setup(1024, 768);
    letters->setup(1024, 768);
    amoeba->setup(1024, 768);

    shader->setup(1024, 768);
    ((Shader *) shader)->setupBlobby();

    syphon->setup(1024, 768);
    ((Syphon *) syphon)->setClient("here", "there");

    bubbles->setup(1024, 768);
    subdivide->setup(1024, 768);
    
    disableAll();
    subdivide->setActive(true);
        
}

//-----------
void ofApp::update(){
}

//-----------
void ofApp::draw(){
//    ofClear(0, 0);

    if (debug->getActive()) debug->draw(0, 0);
    if (rivers->getActive()) rivers->draw(0, 0);
    if (agents->getActive()) agents->draw(0, 0);
    if (movie->getActive()) movie->draw(0, 0);
    if (shader->getActive()) shader->draw(0, 0);
    if (syphon->getActive()) syphon->draw(0, 0);
    if (letters->getActive()) letters->draw(0, 0);
    if (amoeba->getActive()) amoeba->draw(0, 0);
    if (bubbles->getActive()) bubbles->draw(0, 0);
    if (subdivide->getActive()) subdivide->draw(0, 0);
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='a') subdivide->setup();
        
    if      (key=='1') ((Shader *) shader)->setupBits();
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
    
    
    else if (key=='q') { disableAll(); debug->setActive(true); }
    else if (key=='w') { disableAll(); rivers->setActive(true); }
    else if (key=='e') { disableAll(); agents->setActive(true); }
    else if (key=='r') { disableAll(); movie->setActive(true); }
    else if (key=='t') { disableAll(); shader->setActive(true); }
    else if (key=='y') { disableAll(); syphon->setActive(true); }
    else if (key=='u') { disableAll(); letters->setActive(true); }
    else if (key=='i') { disableAll(); amoeba->setActive(true); }
    else if (key=='o') { disableAll(); bubbles->setActive(true); }
}