#include "MoviePlayer.h"

//--------
void MoviePlayer::setup() {
    setName("MoviePlayer");
    
    control.registerParameter("speed", &speed, -3.0f, 3.0f);
    control.registerEvent("jump", this, &MoviePlayer::jumpBack);
    control.registerEvent("random", this, &MoviePlayer::jumpRandom);
    
    //player.loadMovie("/Users/Gene/Media/TheRub/bubbles1.mov");
    player.loadMovie("/Users/Gene/Media/sinuses.mov");
    player.play();

    speed = 1.0f;
    oldSpeed = speed;
    
    /*
    w = width;
    h = player.getHeight() * w / player.getWidth();
     */
    
    h = height;
    w = player.getWidth() * h / player.getHeight();

}

//--------
void MoviePlayer::jumpBack() {
    player.setFrame(player.getCurrentFrame()-15);
}

//--------
void MoviePlayer::jumpRandom() {
    player.setFrame(ofRandom(player.getTotalNumFrames()));  
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

