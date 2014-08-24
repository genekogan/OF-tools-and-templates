#include "Modifier.h"


//--------
Modifier::~Modifier() {
    delete shader;
}

//--------
Modifier::Modifier() {

}

//--------
void Modifier::setup(int width, int height) {
    shader = new Shader();
    shader->setup(width, height);
    ((Shader *) shader)->setupBrCoSa();
    

    vector<string> choices;
    choices.push_back("brcosa");
    choices.push_back("pixelate");
    choices.push_back("bilateral");
    choices.push_back("blur");
    choices.push_back("channels");
    choices.push_back("deform");
    choices.push_back("edges");
    choices.push_back("cmyk");
    choices.push_back("halftone");
    choices.push_back("hue");
    choices.push_back("invert");
    choices.push_back("neon");
    choices.push_back("patches");
    choices.push_back("pixelrolls");
    choices.push_back("threshold");
    choices.push_back("wrap");
    
    control.registerMenu("select scene", this, &Modifier::selectModifier, choices);

}

//--------
void Modifier::setGuiPosition(int x, int y) {
    control.setGuiPosition(x, y);
    shader->setGuiPosition(x+210, y);
}

//--------
void Modifier::toggleGuiVisible() {
}

//--------
void Modifier::apply(ofFbo *fboTex) {
    ((Shader *) shader)->update(fboTex);
}

//--------
void Modifier::draw(int x, int y) {
    shader->draw(x, y);
}

//--------
void Modifier::selectModifier(string &s) {
    if      (s == "brcosa")     ((Shader *) shader)->setupBrCoSa();
    else if (s == "pixelate")   ((Shader *) shader)->setupPixelate();
    else if (s == "bilateral")  ((Shader *) shader)->setupBilateralFilter();
    else if (s == "blur")       ((Shader *) shader)->setupBlur();
    else if (s == "channels")   ((Shader *) shader)->setupChannels();
    else if (s == "deform")     ((Shader *) shader)->setupDeform();
    else if (s == "edges")      ((Shader *) shader)->setupEdges();
    else if (s == "cmyk")       ((Shader *) shader)->setupHalftoneCmyk();
    else if (s == "halftone")   ((Shader *) shader)->setupHalftone();
    else if (s == "hue")        ((Shader *) shader)->setupHue();
    else if (s == "invert")     ((Shader *) shader)->setupInvert();
    else if (s == "neon")       ((Shader *) shader)->setupNeon();
    else if (s == "patches")    ((Shader *) shader)->setupPatches();
    else if (s == "pixelrolls") ((Shader *) shader)->setupPixelRolls();
    else if (s == "threshold")  ((Shader *) shader)->setupThreshold();
    else if (s == "wrap")       ((Shader *) shader)->setupWrap();
}