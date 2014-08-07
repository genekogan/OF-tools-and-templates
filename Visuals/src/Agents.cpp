#include "Agents.h"


//---------
void Agents::setup() {
    setName(typeid(this).name());

    control.registerParameter("numAgents", &numAgents, 1, 120);
    control.registerParameter("speed", &speed, -5.0f, 5.0f);
    control.registerParameter("size", &size, 30, 800);
    
    control.registerParameter("color", &color, ofColor(0), ofColor(255));
    control.registerParameter("fillAlpha", &fillAlpha, 0, 255);
    control.registerParameter("strokeAlpha", &strokeAlpha, 0, 255);

    control.registerParameter("t1", &t1, 80.0f, 1000.0f);
    control.registerParameter("t2", &t2, 1.0f, 10.0f);
    
    control.registerParameter("wrap", &wrapping);
    control.registerParameter("debug", &debug);
    
    numAgents = 16;
    wrapping = true;
    debug = false;
    speed = 1.0;
    size = 200;
    color = ofColor(255);
    fillAlpha = 25;
    strokeAlpha = 25;
    t1 = 300;
    t2 = 2;

    for (int i=0; i<numAgents; i++) {
        addNewAgent();
    }
}

//---------
void Agents::update() {
    numAgents = (int) numAgents;
    
    while (agents.size() < numAgents) {
        addNewAgent();
    }
    while (agents.size() > numAgents) {
        agents.pop_back();
    }
    
    for (int i=0; i<numAgents; i++) {
        agents[i].setSpeed(speed);
        agents[i].setSize(size);
        agents[i].update();
        if (wrapping) {
            agents[i].wrap();
        }
    }
    for (int i=0; i<numAgents; i++) {
        for (int j=i+1; j<numAgents; j++) {
            bool intersecting = agents[i].isIntersecting(&agents[j]);
            if (intersecting) {
                agents[i].addIntersectingAgent(&agents[j]);
                agents[j].addIntersectingAgent(&agents[i]);
            }
        }
    }
}

//---------
void Agents::refresh() {
    ofBackground(0);
}

//---------
void Agents::addNewAgent(){
    Agent agent;
    agent.setup(ofRandom(0, width), ofRandom(0, height), ofRandom(1), width, height);
    agents.push_back(agent);
}

//---------
void Agents::drawDebug(){
    ofBackground(0);
    for (int i=0; i<numAgents; i++) {
        agents[i].draw();
    }
}

//---------
void Agents::draw(){
    if (debug) {
        drawDebug();
        return;
    }
    
    // draw lines between all intersecting agents
    if (type == LINES) {
        for (int i=0; i<numAgents; i++) {
            if (agents[i].getIntersecting().size() > 0) {
                for (int j=0; j<agents[i].getIntersecting().size(); j++) {
                    Agent *a = agents[i].getIntersecting()[j];
                    ofSetColor(color, fillAlpha);
                    int t = ofGetFrameNum() + agents[i].tOffset;
                    if ((t - (t % (int) t1)) % ((int)t2 * (int)t1) == 0) {                        
                        ofSetColor(color, fillAlpha);
                        ofLine(agents[i].pos.x, agents[i].pos.y, a->pos.x, a->pos.y);
                    }
                }
            }
        }   
    }
    
    // when intersecting exactly with two agents, draw triangle
    else if (type == TRIANGLES) {
        for (int i=0; i<numAgents; i++) {
            if (agents[i].getIntersecting().size() == 2) {
                Agent *a1 = agents[i].getIntersecting()[0];
                Agent *a2 = agents[i].getIntersecting()[1];
                int t = ofGetFrameNum() + agents[i].tOffset;
                if ((t - (t % (int) t1)) % ((int)t2 * (int)t1) == 0) {                    
                    ofSetColor(color, fillAlpha);
                    ofFill();
                    ofTriangle(agents[i].pos.x, agents[i].pos.y,
                               a1->pos.x, a1->pos.y, a2->pos.x, a2->pos.y);
                    
                    ofSetColor(0, strokeAlpha);
                    ofNoFill();
                    ofTriangle(agents[i].pos.x, agents[i].pos.y,
                               a1->pos.x, a1->pos.y, a2->pos.x, a2->pos.y);
                }
                
            }
        }        
    }
}


//---------
void Agent::setup(int x, int y, float baseRad, int width, int height) {
    this->width = width;
    this->height = height;
    pos.set(x, y);
    vel.set(ofRandom(-1, 1), ofRandom(-1, 1));
    this->baseRad = baseRad;
    size = 50.0;
    speed = 1.0;
    rad = baseRad * size;
    tOffset = (int) ofRandom(1000);
}

//---------
void Agent::setSpeed(float speed) {
    this->speed = speed;
}

//---------
void Agent::setSize(float size) {
    this->size = size;
}

//---------
void Agent::update() {
    intersecting.clear();
    rad = baseRad * size;
    pos += (speed * vel);
}

//---------
bool Agent::isIntersecting(Agent *other) {
    bool isIntersecting = false;
    if ((pos.x - other->pos.x)*(pos.x - other->pos.x) +
        (pos.y - other->pos.y)*(pos.y - other->pos.y) <
        (rad + other->rad)*(rad + other->rad)) {
        isIntersecting = true;
    }
    return isIntersecting;
}

//---------
vector<Agent *> Agent::getIntersecting() {
    return intersecting;
}

//---------
void Agent::addIntersectingAgent(Agent *other) {
    intersecting.push_back(other);
}

//---------
void Agent::wrap() {
    if      (pos.x < 0)             pos.set(width, pos.y);
    else if (pos.x > width)         pos.set(0, pos.y);
    if      (pos.y < 0)             pos.set(pos.x , height);
    else if (pos.y > height)        pos.set(pos.x, 0);
}

//---------
void Agent::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofSetColor(255);
    ofNoFill();
    ofEllipse(0, 0, 2*rad, 2*rad);
    ofRotate(ofRadToDeg(atan(vel.y / vel.x) + (vel.x < 0 ? PI : 0)), 0, 0, 1);
    ofLine(0, 0, rad, 0);
    ofPopMatrix();
}


