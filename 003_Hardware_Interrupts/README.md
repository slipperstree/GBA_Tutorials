# 003_Hardware_Interrupts

This tutorial covers setting up interrupt handlers on the GBA. I've covered this very early on even though it is quite an in depth topic especially considering the matter of swapping between ARM and Thumb instructions on the GBA.
This tutorial glosses over these to an extent and only really continues to utilise the C programming language, other than an C asm call to execute the SWI instruction.

Interrupts are used to allow the system hardware to call into software when certain hardware conditions are met, such as a horizontal blank, a vertical blank or the game pak being removed.

This tutorial implements the VBLANK Interrupt, or more specifically the bios VBLANK Wait Interrupt, this interrupt is really useful for GBA development as when called it will put the CPU of the GBA to sleep until a VBLANK is triggered then the CPU will receive the interrupt and wake up and continue execution from where the instruction to halt occured.

For a handheld system this is a great way to save on power as there is less batter consumption occuring and there is not the constant polling of the VCOUNT register to figure out if we are actually within a VBLANK block.

I have a [Blog Post](https://jamiedstewart.github.io/gba%20dev/2019/09/11/GBA-Dev-Hardware-Interrupts.html) which explains some of the process behind the implementation of this tutorial.

An accompanying Youtube video will be created to accompany this example soon.

**.c_cpp_properties.json**\
**Line 3 should read**: "proj_name":"003_Hardware_Interrupts"\

**launch.json**  
Line 7 : *"proj_name":"003_Hardware_Interrupts"* \
Line 20: *"program": "${workspaceFolder}/003_Hardware_Interrupts/003_GBA_Hardware_Interrupts.elf",*\

**tasks.json**
Lines 05 - 10 :\

```JSON
"options": {
        "env": {
          "proj_working_dir": "PWD=003_Hardware_Interrupts",
          "proj_name":"003_Hardware_Interrupts"
        }
     },
```

 **Only need to modify if you are using PC** \
Line 71: *"command": "**path_to_mGBA**/mGBA.exe -g ${workspaceFolder}/$env:proj_name/$env:proj_name.gba;sleep 5;echo debuggerReady"* \
Line 90: *"command": "**path_to_mGBA**/mGBA-0.6.3-win32/mGBA.exe ${cwd}/$env:proj_name/$env:proj_name.gba"* \
**Only need to modify if you are using OS X** \
Line 74 & 93: *"command": "**path_to_mGBA**/mGBA.app/Contents/MacOS/mGBA",* \

![Look It Works](./images/screenshot.PNG)
