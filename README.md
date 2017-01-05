# ShaderScene
A WIP OpenGL application that allows loading and linking of meshes and shaders.

Shaders are loaded seperately and can be linked to unique shader programs.
Meshes can support multiple shapes.

WorldObjects are the means by which Meshes are rendered. They hold references 
to a given mesh and shader program, and each can contain unique data for each 
uniform in the shader program. Once the data is set, it is all handled 
automatically.

Current Goals:
- Loading and rendering textures.
- Automatic reading and setup of uniforms.
- Light source objects.
- Automatic management of loaded resources.
- More shaders.
- UI for switching meshes/shaders/uniform data in application.


This project makes use of OpenGL, GLFW, GLM, and tinyobjloader
