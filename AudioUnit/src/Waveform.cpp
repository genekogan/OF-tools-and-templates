#include "Waveform.h"


Waveform::~Waveform()
{
    ofRemoveListener(ofEvents().mousePressed, this, &Waveform::mousePressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &Waveform::mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &Waveform::mouseReleased);
    delete myData;
    myChunkSize = NULL;
    mySubChunk1Size = NULL;
    myFormat = NULL;
    myChannels = NULL;
    mySampleRate = NULL;
    myByteRate = NULL;
    myBlockAlign = NULL;
    myBitsPerSample = NULL;
    myDataSize = NULL;
}

// empty constructor
Waveform::Waveform()
{
    position = 0;
    speed = 1.0;
    soundStatus = NONE;
    
    x = 0;
    y = 0;
    width = 1;
    height = 1;
    amp = 0;
    
    ofAddListener(ofEvents().mousePressed, this, &Waveform::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &Waveform::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &Waveform::mouseReleased);
}

// constructor takes a wav path
Waveform::Waveform(string tmpPath)
{
    position = 0;
    speed = 1.0;
    soundStatus = NONE;
    
    myPath = tmpPath;
    read();
}

void Waveform::setPath(string tmpPath)
{
    myPath = tmpPath;
}

void Waveform::setLooping(bool loop)
{
    if(loop) {
        soundStatus |= LOOPING;
    }
    else {
        soundStatus &= ~LOOPING;
    }
}

bool Waveform::getIsLooping()
{
    if(soundStatus & LOOPING) return true;
    else return false;
}

bool Waveform::getIsLoaded()
{
    if(soundStatus & LOADED) return true;
    else return false;
}

bool Waveform::getIsPlaying()
{
    if(soundStatus & PLAYING) return true;
    else return false;
}

bool Waveform::getIsPaused()
{
    if(soundStatus & PAUSED) return true;
    else return false;
}

bool Waveform::load(string tmpPath) {
    myPath = tmpPath;
	bool result = read();
	return result;
}

void Waveform::generateWaveForm()
{
	waveForm.clear();
    
	bool loopState = getIsLooping();
	setLooping(false);
	bool playState = getIsPlaying();
	double tmpSpeed = getSpeed();
	setSpeed(0.25f);
    play();
	// waveform display method based on this discussion http://answers.google.com/answers/threadview/id/66003.html
    
    double sampleL, sampleR;
	while ((long)position<getLength()) {
        
		MiniMaxima mm;
		mm.minL = mm.minR = mm.maxL = mm.maxR = 0;
        
		for (int i = 0; i < 256; i++){
            
		    if(myChannels == 1) {
                sampleL = update();
                sampleR = sampleL;
		    }
		    else {
                sampleL = update()*0.5;
                sampleR = update()*0.5;
		    }
            
			mm.minL = MIN(mm.minL, sampleL);
			mm.minR = MIN(mm.minR, sampleR);
			mm.maxL = MAX(mm.maxL, sampleL);
			mm.maxR = MAX(mm.maxR, sampleR);
            
		}
        
		waveForm.push_back(mm);
	}
    
	position = 0;
	setLooping(loopState);
	setSpeed(tmpSpeed);
	if(playState) play();
}

void Waveform::drawWaveForm(int _x, int _y, int _w, int _h, float _amp)
{
    if (dragging || x!=_x || y!=_y || width!=_w || height!=_h || amp!=_amp) {
        this->x = _x;
        this->y = _y;
        this->width = _w;
        this->height = _h;
        this->amp = _amp;
        waveformRect = ofRectangle(x, y, width, height);
        redraw();
    }
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(x, y);
    ofSetColor(255);
    fbo.draw(0, 0);
    ofPopMatrix();
    ofPopMatrix();
}

