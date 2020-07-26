# [LogicWorld](https://logicworld.net/) board converter
A simple commandline tool for converting Logic World boards  
Thanks a lot to DayDun for reverse engeneering the file format and jrnvnjk for helping me debug some issues

## Features
- [x] obj exporter (does not support vertex colors)
- [X] ply exporter (does support vertex colors)
- [x] modular code

## Using the tool
* download the latest release or build the source  
* run the tool and input the file path of the model.tungmodel file
* choose a export method
* let the tool run
* import the converted files into your aplication of choise and set up the rest of the materials

## Usefull information
* draging the file into cmd also works
* ply has vertx color support, it is objectivly better
* the tool could take a few seconds to minuites depending on the board and your cpu
* the grid texture is a black and white mask
