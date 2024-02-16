# Simulating Ocean

The code is splitted into multiple projects : the engine, the application, and Glfw, Glad and ImGui. However, only the files from the Engine and the Application are interesting to check.

## The Engine

This project uses a very early version of the Hazel Game Engine. The version used was composed of a simple 2D renderer with a orthographic camera, basic OpenGL abstraction for shaders, buffers, textures. The code was all rewritten by hand by me to learn both OpenGL and C++.

I added a perspective camera and its controller, the mesh class, the RenderTexture class that I used to passe texture to compute shaders and shaders, a compute shader class and ssbo to pass some struct to the gpu. 

All the file of the Engine are in the Judas-Engine file. As every **src/Renderer** file is an abstraction of its OpenGL implementation (the goal is to be able to add support for Vulkan and other graphics library easily), the 'interesting files' are all in the **src/platform/OpenGL** folder. The following files might be interesting to check to better understand the code of the ocean simulation :

* OpenGLRenderTexture
* OpenGLComputeShader
* OpenGLBuffer (especially the **OpenGLDataBufferObject** class)

## The 'Ocean Simulation' Application

The Application provides multiple vizualization in the ImGui window : one to vizualize the FFT Butterfly texture containing all the information for the FFT, the Spectra display and the Waves. The spectra display enables to vizualize the different spectra. The waves enables the ocean vizualization, the spectrum can be choosen in the ImGui window that appeared, and its parameters can be changed in this same window. 

# Note

This project was entirely done in Windows using Visual Studio and Premake (like make but easier to use), I am not sure if it compiles on Linux.