# 001_GBA_Hello_Pixel

This tutorial content comes from the tutoiral posted up [My First Pixels](https://jamiedstewart.github.io/gba%20dev/2019/02/16/GBA-Dev-My-First-Pixels.html) with the supporting YouTube video see here:

[[YouTube Video](https://img.youtube.com/vi/Rj0lf46iljc/0.jpg)](https://youtu.be/Rj0lf46iljc)

This program introduces some of the code that goes into creating a game like Pong. How to read the IO registers for button input on the GBA is not covered in this tutorial, this tutorial covers the creation of a simple scene from the game of Pong up to the point that user input would be required to take the implementation further.

Whilst there is some assumption that the person viewing this code has some idea of how to program in any programming language, so things like what is a variable are not covered explicitly, the basics of what constitutes a **struct** in **C Programming Language** are covered. This tutorial does mention bit and byte sizes, but does not go into a great amount of deatial about these things just yet, this is planned for a later tutorial in the series.

A Simple Pixel plotter is created that allows for 16bit unsigned integer data to be written to the memory location of the *SCREENBUFFER* in one of the GBA's bitmap rendering modes. This pixel plotter covers line drawing and filling a rectangular area with a solid colour. The line plotting algorithm is taken from [Bresenham's Line Drawing Algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm) on Wikipedia. 

To get this project to build and run from the root directory, the following files in the **.vscode** directory will need to be modified. On the following lines:

**.c_cpp_properties.json**\
**On OS X modify**: Lines 8, 16\
**On Linux modify**: Lines 31, 39\
**On PC modify**: Lines 52, 69\
All lines should have the following text:  *"${workspaceFolder}/001_Hello_Pixel/include"*  

**launch.json**  
Line 16 : *"program": "${workspaceFolder}/001_Hello_Pixel/001_Hello_Pixel.elf",* \
**Only need to modify this line if you are using PC** \
Line 27 : *"text": "file **path_to_root_directory**/001_Hello_Pixel/001_Hello_Pixel.elf -enable-pretty-printing"* \
**Only need to modify this line if you are using OS X** \
Line 36 : *"text": "file **path_to_root_directory**/001_Hello_Pixel/001_Hello_Pixel.elf -enable-pretty-printing"* \

**tasks.json**
Line 06 : *"cwd":"${workspaceFolder}/001_Hello_Pixel"* \
 **Only need to modify if you are using PC** \
Line 54 & 76 : *"command": "**path_to_mGBA**/mGBA.exe -g ${workspaceFolder}/001_Hello_Pixel/001_Hello_Pixel.gba;sleep 5;echo debuggerReady"* \ 
*line 76 does not need from ";sleep..." onwards.* \
**Only need to modify if you are using OS X** \
Line 60 & 81: *"${workspaceFolder}/001_Hello_Pixel/001_Hello_Pixel.gba"* \
