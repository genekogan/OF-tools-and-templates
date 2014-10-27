#pragma once

#include "ofMain.h"


class Waveform
{
public:

    // constructors/desctructor
    ~Waveform();
    Waveform();
	Waveform(string tmpPath);
    
	// methods
	string	getPath() { return myPath;}
	bool    getIsLooping();
	bool    getIsLoaded();
	bool    getIsPlaying();
	bool    getIsPaused();
	double	getPosition();
	double  getSpeed();
    
	void	setPath(string newPath);
	void	setLooping(bool loop);
	void	setPosition(double _position);
	void    setPaused(bool bPaused);
    void    setSpeed(double speed);
    
	bool	load(string tmpPath);
	bool	read();
	void	play();
	void    stop();
	double  update();
    
    bool	save();
    char *	getSummary();
    int		getChannels();
    int		getSampleRate();
	long	getLength();
    
	void	generateWaveForm();
	void	drawWaveForm(int _x, int _y, int _w, int _h, float amp);
    // public variables
    char* 	myData;
    
    ofEvent<ofPoint> selectionEvent;
    void setSelection(float sel1, float sel2);
    
private:

    typedef struct {
        double minL;
        double maxL;
        double minR;
        double maxR;
    } MiniMaxima;

    void redraw();
    
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);

    enum SoundFlags { NONE = 0, LOADED = 1, PLAYING = 2, PAUSED = 4, LOOPING = 8 };
    
	string  myPath;
	int 	myChunkSize;
	int		mySubChunk1Size;
	short 	myFormat;
	short 	myChannels;
	int   	mySampleRate;
	int   	myByteRate;
	short 	myBlockAlign;
	short 	myBitsPerSample;
	int		myDataSize;
	double	position;
	double	speed;
	double	output;
	bool    isLooping;
	unsigned char soundStatus;
    
    // draw variables
    ofFbo fbo;
    int x, y, width, height;
    float amp;
    vector<MiniMaxima> waveForm;
    ofRectangle waveformRect;
    float t1, t2;
    bool dragging;
    float select1, select2;
};
