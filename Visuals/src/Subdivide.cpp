#include "Subdivide.h"

Subdivision::Subdivision(int generation, int x, int y, int width, int height, ofParameter<ofColor> *color, ofParameter<ofColor> *varColor) {
    this->generation = generation;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->varColor = varColor;
    parent = NULL;
    subdivide();

}

Subdivision::Subdivision(int generation, Subdivision *parent, bool topleft) {
    this->generation = generation;
    this->parent = parent;
    this->topleft = topleft;
    color = new ofParameter<ofColor>();
    varColor = parent->varColor;
    offset = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    subdivide();
    update();
}

void Subdivision::subdivide() {
    if (generation > 0) {
        ratio = ofRandom(1);
        horizontal = ofRandom(1) < 0.5 ? true : false;
        child1 = new Subdivision(generation-1, this, true);
        child2 = new Subdivision(generation-1, this, false);
    }
}

void Subdivision::update() {
    if (parent == NULL) return;
    if (parent->horizontal) {
        x = parent->x + (topleft ? 0.0 : parent->width * parent->ratio);
        y = parent->y;
        width = parent->width * (topleft ? parent->ratio : 1.0 - parent->ratio);
        height = parent->height;
    }
    else {
        y = parent->y + (topleft ? 0.0 : parent->height * parent->ratio);
        x = parent->x;
        width = parent->width;
        height = parent->height * (topleft ? parent->ratio : 1.0-parent->ratio);
    }
    color->set(ofColor(parent->color->get().r + varColor->get().r * offset.x,
                       parent->color->get().g + varColor->get().g * offset.y,
                       parent->color->get().b + varColor->get().b * offset.z));
}

void Subdivision::draw() {
    update();
    if (generation == 0) {
        ofSetColor(*color);
        ofFill();
        ofRect(x, y, width, height);
        ofNoFill();
        ofSetColor(0);
        ofSetLineWidth(1);
        ofRect(x, y, width, height);
    }
    else {
        child1->draw();
        child2->draw();
    }
}


void Subdivide::setup() {
    setName(typeid(this).name());

    control.registerParameter("color", &color, ofColor(0), ofColor(255));
    control.registerParameter("varColor", &varColor, ofColor(0), ofColor(255));

    
    start = new Subdivision(11, 0, 0, width, height, &color, &varColor);
//    start->setup();
}

void Subdivide::update() {
    
}

void Subdivide::draw() {
    start->draw();
}