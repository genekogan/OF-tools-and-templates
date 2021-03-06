#include "ModuleImageAnalyzer.h"


//-----------
ModuleImageAnalyzer::ModuleImageAnalyzer() : Learn() {
    setGuiInputsVisible(false);
    
    width = 320;
    height = 240;
    
    stepSize = 32;
    numBins = 30;
    vLerpRate = 0.2;
    
    
    if (mode == 0) {
        cam.initGrabber(width, height);
        imitate(previous, cam);
        imitate(diff, cam);
        frameDiffCol.resize(cam.getWidth());
        ySteps = cam.getHeight() / stepSize;
        xSteps = cam.getWidth() / stepSize;
    }
    else if (mode == 1) {
        player.loadMovie("/Users/Gene/Desktop/german_train_320x240.mp4");
        player.setLoopState(OF_LOOP_NORMAL);
        player.play();
        imitate(previous, player);
        imitate(diff, player);
        frameDiffCol.resize(player.getWidth());
        ySteps = cam.getHeight() / stepSize;
        xSteps = cam.getWidth() / stepSize;
    }
    else if (mode == 2) {
        ofImage img;
        img.loadImage("/Users/Gene/Code/openFrameworks/examples/graphics/imageLoaderExample/bin/data/images/bikers.jpg");
        img.resize(width, height);
        image.setFromPixels(img.getPixels(), width, height);
        imitate(previous, image);
        imitate(diff, image);
        frameDiffCol.resize(image.getWidth());
        ySteps = cam.getHeight() / stepSize;
        xSteps = cam.getWidth() / stepSize;
    }
    
    
    
    control.setName("ImageAnalyzer");
    control.addParameter("velocity lerp", &vLerpRate, 0.0f, 1.0f);
    control.addParameter("bins", &numBins, 6, 64);
    
    
    
    // set up materials
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for(int y = 0; y < ySteps; y++) {
		for(int x = 0; x < xSteps; x++) {
			mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
			mesh.addTexCoord(ofVec2f(x * stepSize, y    * stepSize));
		}
	}
	for(int y = 0; y + 1 < ySteps; y++) {
		for(int x = 0; x + 1 < xSteps; x++) {
			int nw = y * xSteps + x;
			int ne = nw + 1;
			int sw = nw + xSteps;
			int se = sw + 1;
			mesh.addIndex(nw);
			mesh.addIndex(ne);
			mesh.addIndex(se);
			mesh.addIndex(nw);
			mesh.addIndex(se);
			mesh.addIndex(sw);
		}
	}

    // setup histograms
    rgb.allocate(width, height);
    hsv.allocate(width, height);
    r.allocate(width, height);
    g.allocate(width, height);
    b.allocate(width, height);
    h.allocate(width, height);
    s.allocate(width, height);
    v.allocate(width, height);
    
    
    
    addInput("frameDiffTotal", &frameDiffTotal, 0.0f, 8000.0f);
    addInput("frameDiffTotalRed", &frameDiffTotalRed, 0.0f, 3000.0f);
    addInput("frameDiffTotalGreen", &frameDiffTotalGreen, 0.0f, 3000.0f);
    addInput("frameDiffTotalBlue", &frameDiffTotalBlue, 0.0f, 3000.0f);
    
    

    
}

//-----------
void ModuleImageAnalyzer::getHistograms() {
    if (mode == 0) {
        rgb.setFromPixels(cam.getPixelsRef());
    }
    else if (mode == 1) {
        rgb.setFromPixels(player.getPixelsRef());
    }
    else if (mode == 2) {
        rgb.setFromPixels(image.getPixelsRef());
    }
    
    // get separate red, green, blue channels
    r.setFromPixels(rgb.getPixelsRef().getChannel(0));
    g.setFromPixels(rgb.getPixelsRef().getChannel(1));
    b.setFromPixels(rgb.getPixelsRef().getChannel(2));
    
    // convert rgb to hsv and grab each channel individually
    cvCvtColor(rgb.getCvImage(), hsv.getCvImage(), CV_BGR2HSV);
    h.setFromPixels(hsv.getPixelsRef().getChannel(0));
    s.setFromPixels(hsv.getPixelsRef().getChannel(1));
    v.setFromPixels(hsv.getPixelsRef().getChannel(2));
    
    // get histograms
    histogramR = histogram.getHistogram(r, numBins); // 30 bins
    histogramG = histogram.getHistogram(g, numBins);
    histogramB = histogram.getHistogram(b, numBins);
    histogramH = histogram.getHistogram(h, numBins);
    histogramS = histogram.getHistogram(s, numBins);
    histogramV = histogram.getHistogram(v, numBins);
}

