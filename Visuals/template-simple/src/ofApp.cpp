#include "ofApp.h"

//-----------
void ofApp::setup(){
    
    ofSetBackgroundAuto(false);
    
    debug = new DebugScreen();
    debug->setup(1024, 768);
    
    rivers = new Rivers();
    rivers->setup(1024, 768);
    
    agents = new Agents();
    agents->setup(1024, 768);
    
    movie = new MoviePlayer();
    movie->setup(1024, 768);
    
    shader = new Shader();
    shader->setup(1024, 768);
    ((Shader *) shader)->setupFractalFlower();
    
    syphon = new Syphon();
    syphon->setup(1024, 768);
    ((Syphon *) syphon)->setClient("serverName", "appName");
    
    letters = new Letters();
    letters->setup(1024, 768);
    
    amoeba = new Amoeba();
    amoeba->setup(1024, 768);
    
    bubbles = new Bubbles();
    bubbles->setup(1024, 768);
    
    subdivide = new Subdivide();
    subdivide->setup(1024, 768);
    
    polar = new Polar();
    polar->setup(1024, 768);
    
    cubes = new Cubes();
    cubes->setup(1024, 768);
    
    gridfly = new GridFly();
    gridfly->setup(1024, 768);
    
    shapespace = new ShapeSpace();
    shapespace->setup(1024, 768);
    
    
    disableAll();
    shapespace->setActive(true);
    
}

//-----------
void ofApp::update(){
    
    if (autoUpdate){
        if (ofGetFrameNum() % 60 == 0) {
            //            ((Shader *) shader)->setupNew4();
        }
    }
    
}

//-----------
void ofApp::draw(){
    
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
    if (polar->getActive()) polar->draw(0, 0);
    if (cubes->getActive()) cubes->draw(0, 0);
    if (gridfly->getActive()) gridfly->draw(0, 0);
    if (shapespace->getActive()) shapespace->draw(0, 0);
    
    if (autoUpdate) { ofSetColor(255, 0, 0); ofEllipse(10, 10, 20, 20); }
}

//-----------
void ofApp::keyPressed(int key){
    if (key==' ') { autoUpdate = !autoUpdate; }
    if (key=='a') subdivide->setup();
    
    if (shader->getActive()) {
        if      (key=='`') ((Shader *) shader)->setupBits();
        else if (key=='1') ((Shader *) shader)->setupBitsExperimental();
        else if (key=='2') ((Shader *) shader)->setupBlobby();
        else if (key=='3') ((Shader *) shader)->setupElectro();
        else if (key=='4') ((Shader *) shader)->setupEye();
        else if (key=='5') ((Shader *) shader)->setupLandscape();
        else if (key=='6') ((Shader *) shader)->setupMonjori();
        else if (key=='7') ((Shader *) shader)->setupRain();
        else if (key=='8') ((Shader *) shader)->setupWave();
        else if (key=='9') ((Shader *) shader)->setupSinewave();
        else if (key=='0') ((Shader *) shader)->setupSinewaveExperimental();
        else if (key=='-') ((Shader *) shader)->setupNoisy();
        else if (key=='=') ((Shader *) shader)->setupHerokuBubbles();
        else if (key=='+') ((Shader *) shader)->setupNebula();
        else if (key=='!') ((Shader *) shader)->setupFractalScope();
        else if (key=='@') ((Shader *) shader)->setupFractalFlower();
        else if (key=='#') ((Shader *) shader)->setupCurtains();
        else if (key=='x')  shader->setUpsideDown(true);
    }
    
    
    if      (key=='q') { disableAll(); debug->setActive(true); }
    else if (key=='w') { disableAll(); rivers->setActive(true); }
    else if (key=='e') { disableAll(); agents->setActive(true); }
    else if (key=='r') { disableAll(); movie->setActive(true); }
    else if (key=='t') { disableAll(); shader->setActive(true); }
    else if (key=='y') { disableAll(); syphon->setActive(true); }
    else if (key=='u') { disableAll(); letters->setActive(true); }
    else if (key=='i') { disableAll(); amoeba->setActive(true); }
    else if (key=='o') { disableAll(); bubbles->setActive(true); }
    else if (key=='p') { disableAll(); polar->setActive(true); }
    else if (key=='[') { disableAll(); cubes->setActive(true); }
    else if (key==']') { disableAll(); gridfly->setActive(true); }
    else if (key=='\\') { disableAll(); shapespace->setActive(true); }
    
    
    if      (key=='1')   ((Cubes *) cubes)->preset1();
    else if (key=='2')   ((Cubes *) cubes)->preset2();
    else if (key=='3')   ((Cubes *) cubes)->preset3();
    else if (key=='4')   ((Cubes *) cubes)->preset4();
    else if (key=='5')   ((Cubes *) cubes)->preset5();
    else if (key=='6')   ((Cubes *) cubes)->preset6();
    else if (key=='7')   ((Cubes *) cubes)->preset7();
    
    
    if (key=='g') {
        cubes->toggeVisible();
    }
}