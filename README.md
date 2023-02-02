# 3DCurlingGame
This code repository is for a 3D curling game played by two teams (5 stones each). 
The game code is in following three files:

Curlinggame3D.cpp
Helper.cpp
Helper.h

Whereas the multithreading and networking are in the following client and server files:

helper_udp.h
server.py

Note: The server is written on python while the client is on C++

# Depencencies required
I have not added the header files used in this project. most of those are available online. I used OpenGL for graphics. But other libraries I used were:
stdafx.h
targetver.h
vecmath.h
glut32.lib

Note: Although you can use the Project1.exe file to play, but if you need to build a game, built the application in your system first to see which files are missing.

## To run the code on a different computer

To run the code from a different system, we need to have the project1.exe and 
glut.dll file in one folder on your system. To run the server, write: 

python ./server.py 127.0.0.1 20001

Use the IP address of the system to run the server with any random port i.e. 20001 
from command prompt and then open another command prompt from the folder you saved the
project1.exe with client 0 or 1, IP address and the port like this:

./Project1.exe 0 127.0.0.1 20001

When both files are executed, you can see the game screen. 

## How to play 
K = for throwing the stones
Cursor up = increase in length of the beam 
Cursor down = decrease in length of the beam
Cursor left = to change the direction of beam to left
Cursor right = to change the direction of beam to right


## Rules
Each team plays alternatively. 
Stone is inside the hog line, score = +10 
Stone is outside the boundary, score = -5 

Scores are calculated on each turn. AMAZING PHYSICS AWAITING!!!