//-----------
void ModuleImageAnalyzer::drawHistograms() {
    ofSetColor(255, 0, 0);
    r.draw(0, 0);
    drawHistogram(histogramR);
    
    ofTranslate(width, 0);
    
    ofSetColor(0, 255, 0);
    g.draw(0, 0);
    drawHistogram(histogramG);
    
    ofTranslate(width, 0);
    
    ofSetColor(0, 0, 255);
    b.draw(0, 0);
    drawHistogram(histogramB);
    
    ofTranslate(-2*width, height);
    
    ofSetColor(255);
    h.draw(0, 0);
    drawHistogram(histogramH);
    
    ofTranslate(width, 0);
    
    ofSetColor(255);
    s.draw(0, 0);
    drawHistogram(histogramS);
    
    ofTranslate(width, 0);
    
    ofSetColor(255);
    v.draw(0, 0);
    drawHistogram(histogramV);
    
    
    ofTranslate(-2*width, -height);
    image.draw(10, 10, 500, 400);
    ofDrawBitmapString("red", 0, 15);
    ofDrawBitmapString("green", width, 15);
    ofDrawBitmapString("blue", 2*width, 15);
    ofDrawBitmapString("hue", 0, 15+height);
    ofDrawBitmapString("saturation", width, 15+height);
    ofDrawBitmapString("value", 2*width, 15+height);
}

//--------------------------------------------------------------
void ModuleImageAnalyzer::drawHistogram(vector<float> & h) {
    ofBeginShape();
    ofNoFill();
    ofSetLineWidth(3);
    for (int i=0; i<h.size(); i++) {
        float x = ofMap(i, 0, h.size(), 0, 320);
        float y = ofMap(h[i], 0, 0.3, 240, 0);
        ofVertex(x, y);
    }
    ofEndShape();
}


//-----------
void ModuleImageAnalyzer::update() {
    
    bool toUpdate = false;
    if (mode == 0) {
        cam.update();
        toUpdate = cam.isFrameNew();
    }
    else if (mode == 1) {
        player.update();
        toUpdate = player.isFrameNew();
    }
    else if (mode == 2) {
        toUpdate = false;
        
        // this has to be done at least once upon processing
    }

	
    if(toUpdate) {
        
		// get frame differencing
        if (mode == 0) {
            absdiff(previous, cam, diff);
            diff.update();
            copy(cam, previous);
        }
        else if (mode == 1) {
            absdiff(previous, player, diff);
            diff.update();
            copy(player, previous);
        }

		columnMean = meanCols(diff);
		diffMean = mean(toCv(diff));
		diffMean *= Scalar(50);


        frameDiffTotalRed = diffMean[0];
        frameDiffTotalGreen = diffMean[1];
        frameDiffTotalBlue = diffMean[2];
        frameDiffTotal = frameDiffTotalRed + frameDiffTotalGreen + frameDiffTotalBlue;
         
        /*
        frameDiffTotalRed = ofLerp(frameDiffTotalRed, diffMean[0], 0.2);
        frameDiffTotalGreen = ofLerp(frameDiffTotalGreen, diffMean[1], 0.2);
        frameDiffTotalBlue = ofLerp(frameDiffTotalBlue, diffMean[2], 0.2);
        frameDiffTotal = ofLerp(frameDiffTotal, frameDiffTotalRed + frameDiffTotalGreen + frameDiffTotalBlue, 0.2);
         */

        
        Vec3b column;
        for(int i = 0; i < columnMean.rows; i++) {
            column = columnMean.at<Vec3b>(i);
            frameDiffCol[i] = float(column[0] + column[1] + column[2]);
        }


        
        
        flow.setWindowSize(8);
        if (mode == 0) {
            flow.calcOpticalFlow(cam);
        }
        else if (mode == 1) {
            flow.calcOpticalFlow(player);
        }
        else if (mode == 2) {
            // this shouldn't work
            flow.calcOpticalFlow(image);
        }
        
		int i = 0;
		float distortionStrength = 4;
		for(int y = 1; y + 1 < ySteps; y++) {
			for(int x = 1; x + 1 < xSteps; x++) {
				int i = y * xSteps + x;
				ofVec2f position(x * stepSize, y * stepSize);
				ofRectangle area(position - ofVec2f(stepSize, stepSize) / 2, stepSize, stepSize);
				ofVec2f offset = flow.getAverageFlowInRegion(area);
				mesh.setVertex(i, position + distortionStrength * offset);
				i++;
			}
		}

        
        getHistograms();
        
	}
    
    Learn::update();
}

