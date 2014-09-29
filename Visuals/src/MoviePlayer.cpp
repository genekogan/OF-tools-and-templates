#include "MoviePlayer.h"

//--------
void MoviePlayer::setup() {
    setName("MoviePlayer");
    
    control.addParameter("speed", &speed, -3.0f, 3.0f);
    control.addEvent("jump", this, &MoviePlayer::jumpBack);
    control.addEvent("random", this, &MoviePlayer::jumpRandom);
    control.addEvent("play", this, &MoviePlayer::play);
    playing = false;
    
    speed = 1.0f;
    oldSpeed = speed;
}

//--------
void MoviePlayer::play(string &s) {
    playing = true;
    player.loadMovie("/Users/Gene/media/sinuses.mov");
    player.play();
    
    /*
     w = width;
     h = player.getHeight() * w / player.getWidth();
     */
    
    h = height;
    w = player.getWidth() * h / player.getHeight();
}

//--------
void MoviePlayer::jumpBack(string &s) {
    player.setFrame(player.getCurrentFrame()-15);
}

//--------
void MoviePlayer::jumpRandom(string &s) {
    player.setFrame(ofRandom(player.getTotalNumFrames()));  
}

//--------
void MoviePlayer::update() {
    if (!playing)   return;
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

