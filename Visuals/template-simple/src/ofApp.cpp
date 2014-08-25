#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    autoUpdate = false;
    
    int w = 1024;
    int h = 768;
    
    //----- Agents
    agents = new Agents();
    agents->setup(w, h);

    //----- Amoeba
    amoeba = new Amoeba();
    amoeba->setup(w, h);
    
    //----- Bubbles
    bubbles = new Bubbles();
    bubbles->setup(w, h);
    
    //----- Cubes
    cubes = new Cubes();
    cubes->setup(w, h);
    
    //----- Debug
    debug = new DebugScreen();
    debug->setup(w, h);
    
    //----- GridFly
    gridfly = new GridFly();
    gridfly->setup(w, h);
    
    //----- Letters
    letters = new Letters();
    letters->setup(w, h);
    
    //----- Meshy
    meshy = new Meshy();
    meshy->setup(w, h);
    
    //----- Movie
    movie = new MoviePlayer();
    movie->setup(w, h);
    
    //----- Polar
    polar = new Polar();
    polar->setup(w, h);
    
    //----- Rivers
    rivers = new Rivers();
    rivers->setup(w, h);
    
    //----- Shader
    shader = new Shader();
    shader->setup(w, h);
    ((Shader *) shader)->setupIkeda();
    
    //----- Shapespace
    shapespace = new ShapeSpace();
    shapespace->setup(w, h);
    
    //----- Subdivide
    subdivide = new Subdivide();
    subdivide->setup(w, h);
    
    //----- Syphon
    syphon = new Syphon();
    syphon->setup(w, h);
    ((Syphon *) syphon)->setClient("serverName", "appName");
    
    
    disableAll();
    shader->setActive(true);
}

//-----------
void ofApp::update(){
    if (autoUpdate && shader->getActive()) {
        if (ofGetFrameNum() % 60 == 0) {
            ((Shader *) shader)->setupIkeda();
        }
    }
}

//-----------
void ofApp::draw() {
    if (agents->getActive())     agents->draw(0, 0);
    if (amoeba->getActive())     amoeba->draw(0, 0);
    if (bubbles->getActive())    bubbles->draw(0, 0);
    if (cubes->getActive())      cubes->draw(0, 0);
    if (debug->getActive())      debug->draw(0, 0);
    if (gridfly->getActive())    gridfly->draw(0, 0);
    if (letters->getActive())    letters->draw(0, 0);
    if (meshy->getActive())      meshy->draw(0, 0);
    if (movie->getActive())      movie->draw(0, 0);
    if (polar->getActive())      polar->draw(0, 0);
    if (rivers->getActive())     rivers->draw(0, 0);
    if (shader->getActive())     shader->draw(0, 0);
    if (shapespace->getActive()) shapespace->draw(0, 0);
    if (subdivide->getActive())  subdivide->draw(0, 0);
    if (syphon->getActive())     syphon->draw(0, 0);
}

//-----------
void ofApp::keyPressed(int key){
    if      (key=='q') { disableAll(); agents->setActive(true); }
    else if (key=='w') { disableAll(); amoeba->setActive(true); }
    else if (key=='e') { disableAll(); bubbles->setActive(true); }
    else if (key=='r') { disableAll(); cubes->setActive(true); }
    else if (key=='t') { disableAll(); debug->setActive(true); }
    else if (key=='y') { disableAll(); gridfly->setActive(true); }
    else if (key=='u') { disableAll(); letters->setActive(true); }
    else if (key=='i') { disableAll(); meshy->setActive(true); }
    else if (key=='o') { disableAll(); movie->setActive(true); }
    else if (key=='p') { disableAll(); polar->setActive(true); }
    else if (key=='a') { disableAll(); rivers->setActive(true); }
    else if (key=='s') { disableAll(); shader->setActive(true); }
    else if (key=='d') { disableAll(); shapespace->setActive(true); }
    else if (key=='f') { disableAll(); subdivide->setActive(true); }
    else if (key=='g') { disableAll(); syphon->setActive(true); }

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
        else if (key=='=') ((Shader *) shader)->setupIkeda();
        else if (key=='~') ((Shader *) shader)->setupHerokuBubbles();
        else if (key=='!') ((Shader *) shader)->setupNebula();
        else if (key=='@') ((Shader *) shader)->setupFractalScope();
        else if (key=='#') ((Shader *) shader)->setupFractalFlower();
        else if (key=='$') ((Shader *) shader)->setupCurtains();
    }
}