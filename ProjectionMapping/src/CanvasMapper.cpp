#include "CanvasMapper.h"


//---------
CanvasMapper::CanvasMapper() {
    mappingResolution = ofVec2f(1.0, 1.0);
}

//---------
void CanvasMapper::addCanvas(Canvas *canvas) {
    canvases.push_back(canvas);
    addQuad(canvas->getWidth(), canvas->getHeight());
}

//---------
void CanvasMapper::setMappingResolution(int width1, int height1, int width2, int height2) {
    setMouseResolution(width2, height2);
    mappingResolution = ofVec2f((float)width1/(float)width2, (float)height1/(float)height2);
}

//---------
void CanvasMapper::toggleDebug() {
    ProjectionMapping::toggleDebug();
    if (getDebug()) {
        for (int i=0; i<canvases.size(); i++) {
            canvases[i]->setVisible(false);
        }
    }
    else {
        if (canvases.size() > 0) {
            canvases[0]->setVisible(true);
        }
    }
}

//---------
void CanvasMapper::drawDebug() {
    ofPushMatrix();
    ofScale(mappingResolution.x, mappingResolution.y);
    draw();
    ofPopMatrix();
}

//---------
void CanvasMapper::draw() {
    for (int i=0; i<canvases.size(); i++) {
        begin(i);
        canvases[i]->draw(0, 0);
        end(i);
    }
}