#include "ModuleImageAnalyzerLearn.h"


//-----------
ModuleImageAnalyzerLearn::ModuleImageAnalyzerLearn() : Learn() {
    setGuiInputsVisible(false);
    
    //cam.initGrabber(640, 480);
    
    control.setName("ImageAnalyzer");
    //control.addParameter("velocity lerp", &vLerpRate, 0.0f, 1.0f);
    
    //vLerpRate = 0.2;
}

//-----------
void ModuleImageAnalyzerLearn::update() {
    //cam.update();
	//if(cam.isFrameNew()) {
    //}
    
    Learn::update();
}

//-----------
void ModuleImageAnalyzerLearn::draw() {
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
void ModuleImageAnalyzerLearn::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    control.setGuiPosition(x, y);
}

//-----------
void ModuleImageAnalyzerLearn::setVisible(bool visible) {
    this->visible = visible;
    Learn::setVisible(visible);
}

//-----------
void ModuleImageAnalyzerLearn::toggleGuiSummaryView() {
    Learn::setGuiSummaryView(!summaryVisible);
    visible = summaryVisible;
}

//-----------
ModuleImageAnalyzerLearn::~ModuleImageAnalyzerLearn() {
    //cam.close();
}