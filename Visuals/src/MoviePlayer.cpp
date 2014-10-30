#include "MoviePlayer.h"

//--------
void MoviePlayer::setup() {
    setName("MoviePlayer");
    
    playing = false;
    speed = 1.0f;
    oldSpeed = speed;

    control.addParameter("speed", &speed, -3.0f, 3.0f);
    control.addEvent("jump", this, &MoviePlayer::jumpBack);
    control.addEvent("random", this, &MoviePlayer::jumpRandom);
    control.addEvent("load", this, &MoviePlayer::selectMedia);
    
    loadMovie("/Users/Gene/media/sinuses.mov");
}

//--------
void MoviePlayer::selectMedia(string &s) {
    ofFileDialogResult file = ofSystemLoadDialog();
    if (file.bSuccess) {
        vector<string> fileSplit = ofSplitString(file.filePath, ".");
        string extension = ofToLower(fileSplit[fileSplit.size()-1]);
        cout << extension << endl;
        if (extension == "png" || extension == "jpg" || extension == "jpeg") {
            loadImage(file.filePath);
        }
        else if (extension == "mov" || extension == "mp4"){
            loadMovie(file.filePath);
        }
    }
}

//--------
void MoviePlayer::loadMovie(string path) {
    mode = MOVIE;

    playing = true;
    player.loadMovie(path);
    player.play();
    

    if ((float)width/height > (float)player.getWidth()/player.getHeight()) {
        h = height;
        w = player.getWidth() * h / player.getHeight();
        centeredHoriz = true;
    }
    else {
        w = width;
        h = player.getHeight() * w / player.getWidth();
        centeredHoriz = false;
    }
}

//--------
void MoviePlayer::loadImage(string path) {
    mode = IMAGE;

    playing = false;
    img.loadImage(path);
    
    if ((float)width/height > (float)img.getWidth()/img.getHeight()) {
        h = height;
        w = img.getWidth() * h / img.getHeight();
        centeredHoriz = true;
    }
    else {
        w = width;
        h = img.getHeight() * w / img.getWidth();
        centeredHoriz = false;
    }
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
    if (mode == IMAGE) {
        if (centeredHoriz) {
            img.draw(0.5 * (width - w), 0, w, h);
        }
        else {
            img.draw(0, 0.5 * (height - h), w, h);
        }
    }
    else if (mode == MOVIE) {
        if (centeredHoriz) {
            player.draw(0.5 * (width - w), 0, w, h);
        }
        else {
            player.draw(0, 0.5 * (height - h), w, h);
        }
    }
}

