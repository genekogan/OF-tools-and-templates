#include "Polar.h"

void Polar::setup() {
    setName("Polar");

    control.registerParameter("a", &a, 0.0f, 500.0f);
    control.registerParameter("b", &b, 0.0f, 100.0f);
    control.registerParameter("dangmax", &dangmax, 0.0f, 2.0f);
    control.registerParameter("w1", &w1, 0.0005f, 0.0015f);
    control.registerParameter("w2", &w2, 0.0005f, 0.0015f);
    control.registerParameter("w3", &w3, 0.0005f, 0.0015f);
    


    p = new PolarEq();
}

void Polar::update() {
    
    
    p->setA(a);
    p->setB(b);
    p->setDangMax(dangmax);
    p->setW1(w1);
    p->setW2(w2);
    p->setW3(w3);

    p->update();
    
}

void Polar::draw() {
    p->draw();
}