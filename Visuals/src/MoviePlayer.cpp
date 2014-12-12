#include "MoviePlayer.h"

//--------
void MoviePlayer::setup() {
    setName("MoviePlayer");
    
    playing = false;
    speed = 1.0f;
    oldSpeed = speed;
    
    clipsHidden = false;
    setupControl();

    player.resize(22);
    idxLoad = 0;
    
    //loadMovie("/Users/Gene/media/sinuses.mov");
}

//--------
void MoviePlayer::setupControl() {
    control.clear();
    control.addParameter("speed", &speed, -3.0f, 3.0f);
    control.addEvent("clipsHidden", this, &MoviePlayer::toggleClipsHidden);
    control.addEvent("jump", this, &MoviePlayer::jumpBack);
    control.addEvent("random", this, &MoviePlayer::jumpRandom);;
    control.addEvent("load", this, &MoviePlayer::selectMedia);
    control.addEvent("next", this, &MoviePlayer::triggerCallback);
    
    if (!clipsHidden) {
        vector<string> paths;
        for (int i=0; i<moviePaths.size(); i++) {
            paths.push_back(moviePaths[i]);
        }
        control.addMenu("movies", paths, this, &MoviePlayer::chooseMovie);
    }
}

//--------
void MoviePlayer::selectMedia(string &s) {
    ofFileDialogResult file = ofSystemLoadDialog();
    if (file.bSuccess) {
        vector<string> fileSplit = ofSplitString(file.filePath, ".");
        string extension = ofToLower(fileSplit[fileSplit.size()-1]);
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
    
    //ofVideoPlayer newPlayer;
    
    //newPlayer.loadMovie(path);
    
    /*
    for (int i=0; i<player.size(); i++) {
        player[active].stop();
    }*/
    
    /*
    newPlayer.setLoopState(OF_LOOP_NORMAL);
    newPlayer.play();
    newPlayer.update();
    newPlayer.setPaused(true);
     */
    
    
    //player[idx].push_back(newPlayer);
    //player[idxLoad] = newPlayer;
    
    
    player[idxLoad].loadMovie(path);
    player[idxLoad].setVolume(0);
    player[idxLoad].setLoopState(OF_LOOP_NORMAL);
    player[idxLoad].play();
    player[idxLoad].update();
    player[idxLoad].setPaused(true);
    
    
    //player[idx].push_back(newPlayer);
    //player[idxLoad] = newPlayer;
    
    
    idxLoad++;
    moviePaths.push_back(path);
    
    if (!clipsHidden) {
        setupControl();
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
    player[active].setFrame(player[active].getCurrentFrame()-15);
}

//--------
void MoviePlayer::jumpRandom(string &s) {
    //player[active].setFrame(ofRandom(player[active].getTotalNumFrames()));
    player[active].setFrame((int)(0.034*(float)player[active].getTotalNumFrames()));
}

//--------
void MoviePlayer::update() {
    if (!playing)   return;
    if (speed != oldSpeed) {
        player[active].setSpeed(speed);
        oldSpeed = speed;
    }
    player[active].update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
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
        
        if ((float)width/height > (float)player[active].getWidth()/player[active].getHeight()) {
            h = height;
            w = player[active].getWidth() * h / player[active].getHeight();
            centeredHoriz = true;
        }
        else {
            w = width;
            h = player[active].getHeight() * w / player[active].getWidth();
            centeredHoriz = false;
        }

        
        if (centeredHoriz) {
            player[active].draw(0.5 * (width - w), 0, w, h);
        }
        else {
            player[active].draw(0, 0.5 * (height - h), w, h);
        }
    }
}

//--------
void MoviePlayer::chooseMovie(string &s) {
    /*
    for (int i=0; i<moviePaths.size(); i++) {
        if (moviePaths[i] == s) {
            triggerMovie(i);
        }
    }
     */
    
    for (int i=0; i<idxLoad; i++) {
        if (moviePaths[i] == s) {
            triggerMovie(i);
        }
    }

}

//--------
void MoviePlayer::triggerMovie(int idx) {
    //if (idx >= player.size())   return;
    if (idx >= idxLoad)   return;
    player[active].setPaused(true);
    active = idx;
    player[active].setPaused(false);
}

//--------
void MoviePlayer::triggerMovie() {
    triggerMovie((active+1)%player.size());
}

//--------
void MoviePlayer::setClipsHidden(bool clipsHidden) {
    this->clipsHidden = clipsHidden;
    setupControl();
}

//--------
void MoviePlayer::toggleClipsHidden(string & s) {
    clipsHidden = !clipsHidden;
    setupControl();
}

//--------
void MoviePlayer::clearMovies() {
    if (player.size() > 0) {
        player[active].stop();
    }
    
    for (int i=0; i<idxLoad; i++) {
        player[i].stop();
        player[i].close();
        player[i].closeMovie();
    }
    
    idxLoad = 0;
    
//    player.erase(player.begin(), player.end());
//    player.clear();
}


