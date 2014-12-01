### Canvas

Canvas is a generic drawing surface for produce visual content. It is structured as a sequence of Layers, where the first layer is a "creator" with the original visual content, something from [Visuals](https://github.com/genekogan/OF-tools-and-templates/tree/master/Visuals/src). Subsequent layers modify the previous layer in some way.

ModifierLayer - modifies the previous layer with a texture shader.

PostFx, PostGlitch, PostProcessor - wraps [ofxPostFx](https://github.com/patriciogonzalezvivo/ofxFX), [ofxPostGlitch](https://github.com/maxillacult/ofxPostGlitch), and [ofxPostProcessing](https://github.com/neilmendoza/ofxPostProcessing) into layers.


Additional functionality
 - save and load Canvas presets. The presets contain all the layers, their parameters, and other control data (like sequencers).
 - interface for projection mapping the Canvas quads.