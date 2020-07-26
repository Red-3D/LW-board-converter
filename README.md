# [LogicWorld](https://logicworld.net/) board converter
A simple tool for converting Logic World boards  
Thanks a lot to:
* DayDun for reverse engineering the file format
* jrnvnjk for helping me debug some issues
* DanielGJ44 and the other LW beta testers for sharing board files

## Features
- [x] obj exporter (does not support vertex colors)
- [X] ply exporter (does support vertex colors)

## Using the tool
1. download the latest release or build the source  
2. run the tool and input the file path of the model.tungmodel file
3. choose a export method and run the tool
4. import the converted files into your application of choice and set up the rest of the materials

## Useful information
* dragging the file into cmd also works
* ply has vertex color support, it is objectively better
* the tool could take a few seconds to minutes depending on the board and your cpu
* the grid texture is a black and white mask
