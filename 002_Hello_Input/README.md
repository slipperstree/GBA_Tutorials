# 002_GBA_Hello_Input

This tutorial content comes from the tutoiral posted up [GBA Button Input](https://jamiedstewart.github.io/gba%20dev/2019/02/16/GBA-Dev-My-First-Pixels.html) with the supporting YouTube video see here:

[![YouTube Video](https://img.youtube.com/vi/Rj0lf46iljc/0.jpg)](https://www.youtube.com/embed/Rj0lf46iljc)

This program continiues the process of creating a game like Pong. The previous tutorial in this series saw us creating a simple version of Pong that had a moving ball and two static paddles, continuing this code we will first break the project down into separate header and source files as working on something in one massive C file and having all game logic contained within main.c is not a great way of making a game.  

This tutorial additionally introduces how to handle the volatile data that cn be found in the input registers for the button presses on the GBA. Here we read from an input register and then use bitwise operations to test this register to see which key has been pressed. We will code additonal logic to calculte if a button has been pressed or released in the current frame or if it is being held down between subsequent update frames.

To get this project to build and run from the root directory, the following files in the **.vscode** directory will need to be modified. On the following lines:

**.c_cpp_properties.json**\
**On OS X modify**: Lines 8, 16\
**On Linux modify**: Lines 31, 39\
**On PC modify**: Lines 52, 69\
All lines should have the following text:  *"${workspaceFolder}/002_Hello_Input/include"*  

**launch.json**  
Line 16 : *"program": "${workspaceFolder}/002_Hello_Input/002_Hello_Input.elf",* \
**Only need to modify this line if you are using PC** \
Line 27 : *"text": "file **path_to_root_directory**/002_Hello_Input/002_Hello_Input.elf -enable-pretty-printing"* \
**Only need to modify this line if you are using OS X** \
Line 36 : *"text": "file **path_to_root_directory**/002_Hello_Input/002_Hello_Input.elf -enable-pretty-printing"* \

**tasks.json**
Line 06 : *"cwd":"${workspaceFolder}/002_Hello_Input"* \
 **Only need to modify if you are using PC** \
Line 54 & 76 : *"command": "**path_to_mGBA**/mGBA.exe -g ${workspaceFolder}/002_Hello_Input/002_Hello_Input.gba;sleep 5;echo debuggerReady"* \
*line 76 does not need from ";sleep..." onwards.* \
**Only need to modify if you are using OS X** \
Line 60 & 81: *"${workspaceFolder}/002_Hello_Input/002_Hello_Input.gba"* \

![Look It Works](./images/screenshot.PNG)