//-----------
void ModuleImageAnalyzer::draw() {
    
    
    
    //ofTranslate(320, 240);
    //ofSetHexColor(0xffffff);
//	colorImg = img;
//	colorImg.draw(20,20);
    /*
	int scale = 10;
	hist_img = outHistImg.getCvImage();
	cvZero ( hist_img );
    
	float max_value = 0;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	for( int h = 0; h < h_bins; h++ ){
		for( int s = 0; s < s_bins; s++ ){
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound( bin_val * 255 / max_value );
            cout << "intensity " << h << " " << s << " " << intensity << " " << bin_val << endl;
			cvRectangle( hist_img, cvPoint( h*scale, s*scale ),
						cvPoint( (h+1)*scale - 1, (s+1)*scale - 1 ),
						CV_RGB( intensity, intensity, intensity ),
						CV_FILLED );
		}
	}
	outImg = hist_img;
	outImg.draw(20, 20);
    */
    
    
    
    
    if (visible) {
        Learn::draw();
        
        ofPushMatrix();
        ofTranslate(x, y);
        //ofScale(0.5, 0.5);

        
        /*
        cam.getTextureReference().bind();
        mesh.draw();
        cam.getTextureReference().unbind();
        if(ofGetMousePressed()) {
            mesh.drawWireframe();
        }
         */

        player.getTextureReference().bind();
        mesh.draw();
        player.getTextureReference().unbind();
        if(ofGetMousePressed()) {
            mesh.drawWireframe();
        }

        diff.draw(640, 0);
        
        
        
        /*
        ofTranslate(0, 400);
        
        
        ofSetColor(255, 0, 0);
        flow.setWindowSize(8);
		flow.calcOpticalFlow(cam);
		int i = 0;
		float distortionStrength = 4;
		for(int y = 1; y + 1 < ySteps; y++) {
			for(int x = 1; x + 1 < xSteps; x++) {
                
                
				int i = y * xSteps + x;
				ofVec2f position(x * stepSize, y * stepSize);
				ofRectangle area(position - ofVec2f(stepSize, stepSize) / 2, stepSize, stepSize);
				ofVec2f offset = flow.getAverageFlowInRegion(area);
                //mesh.setVertex(i, position + distortionStrength * offset);
                ofPushMatrix();
                ofTranslate(x*20, y*20);
                ofLine(0, 0, distortionStrength*offset.x, distortionStrength*offset.y);
                ofPopMatrix();
				i++;
			}
		}
        */
        

        ofTranslate(0, 400);
        drawHistograms();

        
        
        ofPopMatrix();
        

    }
    
    
}

//-----------
void ModuleImageAnalyzer::setGuiPosition(int x, int y) {
    this->x = x;
    this->y = y;
    control.setGuiPosition(x, y);
}

//-----------
void ModuleImageAnalyzer::setVisible(bool visible) {
    this->visible = visible;
    Learn::setVisible(visible);
}

//-----------
void ModuleImageAnalyzer::toggleGuiSummaryView() {
    Learn::setGuiSummaryView(!summaryVisible);
    visible = summaryVisible;
}

//-----------
ModuleImageAnalyzer::~ModuleImageAnalyzer() {
    cam.close();
}