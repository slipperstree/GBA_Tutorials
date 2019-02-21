# GBA Programming Tutorial Series

This repo contains a series of tutorial code that is aimed at teaching anyone interested how to get up and running with Game Boy Advance development.

Visual Studio Code is being usd as the IDE for this series, with the intent being that the root folder of this repository is the folder that will be opened with VS Code with each specific project functioning as a sub folder from that root folder.

There is some small amount of set up involved in this where the **${cwd}** variable within **task.json** will need to be modified to set the current working project/tutorial and some modification to **c_cpp_properties.json** will need to be made to make sure the the correct include directories are specified.

There will be some modification to the **launch.json** file to accommodate for the project/tutorial that is to be run and the correct GBA/ELF files to be launched buy the debugger. 

I'm not 100% satisfied with this, and it may have been better to have each sub folder contain their own *.vscode* folders with this data kept, however I found this way of working and using GitHub to be quite easy to facilite and to improve my familiarity with the way that VS Code sets up and makes use of task, launch and property json files.  There is the added benefit that as the *${workspaceFolder}* variable will now point to the root folder here it makes changes to specify which project to compile in the appropriate subfolder easier as there is no need to accommodate for where someone using these tutorials may have cloned the root folder to on their drive. I'll take that as a little win.  \ 

It would be nice if the *${cwd}* variable worked consistently across each launch/task/properties file, but this seems not to be the case. If this gets resolved then working with these files would become even more streamlined.

These tutorials will be tested on both PC and Mac, if any problems occur with building then hit me up on the associated youtube videos that will accompany each tutorial or use GitHub to raise an issue.  
  
Additionally **each sub folder** contains their own readme.MD which has the relevant links to the tutorial as they are listed on my [website](jamiedstewart.github.io) and to the relevant YouTube video.

Happy Coding and have fun.

.kthanxby.
