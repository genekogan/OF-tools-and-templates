#include "Rivers.h"


//--------
void Streak::setup(float x, float y, int maxAge, float speed,
                   int maxThickness, float strokeAlpha,
                   int width, int height, ofColor color){
    pos.set(x, y);
    prev.set(x, y);
    age = 0;

    this->width = width;
    this->height = height;
    this->maxAge = maxAge;
    this->speed = speed;
    this->maxThickness = maxThickness;
    this->strokeAlpha = strokeAlpha;
    this->color = color;
}

//--------
void Streak::update(ofPoint vel){
    prev.set(pos.x, pos.y);
    pos += (speed * vel);
    age++;
    if (pos.x > width  || pos.x < 0 ||
        pos.y > height || pos.y < 0) {
        age = maxAge;
    }
}

//--------
void Streak::draw(){
    float size = ofMap(abs(age - 0.5 * maxAge), 0.5 * maxAge, 0, 0, maxThickness);
    float alph = ofMap(abs(age - 0.5 * maxAge), 0.5 * maxAge, 0, 1, strokeAlpha);
    ofSetColor(color.r, color.g, color.b, alph);
    ofEllipse(pos.x, pos.y, size, size);
}

//--------
bool Streak::isActive() {
    return age<maxAge;
}



//--------
void Rivers::setup(){
    setName("Rivers");
    
    numStreaks = 1500;
    complexity = 0.01;
    baseSpeed = 1.0;
    speedVar = 0.5;
    minAge = 0.0;
    maxAge = 120.0;
    refreshAlpha = 5;
    streakAlpha = 50;
    maxThickness = 8;
    color = ofColor(255);
    colorVar = 10;
    
    control.addParameter("numStreaks", &numStreaks, 1, 60000);
    control.addParameter("complexity", &complexity, 0.001f, 0.1f);
    control.addParameter("baseSpeed", &baseSpeed, -5.0f, 5.0f);
    control.addParameter("speedVar", &speedVar, 0.0f, 1.0f);
    control.addParameter("maxAge", &minAge, 0, 900);
    control.addParameter("maxAge", &maxAge, 1, 1000);
    control.addParameter("refreshAlpha", &refreshAlpha, 0, 255);
    control.addParameter("streakAlpha", &streakAlpha, 0, 255);
    control.addParameter("maxThickness", &maxThickness, 0.1f, 36.0f);
    control.addColor("color", &color);
    control.addParameter("colorVar", &colorVar, 0, 200);
    control.addEvent("refresh", this, &Rivers::setupForceField);

    string s;
    setupForceField(s);
    for (int i=0; i<numStreaks; i++) {
        addNewStreak();
    }
}

//--------
void Rivers::setupForceField(string &s) {
    noiseSeed = ofRandom(100);
    float nx, ny;
    for (int i=0; i<FORCE_RESOLUTION; i++) {
        for (int j=0; j<FORCE_RESOLUTION; j++) {
            nx = ofMap(ofNoise(complexity * i, complexity * j, noiseSeed),
                       0, 1, -1, 1);
            ny = ofMap(ofNoise(complexity * i, complexity * j, noiseSeed+5),
                       0, 1, -1, 1);
            force[i][j].set(nx, ny);
        }
    }
}

//--------
void Rivers::setupStreak(Streak &streak) {
    int x = ofRandom(width);
    int y = ofRandom(height);
    int age = ofRandom(minAge, maxAge);
    float speed = ofRandom(1.0-speedVar, 1.0+speedVar) * baseSpeed;
    ofColor newColor = ofColor(ofClamp(color.r + ofRandom(-colorVar,colorVar), 0, 255),
                               ofClamp(color.g + ofRandom(-colorVar,colorVar), 0, 255),
                               ofClamp(color.b + ofRandom(-colorVar,colorVar), 0, 255));
    streak.setup(x, y, age, speed,
                 maxThickness, streakAlpha,
                 width, height, newColor);
}

//--------
void Rivers::addNewStreak(){
    Streak streak; // = new Streak();
    setupStreak(streak);
    streaks.push_back(streak);
}

//--------
void Rivers::update(){
    while (streaks.size() < numStreaks) {
        addNewStreak();
        
    }
    while (streaks.size() > numStreaks) {
        streaks.pop_back();
    }
    
    vector<Streak>::iterator streak = streaks.begin();
    while (streak != streaks.end()) {
        int ix = ofClamp(ofMap((*streak).pos.x, 0, width, 0, FORCE_RESOLUTION), 0, FORCE_RESOLUTION-1);
        int iy = ofClamp(ofMap((*streak).pos.y, 0, height, 0, FORCE_RESOLUTION), 0, FORCE_RESOLUTION-1);
        (*streak).update(force[ix][iy]);
        if (!(*streak).isActive()) {
            setupStreak(*streak);            
        }
        streak++;
    }
}

//--------
void Rivers::draw() {
    for (vector<Streak>::iterator it = streaks.begin(); it != streaks.end(); ++it){
        (*it).draw();
    }
}