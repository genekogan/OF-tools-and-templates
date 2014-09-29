
//----------------
class MyCustomScene : public Scene
{
public:
    void setup() {
        setName("myScene");
        
        control.addParameter("position", &position, ofVec2f(200, 200), ofVec2f(800, 600));
        control.addParameter("size", &size, ofVec2f(10, 10), ofVec2f(200, 200));
        control.addColor("color", &color);
        
        position = ofVec2f(300, 300);
        size = ofVec2f(100, 100);
        color = ofColor(255, 0, 20);
    }
    
    void update() {

    }
    
    void draw() {
        ofSetColor(color);
        ofEllipse(position.x, position.y, size.x, size.y);
    }
    
private:
    ofVec2f position;
    ofVec2f size;
    ofColor color;
};



//----------------
class CustomCreatorLayer : public CreatorLayer
{
public:
    void setupCreators() {
        custom1 = new MyCustomScene();
        custom2 = new MyCustomScene();
        custom3 = new Shader();
        agents = new Agents();
        
        scene = custom1;
        string s = "customScene1";
        selectScene(s);
    }
    
    void setupChoices() {
        string vj[] = {"customScene1", "customScene2", "customShader", "agents"};
        string mods[] = {"customCurtains"};
        choices = vector<string>(vj, vj + sizeof(vj) / sizeof(vj[0]));
        shaders = vector<string>(mods, mods + sizeof(mods) / sizeof(mods[0]));
    }
    
    void setupScene(string s) {
        if      (s == "customScene1")     scene = custom1;
        else if (s == "customScene2")     scene = custom2;
        else if (s == "customShader")     scene = custom3;
        else if (s == "agents")           scene = agents;
        if (s == "customShader") {
            string sh = "customCurtains";
            selectShader(sh);
        }
    }
    
    void setupShader(string s){
        if (s == "customCurtains") {
            ((Shader *) scene)->setShader("shaders_color/standard.vert", "shaders_color/curtains.frag");
            ((Shader *) scene)->addParameter("zoom", 0, 1);
            ((Shader *) scene)->addParameter("shift", 0, 2*TWO_PI);
            ((Shader *) scene)->addParameter("speed", 0, 1);
            ((Shader *) scene)->addParameter("fall", 0, 100);
            ((Shader *) scene)->addParameter("rows", 1, 8);
            ((Shader *) scene)->addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
        }
    }
    
    Scene *custom1, *custom2, *custom3, *agents;
};



//----------------
class CustomModifierLayer : public ModifierLayer
{
public:
    void setupChoices() {
        string mods[] = {"customBrCoSa", "customChannels" };
        choices = vector<string>(mods, mods + sizeof(mods) / sizeof(mods[0]));
    }
    
    void setupScene(string s) {
        if (s == "customBrCoSa") {
            ((Shader *) scene)->setShader("shaders_texture/standard.vert", "shaders_texture/brcosa.frag");
            ((Shader *) scene)->addParameter("brightness", 0, 2);
            ((Shader *) scene)->addParameter("saturation", -3, 5);
            ((Shader *) scene)->addParameter("contrast", -3, 5);
        }
        else if (s == "customChannels") {
            ((Shader *) scene)->setShader("shaders_texture/standard.vert", "shaders_texture/channels.frag");
            ((Shader *) scene)->addParameter("biasx", ofVec3f(-200, -200, -200), ofVec3f(200, 200, 200));
            ((Shader *) scene)->addParameter("biasy", ofVec3f(-200, -200, -200), ofVec3f(200, 200, 200));
            ((Shader *) scene)->addParameter("multx", ofVec3f(0.8, 0.8, 0.8), ofVec3f(1.2, 1.2, 1.2));
            ((Shader *) scene)->addParameter("multy", ofVec3f(0.8, 0.8, 0.8), ofVec3f(1.2, 1.2, 1.2));
        }
    }
};

