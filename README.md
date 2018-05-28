# TetraEngine
A rendering Engine in OpenGL 4.3

This is a small attempt at learning openGL and graphics rendering in general.
For now I can load basic wavefront models and give associate materials to them.
I managed to get some simple shapes and shaders to render including parralax occlusion mapping and specularity.

I have a basic rendering pipeline that offers the ability to do post processing.

There are 3 main projects here, one library that handles the whole 3D thing, loading and stuffs.
One is a tool do visaualize scene, it allows me to debug stuffs, and see if what I implemented works, it is mostly just a window to load an openGL context and a call to render a scene. All th rest is done by the library I built.
The last project is barely started I have justa  skeleton, the purpose is to get a scene and material editor.

## Main features
- object loading and display
- cube maps
- multiple texture channels for normal, specular, Parralax Occlusion Mapping...
- a scene loader that uses JSON files tailored to this engine that can load models, shaders, materials, lights that can project orthogonal shadows.
- realtime shadows with light maps.
- Possibility to mix deferred and forward rendering.
- depth of field post processing effect.
- Bloom
- Gamma Correction
- Tone mapping

## wanted features
- more scene files to hilight different features
- cascaded shadows
- controllers on the different objects to interact with them through an interface.
- a user interface to edit scenes and materials in real time.
- instanciation for particle system.
- use tiled rendering and a compute shader to diminish the amount of light/fragment calculation.

It is all very simple, but allowed me to learn about a lot of different techniques. And I try to build the library so it is easy to add more to it fast.
