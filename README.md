# TetraEngine
A rendering Engine in OpenGL 4.0

This is a small attempt at learning openGL and graphics rendering in general.
For now I can load basic wavefront models and give associate materials to them.
I managed to get some simple shapes and shaders to render including parralax occlusion mapping and specularity.

I have a basic rendering pipeline that offers the ability to do post processing.

There are 3 main projects here, one library that handles the whole 3D thing, loading and stuffs.
One is a tool do visaualize scene, it allows me to debug stuffs, and see if what I implemented works, it is mostly just a window to load an openGL context and a call to render a scene. All th rest is done by the library I built.
The last project is barely started I have justa  skeleton, the purpose is to get a scene and material editor.

## Main features
-object loading
-object display.
-texture loading and display
-multiple texture channels for normal, specular, Parralax Occlusion Mapping...
-per fragment ligt calculation.
-realtime shadow.
-basis of deffered shading
-depth of field post processing effect.

# wanted features
-Bloom
-more advanced model loading.
-better handling of lights and shadows for more flexibility in shaders and in loading..

It is all very simple, but allowed me to learn about a lot of different techniques. And I try to build the library so it is easy to add more to it fast.
