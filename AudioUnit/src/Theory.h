#pragma once

#include "ofMain.h"

class Theory {
public:
    //static const int * major, minor;
    
    vector<int> major, minor;
    
    Theory();
    
    void getChord(int chord[], int root, int octave=0) {
        for (int i=0; i<3; i++) {
            chord[i] = octave * 12 + major[(root + 2*i) % 7];
        }
    }
    
    int getNote(int root, int degree, int octave=0) {
        //cout << "get note " << root << " " << degree<< " :: " << octave * 12 + root + major[degree % 8]<<endl;
        return octave * 12 + root + major[degree % 7];
    }
    
    int getNoteAtScaleDegree(int root, int degree, int idx, int octave=0) {
        return octave * 12 + root + major[(degree+idx) % 7];
    }
};