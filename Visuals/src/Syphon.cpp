#include "Syphon.h"


//--------
void Syphon::setup() {
    setName(typeid(this).name());
    client.setup();
}

//--------
void Syphon::setClient(string serverName, string appName) {
    client.set(serverName, appName);
    setName(appName);
}

//--------
void Syphon::update() {

}

//--------
void Syphon::draw() {
    client.draw(0, 0, width, height);
}

