## Learn

Learn is a large tool centered around the addon [ofxLearn](https://github.com/genekogan/ofxLearn). ofxLearn adds general-purpose [machine learning](http://en.wikipedia.org/wiki/Machine_learning) functionality to openFrameworks, and the Learn tool is intended to create high-level applications which use machine learning in interesting ways.

In the broadest sense, the Learn tool takes one set of input parameters from [Control](https://github.com/genekogan/OF-tools-and-templates/tree/master/Control) and "maps" them to another set of output parameters. Inputs can come from many sources, such as parameters controlling visual/audio content, or parameters derived from statistical features coming from CV routines, [Kinect](https://github.com/genekogan/OF-tools-and-templates/tree/master/OpenNi), [LeapMotion](https://github.com/genekogan/OF-tools-and-templates/tree/master/LeapMotion), etc.  Outputs can then go to many destinations, like to visuals/audio, or out through OSC/Midi. The Learn tool contains a growing number of templates for input/output combinations, e.g. OpenNi -> AudioUnit, FaceTracker -> OSC, etc

The relationship mapped between inputs and outputs can be structured in simple ways, such as selecting a simple linear relationship between an input or set of inputs and a single output. Or it can be structured in very complex ways which are managed using machine learning procedures. 


### Training outputs

To train an output parameter, the following procedure is used.

 - Select a set of input parameters as features
 - Compile a training set by manually collecting examples of the input parameters paired with a desired value for the output value. Examples can be edited live in spreadsheet form (click the "examples" button in the interface)
 - Train the output parameter on the training set
 - Once trained, Learn can then make predictions of the output value, given previously unseen input parameter values, and handles all of it in the background


### Other features

You can save and load presets which save inputs and outputs and all their corresponding metadata, as well as training sets, and learned models. Thus you can train and save a model to be used again later. 

Additionally, there is an experimental "analyzer" feature which can be activated through the interface. The analyzer collects a sequence of input points, corrects their min/max ranges, and fits a [probability distribution (GMM)](https://github.com/genekogan/ofxGMM) to them. Using the analyzer, you can skip the manual data collection and automatically generate a training set from the GMM (by associating an output value to each of the Gaussian means). This tool is under development and might be useless, but has some potential to speed up the training process and give it more dynamic behavior.


### Modules

Getting quality results depends on getting reliable and high-level input features. In the src folder, there are a number of `Modules` for producing good input features from various sources, including [OpenNi](https://github.com/genekogan/OF-tools-and-templates/tree/master/OpenNi), [ofxFaceTracker](https://github.com/kylemcdonald/ofxFaceTracker), [LeapMotion](https://github.com/genekogan/OF-tools-and-templates/tree/master/LeapMotion), image analysis, and others. Each of the modules derive useful high-level features, and feed them as parameters to Learn. Each of the modules have corresponding templates.


### Development

Learn is a work in progress, has many bugs and incomplete features and stubs for future features -- thus it is probably highly confusing for most people right now. But this will improve as I add tutorials on how to use it, as well as add more templates which should work "out of the box" with little intervention.
