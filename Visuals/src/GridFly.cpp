#include "GridFly.h"


//-------
void GridFly::setup(){
    setName("GridFly");
    
    control.registerParameter("nx", &nx, 10, 400);
    control.registerParameter("ny", &ny, 10, 400);
    control.registerParameter("length", &length, ofVec2f(100,100), ofVec2f(4000,4000));
    control.registerParameter("margin", &margin, 0.0f, 1000.0f);
    control.registerParameter("noiseFactor", &noiseFactor, ofVec2f(0, 0), ofVec2f(0.01, 0.01));
    control.registerParameter("speed", &speed, 0.0f, 100.0f);
    control.registerParameter("angle", &angle, 0.0f, 360.0f);
    control.registerParameter("axis", &axis, ofVec3f(0, -1, -1), ofVec3f(1, 1, 1));
    control.registerParameter("color", &color, ofColor(0, 0), ofColor(255, 255));
    
    nx = 100;
    ny = 100;
    length = ofVec2f(2000, 2000);
    margin = 100;
    noiseFactor = ofVec2f(0.001, 0.001);
    speed = 20;
    angle = 90;
    axis = ofVec3f(1, 0, 0);
    color = ofColor(255, 100);

    vertices = &mesh.getVertices();
    for (int j=0; j<ny; j++) {
        addRow();
        y++;
    }
    minY = -length.get().y;
    dy = length.get().y / ny;
    y = 0;
}

//-------
void GridFly::update(){
    Z += speed;
    
    cam.resetTransform();
    cam.rotate(angle, axis.get().x, axis.get().y, axis.get().z);
    cam.setPosition(camPosition.x, Z, camPosition.y);
    
    if (vertices->size() > nx*ny*6) {
        vertices->erase(vertices->begin(), vertices->begin() + (vertices->size() - nx*ny));
    }
    while (cam.getPosition().y > minY) {
        vertices->erase(vertices->begin(), vertices->begin() + nx*6);
        minY = vertices->at(0).y;
        addRow();
        y++;
    }
}

//-------
void GridFly::addRow() {
    for (int x=0; x<nx; x++) {
        
        float x1 = ofMap(x, 0, nx, -0.5*length.get().x, 0.5*length.get().x);
        float x2 = ofMap(x+1, 0, nx, -0.5*length.get().x, 0.5*length.get().x);
        float y1 = ofMap(y, 0, ny, -length.get().y, 0);
        float y2 = ofMap(y+1, 0, ny, -length.get().y, 0);
        
        float z11 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.get().x * x1, noiseFactor.get().y * y1));
        float z12 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.get().x * x1, noiseFactor.get().y * y2));
        float z21 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.get().x * x2, noiseFactor.get().y * y1));
        float z22 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.get().x * x2, noiseFactor.get().y * y2));
        
        mesh.addVertex(ofVec3f(x1, y1, z11));
        mesh.addVertex(ofVec3f(x2, y1, z21));
        mesh.addVertex(ofVec3f(x2, y2, z22));
        
        mesh.addVertex(ofVec3f(x1, y1, z11));
        mesh.addVertex(ofVec3f(x1, y2, z12));
        mesh.addVertex(ofVec3f(x2, y2, z22));
    }
}

//-------
void GridFly::draw(){
    ofNoFill();
    ofSetColor(color);
    cam.begin();
    mesh.drawWireframe();
    cam.end();
}