void Waveform::redraw() {
    float w = width;
    float h = height*0.5;

    fbo.allocate(width, height);
    fbo.begin();
    
    ofPushMatrix();
    ofPushStyle();
    
    ofClear(0, 0);
    ofSetColor(0);
    ofRect(0, 0, width, height);

    ofSetColor(0, 255, 0);
    
    if (dragging) {
        int selX = width * (select1-t1) / (t2 - t1);
        int selW = width * (select2-select1) / (t2 - t1);
        ofRect(selX, 0, selW, height);
    }
    ofTranslate(0, h*0.5);
    
    float waveFormZoomX = (float)waveForm.size()/(float)w;

    int start = ofMap(t1, 0, 1, 1, waveForm.size());
    int end   = ofMap(t2, 0, 1, 1, waveForm.size());
    
	//for (unsigned int i = 1; i < waveForm.size(); i++){
    float di = 1.0;
    for (float i = start; i < end; i+=di){
        int x0 = ofMap(i-1, start, end, 0, w);
        int x1 = ofMap(i,   start, end, 0, w);
        
	    if(myChannels == 1) {
            ofSetColor(0, 0, 255);
            ofLine(x0, h + (int)(waveForm.at(i-1).minR*h), x1, h + (int) (waveForm.at(i).maxR*h*amp));
            ofLine(x1, h + (int)(waveForm.at(i).maxR*h),   x1, h + (int) (waveForm.at(i).minR*h*amp));
	    } else {
            ofSetColor(255, 0, 0);
            ofLine(x0, (int)(waveForm.at(i-1).minL*h), x1, (int) (waveForm.at(i).maxL*h*amp));
            ofLine(x1, (int)(waveForm.at(i).maxL*h),   x1, (int) (waveForm.at(i).minL*h*amp));
            
            ofSetColor(0, 0, 255);
            ofLine(x0, h + (int)(waveForm.at(i-1).minR*h), x1, h + (int) (waveForm.at(i).maxR*h*amp));
            ofLine(x1, h + (int)(waveForm.at(i).maxR*h),   x1, h + (int) (waveForm.at(i).minR*h*amp));
	    }
	}
    
	ofSetColor(0, 255, 0);

	float waveFormDisplayScale = getLength()/w;
    if(myChannels == 1) {
        ofLine(position/waveFormDisplayScale, -(float)h*0.0, position/waveFormDisplayScale, (float)h*2.0);
    }
    else
    {
        ofLine(position/waveFormDisplayScale, -(float)h*0.5, position/waveFormDisplayScale, (float)h*1.5);
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    fbo.end();
}

void Waveform::setSelection(float sel1, float sel2) {
    t1 = sel1;
    t2 = sel2;
    redraw();
}

void Waveform::stop()
{
    position = 0;
    soundStatus &= ~PAUSED;
    soundStatus &= ~PLAYING;
}

void Waveform::play()
{
    if(speed > 0)
        position = 0;
    else
        position = getLength();
    soundStatus |= PLAYING;
}

void Waveform::setPaused(bool bPaused)
{
    if(bPaused) {
        soundStatus |= PAUSED;
    }
    else {
        soundStatus &= ~PAUSED;
    }
}

void Waveform::setSpeed(double spd)
{
    speed = spd;
}

double Waveform::getSpeed()
{
    return speed;
}

int Waveform::getSampleRate()
{
    return mySampleRate;
}

double Waveform::update()
{
    if(!(soundStatus & PLAYING)) return 0;
    if(soundStatus & PAUSED) return 0;
    if(!getIsLoaded()) return 0;
    
    long length=getLength();
	double remainder;
	short* buffer = (short *)myData;
	position=(position+speed);
	remainder = position - (long) position;
    
    // check if reached EOF
	if ((long) position>length) {
	    if(getIsLooping()) {
            position=0;
	    }
        else {
            soundStatus &= ~PLAYING;
            return 0;
	    }
	}
    
    //check if position less than zero (reverse)
	if ((long) position < 0) {
	    if(getIsLooping()) {
            position = length;
	    }
        else {
            soundStatus &= ~PLAYING;
            return 0;
	    }
	}
    
	output = (double) ((1.0-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767.0;//linear interpolation
    
	return(output);
}


long Waveform::getLength()
{
	long length;
	length=myDataSize*0.5;
	return(length);
}

double Waveform::getPosition()
{
    double pos = position/getLength();
    pos = ofClamp(pos,0.0,1.0);
	return pos;
}

void Waveform::setPosition(double _position)
{
	double pct = ofClamp(_position,0.0,1.0);
	position = pct * getLength();
}

// return a printable summary of the wav file
char* Waveform::getSummary()
{
    char *summary = new char[250];
    sprintf(summary, " Format: %d\n Channels: %d\n SampleRate: %d\n ByteRate: %d\n BlockAlign: %d\n BitsPerSample: %d\n DataSize: %d\n", myFormat, myChannels, mySampleRate, myByteRate, myBlockAlign, myBitsPerSample, myDataSize);
    std::cout << myDataSize;
    return summary;
}

int Waveform::getChannels()
{
    return myChannels;
}

// write out the wav file
bool Waveform::save()
{
    ofToDataPath(myPath);
    fstream myFile (myPath.c_str(), ios::out | ios::binary);
    
    // write the wav file per the wav file format
    myFile.seekp (0, ios::beg);
    myFile.write ("RIFF", 4);
    myFile.write ((char*) &myChunkSize, 4);
    myFile.write ("WAVE", 4);
    myFile.write ("fmt ", 4);
    myFile.write ((char*) &mySubChunk1Size, 4);
    myFile.write ((char*) &myFormat, 2);
    myFile.write ((char*) &myChannels, 2);
    myFile.write ((char*) &mySampleRate, 4);
    myFile.write ((char*) &myByteRate, 4);
    myFile.write ((char*) &myBlockAlign, 2);
    myFile.write ((char*) &myBitsPerSample, 2);
    myFile.write ("data", 4);
    myFile.write ((char*) &myDataSize, 4);
    myFile.write (myData, myDataSize);
    
    return true;
}



// read a wav file into this class
bool Waveform::read()
{
    myPath = ofToDataPath(myPath,true).c_str();
    ifstream inFile( myPath.c_str(), ios::in | ios::binary);
    
    ofLog(OF_LOG_NOTICE, "Reading file %s",myPath.c_str());
    
    if(!inFile.is_open())
    {
        ofLog(OF_LOG_ERROR,"Error opening file. File not loaded.");
        return false;
    }
    
    char id[4];
    inFile.read((char*) &id, 4);
    if(strncmp(id,"RIFF",4) != 0)
    {
        ofLog(OF_LOG_ERROR,"Error reading sample file. File is not a RIFF (.wav) file");
        return false;
    }
    
    inFile.seekg(4, ios::beg);
    inFile.read( (char*) &myChunkSize, 4 ); // read the ChunkSize
    
    inFile.seekg(16, ios::beg);
    inFile.read( (char*) &mySubChunk1Size, 4 ); // read the SubChunk1Size
    
    //inFile.seekg(20, ios::beg);
    inFile.read( (char*) &myFormat, sizeof(short) ); // read the file format.  This should be 1 for PCM
    if(myFormat != 1) {
        ofLog(OF_LOG_ERROR, "File format should be PCM, sample file failed to load.");
        return false;
    }
    
    //inFile.seekg(22, ios::beg);
    inFile.read( (char*) &myChannels, sizeof(short) ); // read the # of channels (1 or 2)
    
    //inFile.seekg(24, ios::beg);
    inFile.read( (char*) &mySampleRate, sizeof(int) ); // read the Samplerate
    
    //inFile.seekg(28, ios::beg);
    inFile.read( (char*) &myByteRate, sizeof(int) ); // read the byterate
    
    //inFile.seekg(32, ios::beg);
    inFile.read( (char*) &myBlockAlign, sizeof(short) ); // read the blockalign
    
    //inFile.seekg(34, ios::beg);
    inFile.read( (char*) &myBitsPerSample, sizeof(short) ); // read the bitsperSample
    
    inFile.seekg(40, ios::beg);
    inFile.read( (char*) &myDataSize, sizeof(int) ); // read the size of the data
    //cout << myDataSize << endl;
    
    // read the data chunk
    myData = new char[myDataSize];
    inFile.seekg(44, ios::beg);
    inFile.read(myData, myDataSize);
    
    inFile.close(); // close the input file
    
    soundStatus |= LOADED;
    
    return true; // this should probably be something more descriptive
}

void Waveform::mousePressed(ofMouseEventArgs &evt) {
    if (waveformRect.inside(evt.x, evt.y)) {
        dragging = true;
        select1 = ofMap(evt.x, waveformRect.getX(), waveformRect.getX()+waveformRect.getWidth(), t1, t2);
        select2 = select1;
    }
}

void Waveform::mouseDragged(ofMouseEventArgs &evt) {
    if (dragging) {
        select2 = ofMap(evt.x, waveformRect.getX(), waveformRect.getX()+waveformRect.getWidth(), t1, t2);
    }
}

void Waveform::mouseReleased(ofMouseEventArgs &evt) {
    if (dragging) {
        dragging = false;
        ofPoint selection(select1, select2);
        ofNotifyEvent(selectionEvent, selection);
    }
}
