#include "MoviePlayer.h"

//--------
void MoviePlayer::setup() {
    setName(typeid(this).name());
    
    control.registerParameter("speed", &speed, -3.0f, 3.0f);
    
    player.loadMovie("/Users/Gene/Media/TheRub/bubbles1.mov");
    player.play();

    speed = 1.0f;
    oldSpeed = speed;
    
    w = width;
    h = player.getHeight() * w / player.getWidth();
}

//--------
void MoviePlayer::update() {
    if (speed != oldSpeed) {
        player.setSpeed(speed);
        oldSpeed = speed;
    }
    player.update();
}

//--------
void MoviePlayer::draw() {
    player.draw(0, 0.5 * (height - h), w, h);
}

