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
- scenes can also be saved.
- realtime shadows with light maps.
- Possibility to mix deferred and forward rendering.
- depth of field post processing effect.
- Bloom
- Gamma Correction
- Tone mapping
- shader preProcessor that allows to understand #include directives to allow for easier shader writing.
- materials can get the uniforms used by it's shader, and offer an interface to edit them if the uniform is of type vec3, float or sampler2D.
-a complete user interface to edit rotation, scale position of objects, add textures, create new materials, recompile shaders, edit material values depending on the content of the shaders, and a tree of items where nodes, lights and object can be added, edited, removed or copy/pasted.

## wanted features
- more scene files to hilight different features
- cascaded shadows
- instanciation for particle system.
- use tiled rendering and a compute shader to diminish the amount of light/fragment calculation.
- better .obj loading that can be slow for more complex objects. 

It is all very simple, but allowed me to learn about a lot of different techniques. And I try to build the library so it is easy to add more to it fast.
