#include "Syphon.h"


//--------
void Syphon::setup(int width, int height){
    Scene::setup(width, height);
    client.setup();
}

//--------
void Syphon::setClient(string serverName, string appName) {
    client.set(serverName, appName);
}

//--------
void Syphon::update() {

}

//--------
void Syphon::draw() {
    client.draw(0, 0, width, height);
}

