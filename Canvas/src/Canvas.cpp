#include "Canvas.h"


//--------
void Canvas::setup(int width, int height, int numCreators, int numModifiers) {
    this->width = width;
    this->height = height;
    this->numCreators = numCreators;
    this->numModifiers = numModifiers;

    for (int i=0; i<numCreators; i++) {
        Scene *newScene = new Agents();
        newScene->setup(width, height);
        scenes.push_back(newScene);

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

        Control *newSelector = new Control();
        selectors.push_back(newSelector);
        
        if      (i == 0)
            newSelector->registerMenu("select scene", this, &Canvas::selectScene0, choices);
        else if (i == 1)
            newSelector->registerMenu("select scene", this, &Canvas::selectScene1, choices);
        else if (i == 2)
            newSelector->registerMenu("select scene", this, &Canvas::selectScene2, choices);
        else if (i == 3)
            newSelector->registerMenu("select scene", this, &Canvas::selectScene3, choices);
    }
    
    for (int i=0; i<numModifiers; i++) {
        Modifier *newModifier = new Modifier();
        newModifier->setup(width, height);
        modifiers.push_back(newModifier);
    }
    
    setGuiPosition(5, 5);
    
    
    fbo1.allocate(width, height);
    fbo2.allocate(width, height);
}

//--------
void Canvas::setGuiPosition(int x, int y) {
    guiPosition.set(x, y);
    // needs to be persistent when scene switched
    for (int i=0; i<scenes.size(); i++) {
        selectors[i]->setGuiPosition(guiPosition.x + i*420, guiPosition.y);
        scenes[i]->setGuiPosition(guiPosition.x + i*420 + 208, guiPosition.y);
    }
    for (int i=0; i<numModifiers; i++) {
        modifiers[i]->setGuiPosition(guiPosition.x + 420*scenes.size() + 210*i, guiPosition.y);
    }
}

//--------
void Canvas::toggleGuiVisible() {
    for (int i=0; i<scenes.size(); i++) {
        scenes[i]->toggeVisible();
        selectors[i]->toggleVisible();
    }
}

//--------
void Canvas::update() {

}

//--------
void Canvas::draw(int x, int y) {
    
    fbo1.begin();
    for (int i=0; i<scenes.size(); i++) {
        scenes[i]->draw(x, y);
    }
    fbo1.end();
    
    
    
    modifiers[0]->apply(&fbo1);

    fbo2.begin();
    modifiers[0]->draw(0, 0);
    fbo2.end();

    fbo2.draw(x, y);
}

//--------
void Canvas::selectScene(int idx, string &s) {
    Scene *newScene;
    if      (s == "debug")      newScene = new DebugScreen();
    else if (s == "agents")     newScene = new Agents();
    else if (s == "amoeba")     newScene = new Amoeba();
    else if (s == "bubbles")    newScene = new Bubbles();
    else if (s == "cubes")      newScene = new Cubes();
    else if (s == "gridfly")    newScene = new GridFly();
    else if (s == "letters")    newScene = new Letters();
    else if (s == "meshy")      newScene = new Meshy();
    else if (s == "movie")      newScene = new MoviePlayer();
    else if (s == "polar")      newScene = new Polar();
    else if (s == "rivers")     newScene = new Rivers();
    else if (s == "shader")     newScene = new Shader();
    else if (s == "shapespace") newScene = new ShapeSpace();
    else if (s == "subdivide")  newScene = new Subdivide();
    else if (s == "syphon")     newScene = new Syphon();
    
    Scene *oldScene = scenes[idx];
    delete oldScene;
    scenes[idx] = newScene;

    newScene->setup(width, height);
    newScene->setGuiPosition(guiPosition.x + idx*420 + 208, guiPosition.y);

    if (s == "shader") {
        ((Shader *) newScene)->setupBlobby();
    }
}