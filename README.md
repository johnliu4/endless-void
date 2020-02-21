# Endless Void

My first indie game project! Inspired by the likes of Subway Surfers and Temple Run, I put my own personal spin on the genre to create Endless Void.

I plan to release this as a free-to-play on Steam when Endless Void is good enough. Right now this project is in its early phase and I'm still writing mostly core infrastructure.

The core engine is being programmed from scratch with C++ and OpenGL; I am developing all physics, rendering, etc logic myself. This will certainly be a difficult but fun challenge.

## Building and Running
Use **cmake**. It should be configured to run at least both Windows and macOS.

## Libraries Used

I packaged the libraries required right into the source. Here are the libraries I use:

- GLFW 3.3.2 for handling the OpenGL context and window creation
- GLM 0.9.9.7 for handling the math with matrices and vectors
- GLAD for loading OpenGL
- lodepng 20190914 for loading PNG files
