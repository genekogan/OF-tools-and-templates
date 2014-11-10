#include "PostFx.h"


//-----------
void PostFxLayer::setup() {
    bloom.allocate(width, height);
    gaussianBlur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    blur.allocate(width, height);
    median.allocate(width, height);
    oldtv.allocate(width, height);
    inverse.allocate(width, height);
    barrelChromaAb.allocate(width, height);
    chromaAb.allocate(width, height);
    chromaGlitch.allocate(width, height);
    grayscale.allocate(width, height);
    
    
    dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
    
    lutChoices.push_back("None");
    for (int i=0; i<dir.size(); i++) {
        lutChoices.push_back(dir.getPath(i));
    }
    string fxStr[] = { "none", "bloom", "gaussian_blur", "bokeh",
                       "glow", "blur", "median", "oldtv", "inverse",
                    "barrelChromaAb", "chromaAb", "chromaGlitch", "grayscale" };
    vector<string> fxChoices(fxStr, fxStr + sizeof(fxStr) / sizeof(fxStr[0]));
    
    /*
    for (int i=0; i<lutChoices.size(); i++) {
        ofxLUT *newLut = new ofxLUT();
        newLut->allocate(width, height);
        newLut->loadLUT(lutChoices[i]);
        lut.push_back(newLut);
    }*/
    
    active = 0;
    
    control.setName("postFx");
    control.addMenu("LUT", lutChoices, this, &PostFxLayer::chooseLut);
    control.addMenu("Fx", fxChoices, this, &PostFxLayer::chooseFx);
    
    control.addParameter("radius", &radius, -15.0f, 15.0f);
    control.addParameter("fade", &fade, -1.0f, 1.0f);
    
    string fx = "none";
    chooseFx(fx);
    
    pass = true;
}

//-----------
void PostFxLayer::chooseLut(string &s ) {
    if (s=="None") {
        pass = true;
    }
    else {
        for (int i=0; i<lutChoices.size(); i++) {
            if (lutChoices[i] == s) {
                active = i;
            }
        }
        pass = false;
    }
}

//-----------
void PostFxLayer::chooseFx(string &s ) {
    selection = s;
}

//-----------
void PostFxLayer::render() {
    if (!pass) {
        lut[active]->setTexture(texLayer->getFbo()->getTextureReference());
        lut[active]->update();
    }
    
    fbo.begin();

    if (selection == "none") {
        if (pass)   texLayer->getFbo()->draw(0, 0);
        else        lut[active]->draw();
    }
    else if (selection == "bloom"){
        if (pass)   bloom << texLayer->getFbo()->getTextureReference();
        else        bloom << *lut[active];
        bloom.update();
        bloom.draw();
    }
    else if (selection == "gaussian_blur"){
        gaussianBlur.setRadius(radius);
        if (pass)   gaussianBlur << texLayer->getFbo()->getTextureReference();
        else        gaussianBlur << *lut[active];
        gaussianBlur.update();
        gaussianBlur.draw();
    }
    else if (selection == "bokeh"){
        bokeh.setRadius(radius);
        if (pass)   bokeh << texLayer->getFbo()->getTextureReference();
        else        bokeh << *lut[active];
        bokeh.update();
        bokeh.draw();
    }
    else if (selection == "glow"){
        glow.setRadius(radius);
        if (pass)   glow << texLayer->getFbo()->getTextureReference();
        else        glow << *lut[active];
        glow.update();
        glow.draw();
    }
    else if (selection == "blur"){
        blur.setFade(fade);
        if (pass)   blur << texLayer->getFbo()->getTextureReference();
        else        blur << *lut[active];
        blur.update();
        blur.draw();
    }
    else if (selection == "median"){
        if (pass)   median << texLayer->getFbo()->getTextureReference();
        else        median << *lut[active];
        median.update();
        median.draw();
    }
    else if (selection == "oldtv"){
        if (pass)   oldtv << texLayer->getFbo()->getTextureReference();
        else        oldtv << *lut[active];
        oldtv.update();
        oldtv.draw();
    }
    else if (selection == "inverse"){
        if (pass)   inverse << texLayer->getFbo()->getTextureReference();
        else        inverse << *lut[active];
        inverse.update();
        inverse.draw();
    }
    else if (selection == "barrelChromaAb"){
        if (pass)   barrelChromaAb << texLayer->getFbo()->getTextureReference();
        else        barrelChromaAb << *lut[active];
        barrelChromaAb.update();
        barrelChromaAb.draw();
    }
    else if (selection == "chromaAb"){
        if (pass)   chromaAb << texLayer->getFbo()->getTextureReference();
        else        chromaAb << *lut[active];
        chromaAb.update();
        chromaAb.draw();
    }
    else if (selection == "chromaGlitch"){
        if (pass)   chromaGlitch << texLayer->getFbo()->getTextureReference();
        else        chromaGlitch << *lut[active];
        chromaGlitch.update();
        chromaGlitch.draw();
    }
    else if (selection == "grayscale"){
        if (pass)   grayscale << texLayer->getFbo()->getTextureReference();
        else        grayscale << *lut[active];
        grayscale.update();
        grayscale.draw();
    }

    fbo.end();
}

//-----------
PostFxLayer::~PostFxLayer() {
    for (int i=0; i<lut.size(); i++) {
        //lut[i]->clear();
        //delete lut[i];
    }
}