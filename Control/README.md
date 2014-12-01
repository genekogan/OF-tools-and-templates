### Control

This is the bottom layer of the tools. Most of the other tools are dependent on Control.

Control is a container class for all of the parameters of another tool, app, class, etc. Register variables with a Control instance, and it will:

 - Generate a gui (using ofxUI)
 - Automatically handle OSC communications, and generate TouchOSC layouts
 - Save and load presets
 - Modulate parameters with a sequencer
