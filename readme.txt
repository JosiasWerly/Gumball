All code here is produce by Josias Werly 


############## dependencies ##############
GLAD, GLFW, Opengl, GLSL
GLM, IMGUI, PicoJson, LUA(embeded)



############## Project Structure ##############

GumballApplication -> is the .exe that loads GumballCore.dll
GumballCore -> is the engine code.
GumballProject -> the project where the programmer can edit while GumballApplication is running, it will generate a .dll and will be hotrealoaded to the application.

-there is no makefile (for now).
-originaly made in vs2022
-std C++x20, winSDK 10.0, ToolSet v143



############## Objectives ##############
- modular engine.
- script language (most likely LUA)
- possible to extend.
- clear patterns && arch (i'm creating this for me so there is almost none comments, however I try my best to create a clear code and pattern).
- there is no physics yet.
- memory management is all over the place.
- LUA script is Still under construction


"dont let good ideias spoil good work" - Bjarne Stroustrup.
"[...] value is not equal to work hour [...]" - StarShip troopers.
This project was rebooted in 11/2021, 09/2022


KUDOS to:
Mr. Cherno: from whom I've learned the opengl from youtube.
Mr. Lucas Pazze: great friend and programmer.