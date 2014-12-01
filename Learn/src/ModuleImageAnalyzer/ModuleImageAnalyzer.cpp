#include "ModuleImageAnalyzer.h"


//-----------
ModuleImageAnalyzer::ModuleImageAnalyzer() : Learn() {
    setGuiInputsVisible(false);
    
    //cam.initGrabber(640, 480);
    
    control.setName("ImageAnalyzer");
    //control.addParameter("velocity lerp", &vLerpRate, 0.0f, 1.0f);
    
    //vLerpRate = 0.2;
}

//-----------
void ModuleImageAnalyzer::update() {
    //cam.update();
	//if(cam.isFrameNew()) {
    //}
    
    Learn::update();
}

//-----------
void ModuleImageAnalyzer::draw() {
    if (visible) {
        Learn::draw();
        
        ofPushMatrix();
        ofTranslate(x, y);
        ofScale(0.5, 0.5);
        //cam.draw(0, 0);
        ofPopMatrix();
    }
}

//-----------
void ModuleImageAnalyzer::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    control.setGuiPosition(x, y);
}

//-----------
void ModuleImageAnalyzer::setVisible(bool visible) {
    this->visible = visible;
    Learn::setVisible(visible);
}

//-----------
void ModuleImageAnalyzer::toggleGuiSummaryView() {
    Learn::setGuiSummaryView(!summaryVisible);
    visible = summaryVisible;
}

//-----------
ModuleImageAnalyzer::~ModuleImageAnalyzer() {
    //cam.close();
}