#include "PostFx.h"


//-----------
void PostFxLayer::setup() {
    lut.allocate(width,height);
    
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
    
    vector<string> lutChoices;
    for (int i=0; i<dir.size(); i++) {
        lutChoices.push_back(dir.getPath(i));
    }
    string fxStr[] = { "none", "bloom", "gaussian_blur", "bokeh",
                       "glow", "blur", "median", "oldtv", "inverse",
                    "barrelChromaAb", "chromaAb", "chromaGlitch", "grayscale" };
    vector<string> fxChoices(fxStr, fxStr + sizeof(fxStr) / sizeof(fxStr[0]));
    
    control.setName("postFx");
    control.addMenu("LUT", lutChoices, this, &PostFxLayer::chooseLut);
    control.addMenu("Fx", fxChoices, this, &PostFxLayer::chooseFx);
    
    control.addParameter("radius", &radius, -15.0f, 15.0f);
    control.addParameter("fade", &fade, -1.0f, 1.0f);
    
    string fx = "none";
    chooseFx(fx);
}

//-----------
void PostFxLayer::chooseLut(string &s ) {
    lut.loadLUT(s);
}

//-----------
void PostFxLayer::chooseFx(string &s ) {
    selection = s;
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
        gaussianBlur.setRadius(radius);
        gaussianBlur << lut;
        gaussianBlur.update();
    }
    else if (selection == "bokeh"){
        bokeh.setRadius(radius);
        bokeh << lut;
        bokeh.update();
    }
    else if (selection == "glow"){
        glow.setRadius(radius);
        glow << lut;
        glow.update();
    }
    else if (selection == "blur"){
        blur.setFade(fade);
        blur << lut;
        blur.update();
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
