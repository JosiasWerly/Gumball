All code here is produce by Josias Werly 


############## Dependencies ##############
GLAD, GLFW, Opengl, GLSL
GLM, IMGUI, PicoJson, LUA(embeded)



############## Project Structure ##############

GumballApplication -> is the .exe that loads GumballCore.dll
GumballCore -> is the engine code.
GumballProject -> the project where the programmer can edit while GumballApplication is running, it will generate a .dll and will be hotrealoaded to the application.

-originaly made in vs2022, std C++x20, winSDK 10.0, ToolSet v143


############## Objectives ##############
- a step further then SFML.
- script language (most likely LUA/Rust)
- possible to extend.
- clear patterns && architecture (i'm creating this for me so there is almost none comments, however I try my best to create a clear code and pattern).


############## Notes ##############
- Zforward, XRight, YUp

"dont let good ideias spoil good work" - Bjarne Stroustrup.
"[...] value is not equal to work hour [...]" - StarShip troopers.
This project was rebooted in 11/2021, 09/2022


KUDOS to:
Mr. Cherno: from whom I've learned the opengl from youtube.
Lucas Pazze: great friend and programmer.
Leonardo Murillo: great friend and colleague.



############## TODO ##############
-improve console widget
-asset system.
-create content widget.
-create scene/level object.
