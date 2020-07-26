# [LogicWorld](https://logicworld.net/) board converter
A simple commandline tool for converting Logic World boards  
Thanks a lot to:
* DayDun for reverse engeneering the file format
* jrnvnjk for helping me debug some issues
* DanielGJ44 and the other LW beta testers for sharing board files

## Features
- [x] obj exporter (does not support vertex colors)
- [X] ply exporter (does support vertex colors)

## Using the tool
1. download the latest release or build the source  
2. run the tool and input the file path of the model.tungmodel file
3. choose a export method and run the tool
4. import the converted files into your aplication of choise and set up the rest of the materials

## Usefull information
* draging the file into cmd also works
* ply has vertx color support, it is objectivly better
* the tool could take a few seconds to minuites depending on the board and your cpu
* the grid texture is a black and white mask
