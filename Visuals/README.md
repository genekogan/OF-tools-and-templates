## Visuals

This is a collection of visual content.  

#### Generative visuals

 - Agents: Render interactions between invisible agents, loosely based on Casey Reas' [Process compendium](https://vimeo.com/22955812)
 - Amoeba: A deformed circle
 - Bubbles: blurry circles floating around the screen
 - Cubes: A row of boxes moves across the screen according to varying strategies
 - DebugScreen: checkerboards and gradients
 - GridFly: Flying through space over an ofMesh surface
 - Letters: Flashing letters on the screen, [Matrix-style](http://en.wikipedia.org/wiki/Matrix_digital_rain)
 - Meshy: Needs work...
 - Polar: A grid of ribbons which move according to [polar rose equations](http://en.wikipedia.org/wiki/Rose_%28mathematics%29)
 - Rivers: Dots moving around according to a 2d Perlin noise field. [Example video](https://vimeo.com/35858818)
 - Shader: Displays and handles parameterization of shaders (more below)
 - Subdivide: Recursive subdivision of quads with modulating divisions
 - ShapeSpace: Needs work...


#### Shaders

Additionally, there are a series of shaders for producing visual content, some of which are modifications of shaders found at [glsl.heroku.com/](http://glsl.heroku.com/).

The `Shader` class handles all things shader.

There are also modifying shaders which take a texture as input. These are the basic elements in the ModifierLayer of Canvas.

The actual shaders can be found inside the `shaders` subfolder of the template's data folder.


#### MoviePlayer
 
Plays images or movies. Right now it does little more than integrates a vector of ofVideoPlayer and ofImage into Canvas, but is intended to support other features in the future, like clever playback tricks.


#### Syphon

Basically a tiny wrapper for ofxSyphon which integrates into Canvas


### Templates and customization

All visuals inherit from the `[Scene](https://github.com/genekogan/OF-tools-and-templates/blob/master/Visuals/src/Scene.h)` class which handles the integration into Canvas and other features in the toolkit.  The template `template-custom` shows how to create a custom visual, and `template-shader` shows how to use another pre-existing shader.