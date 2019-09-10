# 003_GBA_Hardware_Interrupts

This tutorial content... will be uploaded soon to the appropriate places.  

**.c_cpp_properties.json**\
**Line 3 should read**: "proj_name":"003_GBA_Hardware_Interrupts"\

**launch.json**  
Line 7 : *"proj_name":"003_GBA_Hardware_Interrupts"* \
Line 20: *"program": "${workspaceFolder}/003_GBA_Hardware_Interrupts/003_GBA_Hardware_Interrupts.elf",*\

**tasks.json**
Lines 05 - 10 :\
```JSON
"options": {
        "env": {
          "proj_working_dir": "PWD=003_GBA_Hardware_Interrupts",
          "proj_name":"003_GBA_Hardware_Interrupts"
        }
     },
```

 **Only need to modify if you are using PC** \
Line 71: *"command": "**path_to_mGBA**/mGBA.exe -g ${workspaceFolder}/003_GBA_Hardware_Interrupts/003_GBA_Hardware_Interrupts.gba;sleep 5;echo debuggerReady"* \
Line 90: *"command": "**path_to_mGBA**/mGBA-0.6.3-win32/mGBA.exe ${cwd}/$env:proj_name/$env:proj_name.gba"* \
**Only need to modify if you are using OS X** \
Line 74 & 93: *"command": "**path_to_mGBA**/mGBA.app/Contents/MacOS/mGBA",* \

![Look It Works](./images/screenshot.PNG)
