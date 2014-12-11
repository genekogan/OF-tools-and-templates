#pragma once

#include "ofThread.h"


class ThreadedLearner : public ofThread
{
public:
    void train(vector<LearnOutputParameter *> *outputs, string trainStrategy) {
        this->outputs = outputs;
        this->trainStrategy = trainStrategy;
        idxTraining = 0;
        finished = false;
        startThread();
    }
    
    void endTraining() {
        unlock();
        stopThread();
    }
    
    void threadedFunction() {
        while (isThreadRunning()) {
            if (lock()) {
                if (outputs->at(idxTraining)->getNumInstances() > 0) {
                    if (trainStrategy == "fast") {
                        outputs->at(idxTraining)->trainClassifierFast();
                    }
                    else if (trainStrategy == "accurate") {
                        outputs->at(idxTraining)->trainClassifierAccurate();
                    }
                }
                idxTraining++;
                if (idxTraining == outputs->size()) {
                    finished = true;
                    unlock();
                    stopThread();
                }
                else {
                    unlock();
                }
            }
            else {
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }
    
    bool getFinished() {return finished;}
    
    int getTrainingIndex() {return idxTraining;}
    
    
private:
    
    vector<LearnOutputParameter *> *outputs;
    string trainStrategy;

    int idxTraining;
    bool finished;

};

