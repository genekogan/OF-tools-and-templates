#include "Canvas.h"

//--------
void CanvasLayer::setup(int width, int height, CanvasLayer *texLayer) {
    this->width = width;
    this->height = height;
    this->texLayer = texLayer;
    fbo.allocate(width, height);
    setup();
    if (texLayer != NULL) {
        ((Shader *) scene)->setTexture(texLayer->getFbo());
    }
}


//--------
void CreatorLayer::setup() {
    vector<string> choices;
    choices.push_back("debug");
    choices.push_back("agents");
    choices.push_back("amoeba");
    choices.push_back("bubbles");
    choices.push_back("cubes");
    choices.push_back("gridfly");
    choices.push_back("letters");
    choices.push_back("meshy");
    choices.push_back("movie");
    choices.push_back("polar");
    choices.push_back("rivers");
    choices.push_back("shader");
    choices.push_back("shapespace");
    choices.push_back("subdivide");
    choices.push_back("syphon");

    scene = new DebugScreen();
    control.registerMenu("select scene", this, &CreatorLayer::select, choices);
}

//--------
void ModifierLayer::setup() {
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
    
    scene = new Shader();
    ((Shader *) scene)->setupBrCoSa();
    control.registerMenu("select scene", this, &ModifierLayer::select, choices);
}

//--------
void CreatorLayer::render() {
    fbo.begin();
    if (texLayer != NULL)
        texLayer->draw(0, 0);
    scene->draw(0, 0);
    fbo.end();
}

//--------
void ModifierLayer::render() {
    fbo.begin();
    scene->draw(0, 0);
    fbo.end();
}

//--------
void CreatorLayer::select(string &s) {
    delete scene;
    if      (s == "debug")      scene = new DebugScreen();
    else if (s == "agents")     scene = new Agents();
    else if (s == "amoeba")     scene = new Amoeba();
    else if (s == "bubbles")    scene = new Bubbles();
    else if (s == "cubes")      scene = new Cubes();
    else if (s == "gridfly")    scene = new GridFly();
    else if (s == "letters")    scene = new Letters();
    else if (s == "meshy")      scene = new Meshy();
    else if (s == "movie")      scene = new MoviePlayer();
    else if (s == "polar")      scene = new Polar();
    else if (s == "rivers")     scene = new Rivers();
    else if (s == "shader")     scene = new Shader();
    else if (s == "shapespace") scene = new ShapeSpace();
    else if (s == "subdivide")  scene = new Subdivide();
    else if (s == "syphon")     scene = new Syphon();
    
    scene->setup(width, height);
    if (s == "shader")  ((Shader *) scene)->setupBlobby();
}

//--------
void ModifierLayer::select(string &s) {
    if      (s == "brcosa")     ((Shader *) scene)->setupBrCoSa();
    else if (s == "pixelate")   ((Shader *) scene)->setupPixelate();
    else if (s == "bilateral")  ((Shader *) scene)->setupBilateralFilter();
    else if (s == "blur")       ((Shader *) scene)->setupBlur();
    else if (s == "channels")   ((Shader *) scene)->setupChannels();
    else if (s == "deform")     ((Shader *) scene)->setupDeform();
    else if (s == "edges")      ((Shader *) scene)->setupEdges();
    else if (s == "cmyk")       ((Shader *) scene)->setupHalftoneCmyk();
    else if (s == "halftone")   ((Shader *) scene)->setupHalftone();
    else if (s == "hue")        ((Shader *) scene)->setupHue();
    else if (s == "invert")     ((Shader *) scene)->setupInvert();
    else if (s == "neon")       ((Shader *) scene)->setupNeon();
    else if (s == "patches")    ((Shader *) scene)->setupPatches();
    else if (s == "pixelrolls") ((Shader *) scene)->setupPixelRolls();
    else if (s == "threshold")  ((Shader *) scene)->setupThreshold();
    else if (s == "wrap")       ((Shader *) scene)->setupWrap();
}


//--------
void Canvas::setup(int width, int height) {
    this->width = width;
    this->height = height;
}

//--------
void Canvas::addCreator(int n) {
    for (int i=0; i<n; i++) {
        CanvasLayer *creator = new CreatorLayer();
        if (layers.size() > 0)
            creator->setup(width, height, layers[layers.size()-1]);
        else
            creator->setup(width, height);
        layers.push_back(creator);
    }
}

//--------
void Canvas::addModifier(int n) {
    for (int i=0; i<n; i++) {
        CanvasLayer *modifier = new ModifierLayer();
        if (layers.size() > 0)
            modifier->setup(width, height, layers[layers.size()-1]);
        else
            modifier->setup(width, height);
        layers.push_back(modifier);
    }
}

//--------
void Canvas::toggleGuiVisible() {
}

//--------
void Canvas::update() {
    for (int i=0; i<layers.size(); i++) {
        layers[i]->render();
    }
}

//--------
void Canvas::draw(int x, int y) {
    layers[layers.size()-1]->draw(x, y);
}

//--------
Canvas::~Canvas() {
    for (int i=0; i<layers.size(); i++) {
        delete layers[i];
    }
}