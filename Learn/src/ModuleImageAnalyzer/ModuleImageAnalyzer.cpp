#include "ModuleImageAnalyzer.h"


//-----------
ModuleImageAnalyzer::ModuleImageAnalyzer() : Learn() {
    setGuiInputsVisible(false);
    
    cam.initGrabber(320, 240);
    
//    player.loadMovie("/Users/Gene/Media/german_train_grid.mov");
    player.loadMovie("/Users/Gene/Desktop/german_train_320x240.mp4");
    player.setLoopState(OF_LOOP_NORMAL);
    player.play();

    
    
    control.setName("ImageAnalyzer");
    //control.addParameter("velocity lerp", &vLerpRate, 0.0f, 1.0f);
    
    //vLerpRate = 0.2;
    
	
    //imitate(previous, cam);
	//imitate(diff, cam);
    imitate(previous, cam);
	imitate(diff, cam);
    
    
    frameDiffCol.resize(cam.getWidth());

    
    
    
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	stepSize = 32;
	ySteps = cam.getHeight() / stepSize;
	xSteps = cam.getWidth() / stepSize;
	for(int y = 0; y < ySteps; y++) {
		for(int x = 0; x < xSteps; x++) {
			mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
			mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
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

    
    
    
    addInput("frameDiffTotal", &frameDiffTotal, 0.0f, 8000.0f);
    addInput("frameDiffTotalRed", &frameDiffTotalRed, 0.0f, 3000.0f);
    addInput("frameDiffTotalGreen", &frameDiffTotalGreen, 0.0f, 3000.0f);
    addInput("frameDiffTotalBlue", &frameDiffTotalBlue, 0.0f, 3000.0f);
    
}

//-----------
void ModuleImageAnalyzer::update() {
    player.update();

	cam.update();

	
//    if(cam.isFrameNew()) {
    if(player.isFrameNew()) {
        
		// get frame differencing
		absdiff(previous, cam, diff);
		diff.update();
		copy(cam, previous);

		columnMean = meanCols(diff);
		diffMean = mean(toCv(diff));
		diffMean *= Scalar(50);

        
        frameDiffTotalRed = diffMean[0];
        frameDiffTotalGreen = diffMean[1];
        frameDiffTotalBlue = diffMean[2];
        frameDiffTotal = frameDiffTotalRed + frameDiffTotalGreen + frameDiffTotalBlue;

        Vec3b column;
        for(int i = 0; i < columnMean.rows; i++) {
            column = columnMean.at<Vec3b>(i);
            frameDiffCol[i] = float(column[0] + column[1] + column[2]);
        }


        
        
        flow.setWindowSize(8);
		flow.calcOpticalFlow(player);
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

        
        
	}
    
    Learn::update();
}

//-----------
void ModuleImageAnalyzer::draw() {
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