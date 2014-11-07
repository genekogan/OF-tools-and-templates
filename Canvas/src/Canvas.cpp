#include "Canvas.h"


//----------------
void Canvas::setup(int width, int height) {
    this->width = width;
    this->height = height;
    guiVisible = true;
    guiPosition = ofPoint(5, 5);
    setupGui();
}

//----------------
void Canvas::setupGui() {
    control.clear();
    control.setName("manage layers");
    control.setGuiPosition(guiPosition.x+170*layers.size(), guiPosition.y);

    // canvas management
    control.addEvent("savePreset", this, &Canvas::savePresetFromGui);
    control.addEvent(" +creator", this, &Canvas::addCreator);
    if (layers.size() > 0) {
        control.addEvent(" +modifier", this, &Canvas::addModifier);
        control.addEvent(" +postProcessor", this, &Canvas::addPostProcessor);
        control.addEvent(" +postGlitch", this, &Canvas::addPostGlitch);
        control.addEvent(" +postFx", this, &Canvas::addPostFx);
    }
    for (int i=0; i<layers.size(); i++) {
        control.addEvent(" -layer "+ofToString(i)+" ("+layers[i]->getName()+")", this, &Canvas::setDeleteLayer);
        layers[i]->setGuiPosition(guiPosition.x+170*i, guiPosition.y);
        if (i>0) layers[i]->setTexLayer(layers[i-1]);
    }
    
    // canvas presets
    ofDirectory dir(ofToDataPath("presets/Canvas/"));
    dir.allowExt("xml");
    dir.listDir();
    vector<string> filenames;
    for(int i = 0; i < dir.numFiles(); i++) {
        filenames.push_back(dir.getName(i));
    }
    control.addMenu("presets", filenames, this, &Canvas::loadPreset);
    control.addParameter("load skip creator", &skipCreator);
}

//----------------
void Canvas::addLayer(CanvasLayer *newLayer) {
    if (layers.size() > 0)
        newLayer->setup(width, height, layers[layers.size()-1]);
    else
        newLayer->setup(width, height);
    layers.push_back(newLayer);
    setupGui();
}

//----------------
CanvasLayer* Canvas::addLayer(LayerType type) {
    CanvasLayer *layer;
    if      (type == CANVAS_CREATOR)         layer = new CreatorLayer();
    else if (type == CANVAS_MODIFIER)        layer = new ModifierLayer();
    else if (type == CANVAS_POST_PROCESSING) layer = new PostProcessingLayer();
    else if (type == CANVAS_POST_GLITCH)     layer = new PostGlitchLayer();
    else if (type == CANVAS_POST_FX)         layer = new PostFxLayer();
    if (layers.size() > 0)
        layer->setup(width, height, layers[layers.size()-1]);
    else
        layer->setup(width, height);
    layers.push_back(layer);
    setupGui();
    return layer;
}

//----------------
void Canvas::setVisible(bool guiVisible) {
    this->guiVisible = guiVisible;
    for (int i=0; i<layers.size(); i++) {
        layers[i]->setVisible(guiVisible);
    }
    control.setVisible(guiVisible);
}

//----------------
void Canvas::toggleGuiVisible() {
    setVisible(!guiVisible);
}

//----------------
void Canvas::update() {
    if (toDeleteLayer) {
        deleteLayer();
        return;
    }
    for (int i=0; i<layers.size(); i++) {
        layers[i]->checkGuiCalls();
        layers[i]->render();
    }
}

//----------------
void Canvas::draw(int x, int y) {
    if (layers.size() == 0) return;
    layers[layers.size()-1]->draw(x, y);
}

//----------------
void Canvas::setDeleteLayer(string &s) {
    idxLayer = ofToInt(ofToString(s[8]));
    toDeleteLayer = true;
}

//----------------
void Canvas::deleteLayer() {
    toDeleteLayer = false;
    CanvasLayer *layerToDelete = layers[idxLayer];
    layers.erase(layers.begin()+idxLayer);
    setupGui();
    delete layerToDelete;
}

