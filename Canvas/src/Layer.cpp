#include "Layer.h"


//----------------
void CreatorLayer::setup() {
    setupChoices();
    control.setName("Creator");
    control.clear();
    control.addMenu("select scene", choices, this, &CreatorLayer::selectScene);
    guiPosition = ofPoint(5, 5);
    setupCreators();
}

//----------------
void CreatorLayer::setupCreators() {
    agents = new Agents();
    amoeba = new Amoeba();
    bubbles = new Bubbles();
    cubes = new Cubes();
    debug = new DebugScreen();
    gridfly = new GridFly();
    letters = new Letters();
    meshy = new Meshy();
    movie = new MoviePlayer();
    polar = new Polar();
    rivers = new Rivers();
    shader = new Shader();
    shapespace = new ShapeSpace();
    subdivide = new Subdivide();
    syphon = new Syphon();

    scene = debug;
    string s = "debug";
    selectScene(s);
    scene->getControl().headerSelected = true;
}

//----------------
void CreatorLayer::setupChoices() {
    string vj[] = {"debug", "agents", "amoeba", "bubbles",
        "cubes", "gridfly", "letters", "meshy", "movie", "polar",
        "rivers", "shader", "shapespace", "subdivide", "syphon"};
    
    string mods[] = {"blobby", "bits", "bits_exp", "electro",
        "eye", "heroku_bubbles", "landscapes", "monjori", "nebula",
        "noisy", "ikeda", "rain", "sinewave", "sinewave_exp", "wave",
        "fractal_scope", "fractal_flower", "curtains"};
    
    choices = vector<string>(vj, vj + sizeof(vj) / sizeof(vj[0]));
    shaders = vector<string>(mods, mods + sizeof(mods) / sizeof(mods[0]));
}

//----------------
void ModifierLayer::setup() {
    setupChoices();
    scene = new Shader();
    control.setName("Modifier");
    control.addMenu("select scene", choices, this, &ModifierLayer::selectScene);
    if (texLayer != NULL) {
        ((Shader *) scene)->setTexture(texLayer->getFbo());
    }
    string s = "brcosa";
    selectScene(s);
    scene->getControl().headerSelected = true;
}

//----------------
void ModifierLayer::setTexLayer(CanvasLayer *texLayer) {
    this->texLayer = texLayer;
    if (texLayer != NULL) {
        ((Shader *) scene)->setTexture(texLayer->getFbo());
    }
}

//----------------
void CreatorLayer::selectScene(string &s) {
    scene->setActive(false);
    setupScene(s);
    scene->setup(width, height);
    scene->setActive(true);
    control.setVisible(false);
    scene->setGuiPosition(guiPosition.x, guiPosition.y);
    if (s == "shader") {
        string sh = "blobby";
        selectShader(sh);
    }
}

//----------------
void CreatorLayer::selectShader(string &s) {
    setupShader(s);
    if (texLayer != NULL) {
        ((Shader *) scene)->setTexture(texLayer->getFbo());
    }
    scene->getControl().addMenu("Select shader", shaders, this, &CreatorLayer::selectShader);
}

//----------------
void CreatorLayer::setupScene(string s) {
    if      (s == "debug")      scene = debug;
    else if (s == "agents")     scene = agents;
    else if (s == "amoeba")     scene = amoeba;
    else if (s == "bubbles")    scene = bubbles;
    else if (s == "cubes")      scene = cubes;
    else if (s == "gridfly")    scene = gridfly;
    else if (s == "letters")    scene = letters;
    else if (s == "meshy")      scene = meshy;
    else if (s == "movie")      scene = movie;
    else if (s == "polar")      scene = polar;
    else if (s == "rivers")     scene = rivers;
    else if (s == "shader")     scene = shader;
    else if (s == "shapespace") scene = shapespace;
    else if (s == "subdivide")  scene = subdivide;
    else if (s == "syphon")     scene = syphon;
}

//----------------
void CreatorLayer::setupShader(string s) {
    if 		(s == "blobby")         ((Shader *) scene)->setupBlobby();
    else if (s == "bits")           ((Shader *) scene)->setupBits();
    else if (s == "bits_exp")       ((Shader *) scene)->setupBitsExperimental();
    else if (s == "electro")        ((Shader *) scene)->setupElectro();
    else if (s == "eye")            ((Shader *) scene)->setupEye();
    else if (s == "heroku_bubbles") ((Shader *) scene)->setupHerokuBubbles();
    else if (s == "landscapes")     ((Shader *) scene)->setupLandscape();
    else if (s == "monjori")        ((Shader *) scene)->setupMonjori();
    else if (s == "nebula")         ((Shader *) scene)->setupNebula();
    else if (s == "noisy")          ((Shader *) scene)->setupNoisy();
    else if (s == "ikeda")          ((Shader *) scene)->setupIkeda();
    else if (s == "rain")           ((Shader *) scene)->setupRain();
    else if (s == "sinewave")       ((Shader *) scene)->setupSinewave();
    else if (s == "sinewave_exp")   ((Shader *) scene)->setupSinewaveExperimental();
    else if (s == "wave")           ((Shader *) scene)->setupWave();
    else if (s == "fractal_scope")  ((Shader *) scene)->setupFractalScope();
    else if (s == "fractal_flower") ((Shader *) scene)->setupFractalFlower();
    else if (s == "curtains")       ((Shader *) scene)->setupCurtains();
}

//----------------
void ModifierLayer::setupChoices() {
    string mods[] = {"brcosa", "pixelate", "bilateral", "blur",
        "channels", "deform", "edges", "cmyk", "halftone",
        "hue", "invert", "neon", "patches", "pixelrolls",
        "grayscale", "threshold", "wrap" };
    choices = vector<string>(mods, mods + sizeof(mods) / sizeof(mods[0]));
}

//----------------
void ModifierLayer::selectScene(string &s) {
    setupScene(s);
    control.setVisible(false);
    scene->setActive(true);
    scene->setGuiPosition(guiPosition.x, guiPosition.y);
}

//----------------
void ModifierLayer::setupScene(string s) {
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
    else if (s == "grayscale")  ((Shader *) scene)->setupGrayscale();
    else if (s == "threshold")  ((Shader *) scene)->setupThreshold();
    else if (s == "wrap")       ((Shader *) scene)->setupWrap();
}

//----------------
void CreatorLayer::render() {
    fbo.begin();
    if (texLayer != NULL)
        texLayer->draw(0, 0);
    scene->draw(0, 0);
    fbo.end();
}

//----------------
void ModifierLayer::render() {
    fbo.begin();
    scene->draw(0, 0);
    fbo.end();
}
