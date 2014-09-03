#include "PostFx.h"


//-----------
void PostFxLayer::setup() {
    lut.allocate(width,height);
    
    bloom.allocate(width, height);
    gaussianBlur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    blur.allocate(width, height);
    contrast.allocate(width, height);
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
    
    vector<string> lutChoices;
    for (int i=0; i<dir.size(); i++) {
        lutChoices.push_back(dir.getPath(i));
    }
    string fxStr[] = { "none", "bloom", "gaussian_blur", "bokeh"
                       "glow", "blur", "contrast", "median",
                       "oldtv", "inverse", "barrelChromaAb", "chromaAb", "chromaGlitch", "grayscale" };
    vector<string> fxChoices(fxStr, fxStr + sizeof(fxStr) / sizeof(fxStr[0]));
    
    control.registerMenu("LUT", this, &PostFxLayer::chooseLut, lutChoices);
    control.registerMenu("Fx", this, &PostFxLayer::chooseFx, fxChoices);
}

//-----------
void PostFxLayer::chooseLut(string &s ) {
    lut.loadLUT(s);
}

//-----------
void PostFxLayer::chooseFx(string &s ) {
    selection = s;
    buildGui();
}

//-----------
void PostFxLayer::render() {
    lut.setTexture(texLayer->getFbo()->getTextureReference());
    lut.update();
    
    if (selection == "bloom"){
        bloom << lut ;
        bloom.update();
    }
    else if (selection == "gaussian_blur"){
        gaussianBlur.setRadius(gaussianBlurRadius);
        gaussianBlur << lut;
        gaussianBlur.update();
    }
    else if (selection == "bokeh"){
        bokeh.setRadius(bokehRadius);
        bokeh << lut;
        bokeh.update();
    }
    else if (selection == "glow"){
        glow.setRadius(glowRadius);
        glow << lut;
        glow.update();
    }
    else if (selection == "blur"){
        blur.setFade(blurFade);
        blur << lut;
        blur.update();
    }
    else if (selection == "contrast"){
        contrast << lut;
        contrast.update();
    }
    else if (selection == "median"){
        median << lut;
        median.update();
    }
    else if (selection == "oldtv"){
        oldtv << lut;
        oldtv.update();
    }
    else if (selection == "inverse"){
        inverse << lut;
        inverse.update();
    }
    else if (selection == "barrelChromaAb"){
        barrelChromaAb << lut;
        barrelChromaAb.update();
    }
    else if (selection == "chromaAb"){
        chromaAb << lut;
        chromaAb.update();
    }
    else if (selection == "chromaGlitch"){
        chromaGlitch << lut;
        chromaGlitch.update();
    }
    else if (selection == "grayscale"){
        grayscale << lut;
        grayscale.update();
    }

    // update fbo
    fbo.begin();
    if (selection == "none")
        lut.draw();
    else if (selection == "bloom")
        bloom.draw();
    else if (selection == "gaussian_blur")
        gaussianBlur.draw();
    else if (selection == "bokeh")
        bokeh.draw();
    else if (selection == "glow")
        glow.draw();
    else if (selection == "blur")
        blur.draw();
    else if (selection == "contrast")
        contrast.draw();
    else if (selection == "median")
        median.draw();
    else if (selection == "oldtv")
        oldtv.draw();
    else if (selection == "inverse")
        inverse.draw();
    else if (selection == "barrelChromaAb")
        barrelChromaAb.draw();
    else if (selection == "chromaAb")
        chromaAb.draw();
    else if (selection == "chromaGlitch")
        chromaGlitch.draw();
    else if (selection == "grayscale")
        grayscale.draw();    
    fbo.end();
}

//-----------
void PostFxLayer::setGuiPosition(int x, int y) {
    this->guiPosition = ofPoint(x, y);
    control.setGuiPosition(x, y);
    gui.setGuiPosition(x+208, y);
}

//-----------
void PostFxLayer::toggleVisible() {
    control.toggleVisible();
    gui.toggleVisible();
}

//-----------
void PostFxLayer::setVisible(bool visible) {
    control.setVisible(visible);
    gui.setVisible(visible);
}

//-----------
void PostFxLayer::buildGui(){
    gui.clearParameters();
    if (selection == "none") {
        gui.registerLabel("none");
    }
    else if (selection == "bloom") {
        gui.registerLabel("bloom");
    }
    else if (selection == "gaussian_blur" ){
        gui.registerLabel("gaussian_blur");
        gui.registerParameter("radius", &gaussianBlurRadius, -10.0f, 10.0f);
    }
    else if (selection == "bokeh"){
        gui.registerLabel("bokeh");
        gui.registerParameter("radius", &bokehRadius, 0.0f, 10.0f);
    }
    else if (selection == "glow"){
        gui.registerLabel("glow");
        gui.registerParameter("radius", &glowRadius, -15.0f, 15.0f);
    }
    else if (selection == "blur"){
        gui.registerLabel("blur");
        gui.registerParameter("fade", &blurFade, -1.0f, 1.0f);
    }
    else if (selection == "contrast"){
        gui.registerLabel("contrast");
    }
    else if (selection == "median"){
        gui.registerLabel("median");
    }
    else if (selection == "oldtv"){
        gui.registerLabel("oldtv");
    }
    else if (selection == "inverse"){
        gui.registerLabel("inverse");
    }
    else if (selection == "barrelChromaAb"){
        gui.registerLabel("barrelChromaAb");
    }
    else if (selection == "chromaAb"){
        gui.registerLabel("chromaAb");
    }
    else if (selection == "chromaGlitch"){
        gui.registerLabel("chromaGlitch");
    }
    else if (selection == "grayscale"){
        gui.registerLabel("grayscale");
    }
}