//----------------
void Canvas::clearLayers(bool skipCreator) {
    int idxStart = skipCreator ? 1 : 0;
    for (int i=idxStart; i<layers.size(); i++) {
        layers[i]->setVisible(false);
        delete layers[i];
    }
    layers.erase(layers.begin()+idxStart, layers.end());
}

//----------------
Canvas::~Canvas() {
    clearLayers(false);
}

//----------------
void Canvas::loadPreset(string &filename) {
    Presets presets;
    ofXml xml;
    
    string path = ofToDataPath("presets/Canvas/"+filename);
    bool xmlLoaded = xml.load(path);
    if (!xmlLoaded) {
        cout << "failed to load preset " << "test.xml" << endl;
        return;
        
    }

    clearLayers(skipCreator);

    xml.setTo("Layers");
    xml.setTo(skipCreator ? "Layer[1]" : "Layer[0]");
    
    do {
        string name = xml.getValue<string>("Name");
        string type = xml.getValue<string>("Type");
        
        xml.setTo("Control");
        
        xml.setTo("Preset");
        ofXml xmlControl;
        xmlControl.addXml(xml);
        xml.setToParent();
        
        CanvasLayer *newLayer;
        if (type == "CreatorLayer")
            newLayer = addLayer(CANVAS_CREATOR);
        else if (type == "ModifierLayer")
            newLayer = addLayer(CANVAS_MODIFIER);
        else if (type == "PostProcessingLayer")
            newLayer = addLayer(CANVAS_POST_PROCESSING);
        else if (type == "PostGlitchLayer")
            newLayer = addLayer(CANVAS_POST_GLITCH);
        else if (type == "PostFxLayer")
            newLayer = addLayer(CANVAS_POST_FX);
        
        // load main control for new layer
        presets.loadPreset(newLayer->control, xmlControl);
        
        xml.setToParent();
        
        if (type == "CreatorLayer" ||
            type == "ModifierLayer") {
            xml.setTo("Scene");
            
            xml.setTo("Preset");
            ofXml xmlControl;
            xmlControl.addXml(xml);
            xml.setToParent();
            
            // load scene for Creator/Modifier
            presets.loadPreset(newLayer->scene->control, xmlControl);
            
            xml.setToParent();
        }
    }
    while (xml.setToSibling());
    xml.setToParent();
}

//----------------
bool Canvas::savePreset(string filename) {
    if (filename=="") {
        filename = ofSystemTextBoxDialog("Choose a filename");
    }
    if (filename=="") {
        return false;
    }
    Presets presets;

    ofXml xml;
    xml.addChild("Layers");
    xml.setTo("Layers");
    
    for (int i=0; i<layers.size(); i++) {
        ofXml xmlLayer;
        xmlLayer.addChild("Layer");
        xmlLayer.setTo("Layer");
        xmlLayer.addValue("Name", layers[i]->getName());
        xmlLayer.addValue("Type", layers[i]->getType());
        
        // add main control
        ofXml xmlControl;
        ofXml xmlControlPreset = presets.getXml(layers[i]->control);
        xmlControl.addChild("Control");
        xmlControl.setTo("Control");
        xmlControl.addXml(xmlControlPreset);
        xmlLayer.addXml(xmlControl);

        // add scene (if creator or modifier)
        if (layers[i]->getType() == "CreatorLayer" ||
            layers[i]->getType() == "ModifierLayer") {
            ofXml xmlScene;
            ofXml xmlScenePreset = presets.getXml(layers[i]->scene->getControl());
            xmlScene.addChild("Scene");
            xmlScene.setTo("Scene");
            xmlScene.addXml(xmlScenePreset);
            xmlLayer.addXml(xmlScene);
        }
        
        xml.addXml(xmlLayer);
    }
    
    string path = ofToDataPath("presets/Canvas/"+filename+".xml");
    xml.save(path);
    return true;
}

//----------------
void Canvas::savePresetFromGui(string &s) {
    bool saved = savePreset();
    if (saved) {
        setupGui();
    }
}
