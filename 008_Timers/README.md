# 007_Tiled_Backgrounds

This tutorial content comes from the tutoiral posted up [GBA Bitmap Loading](https://jamiedstewart.github.io/gba%20dev/2019/02/16/GBA-Dev-My-First-Pixels.html) with the supporting YouTube video see here:

[![YouTube Video](https://img.youtube.com/vi/Rj0lf46iljc/0.jpg)](https://www.youtube.com/embed/Rj0lf46iljc)

 

To get this project to build and run from the root directory, the following files in the **.vscode** directory will need to be modified. On the following lines:

The food sprite sheet that is used for this tutorial comes from Estudio Vaca Roxa and is available for free from [itch.io](https://bakudas.itch.io/generic-rpg-pack) free sprites

**.c_cpp_properties.json**\
**Line 3 should read**: "proj_name":"007_Tiled_Backgrounds"\

**launch.json**  
Line 7 : *"proj_name":"007_Tiled_Backgrounds"* \
Line 20: *"program": "${workspaceFolder}/007_Tiled_Backgrounds/007_Tiled_Backgrounds.elf",*\

**tasks.json**
Lines 05 - 10 :\

```JSON
"options": {
        "env": {
          "proj_working_dir": "PWD=007_Tiled_Backgrounds",
          "proj_name":"007_Tiled_Backgrounds"
        }
     },
```

![Look It Works](./images/scrolling.gif)
