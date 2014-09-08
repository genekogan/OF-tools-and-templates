#include "Layer.h"



//----------------
void CreatorLayer::setup() {
    setupGui(false);
    scene = new DebugScreen();
    scene->setup(width, height);
    control.setName("Creator");
}

//----------------
void CreatorLayer::setupGui(bool isShader) {
    settingUp = true;
    setupChoices();
    control.clearParameters();
    control.registerMenu("select scene", this, &CreatorLayer::selectScene, choices);
    if (isShader) {
        control.registerMenu("Select shader", this, &CreatorLayer::selectShader, shaders);
    }
    settingUp = false;
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
    ((Shader *) scene)->setupBrCoSa();
    control.setName("Modifier");
    control.registerMenu("select scene", this, &ModifierLayer::selectScene, choices);
    if (texLayer != NULL) {
        ((Shader *) scene)->setTexture(texLayer->getFbo());
    }
}

void ModifierLayer::setupChoices() {
    string mods[] = {"brcosa", "pixelate", "bilateral", "blur",
        "channels", "deform", "edges", "cmyk", "halftone",
        "hue", "invert", "neon", "patches", "pixelrolls",
        "grayscale", "threshold", "wrap" };
    choices = vector<string>(mods, mods + sizeof(mods) / sizeof(mods[0]));
}

//----------------
void CreatorLayer::selectScene(string &s) {
    if (settingUp)  return;
    setupScene(s);
    if (s == "shader") {
        ((Shader *) scene)->setupBlobby();
    }
}

//----------------
void CreatorLayer::setupScene(string s) {
    if      (s == "debug")      setScene(new DebugScreen());
    else if (s == "agents")     setScene(new Agents());
    else if (s == "amoeba")     setScene(new Amoeba());
    else if (s == "bubbles")    setScene(new Bubbles());
    else if (s == "cubes")      setScene(new Cubes());
    else if (s == "gridfly")    setScene(new GridFly());
    else if (s == "letters")    setScene(new Letters());
    else if (s == "meshy")      setScene(new Meshy());
    else if (s == "movie")      setScene(new MoviePlayer());
    else if (s == "polar")      setScene(new Polar());
    else if (s == "rivers")     setScene(new Rivers());
    else if (s == "shader")     setScene(new Shader(), true);
    else if (s == "shapespace") setScene(new ShapeSpace());
    else if (s == "subdivide")  setScene(new Subdivide());
    else if (s == "syphon")     setScene(new Syphon());
}

//----------------
void CreatorLayer::setScene(Scene *newScene, bool redrawGui) {
    delete scene;
    scene = newScene;
    scene->setup(width, height);
    scene->setGuiPosition(guiPosition.x+208, guiPosition.y);
    if (redrawGui)  setupGui(true);
}

//----------------
void CreatorLayer::selectShader(string &s) {
    if (settingUp)  return;
    setupShader(s);
    if (texLayer != NULL) {
        ((Shader *) scene)->setTexture(texLayer->getFbo());
    }
}

//----------------
void CreatorLayer::setupShader(string s) {
    cout << "old setup "<<endl;
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
void ModifierLayer::selectScene(string &s) {
    setupScene(s);
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
