#pragma once

#include "ofMain.h"

#include "ofxBox2d.h"
#include "ofxFluid.h"

#include "ofxOpenNi.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxKinectFeatures.h"

using namespace ofxCv;
using namespace cv;

enum ContourStrategy { BLOBS, SEGMENTATION };

#include "Control.h"

#include "OpenNiTracker.h"
#include "ContourRenderer.h"
#include "SkeletonRenderer.h"

